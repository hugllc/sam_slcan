#ifndef _SLCAN_DECODE_H_
#define _SLCAN_DECODE_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan_defines.h"

bool decodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *cmd);

#endif // _SLCAN__DECODE_H_