#include "PWM.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t PrescalerValue = 0;

/* initialize and configure PWM signal */
void PWM_Config_and_En(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOC and GPIOB clock enable */
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/*GPIOB Configuration: TIM4 CH2 (PB6) and TIM4 CH3 (PB8) and TIM4 CH4 (PB9)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect TIM4 pins to AF2 */
	//GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM4)
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

	  /* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;

	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	  /* PWM1 Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0;//CCR1_Val;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel2 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0;//CCR2_Val;

	  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel3 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0;//CCR3_Val;

	  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  /* PWM1 Mode configuration: Channel4 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0;//CCR4_Val;

	  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	  TIM_ARRPreloadConfig(TIM4, ENABLE);

	  /* TIM3 enable counter */
	  TIM_Cmd(TIM4, ENABLE);
}

/* reset all PWM signal */
void PWM_reset(){
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
}

/* set the selected color to lightval intensity */
void PWM_set(color_t color, unsigned char lightval){
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	switch (color) {
		case RED:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case GREEN:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC3Init(TIM4, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case BLUE:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC4Init(TIM4, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case WHITE:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC3Init(TIM4, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC4Init(TIM4, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case PURPLE:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC4Init(TIM4, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case YELLOW:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC3Init(TIM4, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case L_BLUE:
			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC3Init(TIM4, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

			TIM_OCInitStructure.TIM_Pulse = (lightval*TIM_TimeBaseStructure.TIM_Period)/255;
			TIM_OC4Init(TIM4, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
	}
}
