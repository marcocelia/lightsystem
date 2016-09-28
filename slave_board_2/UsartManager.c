/*
 * UsartManager.c
 *
 *  Created on: 23/gen/2015
 *      Author: paolo
 */
#include "UsartManager.h"

USART_InitTypeDef USART_InitStructure;

/******************************************************************************/
/*                          Redefinitions of printf for USART                 */
/******************************************************************************/

PUTCHAR_PROTOTYPE
{

#ifdef __GNUC__
	/* Write "len" of char from "ptr" to file id "fd"
	 * Return number of char written.
	 * Need implementing with UART here. */
	EE_INT32 i;

	for (i = 0; i < len; i++) {
		/* Place your implementation of fputc here */
		/* e.g. write a character to the USART */
		USART_SendData(EVAL_COM1, (uint8_t) ptr[i]);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
	}

	return len;
#else

	USART_SendData(EVAL_COM1, (uint8_t) ch);
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);

	return ch;

#endif
}

void console_init()											    /* init USART */
{
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

