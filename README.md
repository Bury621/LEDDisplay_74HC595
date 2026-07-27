# LEDDisplay — 74HC595 数码管驱动

STM32 通过 GPIO 模拟时序驱动级联 74HC595 的共阳数码管模块。

## 硬件连线

两个 74HC595 级联：
- **595 #1** → 段选（a~g + dp）
- **595 #2** → 位选（共阳公共端）

| 74HC595 引脚 | 功能 | 接 STM32 |
|---|---|---|
| DS (SER) | 串行数据 | 任意 GPIO |
| SHCP (SRCLK) | 移位时钟，上升沿有效 | 任意 GPIO |
| STCP (RCLK) | 锁存/刷新，上升沿有效 | 任意 GPIO |

当前使用 PF0~PF2，见代码顶部 Set_* 函数。

## 移植

只需改 3 个 Set_* 函数的引脚和端口：

`c
static void Set_LE(uint8_t v)  { HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, v); }  // STCP
static void Set_CLK(uint8_t v) { HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, v); }  // SHCP
static void Set_Dat(uint8_t v) { HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, v); }  // DS
`

同时改 main.h / CubeMX 中对应引脚的标签（LEDDisplay_*），保持一致。

如果是共阴数码管，把 LED_0F[] 段码表每个字节取反（~）即可。

## 使用

`c
#include "LEDDisplay.h"

uint8_t buf[4] = {1, 2, 3, 4};    // 显示 1234

// 在主循环或定时中断中持续调用以刷新：
while (1) {
    LED4_Display(buf);
    HAL_Delay(1);                  // 适当延时避免闪烁
}
`

4 位数码管用 LED4_Display，8 位用 LED8_Display。参数是显示缓冲区指针，每个元素为待显示数字（0~F，查 LED_0F 表）。

## 注意事项

- 74HC595 供电 5V 时，STM32 的 3.3V GPIO 高电平处于临界（Vih ≥ 3.5V）。建议用 **74HCT595**（TTL 兼容，Vih ≥ 2.0V），或模块用 3.3V 供电。
- 主循环中需持续调用刷新，否则只有最后锁存的那一位亮。