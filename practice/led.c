#include<lpc214x.h>
#include"FreeRTOS.h"
#include"task.h"

#define led1 (1<<10)
#define led2 (1<<11)

void task1(void *pvparameters)
{
	while(1)
	{
		IO0SET=led1;
		vTaskDelay(pdMS_TO_TICKS(1000));
		
		IO0CLR=led1;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void task2(void *pvparameters)
{
	while(1)
	{
		IO0SET=led2;
		vTaskDelay(pdMS_TO_TICKS(500));
		
		IO0CLR=led2;
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int main()
{
	IODIR0|=led1|led2;
	
	xTaskCreate(task1,"led1",100,	NULL,1,NULL);
	
	xTaskCreate(task2,"led2",100,	NULL,1,NULL);
	
	vTaskStartScheduler();
	
	while(1);
}

