#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"

/* LED connected to P0.10 */
#define LED1   (1<<10)
#define LED2   (1<<11)

/* Task 1: Blink LED1 every 1 second */
void vTask1(void *pvParameters)
{
    while(1)
    {
        IO0SET = LED1;
        vTaskDelay(pdMS_TO_TICKS(1000));

        IO0CLR = LED1;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task 2: Blink LED2 every 500ms */
void vTask2(void *pvParameters)
{
    while(1)
    {
        IO0SET = LED2;
        vTaskDelay(pdMS_TO_TICKS(500));

        IO0CLR = LED2;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
    /* Configure P0.10 and P0.11 as output */
    IODIR0 |= LED1 | LED2;

    /* Create Task 1 */
    xTaskCreate(vTask1,
                "LED1",
                100,
                NULL,
                1,
                NULL);

    /* Create Task 2 */
    xTaskCreate(vTask2, 
                "LED2",
                100,
                NULL,
                1,
                NULL);

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    while(1);
}
