#include"iwdg.h"
#include"stdio.h"
IWDG_HandleTypeDef ihiwdg;
uint32_t calculate_reload(uint32_t outTime,uint32_t prer);
/**
 * @brief  IWDG模块初始化函数
 * @param  outTime: IWDG超时时间，单位为ms，
 * 
 * @param  prer: IWDG预分频值，看门狗时钟预分频值（IWDG_PR 值），范围为 0~7
 * 不同分频值下，最大超时时间不同，如下：
 * prer = 0: 1638
 * prer = 1: 3276
 * prer = 2: 6553
 * prer = 3: 13107
 * prer = 4: 26214
 * prer = 5: 52428
 * prer = 6: 104857
 * prer = 7: 209715
 */
void iwdg_init(uint32_t outTime,uint32_t prer){
    
    ihiwdg.Instance=IWDG;
    ihiwdg.Init.Prescaler=prer;
    uint32_t rlr=calculate_reload(outTime,prer);
    printf("%d \r\n",rlr);
    ihiwdg.Init.Reload=rlr;
    printf("prer=%d\r\n",prer);
    printf("rlr=%d\r\n",rlr);
    printf("outTime=%d\r\n",outTime);
    HAL_IWDG_Init(&ihiwdg);
}

void iwdg_start(void)
{
    __HAL_IWDG_START(&ihiwdg);
}

void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&ihiwdg);
}

//计算重装载值
uint32_t calculate_reload(uint32_t outTime,uint32_t prer){
    return (10 * outTime) / (1 << prer);
}
