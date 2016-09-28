/*
 * UsartManager.h
 *
 *  Created on: 23/gen/2015
 *      Author: paolo
 */

#ifndef USARTMANAGER_H_
#define USARTMANAGER_H_

#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls _write() */
#define PUTCHAR_PROTOTYPE int _write(int fd, char *ptr, int len)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void console_init();


#endif /* USARTMANAGER_H_ */
