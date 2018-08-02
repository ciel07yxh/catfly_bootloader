#include "commandCore.h"



//命令处理模块初始化
void Command_Init(u16 ms)
{
	//暂时不知道写什么
	//如果是定时在中断中处理命令,此处就要初始化定时器,并写好中断处理程序
}


//扫描命令字符串,并调用相应处理函数
void CommandScan(void)
{
	u8 commandLength1;
	u8 commandLength2;
	u8 i = 0,j = 0;
	//数据满
	if((serial_Buffer_Length & 0x8000) == 0x8000)
	{
		//检测命令不是全为空格
		if(Command_Is_Vailed())
		{
			Command_Copy();//copy命令字符串等待处理
			//去除命令头上的空白
			Command_Remove_Space_Head();
			//去除命令尾巴上的空格
			Command_Remove_Space_End();
			//去除中间的重复空格
			Command_Remove_Space_Inner();
			commandLength1 = Command_Find_Space_Postion(1);//获取长度
			if(commandLength1 == 0)commandLength1 = commandStringLength;//当第二个空格获取返回0的时候,说明没有参数,纯命令,所以没有空格
			for(i = 0; i < COMMAND_NUM; i++)
			{
				commandLength2 = StringGetLength(commandStringList[i]);
				if(commandLength1 == commandLength2)
				{
					//长度相同,比对每个字符
					for(j = 0; j < commandLength1; j++)
					{
						if(commandStringBuffer[j] == commandStringList[i][j])continue;
						else break;
					}
					if(j == commandLength1)//比对成功
					{
						//调用函数
						Command_Proc_Func_Table[i]();
						return;
					}
				}
				else
				{
					//直接长度不同,不需要比对了
					continue;
				}
			}
			if(i == COMMAND_NUM)
			{
				//没找到对应命令
				printf("not find command\r\n");
			}
			
		}
		else
		{
			printf("command can't all space\r\n");
			serial_Buffer_Length = 0;
		}
		
	}
}










