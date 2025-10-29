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
	
	
	
	Serial_Init();
	
	
	
	
	while(1)
	{
		OLED_ShowString(0,0,"Hello,世界",OLED_8X16);
		OLED_ShowFloatNum(0,16,12.345,2,3,OLED_8X16);
		OLED_Printf(0,32,OLED_8X16,"Num=%d",666);
		OLED_Update();
			
	}
	
}

