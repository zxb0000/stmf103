#ifndef __GTIME_H
#define __GTIME_H
#include"stm32f1xx_hal.h"
#include"stm32f1xx_hal_tim.h"
void gtime_init(uint32_t prs,uint32_t time,uint32_t Tclk);
__weak void GPeriodElapsedCallback(void);
#endif
