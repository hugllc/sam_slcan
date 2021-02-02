#include "slcan_defines.h"
#include "slcanbuf.h"

/**
 * Parses a single text digit and returns the value
 * 
 * @param digit The character to encode
 * 
 * @return The value of the digit encoded
 */
uint8_t encodeDigit(uint8_t digit)
{
    digit &= 0xF;
    if (digit < 10) {
        return digit + '0';
    }
    return digit + 'A' - 10;
}

/**
 * 
 * This decodes a data packet
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param frame  The frame struct to put the data into
 * 
 * @return The number of bytes encoded
 */
uint16_t encodeCANFrame(uint8_t *buf, uint8_t length, CANFrame *frame)
{
    uint8_t index = 0;
    uint8_t i;
    uint32_t id;
    if (frame->rtr) {
        if (frame->ext) {
            buf[index++] = 'R';
        } else {
            buf[index++] = 'r';
        }
    } else {
        if (frame->ext) {
            buf[index++] = 'T';
        } else {
            buf[index++] = 't';
        }
    }
    if (frame->ext) {
        id = frame->id & EXT_ID_MASK;
        for (i = 0; i < 8; i++) {
            buf[index + 7 - i] = encodeDigit(id);
            id >>= 4;
        }
        index += 8;
    } else {
        id = frame->id & ID_MASK;
        for (i = 0; i < 3; i++) {
            buf[index + 2 - i] = encodeDigit(id);
            id >>= 4;
        }
        index += 3;
    }
    buf[index++] = encodeDigit(frame->length & 0xF);
    if (!frame->rtr) {
        for (i = 0; i < frame->length; i++) {
            buf[index++] = encodeDigit(frame->data[i] >> 4);
            buf[index++] = encodeDigit(frame->data[i]);
        }
    }
    buf[index++] = '\r';
    return index;
}

/**
 * 
 * This decodes a SL Command
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param pkt    The packet to encode
 * 
 * @return The number of bytes that were written to buf
 */
uint16_t encodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *pkt)
{
    if ((length == 0) || (buf == NULL) || (pkt == NULL)) {
        // Packet Too Short
        return false;
    }
    uint16_t index = 0;
    switch (pkt->type) {
        case Bad:
            buf[index++] = 7;
            break;
        case Frame:
            index = encodeCANFrame(buf, length, &(pkt->frame));
            break;
        case Open:
        case Close:
        case Listen:
        case Speed:
        default:
            buf[index++] = '\r';
            break;
    }
    return index;
}
