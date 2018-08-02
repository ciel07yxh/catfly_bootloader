#include "mainInclude.h"
#include "debugSerial.h"
#include "sys.h"
int main(void)
{
	NVIC_Group_Init();//系统默认中断分组
	Debug_Serial_Init(115200);
	Delay_Init();
	while(1)
	{
		if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR) == 0x5555)
		{
			//直接跳转到APP
			iap_jump_app_s();
		}
		CommandScan();//命令处理
	}
}




