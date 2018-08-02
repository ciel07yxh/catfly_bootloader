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

#define APP2BOOT_TIMEOUT   100

usart_driver *usart = &__usart_driver;
zxy_framer *datalink = &__framer;
/* uart interrpt function*/
void signal_handler_IO (int status)
{

    char c;
    SerialGetChar(&c);

    printf(" %c  ",c);
    usart->read(usart,c);
}



static uint32_t SerialUpload(const char* filename)
{
    uint32_t status = 0;
    FILE* fp;
    int32_t fileSize;
    char* buf;

    debug_print("Enter function: SerialUpload...\n");
    if(filename == NULL)
    {
        printf("no image file.\n");
        return -1;
    }

    fp = fopen(filename,"r");
    if(fp<0)
    {
        printf("open the image file failed...\n");
        return -1;
    }

    /*get file size*/
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp,0,SEEK_SET);

    /*read file into buffer*/
    buf = malloc(fileSize);
    if(buf == NULL)
    {
        printf("malloc buffer failed...\n");
        return -1;
    }

    if(fread(buf,1,fileSize,fp) != fileSize)
    {
        printf("read file failed...\n");
    }

    free(buf);
    buf =NULL;
    fclose(fp);
    return status;
}


int main(int argc, char *argv[])
{
    int8_t key;
    int8_t recv;
    char *device, *imageFile;
    uint32_t i = 0;
    uint16_t moteid = 0;
    char sbuf[8];
    /*Parameters*/

    zxy_control *control = &__control;
    uart_frame_t        payload;
    frame_ack_t         frame_ackFrame;
    frame_app2boot_t    frame_app2boot;
    frame_file_t        frame_file;

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

    while(1)
    {
        switch(control->status)
        {
        case HOST_REQUEST_REBOOT:

        /*
            ackFrame.status='Y';
            memcpy(&frame.buf,&ackFrame,sizeof(frame_ack_t));
            frame.payload_len = sizeof(frame_ack_t);
            frame.type='A';
            datalink->send(datalink,&frame);

        */
            //send reboot command
            payload.payload_len=0;
            payload.type=APP2BOOT;
            datalink->send(datalink,&payload);
            //set timer
            control->change_status(control,HOST_WAIT_BOOTLOADER,HOST_CMD_TIMEOUT);

        break;

        case HOST_WAIT_BOOTLOADER:

            //max time out
            if(control->is_max_time_out(control,APP2BOOT_TIMEOUT))
            {
                return 0;
            }
            //if time out change staus to last
            if(control->check_timer(control,NULL))
            {
                control->change_status(control,HOST_REQUEST_REBOOT,0);
                break;
            }

        break;

        case HOST_START_TRANS:

        break;

        case HOST_WAIT_FILE_ACK:

        break;

        case HOST_SEND_MOTE_ID:

        break;

        case HOST_WAIT_MOTE_ID_ACK:

        break;

        case HOST_FINISHED:

        break;

        }
       // SerialClose();

    }





    return 1;
}



