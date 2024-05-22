#include "FreeRTOS.h"
#include "task.h"
#include "led.h"


void Delay(unsigned int uiMiliSec) 
{
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++){}
}


typedef struct sSystemData 
{
	unsigned char ucBlinkingFreq;
	unsigned char ucBlinkingLed;
}sSystemData;


void LedBlink( void *pvParameters )
{
	sSystemData * psSystemData = (sSystemData *)pvParameters;
	
	while(1)
	{
		Led_Toggle(psSystemData->ucBlinkingLed);
		vTaskDelay((1000/(psSystemData->ucBlinkingFreq))/2);
	}
}



void LedCtrl( void *pvParameters )
{
	sSystemData * psSystemData = (sSystemData*)pvParameters;
	static unsigned char ucCtr = 1;
	
	while(1)
	{
		if(ucCtr % 2 == 0)
		{
			(psSystemData->ucBlinkingLed)++;
			if(psSystemData->ucBlinkingLed == 4)
			{
				psSystemData->ucBlinkingLed = 0;
			}
		}
		
		(psSystemData->ucBlinkingFreq)++;
		
		ucCtr++;
		vTaskDelay(1000);
	}
}



int main( void )
{
	sSystemData myData = {1, 0};
	
	Led_Init();
	xTaskCreate(LedBlink, NULL , 100 , &myData, 2 , NULL );
	xTaskCreate(LedCtrl, NULL , 100 , &myData, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}

