/*
 * screenMng.h
 *
 *  Created on: 22/gen/2015
 *      Author: paolo
 */

#ifndef SCREENMNG_H_
#define SCREENMNG_H_

#include "Immagini.h"
#include "stm32f4_discovery_lcd.h"
#include "data.h"
#include "TimeManager.h"

#define DEF_BKG 0

void drawBackground(unsigned char mode);
void display_label(unsigned char val, unsigned char colon, unsigned char line);
void initScreenMode(mymode_t mode);
void initScreenSubMode(day_t day, unsigned char current_slot);
void DisplayCSSettings(day_t day, unsigned char current_slot);
void printError(unsigned short line, unsigned char col, unsigned char* message);
void print_button_color(color_t val, uint16_t Xpos, uint16_t Ypos);
void DisplayScroll(unsigned char num_slot, unsigned char current_slot);
void DisplayTime(tod *t);
void DisplayTimeHMD(unsigned char hour, unsigned char minute, unsigned char day);
void DisplayTimeHM(unsigned char hour, unsigned char minute);

#endif /* SCREENMNG_H_ */
