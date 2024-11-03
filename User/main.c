#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
void led_init(void); /* LED 初始化函数声明 */
int main(void)
{
 HAL_Init(); /* 初始化 HAL 库 */
 sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
 delay_init(72); /* 延时初始化 */
 led_init(); /* LED 初始化 */
 while(1)
 {
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET); /* PB5 置 1 */
 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); /* PE5 置 0 */
 delay_ms(500);
 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); /* PB5 置 0 */
 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET); /* PE5 置 1 */
 delay_ms(500);
 }
}
/**
* @brief 初始化 LED 相关 IO 口, 并使能时钟
* @param 无
* @retval 无
*/
void led_init(void)
{
GPIO_InitTypeDef gpio_initstruct;
__HAL_RCC_GPIOB_CLK_ENABLE();
__HAL_RCC_GPIOE_CLK_ENABLE();
 gpio_initstruct.Pin = GPIO_PIN_5; /* LED0 引脚 */
 gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
 gpio_initstruct.Pull = GPIO_PULLUP; /* 上拉 */
 gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
 HAL_GPIO_Init(GPIOB, &gpio_initstruct); /* 初始化 LED0 引脚 */
 gpio_initstruct.Pin = GPIO_PIN_5; /* LED1 引脚 */
 HAL_GPIO_Init(GPIOE, &gpio_initstruct); /* 初始化 LED1 引脚 */
}
