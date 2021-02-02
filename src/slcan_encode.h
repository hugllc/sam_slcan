#ifndef _SLCAN_ENCODE_H_
#define _SLCAN_ENCODE_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan_defines.h"

uint16_t encodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *frame);

#endif // _SLCAN_ENCODE_H_