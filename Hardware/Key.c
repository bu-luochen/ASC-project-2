#include "stm32f10x.h"                  // Device header
#include "Delay.h"
uint8_t Key_Num;
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void)
{
	uint8_t temp;
	if(Key_Num){
		temp = Key_Num;
		Key_Num = 0;
		return temp;
	}
	return 0;
}

uint8_t Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){
		return 1;
	}
	return 0;
}

void Key_Tick(void){
	static int16_t Time;
	static uint8_t current;
	static uint8_t previous;
	Time ++;
	if (Time >= 20){
		Time = 0;
		
		previous = current;
		current = Key_GetState();
		if(current == 0 && previous != 0){
			Key_Num = previous;
		}
	}
	
}
