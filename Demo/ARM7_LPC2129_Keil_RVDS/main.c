#include "FreeRTOS.h"
#include "task.h"
#include "led.h"


xTaskHandle xLedBlink = NULL;


void LedBlink( void *pvParameters )
{
	
	while(1)
	{
		Led_Toggle(0);
		vTaskDelay((1000/5)/2);
	}
}


void LedCtrl( void *pvParameters )
{
	static unsigned char ucCtr = 0;
	
	while(1)
	{
		if(ucCtr % 2 == 0)
		{
			vTaskSuspend(xLedBlink);
		}
		else
		{
			vTaskResume(xLedBlink);
		}
		
		ucCtr++;
		vTaskDelay(1000);
	}
}



int main( void )
{
	Led_Init();
	xTaskCreate(LedBlink, NULL , 100 , NULL, 2 , &xLedBlink);
	xTaskCreate(LedCtrl, NULL , 100 , NULL, 2 , NULL);
	vTaskStartScheduler();
	while(1);
}

