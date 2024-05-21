#include <lpc21xx.h>
#include "led.h"
void Delay(unsigned int uiMiliSec)
{

	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++) {}
}

int main( void )
{
	unsigned int time_ctr = 0;
	Led_Init();
	
	while(1)
	{
		if(time_ctr % 2 == 0) 	Led_Toggle(1);
		if(time_ctr % 5 == 0)	Led_Toggle(0);
		
		time_ctr++;
		Delay(50);
	}
}
