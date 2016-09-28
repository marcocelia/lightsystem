#ifndef __DATA_H
#define __DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#include "ScreenPositions.h"
#include "Immagini.h"
#include "stm32f4_discovery_lcd.h"
#include "comunication.h"
#include <string.h>
#include <stdio.h>

typedef enum mymode_t{ 	INIT,
						MAIN,
						MANUAL_SETTING,
						AUTOMATIC_SETTING,
						DAY_SCHEDULE,
						DAY_SCHEDULE_POPUP,
						MANUAL_ACTIVE,
						AUTOMATIC_ACTIVE,
						MAX_MODE
} mymode_t;

typedef enum day_t{ MONDAY,
					TUESDAY,
					WEDNESDAY,
					THURSDAY,
					FRIDAY,
					SATURDAY,
					SUNDAY,
					MAX_DAY
} day_t;

typedef enum color_t {RED,
					  GREEN,
					  BLUE,
					  WHITE,
					  PURPLE,
					  YELLOW,
					  L_BLUE,
					  MAX_COLOR
} color_t;

typedef struct subset_t {
	unsigned char ligth_val;
	color_t color;
} subset_t;

typedef struct time_t {
	unsigned char hour;
	unsigned char minutes;
} mytime_t;

typedef struct set_t  {
	subset_t led;
	mytime_t start_time;
	mytime_t end_time;
} set_t;

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;


/* paramentri per modalità manuale */
extern subset_t manual_settings[2];

/* parametri per modalità automatica, 7 dayweek, 4 fascie per dayweek */
extern set_t automatic_settings[7][4];

/*time and day system*/
extern day_t actual_day;
extern unsigned char actual_hour;
extern unsigned char actual_minutes;
extern unsigned char actual_seconds;
extern unsigned char actual_tenths;

/*stringa*/
extern unsigned char mystring[40];

/* inizializza struttura parametri manuali a 0 */
void manual_init();

/* inizializza struttura parametri automatici a 0 */
void automatic_init();

/* inizializza la schermata iniziale*/
void initial_screen_init();

/* add a new contiguous time slot after current_slot */
void addTimeSlot(day_t day, unsigned char current_slot);

/* remove the current time slot */
void removeTimeSlot(day_t day, unsigned char current_slot);

/* print color enum value in string */
char* color_to_string(color_t val);

/* print day enum value in string */
char* day_to_string(day_t day);

/* count the number of slot in one day */
unsigned char numberOfFullSlotForDay(day_t day);

/* return the current time slot*/
void GetCurrentSlot(day_t *day, unsigned char *slot);

#endif  /*__DATA_H */
