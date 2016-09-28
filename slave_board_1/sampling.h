#ifndef __SAMPLING_H
#define __SAMPLING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#include "ee.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>

#define ADC2_DR_ADDRESS     ((uint32_t)0x4001214C)

extern __IO uint16_t ADC2ConvertedValue;
extern __IO uint32_t ADC2ConvertedVoltage;

/**
  * @brief  ADC3 channel12 with DMA configuration
  * @param  None
  * @retval None
  */
void ADC2_CH06_DMA_Config();


#endif  /*__SAMPLING_H */
