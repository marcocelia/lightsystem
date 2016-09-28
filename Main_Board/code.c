/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */
#include "ee.h"
#include <string.h>
#include <stdio.h>



#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "LCD_Touch_Calibration.h"

//#include "buttons.h"
#include "data.h"
#include "comunication.h"
#include "ScreenManager.h"
#include "InterfaceManager.h"
#include "TimeManager.h"
#include "touchEvents.h"

/******************************************************************************/
/*                         Global Variables                                   */
/******************************************************************************/

extern void delay(__IO uint32_t nCount);
//extern unsigned char mystring[40];


/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}



/******************************************************************************/
/*                                  TASKs 					                  */
/******************************************************************************/
/*
 * Task for receive two samples form slave boards
 */
TASK(UwlReceive)
{
	/* static variables to control the message flow */
    static unsigned char sl1_recive=0;
    static unsigned char sl2_recive=0;

    if (uwl_packet[2] == SLAVE1_ADD) {					/* don't send a sample_request to sl2 if a sample from sl1 is not yet arrived */
    	sl1_recive = 1;
    	sample = uwl_packet[0];
    	r_light_val = uwl_packet[1];
    	if (com_send(SAMPLE_REQUEST) < 0)
    		printError(12, 3 ,"send sample sl2 error;");
    }
    if (uwl_packet[2] == SLEVE2_ADD)
    	sl2_recive = 1;

    if (sl1_recive && sl2_recive){
    	sample2 = uwl_packet[0];
    	r_light_val2 = uwl_packet[1];

    	if (com_send(ACTUATE) < 0){
    		printError(12, 3, "send actuate error;");
    	}else{
    		sl1_recive = 0;
    		sl2_recive = 0;
    	}
    }
}
/*
 * Task for control slot update
 */
TASK(Task_Update_Slot)
{
	if (com_send(SWITCH_SLOT) < 0)
		printError(12, 3, "send update slot error;");
}
/*
 * Task time manager
 */
TASK(Task_Time)
{
	tod myT;

	tick();
	get_myT(&myT);
	if (GetState()==AUTOMATIC_ACTIVE && myT.tenth==0) {
		DisplayTime(&myT);
		if ((myT.minute == 0 || myT.minute == 30) && myT.second == 0) {
			PrintSlots();
			ActivateTask(Task_Update_Slot);
		}
	}
}
/*
 * Task_Sample
 */
TASK(Task_Sample)
{
	if (com_send(SAMPLE_REQUEST) < 0)
		printError(12, 3, "send sample sl1 error;");
}

/*
 * TASK 1
 */
TASK(Task1)
{
	TouchEvtType t_evt;

	t_evt = GetTouchEvt(GetState());
	if (t_evt != EVT_NOEVT)
		dispatch(t_evt);
}

/* Inserts a delay time of nCount length */
void delay(volatile uint32_t nCount)
{
  volatile uint32_t index = 0;
  for (index = (100000 * nCount); index != 0; index--);
}
/*
 * MAIN TASK
 */
int main(void)
{
	unsigned char init_time=0;

	SystemInit();
  /*Initializes Erika related stuffs*/
	EE_system_init();

	STM_EVAL_LEDInit(LED3);

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	IOE_Config();

	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();

	/* Clear the LCD */
	LCD_Clear(White);

	/* Set the LCD Text size */
	LCD_SetFont(&Font8x12);

	/* Set text background color */
	LCD_SetBackColor(backgroundColor);

	/* Screen Calibration */
	Lcd_Touch_Auto_Calibration();

	/*Network init*/
	com_init();

	/* Initialize States*/
	init_modes();

	/* Initialize Manual Settings */
	manual_init();

	/* Initialize Automatic Settings */
	automatic_init();

	/* Screen init */
	initial_screen_init();

	/*Setting initial day and time*/
	while (GetState() == INIT){
		dispatch(GetTouchEvt(INIT));
		delay(20);
	}

	/*main mode image*/
	SetRelAlarm(AlarmTask_Time, 10, 100);
	SetRelAlarm(AlarmTask1, 20, 110);

  /* Forever loop: background activities (if any) should go here */
	for (;;) { 
	}

}


