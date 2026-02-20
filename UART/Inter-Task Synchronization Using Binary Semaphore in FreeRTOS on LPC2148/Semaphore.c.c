#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uart.h"

static xSemaphoreHandle binarysem;

static void task1(void *pv);
static void task2(void *pv);

int main(void)
{

    initserial();   // UART init from your uart.c

    vSemaphoreCreateBinary(binarysem);
		xSemaphoreGive(binarysem);

    xTaskCreate(task1, "T1", 128, NULL, 1, NULL);
    xTaskCreate(task2, "T2", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

static void task1(void *pv)
{
    (void)pv;

    while(1)
    {
        xSemaphoreTake(binarysem, portMAX_DELAY);
        sendsserial("Task1 functioning\r\n");
        xSemaphoreGive(binarysem);

        vTaskDelay(pdMS_TO_TICKS(1000));   // 1 sec
    }
}

static void task2(void *pv)
{
    (void)pv;

    while(1)
    {
        xSemaphoreTake(binarysem, portMAX_DELAY);
        sendsserial("Task2 functioning\r\n");
        xSemaphoreGive(binarysem);

        vTaskDelay(pdMS_TO_TICKS(500));    // 500 ms
    }
}

