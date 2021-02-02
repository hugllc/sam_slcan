#ifndef _SLCAN_H_
#define _SLCAN_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    Open,
    Close,
    Listen,
    Speed,
    Frame,
    Bad
} SLPacketType;

typedef struct {
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
    bool ext;
    bool rtr;
    uint16_t timestamp;
} CANFrame;

typedef struct {
    SLPacketType type;
    uint32_t data;
    char cmd;
    CANFrame frame;
} SLPacket;

bool decodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *cmd);
uint8_t encodeCANFrame(uint8_t *buf, uint8_t length, CANFrame *frame);
uint8_t encodeSLPacketReply(uint8_t *buf, uint8_t length);
void slcan_init(void);

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
    return (length > 0) && (buf != NULL) && (buf[length - 1] == '\r');
}

/**
 * Checks if this is a complete packet or not.  A complete packet ends with '\r'
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * 
 * @return True if this is a full packet false otherwise
 */
static inline bool isCANFrame(uint8_t *buf, uint8_t length)
{
    return (length > 0) && (buf != NULL) && ((buf[0] == 't') || (buf[0] == 'T') || (buf[0] == 'r') || (buf[0] == 'R'));
}

/**
 * Initializes a frame struct
 * 
 * @param frame The frame to initialize
 * 
 * @return True if this is a full packet false otherwise
 */
static inline void initCANFrame(CANFrame *frame)
{
    if (frame != NULL) {
        memset(frame, 0, sizeof(CANFrame));
    }
}


#endif // _SLCAN_H_