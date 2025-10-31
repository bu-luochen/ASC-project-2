#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSenser.h"
#include "OLED.h"
#include "CountSenser.h"
#include "Encoder.h"
#include "Timer.h"
#include "PWM.h"
#include "IC.h"
#include "EI.h"
#include "Serial.h"
#include <string.h>



int main ()
{	
	OLED_Init();
	LED_Init();
	Timer_Init();
	Key_Init();
	Serial_Init();
	uint8_t mode =0;
	
	while(1)
	{
		
		
		if ( Key_GetNum() == 1){
			mode = (mode + 1) % 2;
		}
		switch(mode){
			case 0:
				
				OLED_ShowString(0,0,"mission1",OLED_8X16);
				break;
			case 1:
				OLED_ShowString(0,0,"mission2",OLED_8X16);
				break;
			
		}
		OLED_Update();
	}
	
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		Serial_Tick();
		Key_Tick();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

