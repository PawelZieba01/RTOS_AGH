#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"


xSemaphoreHandle xSemaphore;

void PulseTrigger( void *pvParameters )
{
	while(1)
	{
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000);
	}
}

void Pulse_LED0( void *pvParameters )
{
	unsigned char ucCtr = 0;
	
	while(1)
	{
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		if(ucCtr % 2 == 0)
		{
			LedOn(1);
		}
		else
		{
			LedOn(0);
		}
		
		vTaskDelay(100);
		LedOn(4);   //wylaczenie leda
		
		ucCtr++;
	}
}


void Pulse_LED0_330ms( void *pvParameters )
{
	vTaskDelay(330);
	while(1)
	{
		xSemaphoreTake(xSemaphore,portMAX_DELAY);
		LedOn(0);
		vTaskDelay(330);
		LedOn(4);   //wylaczenie leda
	}
}



int main( void )
{
	Led_Init();
	vSemaphoreCreateBinary( xSemaphore );
	
	xTaskCreate(PulseTrigger, NULL , 100 , NULL, 2 , NULL);
	xTaskCreate(Pulse_LED0, NULL , 100 , NULL, 2 , NULL);
	xTaskCreate(Pulse_LED0, NULL , 100 , NULL, 2 , NULL);
	xTaskCreate(Pulse_LED0_330ms, NULL , 100 , NULL, 2 , NULL);
	
	vTaskStartScheduler();
	while(1);
}

