#include "mainInclude.h"
#include "debugSerial.h"
#include "frame_phase.h"
#include "usart_driver.h"
#include "sys.h"
#include <string.h>

int main(void)
{
	zxy_framer *framer = &__framer;
	usart_driver * uart_driver = &__usart_driver;
	frame_ack_t ack_frame;
  uart_frame_t *payload;
	uart_frame_t response;
	response.type = ACK_FRAME;
	NVIC_Group_Init();//系统默认中断分组
	Debug_Serial_Init(115200);
	Delay_Init();
	 
		if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR) == 0x5555)
		{
			//直接跳转到APP
			iap_jump_app_s();
		}
	while(1)
	{

		if(framer->crcNotVailed)
		{
			framer->crcNotVailed = RESET;
			payload = (uart_frame_t *)uart_driver->rec_buf;
			ack_frame.command = payload->type;
			ack_frame.status = ACK_FAILED;
			response.payload_len = sizeof(frame_ack_t);
			memcpy(response.buf,&ack_frame,response.payload_len);
			framer->send(framer,&response);
		}
		if(framer->crcVailed)
		{
			framer->input(framer,uart_driver->rec_buf,uart_driver->receiveLen-2);
			
			payload = (uart_frame_t *)uart_driver->rec_buf;
			ack_frame.command = payload->type;
			ack_frame.status = ACK_SUCCESS;
			
			response.payload_len = sizeof(frame_ack_t);
			memcpy(response.buf,&ack_frame,response.payload_len);
			framer->send(framer,&response);
			
			framer->crcVailed = RESET;
		}
		
		//CommandScan();//命令处理
	}
}




