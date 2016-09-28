#include <string.h>
#include <stdio.h>
#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "lcd_log.h"
#include "comunication.h"
#include "PWM.h"
#include "sampling.h"
#include "UsartManager.h"


/******************************************************************************/
/*                         Global Variables		                              */
/******************************************************************************/
unsigned char last_light_val;					 /* last light_value received */


/******************************************************************************/
/*                          Function Definitions                              */
/******************************************************************************/

ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(taskCounter);
}

void leds_init(){
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	/* All LEDs initaly off*/
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);
}

void leds_blink() {							/* Used for Network errors. NOTE: leds_blink() never returns!!!*/
	volatile uint32_t i;

	do {
		STM_EVAL_LEDToggle(LED3);
		STM_EVAL_LEDToggle(LED4);
		STM_EVAL_LEDToggle(LED5);
		STM_EVAL_LEDToggle(LED6);
		i = 0;
		while (i++ < 500000);
	} while (1);
}

/******************************************************************************/
/*                                  TASKs                                     */
/******************************************************************************/
/*
 * Task for receive a new command from master board
 */
TASK(UwlReceive)
{
	static unsigned char sw=0;
	static unsigned char last_color;
    printf("\r\nUwlReceive: %d %d %d %d %d %d\r\n", rx_packet[0], rx_packet[1], rx_packet[2], rx_packet[3], rx_packet[4], rx_packet[5]);
	switch(rx_packet[0]){
		case START_MANUAL:
			PWM_reset();
			PWM_set(rx_packet[1], rx_packet[2]);
			last_light_val=rx_packet[2];
			break;
		case STOP_MANUAL:
			 PWM_reset();
			break;
		case START_AUTOMATIC:
			PWM_reset();
			PWM_set(rx_packet[1], rx_packet[2]);
			last_light_val = rx_packet[2];
			ADC2ConvertedVoltage = (ADC2ConvertedValue *5000) / 65520;
		    printf("SLAVE1: sample: %d  ; %d [V]\r\n", ADC2ConvertedValue, ADC2ConvertedVoltage);
			break;
		case STOP_AUTOMATIC:
			PWM_reset();
			break;
		case SAMPLE_REQUEST:
			if (rx_packet[5] == MYADDR){
				ADC2ConvertedVoltage = (ADC2ConvertedValue *5000) / 65520;
				printf("SLAVE1: sample: %d  ; %d [V]\r\n", ADC2ConvertedValue, ADC2ConvertedVoltage);
				buildPacket(ADC2ConvertedValue, last_light_val);
				if (uwl_simple154_send(tx_packet, UWL_PACKET_SIZE, UWL_COORD_ADDR, USE_GTS) < 0)
					printf("\r\nSLAVE1: send error \r\n");
			}else
				printf("\r\nScarto il pacchetto!!!\r\n");
			break;
		case ACTUATE:
			if (sw){
				PWM_set(last_color, rx_packet[2]);
				sw=0;
			}else{
				ADC2ConvertedVoltage = (ADC2ConvertedValue *5000) / 65520;
				printf("SLAVE1: sample: %d  ; %d [V]\r\n", ADC2ConvertedValue, ADC2ConvertedVoltage);
				PWM_set(rx_packet[1], rx_packet[2]);
				}
			last_light_val = rx_packet[2];
			break;
		case SWITCH_SLOT:
			 PWM_reset();
			 PWM_set(rx_packet[1], rx_packet[2]);
			 last_light_val = rx_packet[2];
			 last_color=rx_packet[1];
			 sw = 1;
			 break;
	}
}


/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/
int main(void)
{
	uint8_t retv;

	SystemInit();
	
	/*Initialize Erika related stuffs*/
	EE_system_init();
	
	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();
	
	/* Initialize USART */
	console_init();

	/* Initialize Leds for debugging purposes */
	leds_init();

	/* Initialize the console device, either the USART. */
	console_init();

	/* Initialize PWM signal */
	PWM_Config_and_En();

	/* Configure and Initialize DMA*/
	ADC2_CH06_DMA_Config();

	/* Start ADC2 Software Conversion */
	ADC_SoftwareStartConv(ADC2);

	if (com_init() < 0) {
		printf("\r\nSLAVE1: S_154 error\nclear GTS");
		leds_blink();
	}

	printf(" SLAVE1: Discovery board initialized!\r\n");				/* Control message */


	
	printf("\r\nSLAVE1: Device node is ready!\r\n");					/* Control message */

	/* Started */
	printf("\r\nSLAVE1: Ready to recive\r\n");

	STM_EVAL_LEDOn(LED6);


	/* Forever loop: background activities (if any) should go here */
	for (;;);

	return 0;
}
