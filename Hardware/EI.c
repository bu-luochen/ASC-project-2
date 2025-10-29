#include "stm32f10x.h"                  // Device header

void EI_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//TIM_InternalClockConfig(TIM3);编码器接口会自动托管时钟
	
	//时机单元的时钟选择
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//不能用|
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;后面会用，可以删掉
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	//TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;与编码器无关
	//TIM_PWMIConfig(TIM3,&TIM_ICInitStructure);不用PWMI因为两个通道的极性相同
	//输入捕获单元
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//编码器接口配置函数
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t EI_Get(void)
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return temp;
}

