/*
 * touchEvents.h
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#ifndef TOUCHEVENTS_H_
#define TOUCHEVENTS_H_

#include "data.h"

typedef enum TouchEvtType{	EVT_NOEVT,
							EVT_MANUALSET,
							EVT_AUTOSET,
							EVT_ACT_MAN,
							EVT_ACT_AUTO,
							EVT_RET,
							EVT_BTNDW0,
							EVT_BTNUP0,
							EVT_BTNDW1,
							EVT_BTNUP1,
							EVT_COL0,
							EVT_COL1,
							EVT_MONDAY,
							EVT_TUESDAY,
							EVT_WEDNESDAY,
							EVT_THURSDAY,
							EVT_FRIDAY,
							EVT_SATURDAY,
							EVT_SUNDAY,
							EVT_BTNPLUS,
							EVT_BTNMINUS,
							EVT_BTNSDB,
							EVT_BTNSUB,
							EVT_BTNCF,
							EVT_ENDINIT,
							EVT_UPHRS,
							EVT_DWHRS,
							EVT_UPMIN,
							EVT_DWMIN,
							EVT_UPDAY,
							EVT_DWDAY,
							MAX_EVENT
} TouchEvtType;

TouchEvtType GetTouchEvt(unsigned char am);



#endif /* TOUCHEVENTS_H_ */
