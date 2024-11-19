#ifndef __TIM_PWM_H
#define __TIM_PWM_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
void setCompare(uint16_t value);
void tim_pwm_init(uint32_t prs,uint32_t time,uint32_t Tclk);
#endif
