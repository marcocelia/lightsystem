#include "comunication.h"

unsigned char sample;
unsigned char sample2;
unsigned char r_light_val;
unsigned char r_light_val2;

/* packet for comunication */
uint8_t packet[CHAT_PACKET_SIZE];
uint8_t uwl_packet[UWL_PACKET_SIZE];
uint16_t uwl_destination_chat;
uint8_t retv;

#define encode_light(light) ((light)*255/100)
#define encode_light_ub(light, thr, base)  ((((light > thr)? base : (light+10))*255)/100)
#define encode_light_lb(light, thr, base)  ((((light < thr)? base : (light-10))*255)/100)

/* Rx callback function: Called by the network stack when data have been received */
static void rx_data(uint8_t status, uint8_t *data, uint8_t len, uint16_t addr)
{
	//sprintf((char*)mystring,"rx %02d:%02d;",actual_seconds,actual_tenths);
    //printError(9, 3, mystring);
	if (len <= UWL_PACKET_SIZE) {
		memcpy(uwl_packet, data, len);
		uwl_destination_chat = addr;
		ActivateTask(UwlReceive);
	}
}


void com_init(){
	/* Initialize the Network Stack */
	uwl_simple154_set_rx_callback(rx_data);

	retv = uwl_simple154_init_coordinator(UWL_COORD_ADDR, UWL_PANID, UWL_CHANNEL, UWL_BO, UWL_SO);

	if (retv < 0)
		printError(12, 3, "\r\nS_154 error\ninit device ");

	retv = uwl_simple154_gts_clear();
	if (retv < 0)
		printError(12, 3, "\r\nS_154 error\nclear GTS");

	retv = uwl_simple154_gts_add(UWL_DEV_A_ADDR, UWL_DEV_GTS_LEN, UWL_MAC_GTS_DIRECTION_OUT);
	if (retv < 0) {
		printf("\r\nS_154 error\nAdd GTS A");
	}

	retv = uwl_simple154_gts_add(UWL_DEV_A_ADDR, UWL_DEV_GTS_LEN, UWL_MAC_GTS_DIRECTION_IN);
	if (retv < 0) {
		printError("\r\nS_154 error\nAdd GTS A+");
	}

}

void color_light_encoding(unsigned char *pkt, unsigned char color1, unsigned char color2, unsigned char light1, unsigned char light2)
{
	pkt[1] = color1;
	pkt[2] = encode_light(light1);
	pkt[3] = color2;
	pkt[4] = encode_light(light2);
}

void build_packet (command_t c, uint8_t *packet){
	unsigned char currentslot;
	day_t day;
	static unsigned char addr = 0;				/* used to select the correct slave address */
	subset_t  *curst;

	/* all light value will be converted in a range [0 - 255] for the PWM signal */
	memset(packet, 0, CHAT_PACKET_SIZE);
	packet[0] = c;
	switch (c) {
		case START_MANUAL:
			color_light_encoding(packet, manual_settings[0].color, manual_settings[1].color,
					manual_settings[0].ligth_val, manual_settings[1].ligth_val);
			break;
		case START_AUTOMATIC:
		case SWITCH_SLOT:
			GetCurrentSlot(&day,&currentslot);
			curst = &(automatic_settings[day][currentslot].led);
			color_light_encoding(packet, curst->color, curst->color, curst->ligth_val, curst->ligth_val);
			break;
		case STOP_AUTOMATIC:
		case STOP_MANUAL:
			break;
		case SAMPLE_REQUEST:
			packet[5] = (addr==0)?SLAVE1_ADD:SLEVE2_ADD;
			addr=(addr+1)%2;
			break;
		case ACTUATE:
			GetCurrentSlot(&day,&currentslot);
			packet[1] = automatic_settings[day][currentslot].led.color;
			packet[3] = automatic_settings[day][currentslot].led.color;
			/* actuation parameter for slave board 1 */
			if (sample < automatic_settings[day][currentslot].led.ligth_val)
				packet[2] = encode_light_ub(r_light_val, 90, 100);
			if (sample >= automatic_settings[day][currentslot].led.ligth_val)
				packet[2] = encode_light_lb(r_light_val, 10, 0);
			/* actuation parameter for slave board 2 */
			if (sample2 < automatic_settings[day][currentslot].led.ligth_val)
				packet[4] = encode_light_ub(r_light_val2, 90, 100);
			if (sample2 >= automatic_settings[day][currentslot].led.ligth_val)
				packet[4] = encode_light_lb(r_light_val2, 10, 0);
			break;
	}
}

unsigned char com_send(command_t cmd){
	unsigned char retv;
	build_packet(cmd, packet);
	GetResource(SEND);
	retv = uwl_simple154_send(packet, CHAT_PACKET_SIZE, UWL_DEV_A_ADDR, USE_GTS);
	ReleaseResource(SEND);
	return retv;
}
