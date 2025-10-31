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
#include "Motor.h"
#include "IC.h"
#include "EI.h"
#include "Serial.h"
#include <string.h>


float Target = 0,Actual = 0,Out = 0;
float Kp = 0.5,Ki = 0.04,Kd = -0.15;
float Error0 = 0,Error1 = 0,Error2 = 0;
uint8_t Actual_mode = 0,Target_mode = 0;
uint8_t mode =0;
int main ()
{	
	OLED_Init();
	LED_Init();
	Timer_Init();
	Key_Init();
	Serial_Init();
	EI_Init();
	Motor_Init();
	
	
	while(1)
	{	
		if(mode == 0){
		
			if(Target_mode == 1){
				Target = 0;
				Actual = 0;
				Target_mode = 0;
			}
			if(Serial_RxFlag == 1){
				Target = 0;
				if(Serial_RxPacket[0] == '-'){
					for(int i = 1;Serial_RxPacket[i] != '%';i++){
						Target = Target * 10 + (Serial_RxPacket[i] - '0');
					}
					if(Target >= 100){Target = 100;}
					Target = -Target;
				} else{  
					for(int i = 0;Serial_RxPacket[i] != '%';i++){
						Target = Target * 10 + (Serial_RxPacket[i] - '0');
					}
					if(Target >= 100){Target = 100;}
				}
				
				Serial_RxFlag = 0;
				
			}
		} else if(mode == 1){
			
			if(Target_mode == 0){
				Target = 0;
				Actual = 0;
				Target_mode = 1;
			}
			Target += (EI_GetTim3() / 3);
			
		}
		
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
		
		
		if(Target == 0 && mode == 0){
			if(Actual_mode == 1){
				Actual_mode = 0;
				Actual = 0;
			}
			Actual += (EI_GetTim3() / 3);//相当于单位为占空比
		} else if(Target != 0 && mode == 0){
			if(Actual_mode == 0){
				Actual_mode = 1;
				Actual = 0;
			}
			Actual = (EI_GetTim3() / 3);
		} else if(mode == 1){
			Actual += (EI_GetTim4() / 3);
			
		}
		
		
		
		OLED_Printf(0,16,OLED_8X16,"Target=%+05.0f",Target);
		OLED_Printf(0,32,OLED_8X16,"Actual=%+05.0f",Actual);
		OLED_Printf(0,48,OLED_8X16,"Out=%+05.0f",Out);
		OLED_Update();

		
	}
	
}

void TIM2_IRQHandler(void)
{
	static uint16_t Count;
	static int16_t Speed;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		Serial_Tick();
		Key_Tick();
		Count ++;
		if(Count >= 20){
			Count = 0;
			
			Error2 = Error1;
			Error1 = Error0;
			Error0 = Target - Actual;
			if(Error0 >= -3 && Error0 <= 3 && Actual_mode == 0){
				Out = 0;
			} else {
				Out += Kp * (Error0 - Error1) + Ki * Error0 + Kd * (Error0 - 2 * Error1 + Error2);
			}
			
			
			if(Out >= 100){Out = 100;}
			if(Out <= -100){Out = -100;}
			
			if(mode == 0){
				Motor_SetSpeed(M1,Out);
			} 
			if(mode == 1){
				Motor_SetSpeed(M2,Out);
			} 
				
		
			
		}
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

