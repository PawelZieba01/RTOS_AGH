#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "uart.h"
#include "keyboard.h"
#include "string.h"

xSemaphoreHandle xSemaphore;

unsigned int uiTime = 0;

void Rtos_Transmiter_SendString (char cString[])
{		
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	Transmiter_SendString(cString);
	while (eTransmiter_GetStatus()!=FREE){};
	xSemaphoreGive(xSemaphore);	
}



void LettersTx (void *pvParameters){
	while(1)
	{
		char cTimeString[16] = "-ABCDEEFGH-:";
		unsigned long uiStartTime; 
		unsigned long uiStopTime;
		
		AppendUIntToString(uiTime, cTimeString);
		AppendString("\n", cTimeString);
		
		uiStartTime = xTaskGetTickCount();
		Rtos_Transmiter_SendString(cTimeString);
		uiStopTime = xTaskGetTickCount();
		
		uiTime = uiStopTime - uiStartTime;
		
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters){
	while(1){
		if(eKeyboardRead() != RELASED)
		{
			Rtos_Transmiter_SendString("-Keyboard-\n");
		}
		vTaskDelay(300);
	}
}



int main( void ){
	UART_InitWithInt(300);
	KeyboardInit();
	
	vSemaphoreCreateBinary( xSemaphore );
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	
	vTaskStartScheduler();
	while(1);
}

