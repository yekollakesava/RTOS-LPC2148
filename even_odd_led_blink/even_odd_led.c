#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"

#define LED1 (1u<<10)
#define LED2 (1u<<11)

void vNumberTask(void *pvParameters)
{
    int i;

    (void)pvParameters;

    for(i = 1; i <= 100; i++)
    {
        if(i % 2 == 0)   // Even number
        {
            IO0SET = LED1 | LED2;     // Both LEDs ON
            vTaskDelay(pdMS_TO_TICKS(300));

            IO0CLR = LED1 | LED2;     // Both LEDs OFF
        }
        else             // Odd number
        {
            IO0SET = LED1;            // Only LED1 ON
            vTaskDelay(pdMS_TO_TICKS(300));

            IO0CLR = LED1;            // LED1 OFF
        }

        vTaskDelay(pdMS_TO_TICKS(200));   // Gap between numbers
    }

    vTaskDelete(NULL);   // Delete task after finishing 1-100
}

int main(void)
{
    // Set GPIO mode
    PINSEL0 &= ~((3u<<20) | (3u<<22));

    // Set as output
    IODIR0 |= LED1 | LED2;

    xTaskCreate(vNumberTask,
                "NumberTask",
                256,
                NULL,
                1,
                NULL);

    vTaskStartScheduler();

    while(1);
}
