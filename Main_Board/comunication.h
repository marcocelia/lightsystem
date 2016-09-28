#ifndef __COMUNICATION_H
#define __COMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#include "data.h"
#include "ee.h"
#include "STMPE811QTR.h"
#include "uwl_ieee802154.h"

/******************************************************************************/
/*                         Constant Definitions                               */
/******************************************************************************/

#define USE_GTS 			1 			//Put 1 if you want to use GTSs

#define UWL_PANID			0x000A
#define UWL_CHANNEL			11
#define UWL_PACKET_SIZE		110
#define UWL_BO				6
#define UWL_SO				6
#define UWL_COORD_ADDR		0x0001
#define UWL_DEV_A_ADDR		0x0002
#define UWL_DEV_B_ADDR		0x0003
#define UWL_DEV_C_ADDR		0x0004
#define UWL_DEV_D_ADDR		0x0005
#define UWL_DEV_GTS_LEN		2

#define SLAVE1_ADD 			0x0001
#define SLEVE2_ADD			0x0002

#define CHAT_PACKET_SIZE	10//prima 100
#define UWL_SEND_OFFSET 	3000
#define UWL_SEND_PERIOD		1000

typedef unsigned char uint8_t;

/* list of possible commands to be sent */
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

typedef enum slave_command_t {
						START,
						STOP,
						MAX_SCOMMAND
} slave_command_t;

/******************************************************************************/
/*                         Global Variables		                              */
/******************************************************************************/

extern uint8_t packet[CHAT_PACKET_SIZE];	/* packet for comunication */
extern uint8_t uwl_packet[UWL_PACKET_SIZE];
extern uint16_t uwl_destination_chat;
extern uint8_t retv;
extern unsigned char sample;				/* last sample received from slave_board1 */
extern unsigned char sample2;				/* last sample received from slave_board2 */
extern unsigned char r_light_val;			/* last current led_light_val received from slave_board1 */
extern unsigned char r_light_val2;			/* last current led_light_val received from slave_board2 */

/******************************************************************************/
/*                         Function definitions	                              */
/******************************************************************************/


/* build a new packet to be sent */
//void build_packet (command_t c, uint8_t *packet);

void com_init();
unsigned char com_send(command_t cmd);


#endif  /*__COMUNICATION_H */
