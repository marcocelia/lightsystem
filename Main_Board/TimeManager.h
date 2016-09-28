/*
 * TimeManager.h
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

#include "data.h"

typedef struct tod_
{
	day_t day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char tenth;

} tod;

#endif /* TIMEMANAGER_H_ */
