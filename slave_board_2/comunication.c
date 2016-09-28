#include "comunication.h"
#include <stdio.h>

//volatile uint8_t keep_sending_gts = 0;
unsigned char rx_packet[UWL_PACKET_SIZE];
unsigned char tx_packet[UWL_PACKET_SIZE];

/* Rx callback function: Called by the network stack when data have been received */
static void rx_data(int8_t status, uint8_t *data, uint8_t len, uint16_t addr)
{
	if (len <= UWL_PACKET_SIZE) {
		memcpy(rx_packet, data, len);
		printf("\r\nSLAVE2::RX_DATA: sto attivando il task!!!!\r\n");
		ActivateTask(UwlReceive);
	}
}

unsigned char com_init(){
	/* Initialize the Nework Stack */
	uwl_simple154_set_rx_callback(rx_data);

	return uwl_simple154_init_device(UWL_DEVICE_ADDR, UWL_COORD_ADDR, UWL_PANID,UWL_CHANNEL);
}

/* build the packet to be sent */
void buildPacket(unsigned short sample, unsigned char ligh_val){
	unsigned char aux=(sample*100)/65520;

	printf("\r\nSLAVE2: sending %d%% to MAIN_BOARD", aux);
	memset(tx_packet, 0, UWL_PACKET_SIZE);
	tx_packet[0] = aux;
	tx_packet[1] = (ligh_val*100)/255;
	tx_packet[2] = MYADDR;
}
