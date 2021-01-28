#ifndef SLCAN_H
#define SLCAN_H

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
    bool ext;
    bool rtr;
    uint16_t timestamp;
} CANFrame;


bool parseDataFrame(uint8_t *buf, uint8_t length, CANFrame *frame);

/**
 * Checks if this is a complete packet or not.  A complete packet ends with '\r'
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * 
 * @return True if this is a full packet false otherwise
 */
static inline bool isPacket(uint8_t *buf, uint8_t length)
{
    return (length > 0) && (buf[length - 1] == '\r');
}


#endif // SLCAN_H