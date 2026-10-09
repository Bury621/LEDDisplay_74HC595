#include "LEDDisplay.h"

//移植改这里就行，用来设置LEDDisplay_Dat/Clk/LE的电平,这里注释起来的是在HAL库里面的示例，根据不同平台自己改
static void Set_LE(uint8_t value)  {
    //HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, value); // PF2 -> STCP (latch)
}
static void Set_CLK(uint8_t value) {
    //HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, value); // PF1 -> SHCP (shift clock)
}
static void Set_Dat(uint8_t value) {
    //HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, value); // PF0 -> DS  (serial data)
}

//查表
static const uint8_t LED_0F[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,  // 0~7
    0x80, 0x90, 0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF,  // 8~F
    0xBF                                                // '-'
};

//LED串行输出
static void LED_OUT(uint8_t X)
{
    uint8_t i;
    for (i = 8; i >= 1; i--)
    {
        Set_Dat((X & 0x80) ? 1 : 0);
        X <<= 1;
        Set_CLK(0);
        Set_CLK(1);
    }
}
//显示n个数字
static void LED_Display_N(uint8_t *LED, uint8_t n)
{
    uint8_t i;
    for (i = 0; i < n; i++)
    {
        LED_OUT(LED_0F[LED[i]]);
        LED_OUT(1 << i);          // digit-select: bit i for i-th digit
        Set_LE(0);
        Set_LE(1);
    }
}

void LED4_Display(uint8_t *LED) { LED_Display_N(LED, 4); }
void LED8_Display(uint8_t *LED) { LED_Display_N(LED, 8); }
