#include <lpc214x.h>
#include "FreeRTOS.h"
#include "task.h"
#define BIT(x)          (1U << (x))

#define LCD_DATA_MASK   (0xFFU << 24)
#define LCD_RS          BIT(16)
#define LCD_RW          BIT(17)
#define LCD_EN          BIT(18)

#define LED_MASK        (0xFFU << 8)

static void lcd_task(void *pv);
static void led_task(void *pv);

static void lcd_delay_short(void)
{
    volatile unsigned int i;
    for (i = 0; i < 3000; i++) { ; }
}

static void lcd_delay_long(void)
{
  
    volatile unsigned int i;
    for (i = 0; i < 60000; i++) { ; }
}

static void lcd_pulse_enable(void)
{
    IO1SET = LCD_EN;
    lcd_delay_short();
    IO1CLR = LCD_EN;
}

static void lcd_cmd(unsigned char a)
{
    IO1CLR = LCD_DATA_MASK;
    IO1SET = ((unsigned int)a << 24);

    IO1CLR = LCD_RS;   // RS=0
    IO1CLR = LCD_RW;   // RW=0
    lcd_pulse_enable();

    if (a == 0x01 || a == 0x02) lcd_delay_long();
    else                        lcd_delay_short();
}

static void lcd_data(unsigned char b)
{
    IO1CLR = LCD_DATA_MASK;
    IO1SET = ((unsigned int)b << 24);

    IO1SET = LCD_RS;   // RS=1
    IO1CLR = LCD_RW;   // RW=0
    lcd_pulse_enable();

    lcd_delay_short();
}

static void lcd_show(const char *s)
{
    while (*s) lcd_data((unsigned char)*s++);
}

static void lcd_init(void)
{
    lcd_delay_long();   // power-on delay

    lcd_cmd(0x38); // 8-bit, 2 line
    lcd_cmd(0x0C); // display ON, cursor OFF
    lcd_cmd(0x01); // clear
    lcd_cmd(0x06); // entry mode
    lcd_cmd(0x80); // line 1 pos 0
}

int main(void)
{
    IO1DIR |= LCD_DATA_MASK | LCD_RS | LCD_RW | LCD_EN;
    IO0DIR |= LED_MASK;

    IO0CLR = LED_MASK;

    lcd_init();

    xTaskCreate(lcd_task, "LCD", 256, NULL, 2, NULL);
    xTaskCreate(led_task, "LED", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) { }
}

static void lcd_task(void *pv)
{
    (void)pv;

    lcd_cmd(0x80);
    lcd_show("LLCD WORKING   "); 

    while (1)
    {
        lcd_cmd(0x18);                    
        vTaskDelay(pdMS_TO_TICKS(800));   
    }
}

static void led_task(void *pv)
{
    (void)pv;

    while (1)
    {
        IO0SET = LED_MASK;
        vTaskDelay(pdMS_TO_TICKS(500));

        IO0CLR = LED_MASK;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}