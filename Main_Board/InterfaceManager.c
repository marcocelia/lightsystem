/*
 * InterfaceManager.c
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */
#include "InterfaceManager.h"

unsigned char mystring[40];				/* all possible message to be printed */
unsigned char current_slot = 0;			/* current_slot = first_slot at the beginning */

mymode_t state;
day_t day_state;

void init_modes(){
	state=INIT;
	day_state=MONDAY;
	initScreenMode(INIT);
}

mymode_t GetState(){
	return state;
}

day_t GetDay(){
	return day_state;
}


void DBPInit (TouchEvtType evt, mymode_t *state){
	switch (evt) {
	case EVT_ENDINIT:							/* confirm button */
		*state = MAIN;
		initScreenMode(MAIN);
		return;
	case EVT_UPHRS:								/* up hours */
		actual_hour=(actual_hour+1)%24;
		break;
	case EVT_DWHRS:								/* down hours */
		if (actual_hour==0) actual_hour=23;
		else actual_hour--;
		break;
	case EVT_UPMIN:								/* up minutes */
		actual_minutes=(actual_minutes+1)%60;
		break;
	case EVT_DWMIN:								/* down minutes */
		if (actual_minutes==0) actual_minutes=59;
		else actual_minutes--;
		break;
	case EVT_UPDAY:								/* up day */
		actual_day=(actual_day+1)%MAX_DAY;
		break;
	case EVT_DWDAY:								/* down day */
		if (actual_day==0) actual_day=SUNDAY;
		else actual_day--;
		break;
	}
	DisplayTimeHMD(actual_hour, actual_minutes, actual_day);
}

/* Detect Button Pressed Main Mode */
void DBPMain (TouchEvtType evt, mymode_t *state){
	unsigned char retv;
	switch (evt) {
	case EVT_MANUALSET:				/* MANAL_SETTING BUTTON*/
		*state = MANUAL_SETTING;
		break;
	case EVT_AUTOSET:				/* AUTOMATIC_SETTING BUTTON*/
		*state = AUTOMATIC_SETTING;
		break;
	case EVT_ACT_MAN:
		*state = MANUAL_ACTIVE;		/* ACTIVE_MANUAL_MODE BUTTON*/
		retv = com_send(START_MANUAL);
		break;
	case EVT_ACT_AUTO:
		*state = AUTOMATIC_ACTIVE;	/* ACTIVE_AUTOMATIC_MODE BUTTON*/
		retv = com_send(START_AUTOMATIC);
		SetRelAlarm(AlarmTask_Sample, 500, 4000);
		break;
	}
	initScreenMode(*state);
	if (retv < 0) 	printError(12, 3, "send error;");
}

/* Detect Button Pressed Manual Mode Setting */
void DBPManual_Setting (TouchEvtType ev, mymode_t *state ){

	switch (ev){
		case EVT_RET:									/* return button */
			*state = MAIN;
			initScreenMode(MAIN);
			break;
		case EVT_BTNDW0:								/* down[0] button */
			if (manual_settings[0].ligth_val > 0){
				manual_settings[0].ligth_val -= 10;
				display_label(manual_settings[0].ligth_val,2,7);
			}
			break;
		case EVT_BTNUP0:								/* up[0] button */
			if (manual_settings[0].ligth_val < 100){
				manual_settings[0].ligth_val += 10;
				display_label(manual_settings[0].ligth_val,2,7);
			}
			break;
		case EVT_BTNDW1:								/* down[1] button */
			if (manual_settings[1].ligth_val > 0){
				manual_settings[1].ligth_val -= 10;
				display_label(manual_settings[1].ligth_val,5,7);
			}
			break;
		case EVT_BTNUP1:								/* up[1] button */
			if (manual_settings[1].ligth_val < 100){
				manual_settings[1].ligth_val += 10;
				display_label(manual_settings[1].ligth_val,5,7);
			}
			break;
		case EVT_COL0:									/* color [0] */
			manual_settings[0].color = (manual_settings[0].color + 1)%MAX_COLOR;
			print_button_color(manual_settings[0].color, 270, 40);
			break;
		case EVT_COL1:									/* color [1] */
			manual_settings[1].color = (manual_settings[1].color + 1)%MAX_COLOR;
			print_button_color(manual_settings[1].color, 270, 111);
			break;
	}
}

/*Detect Button Pressed Active Manual Mode*/
void DBPActive_Manual (TouchEvtType ev, mymode_t *state ){

	if (EVT_RET == ev){				/* return button */
		*state = MAIN;
		initScreenMode(MAIN);
		if (com_send(STOP_MANUAL) < 0)
			printError(12,3,"send stop_manual error;");
		return;
	}
}

/*Detect Button Pressed Automatic mymode_t Setting*/
void DBPAutomatic_Setting (TouchEvtType ev, mymode_t *state, day_t *day_state){

	switch(ev){
		case EVT_RET:							/* return button */
			*state = MAIN;
			initScreenMode(MAIN);
			break;
		case EVT_MONDAY:						/* monday button */
			*state = DAY_SCHEDULE;
			*day_state = MONDAY;
			initScreenSubMode(MONDAY, 0);
			break;
		case EVT_TUESDAY:						/* tuesday button */
			*state = DAY_SCHEDULE;
			*day_state = TUESDAY;
			initScreenSubMode(TUESDAY, 0);
			break;
		case EVT_WEDNESDAY:						/* wednesday button */
			*state = DAY_SCHEDULE;
			*day_state = WEDNESDAY;
			initScreenSubMode(WEDNESDAY, 0);
			break;
		case EVT_THURSDAY:						/* thursday button */
			*state = DAY_SCHEDULE;
			*day_state = THURSDAY;
			initScreenSubMode(THURSDAY, 0);
			break;
		case EVT_FRIDAY:						/* friday button */
			*state = DAY_SCHEDULE;
			*day_state = FRIDAY;
			initScreenSubMode(FRIDAY, 0);
			break;
		case EVT_SATURDAY:						/* saturday button */
			*state = DAY_SCHEDULE;
			*day_state = SATURDAY;
			initScreenSubMode(SATURDAY,0);
			break;
		case EVT_SUNDAY:						/* sunday button */
			*state = DAY_SCHEDULE;
			*day_state = SUNDAY;
			initScreenSubMode(SUNDAY, 0);
			break;
	}

}

/*Detect Button Pressed Active Automatic Mode*/
void DBPActive_Automatic (TouchEvtType ev, mymode_t *state ){

	if (EVT_RET == ev){			/* return button */
		*state = MAIN;
		initScreenMode(MAIN);
		CancelAlarm(AlarmTask_Sample);
		if (com_send(STOP_AUTOMATIC) < 0)
			printError(12, 3, "send stop_automatic error;");
		return;
	}
}

/*Detect Button Pressed Day Schedule Mode*/
void DBPDay_Schedule (TouchEvtType ev, mymode_t *state, day_t *day_state  ){

	unsigned char num_slot = numberOfFullSlotForDay(*day_state);
	switch (ev) {
	case EVT_RET:			/* return button*/
		*state = AUTOMATIC_SETTING;
		initScreenMode(*state);
		current_slot = 0;
		break;
	case EVT_BTNPLUS:		/* plus button */
		if (numberOfFullSlotForDay(*day_state) >= 4){					/* can not be more than 4 slot in one day */
			printError(3, 2, "there can be at most 4 slot for day");
			return;
		}
		unsigned char s_hour = automatic_settings[*day_state][current_slot].start_time.hour;
		unsigned char s_minute = automatic_settings[*day_state][current_slot].start_time.minutes;
		unsigned char e_hour = automatic_settings[*day_state][current_slot].end_time.hour;
		unsigned char e_minute = automatic_settings[*day_state][current_slot].end_time.minutes;

		if (s_hour == e_hour && (e_minute-s_minute) <= 30){				/* a slot must be at least 30 minutes */
			printError(3, 2, "duration must be at least 30 min");
			return;
		}
		*state = DAY_SCHEDULE_POPUP;
		addTimeSlot(*day_state, current_slot);
		initScreenPopUp(current_slot,day_state);
		break;
	case EVT_BTNMINUS:		/* minus button */
		if (current_slot == 0) {										/* the first slot cannot be removed */
			printError(3, 1, "first slot cannot be removed");
			return;
		}
		removeTimeSlot(*day_state, current_slot);
		current_slot--;
		initScreenSubMode(*day_state, current_slot);
		break;
	case EVT_BTNUP0:		/* up button */
		if (automatic_settings[*day_state][current_slot].led.ligth_val < 100){
			automatic_settings[*day_state][current_slot].led.ligth_val += 10;
			display_label(automatic_settings[*day_state][current_slot].led.ligth_val,6,7);
		}
		break;
	case EVT_BTNDW0:		/* down button */
		if (automatic_settings[*day_state][current_slot].led.ligth_val > 0){
			automatic_settings[*day_state][current_slot].led.ligth_val -= 10;
			display_label(automatic_settings[*day_state][current_slot].led.ligth_val,6,7);
		}
		break;
	case EVT_COL0:
		automatic_settings[*day_state][current_slot].led.color=(automatic_settings[*day_state][current_slot].led.color+1)%MAX_COLOR;
		print_button_color(automatic_settings[*day_state][current_slot].led.color, 263, 133);
		break;
	case EVT_BTNSDB:		/* scroll down button */
		if (current_slot < num_slot -1){
			current_slot++;
			DisplayScroll(num_slot,current_slot);
			DisplayCSSettings(*day_state, current_slot);
		}
		break;
	case EVT_BTNSUB:		/* scroll up button */
		if (current_slot > 0){
			current_slot--;
			DisplayScroll(num_slot, current_slot);
			DisplayCSSettings(*day_state,current_slot);
		}
		break;
	}
}

/*Detect Button Pressed Day Schedule Popup Mode*/
void DBPDay_Schedule_Popup (TouchEvtType ev, mymode_t *state, day_t *day_state  ){
	set_t *cur=&(automatic_settings[*day_state][current_slot]);
	set_t *cur_next=&(automatic_settings[*day_state][current_slot+1]);
	switch (ev) {
	case EVT_BTNMINUS: /* minus button */
		if ((cur->start_time.minutes == 0) && (cur_next->start_time.hour == cur->start_time.hour)) return;
		if ((cur->start_time.minutes == 30) && (cur_next->start_time.hour == cur->start_time.hour+1) &&
			(cur_next->start_time.minutes == 0)) return;
		cur_next->start_time.minutes = (cur_next->start_time.minutes+30) % 60;
		if (cur_next->start_time.minutes == 30){
			cur_next->start_time.hour = cur_next->start_time.hour - 1;
			cur->end_time.hour =cur_next->start_time.hour;
			cur->end_time.minutes = 29;
		} else {
			cur->end_time.hour = cur_next->start_time.hour - 1;
			cur->end_time.minutes = 59;
		}
		DisplayTimeHM(cur_next->start_time.hour,cur_next->start_time.minutes);
		break;
	case EVT_BTNPLUS: /* plus button */
		if ((cur_next->end_time.minutes == 29) && (cur_next->start_time.hour == cur_next->end_time.hour)) return;
		if ((cur_next->end_time.minutes == 59) && (cur_next->start_time.hour == cur_next->end_time.hour) &&
			(cur_next->start_time.minutes == 30)) return;

		cur_next->start_time.minutes = (cur_next->start_time.minutes + 30) % 60;
		if (cur_next->start_time.minutes == 0){
			cur_next->start_time.hour=cur_next->start_time.hour + 1;
			cur->end_time.hour =cur_next->start_time.hour-1;
			cur->end_time.minutes = 59;
		} else {
			cur->end_time.hour =cur_next->start_time.hour;
			cur->end_time.minutes = 29;
		}
		DisplayTimeHM(cur_next->start_time.hour,cur_next->start_time.minutes);
		break;
	case EVT_BTNCF:		/* confirm button */
		*state = DAY_SCHEDULE;
		initScreenSubMode(*day_state, current_slot);
		break;
	}

}

void dispatch(TouchEvtType ev)
{
	drawBackground(state);
	switch (state) {
	case INIT:
		DBPInit(ev, &state);
		break;
	case MAIN:
        DBPMain(ev,&state);
		break;
	case MANUAL_SETTING:
		DBPManual_Setting(ev,&state);
		break;
	case AUTOMATIC_SETTING:
		DBPAutomatic_Setting(ev,&state,&day_state);
		break;
	case MANUAL_ACTIVE:
		DBPActive_Manual(ev,&state);
		break;
	case AUTOMATIC_ACTIVE:
		DBPActive_Automatic(ev,&state);
		break;
	case DAY_SCHEDULE:
		DBPDay_Schedule (ev, &state, &day_state);
		break;
	case DAY_SCHEDULE_POPUP:
		DBPDay_Schedule_Popup (ev, &state, &day_state);
		break;
	}
}
