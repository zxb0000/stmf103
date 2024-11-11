#ifndef __WWDG_H
#define __WWDG_H
#include "stm32f1xx.h"
#include"stm32f1xx_hal_wwdg.h"
/**
 * @brief wwdg初始化函数
 * @param pre 预分频
 * @param wind 窗口值
 * @param counter 刷新值
 * @param ew 中断使能
 */
void wwdg_init(uint32_t pre,uint32_t wind ,uint32_t counter ,uint32_t ew);
__weak void WWDGCallBack(void);

#endif

