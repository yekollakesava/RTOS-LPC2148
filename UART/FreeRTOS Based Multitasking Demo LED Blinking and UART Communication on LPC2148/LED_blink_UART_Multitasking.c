#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"

#define LED (1<<10)

void led_blink(void *pvParameters)
{
    while(1)
    {
        IO0SET = LED;
        vTaskDelay(pdMS_TO_TICKS(1000));

        IO0CLR = LED;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void UART_Print(const char *str)
{
    while(*str)
    {
        while(!(U0LSR & (1<<5)));
        U0THR = *str++;
    }
}

void UART0_INIT(void)
{
    PINSEL0 = (PINSEL0 & ~0x0000000F) | 0x00000005; // P0.0 TXD0, P0.1 RXD0

    U0LCR = 0x83;   // 8-bit, DLAB=1
    U0DLL = 97;     // 9600 baud
    U0DLM = 0;
    U0LCR = 0x03;   // DLAB=0
}

void UART_TASK(void *pvParameters)
{
    while(1)
    {
        UART_Print("Running Successfully..!\r\n");
        vTaskDelay(pdMS_TO_TICKS(2000));   // 2 sec
    }
}

int main(void)
{
    IO0DIR |= LED;
    UART0_INIT();

    xTaskCreate(led_blink, "LED", 128, NULL, 1, NULL);
    xTaskCreate(UART_TASK, "UART", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

