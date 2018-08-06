#include "frame_phase.h"
#include <stdio.h>
#include "control.h"

void dateLinkSend(struct zxy_framer *framer,uart_frame_t *frame)
{
usart_driver *usart = &__usart_driver;
uint8_t len = sizeof(uart_frame_t)-MAX_MTU_LEN+frame->payload_len;
frame->seq = framer->seq;
//printf("uart_frame_t len is %d  MAX_MTU_LEN is %d, payload_len is %d %d\r\n",sizeof(uart_frame_t),MAX_MTU_LEN,frame->payload_len,len);
usart->send(usart,(uint8_t *)frame,len);
framer->seq++;
}

void input(struct zxy_framer *phaser,uint8_t *buf,uint8_t len)
{
    zxy_control *control = &__control;
    uart_frame_t *frame = (uart_frame_t *)buf;
    frame_ack_t *ack;
    frame->payload_len = len;
    switch(frame->type)
    {
        case ACK_FRAME:
        ack=(frame_ack_t *)frame->buf;
        if(ack->status == SUCCESS)
        {
            switch(ack->command)
            {
                case APP2BOOT:
                    //length veify
                    printf("\nMCU enter Bootloader aleady\n");
                    //control->change_status(control,)
                    control->change_status(control,HOST_DOWNLOAD_BIN,HOST_PREPARE_BIN,100);
                break;

                case FILE_FRAME:
                     control->change_status(control,control->last_status,HOST_PREPARE_BIN,100);
                     printf("\nsend next file frame\n");
                break;

                case MOTEID_FRAME:
                    printf("\nmote id write success\n");
                    control->change_status(control,control->last_status,HOST_FINISHED,100);
                break;
            }

        }
        else
        {
        // command failed restrans

        }


        //ack frame callback
        break;

        case FILE_FRAME:
        //FILE_FRAME_callback
        printf("FILE");
        break;

        default:
        //unrecognise frame
        break;

    }

}

zxy_framer __framer={
0,
dateLinkSend,
input
};

