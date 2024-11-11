#include "wwdg.h"
#include "stdio.h"
WWDG_HandleTypeDef mhwwdg;
void wwdg_init(uint32_t pre,uint32_t wind ,uint32_t counter ,uint32_t ew){
    __HAL_RCC_WWDG_CLK_ENABLE();
    mhwwdg.Instance=WWDG;
    mhwwdg.Init.Prescaler=pre;
    mhwwdg.Init.Window=wind;
    mhwwdg.Init.Counter=counter;
    mhwwdg.Init.EWIMode=ew;
    HAL_WWDG_Init(&mhwwdg);

}
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg){
    __HAL_RCC_WWDG_CLK_ENABLE();
    if(hwwdg->Instance==WWDG && hwwdg->Init.EWIMode==WWDG_EWI_ENABLE){
        HAL_NVIC_SetPriority(WWDG_IRQn,2,3);
        HAL_NVIC_EnableIRQ(WWDG_IRQn);
    }

}
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&mhwwdg);//HAL中断处理程序
}

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg){
    
    HAL_WWDG_Refresh(hwwdg);
    WWDGCallBack();

}
__weak void WWDGCallBack(){
    
}


