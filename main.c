/*
   catflyFlash - Open Source ST STM32 flash program for *nix
   Copyright (C) 2018 Zhang XiaoYuan  <569hmq@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "serial_posix.h"
#include "serial.h"
#include "debug.h"
#include "control.h"
#include "usart_driver.h"
#include "frame_phase.h"
#include "file.h"
#define APP2BOOT_TIMEOUT   100

usart_driver *usart = &__usart_driver;
zxy_framer *datalink = &__framer;
file_ops *fops =&__file_ops;
  char c;
/* uart interrpt function*/
void signal_handler_IO (int status)
{

}



int main(int argc, char *argv[])
{
    char *device, *imageFile;
    uint16_t moteid = 0;
    /*Parameters*/
    zxy_control *control = &__control;
    uart_frame_t        payload;
    uart_frame_t        file_payload;
    //frame_app2boot_t    frame_app2boot;
    frame_file_t        frame_file;
    frame_moteid_t      frame_moteid;

    if( argc < 4)
    {
        printf("Please input image file name and serial device name: \n");
        printf("FORMAT: emoteflash imageFile moteid deviceName \n");
        return 0;
    }
    else
    {
        imageFile = argv[1];
        moteid = atoi(argv[2]);
        device = argv[3];
    }

    /*Print version information*/
    printf("CatFly stm32 flash progrmming version 1.0.0, COPYRIGHT ZXY 2018.\n");
    /*Init serial port*/
    if(SerialInt(device)<0)
    {
        printf("serial port init failed...\n");
        return 0;
    }
     printf("Serial open success\n");

    fops->open(fops,imageFile);

    while(1)
    {
        switch(control->status)
        {
        case HOST_REQUEST_REBOOT:

            //send reboot command
            payload.payload_len=0;
            payload.type=APP2BOOT;
            datalink->send(datalink,&payload);
            //set timer
            printf("requset downding\n");
            control->change_status(control,HOST_REQUEST_REBOOT,HOST_WAIT_ACK,HOST_CMD_TIMEOUT);

        break;

        case HOST_PREPARE_BIN:

            file_payload.payload_len=fops->create(fops,&frame_file);
            if(file_payload.payload_len){
                printf("prepare downding\n");
                file_payload.type = FILE_FRAME;
                memcpy(payload.buf,&frame_file,file_payload.payload_len);
                control->change_status(control,HOST_PREPARE_BIN,HOST_DOWNLOAD_BIN,HOST_CMD_TIMEOUT);
            }
            else
            {
                printf("bin file transmit finished \n");
                control->change_status(control,HOST_PREPARE_BIN,HOST_SEND_MOTE_ID,HOST_CMD_TIMEOUT);
            }


        break;

        case HOST_DOWNLOAD_BIN:
            printf("start downding\n");
            datalink->send(datalink,&file_payload);
            control->change_status(control,HOST_DOWNLOAD_BIN,HOST_WAIT_ACK,HOST_CMD_TIMEOUT);
        break;


        case HOST_SEND_MOTE_ID:
            printf("send mote id \n");
            payload.type = MOTEID_FRAME;
            payload.payload_len = sizeof(frame_moteid);
            frame_moteid.moteid = moteid;
            memcpy(payload.buf,&frame_moteid,payload.payload_len);
            datalink->send(datalink,&payload);

            control->change_status(control,HOST_SEND_MOTE_ID,HOST_WAIT_ACK,HOST_CMD_TIMEOUT);

        break;


        case HOST_FINISHED:
            printf("mote flash succeess \n");
            SerialClose();
            fops->close(fops);
            return 0;
        break;

        case HOST_WAIT_ACK:
            if(SerialGetChar(&c))
                usart->read(usart,c);
            //max time out
            if(control->is_max_time_out(control,APP2BOOT_TIMEOUT))
            {
                SerialClose();
                return 0;
            }
            //if time out change staus to last
            if(control->check_timer(control))
            {
                control->resume_last_mission(control);
                break;
            }

        break;

        }
       // serial reciver process


    }





    return 1;
}



