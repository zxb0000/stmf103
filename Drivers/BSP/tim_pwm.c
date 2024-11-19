#include"tim_pwm.h"
TIM_HandleTypeDef timPWMHandle;
TIM_OC_InitTypeDef sConfigOC;
void tim_pwm_init(uint32_t prs,uint32_t arr,uint32_t Tclk){
    
    timPWMHandle.Instance=TIM3;
    timPWMHandle.Init.Prescaler=prs-1;
    timPWMHandle.Init.CounterMode=TIM_COUNTERMODE_UP;
    timPWMHandle.Init.Period=arr;
    HAL_TIM_PWM_Init(&timPWMHandle);
    
    sConfigOC.OCMode=TIM_OCMODE_PWM1;
    sConfigOC.Pulse=arr/2;
    sConfigOC.OCPolarity=TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&timPWMHandle,&sConfigOC,TIM_CHANNEL_2);
    
    HAL_TIM_PWM_Start(&timPWMHandle,TIM_CHANNEL_2);
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM3){
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
    
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_TIM3_PARTIAL();
    }  
}
void setCompare(uint16_t value){
    __HAL_TIM_SET_COMPARE(&timPWMHandle,TIM_CHANNEL_2,value);
}
