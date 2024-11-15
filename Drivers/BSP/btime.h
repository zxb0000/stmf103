#ifndef __B_TIME_H
#define __B_TIME_H
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
__weak void BPeriodElapsedCallback(void);
void btime_init(uint32_t prescaler,uint32_t timeOut,uint32_t Tclk);


#endif
