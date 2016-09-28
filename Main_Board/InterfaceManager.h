/*
 * InterfaceManager.h
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#ifndef INTERFACEMANAGER_H_
#define INTERFACEMANAGER_H_

#include "ee.h"
#include "data.h"
#include "ScreenManager.h"
#include "touchEvents.h"

void init_modes();

mymode_t GetState();

day_t GetDay();

void dispatch(TouchEvtType ev);

#endif /* INTERFACEMANAGER_H_ */
