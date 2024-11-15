#include"gtime.h"
TIM_HandleTypeDef htim1;
void gtime_init(uint32_t prs,uint32_t time,uint32_t Tclk)
{
    __HAL_RCC_TIM2_CLK_ENABLE();
    htim1.Instance=TIM2;
    htim1.Init.Prescaler=prs-1;
    htim1.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    htim1.Init.Period=time*Tclk/(prs)-1;
    HAL_TIM_Base_Init(&htim1);
    
    HAL_NVIC_SetPriority(TIM2_IRQn,2,0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    HAL_TIM_Base_Start_IT(&htim1);
}

void TIM2_IRQHandler(){
    if(__HAL_TIM_GET_FLAG(&htim1,TIM_FLAG_UPDATE)){
        GPeriodElapsedCallback();
        __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_UPDATE);
    }
}
__weak void GPeriodElapsedCallback(){

}
