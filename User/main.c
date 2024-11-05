
#include "stm32f1xx_hal.h"

void SystemClock_Config(void);
void led_init(void);
void key_init(void);
void exti_init(void);
int key_status(void);
#define LED0 GPIOB
#define LED1 GPIOE
#define LED_PIN GPIO_PIN_5
#define LED0_OFF do{HAL_GPIO_WritePin(LED0, LED_PIN, GPIO_PIN_SET);}while (0)
#define LED1_OFF do{HAL_GPIO_WritePin(LED1, LED_PIN, GPIO_PIN_SET) ;}while (0)
#define LED0_ON do{HAL_GPIO_WritePin(LED0, LED_PIN, GPIO_PIN_RESET); }while (0)
#define LED1_ON do{HAL_GPIO_WritePin(LED1, LED_PIN, GPIO_PIN_RESET);}while (0)
#define LED1_TOGGLE do{HAL_GPIO_TogglePin(LED1, LED_PIN);}while (0)
#define LED0_TOGGLE do{HAL_GPIO_TogglePin(LED0, LED_PIN);}while (0)
#define KEY1 GPIOE
#define KEY0 GPIOE
#define KEY1_PIN GPIO_PIN_3
#define KEY0_PIN GPIO_PIN_4
int main(void)
{

  HAL_Init();
  SystemClock_Config();
	led_init();
	exti_init();
  while (1)
  {
		HAL_Delay(100);
  }
}
void led_init(){
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init={0};
  gpio_init.Mode=GPIO_MODE_OUTPUT_PP;
	gpio_init.Pin=LED_PIN;
  gpio_init.Speed=GPIO_SPEED_FREQ_HIGH;
  gpio_init.Pull=GPIO_PULLDOWN;
  HAL_GPIO_Init(LED1, &gpio_init);
	HAL_GPIO_Init(LED0, &gpio_init);
	
}

void key_init()
{
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	GPIO_InitTypeDef gpio_init={0};
  gpio_init.Mode=GPIO_MODE_INPUT;
	gpio_init.Pin=KEY0_PIN;
  gpio_init.Speed=GPIO_SPEED_FREQ_HIGH;
  gpio_init.Pull=GPIO_PULLUP;
  HAL_GPIO_Init(KEY0, &gpio_init);
	gpio_init.Pin=KEY1_PIN;
	HAL_GPIO_Init(KEY1, &gpio_init);
		
}
void exti_init(void)
{
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitTypeDef init_def = {0};
  init_def.Pin=KEY0_PIN;
  init_def.Mode=GPIO_MODE_IT_FALLING;
  init_def.Speed=GPIO_SPEED_FREQ_HIGH;
  init_def.Pull=GPIO_PULLUP;
  HAL_GPIO_Init(KEY0, &init_def);
  init_def.Pin=KEY1_PIN;
  HAL_GPIO_Init(KEY1, &init_def);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_SetPriority(EXTI3_IRQn,2,1);
  HAL_NVIC_SetPriority(EXTI4_IRQn,2,2);
  
}
void EXTI3_IRQHandler()
{

	__HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN);
  HAL_Delay(20);
  if(HAL_GPIO_ReadPin(KEY1,KEY1_PIN)==GPIO_PIN_SET) return ;
  LED1_TOGGLE;
	__HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN);



}
void EXTI4_IRQHandler()
{
  
  __HAL_GPIO_EXTI_CLEAR_IT(KEY0_PIN);
  HAL_Delay(20);
  if(HAL_GPIO_ReadPin(KEY0,KEY0_PIN)==GPIO_PIN_SET) return ;
  LED0_TOGGLE;
  __HAL_GPIO_EXTI_CLEAR_IT(KEY0_PIN);
  
}

int key_status()
{
	static int pressed=0;
	
	if(!HAL_GPIO_ReadPin(KEY0,KEY0_PIN)){
		
		HAL_Delay(10);
		if(pressed){
		return 3;
		}
		if(!HAL_GPIO_ReadPin(KEY0,KEY0_PIN)){
			pressed=1;
		return 0;
		}
		
	};
	if(!HAL_GPIO_ReadPin(KEY1,KEY1_PIN)){
		HAL_Delay(10);
		if(pressed){
		return 3;
		}
		if(!HAL_GPIO_ReadPin(KEY1,KEY1_PIN)){
			pressed=1;
			return 1;
		}
	};
	pressed=0;
	return 3;
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
	//设置滴答定时器的中断优先级 只要比按键中断的优先级高就可以了
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
}

