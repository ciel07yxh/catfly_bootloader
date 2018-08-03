#include "frame_phase.h"
#include "flash.h"
#include <string.h>
#define MOIEID_ADDR 		0x08005000
#define APP_ADDR 				0x08006000
#define APP2BOOT_ADDR		0x08004000

#define SECTOR_SIZE			2048

uint8_t file_buf[SECTOR_SIZE];
uint32_t app_addr = APP_ADDR;
uint16_t buf_index=0;
uint16_t file_seq;

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
		frame_moteid_t *moteid_frame;
		frame_file_t  *file_frame;
    //frame->payload_len = len;
    switch(frame->type)
    {
        case APP2BOOT:
					app_addr  = APP_ADDR;
					file_seq  = 0;
					buf_index = 0;
        //ack frame callback
        break;

        case FILE_FRAME:
        //FILE_FRAME_callback
				file_frame = (frame_file_t *)frame->buf;
				if(file_frame->Sequence < file_seq)
					break;
				
				if(file_frame->length<=128)
				{
					memcpy(file_buf,file_frame->buf,file_frame->length);
					buf_index+=file_frame->length;
					file_seq++;
				}
				if(buf_index == 2048)
				{
					STMFLASH_Write(app_addr,(uint16_t *)file_buf,1042);
					app_addr+=2048;
					buf_index=0;
				}
					
        break;
				
				case MOTEID_FRAME:
					
					if(buf_index !=0 )
					{
						STMFLASH_Write(app_addr,(uint16_t *)file_buf,(buf_index+1)/2);
						app_addr+=buf_index;
						buf_index=0;
					}
					
					if(sizeof(frame_moteid_t)==frame->payload_len)
					{
						moteid_frame = (frame_moteid_t *)frame->buf;
						STMFLASH_Write(MOIEID_ADDR,(u16 *)moteid_frame,1);
						STMFLASH_Write(APP2BOOT_ADDR,(u16 *)0xFFFF,1);
					}

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

