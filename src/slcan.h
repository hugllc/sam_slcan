#ifndef _SLCAN_H_
#define _SLCAN_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan_defines.h"

void slcan_init(void);
uint16_t slcan_read_tx_buf(uint8_t *buf, uint16_t length);
bool slcan_packet_rx(SLPacket *pkt);
bool slcan_frame_rx(SLCANFrame *frame);
void slcan_add_rx_byte(uint8_t byte);
bool slcan_send(SLPacket *pkt);
bool slcan_send_frame(uint32_t id, uint8_t length, uint8_t *data, bool ext);
bool slcan_read_tx_byte(uint8_t *byte);
bool slcan_tx_has_byte(void);


#endif // _SLCAN_H_