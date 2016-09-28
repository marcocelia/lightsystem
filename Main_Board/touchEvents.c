/*
 * touchEvents.c
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#include "touchEvents.h"

TouchEvtType GetTouchEvt(mymode_t appmode)
{
	TouchEvtType evt = EVT_NOEVT;
	unsigned int x,y;
	if (GetTouch_SC_async (&x, &y)){
		switch(appmode) {
		case INIT:
			if ((x > 30 && x < 70) && (y > 55 && y < 89)) return EVT_UPHRS;				/* up hours */
			if ((x > 30 && x < 70) && (y > 144 && y < 178)) return EVT_DWHRS;			/* down hours */
			if ((x > 119 && x < 159) && (y > 55 && y < 89)) return EVT_UPMIN;			/* up minutes */
			if ((x > 119 && x < 159) && (y > 144 && y < 178)) return EVT_DWMIN;			/* down minutes */
			if ((x > 218 && x < 258) && (y > 55 && y < 89)) return EVT_UPDAY;			/* up day */
			if ((x > 218 && x < 258) && (y > 144 && y < 178)) return EVT_DWDAY;			/* down day */
			if ((x > 143 && x < 183) && (y > 198 && y < 238)) return EVT_ENDINIT;		/* confirm button */
			break;
		case MAIN:
			if ((x > 100 && x < 212) && (y > 27 && y < 70)) return EVT_MANUALSET;
			if ((x > 100 && x < 212) && (y > 127 && y < 170)) return EVT_AUTOSET;
			if ((x > 58 && x < 135) && (y > 189 && y < 229)) return EVT_ACT_MAN;
			if ((x > 196 && x < 242) && (y > 181 && y < 260)) return EVT_ACT_AUTO;
			break;
		case MANUAL_SETTING:
			if ((x > 142 && x < 176) && (y > 197 && y < 231)) return EVT_RET;
			if ((x > 10 && x < 44) && (y > 43 && y < 75)) return EVT_BTNDW0;
			if ((x > 228 && x < 261) && (y > 43 && y <75)) return EVT_BTNUP0;
			if ((x > 10 && x < 44) && (y > 113 && y < 147)) return EVT_BTNDW1;
			if ((x > 228 && x < 261) && (y > 113 && y < 147)) return EVT_BTNUP1;
			if ((x > 281 && x < 315) && (y > 43 && y < 75)) return EVT_COL0;
			if ((x > 281 && x < 315) && (y > 113 && y < 147)) return EVT_COL1;
			break;
		case AUTOMATIC_SETTING:
			if ((x > 142 && x < 176) && (y > 197 && y < 231)) return EVT_RET;
			if ((x > 17 && x < 150) && (y > 10 && y < 36)) return EVT_MONDAY;
			if ((x > 117 && x < 312) && (y > 10 && y < 36)) return EVT_TUESDAY;
			if ((x > 17 && x < 150) && (y > 54 && y < 83)) return EVT_WEDNESDAY;
			if ((x > 117 && x < 312) && (y > 54 && y < 83)) return EVT_THURSDAY;
			if ((x > 17 && x < 150) && (y > 97 && y < 128)) return EVT_FRIDAY;
			if ((x > 117 && x < 312) && (y > 97 && y < 128)) return EVT_SATURDAY;
			if ((x > 83 && x < 230) && (y > 140 && y < 173)) return EVT_SUNDAY;
			break;
		case MANUAL_ACTIVE:
			if ((x > 142 && x < 176) && (y > 197 && y < 231)) return EVT_RET;
			break;
		case AUTOMATIC_ACTIVE:
			if ((x > 142 && x < 176) && (y > 197 && y < 231)) return EVT_RET;
			break;
		case DAY_SCHEDULE:
			if ((x > 142 && x < 176) && (y > 197 && y < 231)) return EVT_RET;
			if ((x > 274 && x < 307) && (y > 65 && y < 99)) return EVT_BTNPLUS;
			if ((x > 14 && x < 37) && (y > 65 && y < 99)) return EVT_BTNMINUS;
			if ((x > 221 && x < 254) && (y > 136 && y < 169)) return EVT_BTNUP0;
			if ((x > 14 && x < 37) && (y > 136 && y < 169)) return EVT_BTNDW0;
			if ((x > 274 && x < 307) && (y > 136 && y < 169)) return EVT_COL0;
			if ((x > 274 && x < 307) && (y > 198 && y < 232)) return EVT_BTNSDB;
			if ((x > 15 && x < 47) && (y > 198 && y < 232)) return EVT_BTNSUB;
			break;
		case DAY_SCHEDULE_POPUP:
			if ((x > 46 && x < 80) && (y > 92 && y < 127)) return EVT_BTNMINUS;
			if ((x > 243 && x < 277) && (y > 92 && y < 127)) return EVT_BTNPLUS;
			if ((x > 145 && x < 179) && (y > 141 && y < 175)) return EVT_BTNCF;
			break;

		}
	}
	return evt;
}
