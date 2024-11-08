#pragma import(__use_no_semihosting) //禁用半主机模式     
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "iwdg.h"
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
#define KEY_UP GPIOA
#define KEY_UP_PIN GPIO_PIN_0
#define KEY1_PIN GPIO_PIN_3
#define KEY0_PIN GPIO_PIN_4
#define BUFFSIZE 1
#define RX_PIN GPIO_PIN_10
#define TX_PIN GPIO_PIN_9
#define RX_PORT GPIOA
#define TX_PORT GPIOA
#define MAX_REC_LEN 128 //一次最大接收的数据长度

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
 
}; 
 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
void SystemClock_Config(void);
void led_init(void);
void key_init(void);
void exti_init(void);
int key_status(void);
void uart_init(void);
UART_HandleTypeDef uart_handle;
uint8_t uart_rx_buf[BUFFSIZE];
uint8_t g_rx_buf[MAX_REC_LEN];
uint16_t flag;

int main(void)
{

  HAL_Init();
  SystemClock_Config();
	led_init();
	exti_init();
  uart_init();
  iwdg_init(8000,3);
  printf("重启！\r\n");
  iwdg_start();
  while (1)
  {
    if (flag>>15)
    {
      
      HAL_UART_Transmit(&uart_handle, g_rx_buf, flag&(0x3FFF), 1000);
      flag=0;
      while(__HAL_UART_GET_FLAG(&uart_handle, UART_FLAG_TC)==RESET);
      printf("\r\n");
    }
    HAL_Delay(10);
    
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef init_def = {0};
  init_def.Pin=KEY0_PIN;
  init_def.Mode=GPIO_MODE_IT_FALLING;
  init_def.Speed=GPIO_SPEED_FREQ_HIGH;
  init_def.Pull=GPIO_PULLUP;
  HAL_GPIO_Init(KEY0, &init_def);
  init_def.Pin=KEY1_PIN;
  HAL_GPIO_Init(KEY1, &init_def);

  init_def.Pin=KEY_UP_PIN;
  init_def.Mode=GPIO_MODE_IT_RISING;
  init_def.Pull=GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_UP, &init_def);

  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn,2,1);
  HAL_NVIC_SetPriority(EXTI4_IRQn,2,2);
  HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);
  
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
void EXTI0_IRHandler()
{


  __HAL_GPIO_EXTI_CLEAR_IT(KEY_UP_PIN);
  HAL_Delay(20);
  if(HAL_GPIO_ReadPin(KEY_UP,KEY_UP_PIN)==GPIO_PIN_RESET) return ;
  iwdg_feed();
  printf("喂狗了！！！\r\n");

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
void uart_init(void)
{
  
  UART_InitTypeDef uart_init;
  uart_init.BaudRate = 115200;
  uart_init.WordLength=UART_WORDLENGTH_8B;
  uart_init.StopBits=UART_STOPBITS_1;
  uart_init.Parity=UART_PARITY_NONE;
  uart_init.HwFlowCtl=UART_HWCONTROL_NONE;
  uart_init.Mode=UART_MODE_TX_RX;
  uart_init.OverSampling=UART_OVERSAMPLING_16;
  uart_handle.Instance = USART1;
  uart_handle.Init = uart_init;
  HAL_UART_Init(&uart_handle);
  HAL_UART_Receive_IT(&uart_handle,uart_rx_buf,BUFFSIZE);
  
}
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance ==USART1){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
  //设置接收RX引脚
  GPIO_InitTypeDef uart_gpio;
  uart_gpio.Mode=GPIO_MODE_AF_INPUT;
  uart_gpio.Pin=RX_PIN;
  uart_gpio.Pull=GPIO_NOPULL;
  uart_gpio.Speed=GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RX_PORT,&uart_gpio);

  //设置发送TX引脚
  uart_gpio.Mode=GPIO_MODE_AF_PP;
  uart_gpio.Pin=TX_PIN;
  uart_gpio.Speed=GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TX_PORT,&uart_gpio);

  //使能中断
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  HAL_NVIC_SetPriority(USART1_IRQn,2,0);
	}
}
void USART1_IRQHandler(){

  //调用公共的中断函数
  HAL_UART_IRQHandler(&uart_handle);

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance==USART1){
   
    if (flag&0x4000)
    {
      if(uart_rx_buf[0]=='\n'){
      flag|=(1<<15);
      }else{
        flag=0;
      }
    }else{
      if (uart_rx_buf[0]=='\r')
      {
        flag|=(1<<14);
      }else{
        g_rx_buf[flag&(0x3FFF)]=uart_rx_buf[0];
        flag++;
        if(flag>(MAX_REC_LEN-1)){
          flag=0;
        }
      }
    }
    
    //重新开启中断
    HAL_UART_Receive_IT(&uart_handle,uart_rx_buf,BUFFSIZE);

  }
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

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
  //二选一,功能一样
  HAL_UART_Transmit (&uart_handle ,(uint8_t *)&ch,1,HAL_MAX_DELAY );
	return ch;
    
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    USART1->DR = (uint8_t) ch;      
//	return ch;
}
