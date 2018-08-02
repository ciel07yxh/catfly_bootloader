/*
   emoteflash - Open Source ST STM32 flash program for *nix
   Copyright (C) 2010 Ling Wang <lngsa.wang@gmail.com>

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

#include "ymodem.h"
#include "serial_posix.h"
#include "serial.h"
#include "debug.h"
#include "control.h"
#include "frame_driver.h"

#define TIMEOUT 	  10
#define USART_SEND_FAILED   -1


zxy_framer *framer = &__zxy_framer;
/* uart interrpt function*/
void signal_handler_IO (int status)
{

    char c;
    SerialGetChar(&c);

    printf(" %c  ",c);
    framer->read(framer,c);
}

static int Serial_ReceiveByte(char* c, int timeout)
{
    int ref = 0;

    if(timeout == -1)
        ref = 1;
    while(timeout-- > 0 || ref)
    {
        if(SerialGetChar(c)== 1)
            return 1;
    }
    return 0;
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


    // if (Serial_ReceiveByte(&c,-1) == 1&& c==CRC16)
    {
        //debug_print("%d",c);
        printf("Uploading image %s...\n",filename);
        status = Ymodem_Transmit((uint8_t *)buf, (const uint8_t *)filename, fileSize);
        if (status != 0)
        {
            printf("Error Occured while Transmitting File!please try again!\n");
        }
        else
        {
            printf("Total %d bytes uploaded Successfully \n", fileSize);
        }
    }
    free(buf);
    buf =NULL;
    fclose(fp);
    return status;
}

void callback(void)
{
//#zxy_control *control = &__control;
    printf("max time out");
    //#control->change_status(control,HOST_SEND_COMMAND,0,1);
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
    printf("Emoteflash stm32 flash progrmming version 1.0.0, COPYRIGHT DNC 2016.\n");
    /*Init serial port*/
    if(SerialInt(device)<0)
    {
        printf("serial port init failed...\n");
        return 0;
    }

    char *str = "AesAtasdFlfDjkalB";
    framer->send(framer,(uint8_t *)str,strlen(str));
    while(1)
    {
        switch(control->status)
        case HOST_SEND_COMMAND:
    {
            //printf("App2Boot request\n");
            key = 0x4c; /*send to application to restart*/
        if(0)
        {
            printf("send byte %c faild.\n", key);
            return -1;
        }
        else
        {
            //debug_print("sending restart command...\n ");
        }

        control->change_status(control,HOST_WAIT_ACK,HOST_CMD_TIMEOUT);

        break;

        case HOST_WAIT_ACK:

            //if(SerialGetChar(&c))
        {
            if(0)
            {
                //printf("start flash\n");
                control->change_status(control,HOST_TX_APP,100);
                break;

            }
            //printf("ACK not received\n");
        }



        if(control->is_max_time_out(control,300))
        {
            callback();
            return 0;
        }

        if(control->check_timer(control,callback))
        {
            //printf("App2Boot timeout\n");
            control->change_status(control,HOST_SEND_COMMAND,0);
            break;
        }

        break;

        case HOST_TX_APP:

            // printf("Y-modem tx start\n");
            printf("[Server] recive ack response from emote...\n");
            if(SerialUpload(imageFile)==0)
            {
                SerialPutChar(0x4D);
                if(Serial_ReceiveByte((char *)&recv, TIMEOUT) == 1 && recv == 0x52);
                {
                    sprintf(sbuf,"%d", moteid);
                    debug_print("%s \n", sbuf);
                    printf("[Server] sending mote id to emote....\n");
                    while(sbuf[i] != '\0')
                    {
                        SerialPutChar(sbuf[i++]);
                    }

                    while(Serial_ReceiveByte((char *)&recv, TIMEOUT) == 1 && recv !=0x16)
                    {
                        printf("%c", recv);
                    }
                    if( recv == 0x16)
                        printf("\nEmote image update has been done successfully.\n");
                    else
                        printf("\nEmote image update failed.Please try again.\n");
                }
            }
            SerialClose();
            return 1;


            //sleep(2);
            return 0;


            break;

        }


    }





    return 1;
}



