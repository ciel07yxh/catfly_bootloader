#include "commandCore.h"



//�����ģ���ʼ��
void Command_Init(u16 ms)
{
	//��ʱ��֪��дʲô
	//����Ƕ�ʱ���ж��д�������,�˴���Ҫ��ʼ����ʱ��,��д���жϴ������
}


//ɨ�������ַ���,��������Ӧ������
void CommandScan(void)
{
	u8 commandLength1;
	u8 commandLength2;
	u8 i = 0,j = 0;
	//������
	if((serial_Buffer_Length & 0x8000) == 0x8000)
	{
		//��������ȫΪ�ո�
		if(Command_Is_Vailed())
		{
			Command_Copy();//copy�����ַ����ȴ�����
			//ȥ������ͷ�ϵĿհ�
			Command_Remove_Space_Head();
			//ȥ������β���ϵĿո�
			Command_Remove_Space_End();
			//ȥ���м���ظ��ո�
			Command_Remove_Space_Inner();
			commandLength1 = Command_Find_Space_Postion(1);//��ȡ����
			if(commandLength1 == 0)commandLength1 = commandStringLength;//���ڶ����ո��ȡ����0��ʱ��,˵��û�в���,������,����û�пո�
			for(i = 0; i < COMMAND_NUM; i++)
			{
				commandLength2 = StringGetLength(commandStringList[i]);
				if(commandLength1 == commandLength2)
				{
					//������ͬ,�ȶ�ÿ���ַ�
					for(j = 0; j < commandLength1; j++)
					{
						if(commandStringBuffer[j] == commandStringList[i][j])continue;
						else break;
					}
					if(j == commandLength1)//�ȶԳɹ�
					{
						//���ú���
						Command_Proc_Func_Table[i]();
						return;
					}
				}
				else
				{
					//ֱ�ӳ��Ȳ�ͬ,����Ҫ�ȶ���
					continue;
				}
			}
			if(i == COMMAND_NUM)
			{
				//û�ҵ���Ӧ����
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










