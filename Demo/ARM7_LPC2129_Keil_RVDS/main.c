#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "uart.h"
#include "keyboard.h"
#include "string.h"
#include "queue.h"

QueueHandle_t xUartQueue;

typedef struct sMessage
{
	char cString[32];
} tMessage;

unsigned int uiTime = 0;



void Rtos_Transmiter_SendString(void *pvParameters)
{		
	while(1)
	{
		tMessage xMessage;
		if(xQueueReceive(xUartQueue, &xMessage, portMAX_DELAY) == pdTRUE)
		{
			Transmiter_SendString(xMessage.cString);
			while (eTransmiter_GetStatus()!=FREE){};
		}
		vTaskDelay(300);
	}
}



void LettersTx (void *pvParameters){
	while(1)
	{
		tMessage xMessage = {"-ABCDEEFGH-:"};
		unsigned long uiStartTime; 
		unsigned long uiStopTime;
		
		AppendUIntToString(uiTime, xMessage.cString);
		AppendString("\n", xMessage.cString);
		
		uiStartTime = xTaskGetTickCount();
		xQueueSend(xUartQueue, xMessage.cString, portMAX_DELAY);
		uiStopTime = xTaskGetTickCount();
		
		uiTime = uiStopTime - uiStartTime;
		
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters){
	while(1){
		if(eKeyboardRead() != RELASED)
		{
			tMessage xMessage = {"-Keyboard-\n"};
			xQueueSend(xUartQueue, &xMessage, portMAX_DELAY);
		}
		vTaskDelay(300);
	}
}



int main( void ){
	UART_InitWithInt(300);
	KeyboardInit();
	
	xUartQueue = xQueueCreate(5, sizeof(tMessage));
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL );
	
	vTaskStartScheduler();
	while(1);
}

