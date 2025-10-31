#ifndef __PWM_H
#define __PWM_H

#define M1    1
#define M2    2

void PWM_Init(void);
void PWM_SetCompare(uint8_t Mx,uint16_t Compare);
void PWM_SetPSC(uint16_t PSC);
#endif
