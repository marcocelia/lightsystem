#ifndef __PWM_H
#define __PWM_H

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

typedef enum color_t {RED,
					  GREEN,
					  BLUE,
					  WHITE,
					  PURPLE,
					  YELLOW,
					  L_BLUE,
					  MAX_COLOR
} color_t;

extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;
extern uint16_t PrescalerValue;

/* initialize and configure PWM signal */
void PWM_Config_and_En(void);
/* reset all PWM signal */
void PWM_reset();
/* set the selected color to lightval intensity */
void PWM_set(unsigned char color, unsigned char lightval);


#endif  /*__PWM_H */
