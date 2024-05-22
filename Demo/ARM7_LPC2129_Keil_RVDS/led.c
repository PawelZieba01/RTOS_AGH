#include "led.h"
#include <LPC21xx.H>

#define LED0_bm (1<<16)
#define LED1_bm (1<<17)
#define LED2_bm (1<<18)
#define LED3_bm (1<<19)


void Led_Init(void)
{
	IO1DIR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	//IO1SET = LED0_bm;
}


void LedOn(unsigned char ucLedIndeks)
{
	IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	
	switch(ucLedIndeks)
	{
		case 0:
            IO1SET = LED0_bm;
            break;
		case 1:
            IO1SET = LED1_bm;
            break;
		case 2:
            IO1SET = LED2_bm;
            break;
		case 3:
            IO1SET = LED3_bm;
            break;
	}
}

void Led_Toggle(unsigned char ucLedIndeks)
{
	
	switch(ucLedIndeks)
	{
		case 0:
            if(IO1SET & LED0_bm)
						{
							IO1CLR = LED0_bm;
						}
						else
						{
							IO1SET = LED0_bm;
						}
            break;
		case 1:
            if(IO1SET & LED1_bm)
						{
							IO1CLR = LED1_bm;
						}
						else
						{
							IO1SET = LED1_bm;
						}
            break;
		case 2:
            if(IO1SET & LED2_bm)
						{
							IO1CLR = LED2_bm;
						}
						else
						{
							IO1SET = LED2_bm;
						}
            break;
		case 3:
            if(IO1SET & LED3_bm)
						{
							IO1CLR = LED3_bm;
						}
						else
						{
							IO1SET = LED3_bm;
						}
            break;
	}
}

enum StepLedDirection {LEFT, RIGHT};

void LedStep(enum StepLedDirection eDirection)
{
	static unsigned int LedModuloCounter = 0;
	
	if(eDirection == LEFT)
	{
		LedModuloCounter++;
	}
	else if(eDirection == RIGHT)
	{
		LedModuloCounter--;
	}
	
	LedOn(LedModuloCounter % 4);
}


void LedStepRight(void)
{
	LedStep(RIGHT);
}


void LedStepLeft(void)
{
	LedStep(LEFT);
}
