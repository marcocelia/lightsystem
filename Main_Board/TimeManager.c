/*
 * TimeManager.c
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#include "TimeManager.h"
#include "data.h"

void tick()
{
	GetResource(CONS_MUTEX);
	actual_tenths = (actual_tenths+1)%10;
	if (actual_tenths==0) {
		actual_seconds=(actual_seconds+1)%60;
	    if (actual_seconds==0) {
	    	actual_minutes=(actual_minutes+1)%60;
	        if (actual_minutes==0) {
	        	actual_hour=(actual_hour+1)%24;
	            if (actual_hour==0) {
	            	actual_day=(actual_day+1)%MAX_DAY;
	            }
	        }
	    }
	}
	ReleaseResource(CONS_MUTEX);
}

void get_myT(tod *t)
{
	GetResource(CONS_MUTEX);
	t->day=actual_day;
	t->hour=actual_hour;
	t->minute=actual_minutes;
	t->second=actual_seconds;
	t->tenth=actual_tenths;
	ReleaseResource(CONS_MUTEX);
}


