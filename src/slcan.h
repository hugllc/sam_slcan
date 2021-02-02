#ifndef _SLCAN_H_
#define _SLCAN_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan_defines.h"

void slcan_init(void);
uint16_t slcan_read_tx_buf(uint8_t *buf, uint16_t length);
bool slcan_packet_rx(SLPacket *pkt);
void slcan_add_rx_byte(uint8_t byte);
bool slcan_send(SLPacket *pkt);


#endif // _SLCAN_H_