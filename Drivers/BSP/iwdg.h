#ifndef __IWDG_H
#define __IWDG_H
#include "stm32f1xx.h"
#include"stm32f1xx_hal_iwdg.h"



/**
 * @brief  IWDG模块初始化函数
 * @param  outTime: IWDG超时时间，单位为ms，
 * 不同分频值下，最大超时时间不同，如下：
 * prer = 0: 1638
 * prer = 1: 3276
 * prer = 2: 6553
 * prer = 3: 13107
 * prer = 4: 26214
 * prer = 5: 52428
 * prer = 6: 104857
 * prer = 7: 209715
 * @param  prer: IWDG预分频值，看门狗时钟预分频值（IWDG_PR 值），范围为 0~7
 */
 void iwdg_init(uint32_t Time,uint32_t prer);
 /**
  * @brief  看门狗启动函数
  */
 void iwdg_start(void);

 
 /**
  * @brief  看门狗喂狗函数
  */
 void iwdg_feed(void);



#endif
