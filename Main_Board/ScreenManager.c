/*
 * screenMng.c
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */
#include "ScreenManager.h"

void drawBackground(mymode_t mode)
{
	switch (mode) {
	default:
		LCD_SetTextColor(White);
		LCD_SetBackColor(Gray);
	}
}


/* print error message and restore the previous font size */
void printError(unsigned short line, unsigned char col, unsigned char* message){
	sFONT* prev = LCD_GetFont();
	LCD_SetBackColor(backgroundColor);
	LCD_SetFont(&Font8x12);
	LCD_SetTextColor(Red);
	LCD_DisplayStringLine(LINE(line), message, col);
	LCD_SetBackColor(Gray);
	LCD_SetFont(prev);
}
/* print slot */
void PrintSlots(){
	day_t cur_day;
	unsigned char cur_slot;
	LCD_DisplayStringLine(LINE(5),"Current Time Slot:",3);
	GetCurrentSlot(&cur_day, &cur_slot);
	sprintf((char*)mystring, "S: %02d:%02d - E: %02d:%02d", automatic_settings[cur_day][cur_slot].start_time.hour,
															automatic_settings[cur_day][cur_slot].start_time.minutes,
															automatic_settings[cur_day][cur_slot].end_time.hour,
															automatic_settings[cur_day][cur_slot].end_time.minutes);
	LCD_DisplayStringLine(LINE(6),mystring,3);
	sprintf((char*)mystring, "%s I:%d%%", color_to_string(automatic_settings[cur_day][cur_slot].led.color),
									      automatic_settings[cur_day][cur_slot].led.ligth_val);
	LCD_DisplayStringLine(LINE(7),mystring,3);

	LCD_ClearLine(LINE(9));
	LCD_ClearLine(LINE(10));
	LCD_ClearLine(LINE(11));
	if (cur_slot < (numberOfFullSlotForDay(cur_day)-1)){
		LCD_DisplayStringLine(LINE(9),"Next Slot:",3);
		sprintf((char*)mystring, "S: %02d:%02d - E: %02d:%02d", automatic_settings[cur_day][cur_slot+1].start_time.hour,
							  	  	  	  	  	  	  	  	  	automatic_settings[cur_day][cur_slot+1].start_time.minutes,
							  	  	  	  	  	  	  	  	  	automatic_settings[cur_day][cur_slot+1].end_time.hour,
							  	  	  	  	  	  	  	  	  	automatic_settings[cur_day][cur_slot+1].end_time.minutes);
		LCD_DisplayStringLine(LINE(10),mystring,3);
		sprintf((char*)mystring, "%s I:%d%%", color_to_string(automatic_settings[cur_day][cur_slot+1].led.color),
											  automatic_settings[cur_day][cur_slot+1].led.ligth_val);
		LCD_DisplayStringLine(LINE(11),mystring,3);
	}
}

void display_label(unsigned char val, unsigned char line, unsigned char colon){
	unsigned char mystring[5];
	LCD_ClearLineRegion(LINE(line), colon, 4);
	sprintf((char*)mystring, "%d%%", val);
	LCD_DisplayStringLine(LINE(line), mystring, colon);
}

void initScreenMode(mymode_t mode)
{
	switch(mode) {
	case INIT:
		LCD_SetBackColor(Gray);
		break;
	case MAIN:
		LCD_DrawPicture(0, 0, 320, 240, (uint8_t*)MMBuffer);
		break;
	case MANUAL_SETTING:
		LCD_Clear(backgroundColor);
		LCD_SetBackColor(backgroundColor);
		LCD_SetFont(&Font12x12);
		LCD_DisplayStringLine(LINE(1), "Manual Setting", 7);
		LCD_DisplayStringLine(LINE(2), "Led 1", 2);
		LCD_DrawPicture(0,40,256,40,(uint8_t*)RMBuffer);
		LCD_DisplayStringLine(LINE(8), "Led 2", 2);
		LCD_DrawPicture(0,111,256,40,(uint8_t*)RMBuffer);
		LCD_DrawPicture(140,195,40,40,(uint8_t*)REBBuffer);
		LCD_SetFont(&Font16x24);
		LCD_SetBackColor(Gray);
		sprintf((char*)mystring, "%d%%", manual_settings[0].ligth_val);
		LCD_DisplayStringLine(LINE(2), mystring, 7);
		sprintf((char*)mystring, "%d%%", manual_settings[1].ligth_val);
		LCD_DisplayStringLine(LINE(5), mystring, 7);
		print_button_color(manual_settings[0].color, 270, 40);
		print_button_color(manual_settings[1].color, 270, 111);
		break;
	case AUTOMATIC_SETTING:
		LCD_DrawPicture(0, 0, 320, 240, (uint8_t*)AMSBuffer);
		break;
	case MANUAL_ACTIVE:
		LCD_Clear(backgroundColor);
		LCD_DrawPicture(140,195,40,40,(uint8_t*)REBBuffer);
		LCD_SetFont(&Font12x12);
		LCD_SetBackColor(backgroundColor);
		sprintf((char*)mystring, "LED1: %d , %s", manual_settings[0].ligth_val, color_to_string(manual_settings[0].color));
		LCD_DisplayStringLine(LINE(3), mystring, 3);
		sprintf((char*)mystring, "LED2: %d , %s", manual_settings[1].ligth_val, color_to_string(manual_settings[1].color));
		LCD_DisplayStringLine(LINE(4), mystring, 3);
		break;
	case AUTOMATIC_ACTIVE:
    	LCD_Clear(backgroundColor);
    	LCD_SetBackColor(backgroundColor);
    	LCD_DrawPicture(140,195,40,40,(uint8_t*)REBBuffer);
		LCD_SetFont(&Font12x12);
		GetResource(CONS_MUTEX);
		sprintf((char*)mystring, "%s  %02d:%02d:%02d ", day_to_string(actual_day), actual_hour, actual_minutes, actual_seconds);
		ReleaseResource(CONS_MUTEX);
		LCD_DisplayStringLine(LINE(3), mystring, 3);
		PrintSlots();
		break;

	}
}

void initScreenPopUp(unsigned char current_slot, day_t *day_state){
	LCD_SetFont(&Font8x12);
	LCD_DrawPicture(0, 0, 320, 240, (uint8_t*)DSMPUBuffer);
	LCD_SetBackColor(backgroundColor);
	LCD_DisplayStringLine(LINE(4), "Insert a value between:", 9);
	sprintf((char*)mystring, "%02d.%02d and %02d.%02d",
			automatic_settings[*day_state][current_slot].start_time.hour,
			automatic_settings[*day_state][current_slot].start_time.minutes,
			automatic_settings[*day_state][current_slot+1].end_time.hour,
			automatic_settings[*day_state][current_slot+1].end_time.minutes);
	LCD_DisplayStringLine(LINE(5), mystring, 12);
	LCD_SetBackColor(Gray);
	LCD_SetFont(&Font16x24);
	sprintf((char*)mystring, "%02d:%02d", automatic_settings[*day_state][current_slot+1].start_time.hour,
			automatic_settings[*day_state][current_slot+1].start_time.minutes);
	LCD_DisplayStringLine(LINE(4), mystring, 8);
}

/*print the button color*/
void print_button_color(color_t val, uint16_t Xpos, uint16_t Ypos){
	switch (val) {
		case RED:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)RBBuffer);
			break;
		case GREEN:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)GBBuffer);
			break;
		case BLUE:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)BBBuffer);
			break;
		case WHITE:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)WBBuffer);
			break;
		case PURPLE:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)PBBuffer);
			break;
		case YELLOW:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)YBBuffer);
			break;
		case L_BLUE:
			LCD_DrawPicture(Xpos,Ypos,40,40,(uint8_t*)LBBBuffer);
			break;
	}
}
/* display current slot settings */
void DisplayCSSettings(day_t day, unsigned char current_slot){
	char mystring[20];
	unsigned char hour = 0, minute = 0;

	LCD_SetFont(&Font12x12);
	LCD_SetBackColor(backgroundColor);
	sprintf((char*)mystring, "%s: time slot %d", day_to_string(day), current_slot+1);
	LCD_DisplayStringLine(LINE(1), mystring, 4);
	LCD_ClearLine(LINE(3));													/* clear possible error messages */
	LCD_SetBackColor(Gray);
	LCD_SetFont(&Font16x24);
	hour = automatic_settings[day][current_slot].start_time.hour;
	minute = automatic_settings[day][current_slot].start_time.minutes;
	sprintf((char*)mystring, "%02d:%02d", hour, minute);
	LCD_DisplayStringLine(LINE(3), mystring, 4);
	hour = automatic_settings[day][current_slot].end_time.hour;
	minute = automatic_settings[day][current_slot].end_time.minutes;
	sprintf((char*)mystring, "%02d:%02d", hour, minute);
	LCD_DisplayStringLine(LINE(3), mystring, 11);
	sprintf((char*)mystring, "%d%%", automatic_settings[day][current_slot].led.ligth_val);
	LCD_ClearLineRegion(LINE(6), 7, 4);
	LCD_DisplayStringLine(LINE(6), mystring, 7);
	print_button_color(automatic_settings[day][current_slot].led.color, 263, 133);
}

void DisplayScroll(unsigned char num_slot, unsigned char current_slot){
	if (current_slot == num_slot -1)
		LCD_DrawPicture(274, 198, 40, 40, (uint8_t*)QBBuffer);
	else
		LCD_DrawPicture(274, 198, 40, 40, (uint8_t*)SDBBuffer);

	if (current_slot == 0)
		LCD_DrawPicture(15, 198, 40, 40, (uint8_t*)QBBuffer);
	else
		LCD_DrawPicture(15, 198, 40, 40, (uint8_t*)SUBBuffer);

}

void DisplayTimeHM(unsigned char hour, unsigned char minute){
	LCD_SetBackColor(backgroundColor);
	sprintf((char*)mystring, "%02d:%02d", hour, minute);
	LCD_DisplayStringLine(LINE(4), mystring, 8);
}

void DisplayTimeHMD(unsigned char hour, unsigned char minute, unsigned char day)
{
	sprintf((char*)mystring, " %02d | %02d |%s", actual_hour, actual_minutes, day_to_string(day));
	LCD_ClearLine(LINE(4));
	LCD_DisplayStringLine(LINE(4), mystring, 1);
}

void DisplayTime(tod *t)
{
	LCD_SetFont(&Font12x12);
	LCD_SetBackColor(backgroundColor);
	LCD_ClearLine(LINE(3));
	sprintf((char*)mystring, "%s  %02d:%02d:%02d ", day_to_string(t->day), t->hour, t->minute, t->second);
	LCD_DisplayStringLine(LINE(3), mystring, 3);
}

void initScreenSubMode(day_t day, unsigned char current_slot){
	char mystring[20];
	unsigned short num_slot = numberOfFullSlotForDay(day);

	LCD_SetFont(&Font16x24);
	LCD_DrawPicture(0,0,320,240,(uint8_t*)DSMBuffer);
	DisplayCSSettings(day, current_slot);
	if (current_slot < num_slot -1)
		LCD_DrawPicture(274, 198, 40, 40, (uint8_t*)SDBBuffer);
	if (current_slot > 0)
		LCD_DrawPicture(15, 198, 40, 40, (uint8_t*)SUBBuffer);
}
