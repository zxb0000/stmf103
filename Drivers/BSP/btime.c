#include"btime.h"

TIM_HandleTypeDef handle;
/**
 * @brief  初始化定时器
 * @param  prescaler:时钟预分频（between Min_Data = 0x0000 and Max_Data = 0xFFFF）
 * @param  timeOut:超时时间单位us
 * @param  Tclk :定时器的时钟源频率（单位为 Hz）
 */
void btime_init(uint32_t prescaler,uint32_t timeOut,uint32_t Tclk)
{
    handle.Instance=TIM6;
    handle.Init.Prescaler=prescaler-1;
    handle.Init.Period=timeOut*Tclk/(prescaler)-1;
    handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&handle);
    HAL_TIM_Base_Start_IT(&handle);
}
 void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM6){
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn,2,0);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
    }
}
 void TIM6_IRQHandler(){
    HAL_TIM_IRQHandler(&handle);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    BPeriodElapsedCallback();
}
__weak void BPeriodElapsedCallback(){
    
}


