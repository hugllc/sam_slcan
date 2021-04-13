#ifndef _SLCAN_DECODE_H_
#define _SLCAN_DECODE_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

bool decodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *pkt);
bool decodeSLCANFrame(uint8_t *buf, uint8_t length, SLCANFrame *frame);

#ifdef __cplusplus
}
#endif

#endif // _SLCAN__DECODE_H_