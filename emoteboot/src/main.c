/**
  ******************************************************************************
  * @file    emote-bootloader/src/main.c 
  * @author  Ling Wang
  * @version V1.0.0
  * @date    3/31/2015
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#define ApplicationAddress 0x08006000
typedef  void (*pFunction)(void);

/* Execute application */
void Jump_to_app()
{
	pFunction Jump_To_Application;
  uint32_t JumpAddress;
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
			{
				//SerialPutChar(0x16);
			  JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);

				/* Jump to user application */
				Jump_To_Application = (pFunction) JumpAddress;
				/* Initialize user application's Stack Pointer */
				__set_MSP(*(__IO uint32_t*) ApplicationAddress);
				Jump_To_Application();
			}
}



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
			payload = (uart_frame_t *)uart_driver->rec_buf;
			ack_frame.command = payload->type;
			ack_frame.status = ACK_FAILED;
			response.payload_len = sizeof(frame_ack_t);
			
								__set_PRIMASK(1);
			framer->input(framer,uart_driver->rec_buf,uart_driver->receiveLen-2);
								__set_PRIMASK(0);
			
			if(ack)
				ack_frame.status = ACK_SUCCESS;
			memcpy(response.buf,&ack_frame,response.payload_len);
			framer->send(framer,&response);
			framer->crcVailed = RESET;
			ack=0; 
			
		}
		
		if(flash_ok)
		{
			
			USART_DeInit(USART1);
			USART_Cmd(USART1,DISABLE);
			RCC_RTCCLKCmd(DISABLE);
			flash_ok =0;
			Jump_to_app();
			//SCB->VTOR=0x08006000;
		}
	//framer->send(framer,&response);
	}
}



