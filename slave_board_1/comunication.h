#ifndef __COMUNICATION_H
#define __COMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#include "ee.h"
#include <stdio.h>
#include "uwl_ieee802154.h"
#include <string.h>

/******************************************************************************/
/*                         Constant Definitions                               */
/******************************************************************************/

#define USE_GTS 			1 					//Put 1 if you want to use GTSs

#define UWL_PANID			0x000A
#define UWL_CHANNEL			11
#define UWL_PACKET_SIZE		10 //prima 100
#define UWL_COORD_ADDR		0x0001
#define UWL_DEVICE_ADDR		0x0002
#define UWL_SEND_OFFSET 	3000
#define UWL_SEND_PERIOD		1000

#define MYADDR				0x0001				/* SLAVE1 HAS ADDRES 1*/

/* list of possible commands to be received */
typedef enum command_t {
						START_MANUAL,
						STOP_MANUAL,
						START_AUTOMATIC,
						STOP_AUTOMATIC,
						SAMPLE_REQUEST,
						ACTUATE,
						SWITCH_SLOT,
						MAX_COMMAND
} command_t;

/******************************************************************************/
/*                         Global Variables		                              */
/******************************************************************************/

extern unsigned char rx_packet[UWL_PACKET_SIZE];
extern unsigned char tx_packet[UWL_PACKET_SIZE];


/******************************************************************************/
/*                         Function definitions	                              */
/******************************************************************************/

/* build the packet to be sent */
void buildPacket(unsigned short sample, unsigned char ligh_val);

unsigned char com_init();

#endif  /*__COMUNCATION_H */
