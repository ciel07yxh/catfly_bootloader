#include "frame_phase.h"

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

    uart_frame_t *frame = (uart_frame_t *)buf;

    frame->payload_len = len;
    switch(frame->type)
    {
        case ACK_FRAME:
				

        //ack frame callback
        break;

        case FILE_FRAME:
        //FILE_FRAME_callback
        break;

        default:
        //unrecognise frame
        break;

    }

}

zxy_framer __framer={
0,
0,
0,
dateLinkSend,
input
};

