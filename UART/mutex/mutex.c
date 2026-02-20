#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ---------- UART0 (simple polling driver) ---------- */
static void UART0_Init_9600(void);
static void UART0_TxChar(char c);
static void UART0_TxString(const char *s);

/* ---------- Tasks ---------- */
static void Task1(void *pv);
static void Task2(void *pv);

/* Mutex handle */
static SemaphoreHandle_t xUartMutex = NULL;

/* Optional: simple busy delay for tiny gaps (not needed usually) */
static void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<6000; j++);
}

int main(void)
{
    /* UART init */
    UART0_Init_9600();
    UART0_TxString("\r\n--- FreeRTOS Mutex Demo (LPC2148) ---\r\n");

    /* Create mutex */
    xUartMutex = xSemaphoreCreateMutex();
    if(xUartMutex == NULL)
    {
        UART0_TxString("Mutex create FAILED!\r\n");
        while(1);
    }

    /* Create tasks */
    xTaskCreate(Task1, "T1", 256, NULL, 2, NULL);
    xTaskCreate(Task2, "T2", 256, NULL, 2, NULL);

    /* Start scheduler */
    vTaskStartScheduler();

    /* If we reach here, heap was not enough */
    UART0_TxString("Scheduler start FAILED!\r\n");
    while(1);
}

/* -------------------- Task1 -------------------- */
static void Task1(void *pv)
{
    (void)pv;

    while(1)
    {
        /* Take mutex before UART prints */
        if(xSemaphoreTake(xUartMutex, portMAX_DELAY) == pdTRUE)
        {
            UART0_TxString("Task1: Hello from Task1\r\n");
            xSemaphoreGive(xUartMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); /* 1 second */
    }
}

/* -------------------- Task2 -------------------- */
static void Task2(void *pv)
{
    (void)pv;

    while(1)
    {
        if(xSemaphoreTake(xUartMutex, portMAX_DELAY) == pdTRUE)
        {
            UART0_TxString("Task2: Hello from Task2\r\n");
            xSemaphoreGive(xUartMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500)); /* 500 ms */
    }
}

/* ---------- UART0 init (PCLK=60MHz, 9600 baud) ---------- */
static void UART0_Init_9600(void)
{
    /* PCLK = CCLK */
    VPBDIV = 0x01;

    /* P0.0 TXD0, P0.1 RXD0 */
    PINSEL0 = (PINSEL0 & ~0x0000000F) | 0x00000005;

    /* 8N1, enable divisor latch */
    U0LCR = 0x83;

    /* Baud rate 9600 with PCLK=60MHz:
       Divisor = PCLK/(16*Baud) = 60,000,000/(16*9600) = 390.625 ~ 391
       Use DLL=0x87 (135), DLM=0x01 => 1*256+135 = 391  */
    U0DLM = 1;
    U0DLL = 135;

    /* Disable divisor latch */
    U0LCR = 0x03;

    /* Enable TX */
    U0TER = (1<<7);
}

static void UART0_TxChar(char c)
{
    while(!(U0LSR & (1<<5))); /* THRE */
    U0THR = c;
}

static void UART0_TxString(const char *s)
{
    while(*s)
        UART0_TxChar(*s++);
}


