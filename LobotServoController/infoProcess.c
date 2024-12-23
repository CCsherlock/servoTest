#include "infoProcess.h"
#include "main.h"
#include "LobotServoController.h"
#include <string.h>
#define MANUL_TEST 0
uint8_t receiveBuffer[16];
uint8_t receiveFlag = 0;
uint16_t pos = 0;
uint8_t ID = 0;
uint16_t time = 1000;
int countOnes(uint8_t *arr, int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == 1)
		{
			count++;
		}
	}
	return count;
}
uint8_t initFlag = 0;
void ProcessMessage()
{
	if (!initFlag)
	{
		for (int i = 0; i < 14; i++) // 初始化舵机
		{
			myServo[i].ID = i;
			myServo[i].Position = 1000;
		}
		moveServosByArray(myServo,14,100);
		initFlag = 1;
		return;
	}
#if !MANUL_TEST
	if (receiveFlag)
	{
		receiveFlag = 0;
		if (receiveBuffer[0] == 0xA5 && receiveBuffer[15] == countOnes(&receiveBuffer[1], 14)) // 校验完成
		{
			for (int i = 0; i < 14; i++) // 发送函数
			{
				if(receiveBuffer[1+i])
				{
					myServo[i].Position = 500;
				}
				else
				{
					myServo[i].Position = 1000;
				}
			} 
			moveServosByArray(myServo,14,100);
		} 
		else memset(receiveBuffer,0x00,sizeof(receiveBuffer));
	}
#else
	if (receiveFlag)
	{
		receiveFlag = 0;
		if (receiveBuffer[0] == 0xA5 && receiveBuffer[15] == countOnes(&receiveBuffer[1], 14)) // 校验完成
		{
			for (int i = 0; i < 14; i++) // 初始化舵机
			{
				myServo[i].ID = i;
				myServo[i].Position = pos;
			}		
		}
		moveServosByArray(myServo,14,500);
//		moveServo(ID,pos,time);
	}
#endif
}

