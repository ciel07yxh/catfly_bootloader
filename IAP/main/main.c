#include "mainInclude.h"

int main(void)
{
	NVIC_Group_Init();//ϵͳĬ���жϷ���
	Debug_Serial_Init(115200);
	Delay_Init();
	Command_Init(100);//100ms
	while(1)
	{
		if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR) == 0x5555)
		{
			//ֱ����ת��APP
			iap_jump_app_s();
		}
		CommandScan();//�����
	}
}




