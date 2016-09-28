#include "data.h"


/*paramentri per modalità manuale*/
subset_t manual_settings [2];

/*parametri per modalità automatica, 7 dayweek, 4 fascie per dayweek*/

set_t automatic_settings[7][4];

/*time and day system*/
day_t actual_day=MONDAY;
unsigned char actual_hour=0;
unsigned char actual_minutes=0;
unsigned char actual_seconds=0;
unsigned char actual_tenths=0;

/*inizializza struttura parametri manuali a 0*/
void manual_init(){
	manual_settings[0].ligth_val = 0;
	manual_settings[0].color = RED;
	manual_settings[1].ligth_val = 0;
	manual_settings[1].color = RED;
}

/*inizializza struttura parametri automatici a 0*/
void automatic_init(){
	int i,j;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 4; j++){
			automatic_settings[i][j].led.ligth_val = 0;
			automatic_settings[i][j].led.color = RED;
			automatic_settings[i][j].start_time.hour = 0;
			automatic_settings[i][j].start_time.minutes = 0;
			automatic_settings[i][j].end_time.hour = (j == 0)? 23 : 0;			/* by default there must be at least one slot for each day */
			automatic_settings[i][j].end_time.minutes = (j == 0)? 59 : 0;		/* by default there must be at least one slot for each day */
		}
}

void initial_screen_init(){
	/* Clear the LCD */
	LCD_Clear(backgroundColor);
	LCD_SetTextColor(White);
	LCD_SetFont(&Font8x12);
	LCD_DisplayStringLine(LINE(1), "Insert hour, minutes and day", 4);
	LCD_SetBackColor(Gray);
	/* Print button*/
	LCD_DrawPicture(30, 51, 40, 40, (uint8_t*)SUBBuffer); /*up hour*/
	LCD_DrawPicture(30, 140, 40, 40, (uint8_t*)SDBBuffer); /*down hour*/
	LCD_DrawPicture(119, 51, 40, 40, (uint8_t*)SUBBuffer); /*up minute*/
	LCD_DrawPicture(119, 140, 40, 40, (uint8_t*)SDBBuffer); /*down minute*/
	LCD_DrawPicture(218, 51, 40, 40, (uint8_t*)SUBBuffer); /*up day*/
	LCD_DrawPicture(218, 140, 40, 40, (uint8_t*)SDBBuffer); /*down day*/
	LCD_DrawPicture(143, 198, 40, 40, (uint8_t*)CBBuffer);
	/*print initial value*/
	LCD_SetFont(&Font16x24);
	LCD_ClearLine(LINE(4));
	sprintf((char*)mystring, " %02d | %02d |%s", actual_hour, actual_minutes, day_to_string(actual_day));
	LCD_DisplayStringLine(LINE(4), mystring, 1);
}

/* utility function used for coping two time slot */
void copy_slot(set_t* src, set_t* dst){
	dst->led.ligth_val = src->led.ligth_val;
	dst->led.color = src->led.color;
	dst->start_time.hour = src->start_time.hour;
	dst->start_time.minutes = src->start_time.minutes;
	dst->end_time.hour = src->end_time.hour;
	dst->end_time.minutes = src->end_time.minutes;
}

/* add a new contiguous time slot after current_slot */
void addTimeSlot(day_t day, unsigned char current_slot){
	unsigned char i = numberOfFullSlotForDay(day);
	unsigned char med_hour;
	unsigned char aux = (automatic_settings[day][current_slot].end_time.hour == automatic_settings[day][current_slot].start_time.hour);

	for (; i > current_slot+1; i--)
		copy_slot(&(automatic_settings[day][i-1]), &(automatic_settings[day][i]));
	/* i == position of the new inserted slot */
	automatic_settings[day][i].led.ligth_val = 0;													/* default light value */
	automatic_settings[day][i].led.color = RED;														/* default color value */
	med_hour = (automatic_settings[day][current_slot].end_time.hour + automatic_settings[day][current_slot].start_time.hour)/2;
	automatic_settings[day][i].start_time.hour = med_hour;
	automatic_settings[day][i].start_time.minutes = (aux)? 30 : 0;
	automatic_settings[day][i].end_time.hour = automatic_settings[day][current_slot].end_time.hour;
	automatic_settings[day][i].end_time.minutes = automatic_settings[day][current_slot].end_time.minutes;
	automatic_settings[day][current_slot].end_time.hour = (aux)? automatic_settings[day][i].start_time.hour : automatic_settings[day][i].start_time.hour-1;
	automatic_settings[day][current_slot].end_time.minutes =(aux)? 29 : 59;
}

/* remove the current time slot merging with the previous one */
void removeTimeSlot(day_t day, unsigned char current_slot){
	unsigned char i, n = numberOfFullSlotForDay(day);

	automatic_settings[day][current_slot-1].end_time.hour = automatic_settings[day][current_slot].end_time.hour;
	automatic_settings[day][current_slot-1].end_time.minutes = automatic_settings[day][current_slot].end_time.minutes;
	for (i = current_slot; i < n-1; i++)
		copy_slot(&automatic_settings[day][i+1], &automatic_settings[day][i]);
	/* i == slot to be restored */
	automatic_settings[day][i].led.ligth_val = 0;
	automatic_settings[day][i].led.color = RED;
	automatic_settings[day][i].start_time.hour = 0;
	automatic_settings[day][i].start_time.minutes = 0;
	automatic_settings[day][i].end_time.hour = 0;
	automatic_settings[day][i].end_time.minutes = 0;
}

/*print color enum value in string*/
char* color_to_string(color_t val){
	switch (val) {
		case RED:
			return "RED";
		case GREEN:
			return "GREEN";
		case BLUE:
			return "BLUE";
		case WHITE:
			return "WHITE";
		case PURPLE:
			return "PURPLE";
		case YELLOW:
			return "YELLOW";
		case L_BLUE:
			return "LIGHT BLUE";
	}
}

/*print day enum value in string*/
char* day_to_string(day_t day){
	switch (day) {
		case MONDAY:
			return "MONDAY";
		case TUESDAY:
			return "TUESDAY";
		case WEDNESDAY:
			return "WEDNESDAY";
		case THURSDAY:
			return "THURSDAY";
		case FRIDAY:
			return "FRIDAY";
		case SATURDAY:
			return "SATURDAY";
		case SUNDAY:
			return "SUNDAY";
	}
}

/*count the number of slot in one day*/
unsigned char numberOfFullSlotForDay(day_t day){
	unsigned char i;

	for (i = 0; i < 4; i++)
		if ((automatic_settings[day][i].end_time.hour == 23) && (automatic_settings[day][i].end_time.minutes == 59))
			return (i+1);
}

/* return the current time slot*/
void GetCurrentSlot(day_t *day, unsigned char *slot){
	GetResource(CONS_MUTEX);
	*day=actual_day;
	unsigned char hour=actual_hour;
	unsigned char minute=actual_minutes;
	ReleaseResource(CONS_MUTEX);
	unsigned char n=numberOfFullSlotForDay(*day);
	for ((*slot)=0; (*slot)<n; (*slot)++)
		if (hour>=automatic_settings[*day][*slot].start_time.hour      &&
			hour<=automatic_settings[*day][*slot].end_time.hour        &&
			minute>=automatic_settings[*day][*slot].start_time.minutes &&
			minute<=automatic_settings[*day][*slot].end_time.minutes)
				return;
}



