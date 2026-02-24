#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define BIT(x) (1U << (x))
#define LATCH_PIN   (1U << 7)    

static QueueHandle_t patQueue;
static inline void latch_pulse(void)
{
    IO0SET = LATCH_PIN;
    __asm { nop }
    __asm { nop }
    IO0CLR = LATCH_PIN;
}

static void SPI0_Init(void)
{
    VPBDIV = 0x01; /* PCLK = CCLK */
    PINSEL0 &= ~((3U<<8) | (3U<<12));  /* clear P0.4, P0.6 function bits */
    PINSEL0 |=  ((1U<<8) | (1U<<12));  
    IO0DIR |= LATCH_PIN;
    IO0CLR  = LATCH_PIN;
	
    S0SPCR = (1U << 5);
    S0SPCCR = 16;
}

static void SPI0_SendByte(unsigned char data)
{
    volatile unsigned char dummy;

    S0SPDR = data;
    while(!(S0SPSR & BIT(7))) { }  
    dummy = S0SPDR;               
    (void)dummy;
}

static void HC595_Write(unsigned char pat)
{
    SPI0_SendByte(pat); 
    latch_pulse();      
}

static void PatternTask(void *pv)
{
    TickType_t last = xTaskGetTickCount();
    unsigned char pat = 0x01;

    (void)pv;

    for(;;)
    {
    
        pat = (unsigned char)((pat << 1) | (pat >> 7));
        if(pat == 0x00) pat = 0x01;

        xQueueSend(patQueue, &pat, 0);

        vTaskDelayUntil(&last, pdMS_TO_TICKS(200));
    }
}
static void SpiTask(void *pv)
{
    unsigned char pat;
    (void)pv;

    for(;;)
    {
        if(xQueueReceive(patQueue, &pat, portMAX_DELAY) == pdTRUE)
        {
            HC595_Write(pat);
        }
    }
}

int main(void)
{
    SPI0_Init();

    patQueue = xQueueCreate(5, sizeof(unsigned char));

    xTaskCreate(PatternTask, "PAT", 256, NULL, 2, NULL);
    xTaskCreate(SpiTask,     "SPI", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

