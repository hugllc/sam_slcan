
#include "slcan_defines.h"
#include "slcanbuf.h"

/**
 * Parses a single text digit and returns the value
 * 
 * @param digit The character to decode
 * 
 * @return The value of the digit decoded
 */
uint8_t decodeDigit(uint8_t digit)
{
    if ((digit >= 'A') && (digit <= 'F')) {
        return digit - 'A' + 10;
    } else if ((digit >= 'a') && (digit <= 'f')) {
        return digit - 'a' + 10;
    } else if ((digit >= '0') && (digit <= '9')) {
        return digit - '0';
    }
    // Don't know what this is
    return 0;
}

/**
 * This decodes a number with an arbitrary number of digits up to 8.
 * The number is read as big endian.
 * 
 * @param buf    The buffer to use
 * @param length The number of digits to decode, max 8
 * 
 * @return The decoded number, unsigned
 */
uint32_t decodeNumber(uint8_t *buf, uint8_t length) {
    uint32_t ret = 0;
    uint8_t i;
    if (buf != NULL) {
        length = (length > 8) ? 8 : length;
        for (i = 0; i < length; i++) {
            ret <<= 4;
            ret += decodeDigit(buf[i]) & 0xF;
        }
    }
    return ret;
}
/**
 * 
 * This decodes byte data
 * 
 * @param buf    The buffer to use
 * @param length The number of characters to decode
 * @param data   The data buffer to put the byte data in
 * 
 * @return True if this was successfully decoded, false otherwise
 */
bool decodeByteData(uint8_t *buf, uint8_t length, uint8_t *data)
{
    if ((buf == NULL) || (data == NULL)) {
        // Can't have null pointers
        return false;
    }
    uint8_t i;
    uint8_t bytes = length / 2;
    for (i = 0; i < bytes; i++) {
        data[i] = (uint8_t)decodeNumber(&(buf[i * 2]), 2);
    }
    return true;
}
/**
 * 
 * This decodes a data packet
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param frame  The frame struct to put the data into
 * 
 * @return True if this was successfully decoded, false otherwise
 */
bool decodeCANFrame(uint8_t *buf, uint8_t length, CANFrame *frame)
{
    initCANFrame(frame);
    if ((length < 5) || (buf == NULL) || (frame == NULL)) {
        // Packet Too Short
        return false;
    }
    length = (buf[length - 1] == '\r') ? length - 1 : length;
    if ((buf[0] == 'T') && (length >= 10)) {
        frame->ext = true;
        frame->rtr = false;
        frame->id = decodeNumber(&buf[1], 8) & EXT_ID_MASK;
        frame->length = (uint8_t)decodeNumber(&buf[9], 1);
        if (length >= (frame->length + frame->length + 10)) {
            return decodeByteData(&buf[10], frame->length * 2, frame->data);
        }
        return false;
    } else if ((buf[0] == 'R') && (length >= 10)) {
        frame->ext = true;
        frame->rtr = true;
        frame->id = decodeNumber(&buf[1], 8) & EXT_ID_MASK;
        frame->length = (uint8_t)decodeNumber(&buf[9], 1);
        return true;
    } else if (buf[0] == 't') {
        frame->ext = false;
        frame->rtr = false;
        frame->id = decodeNumber(&buf[1], 3) & ID_MASK;
        frame->length = (uint8_t)decodeNumber(&buf[4], 1);
        if (length >= (frame->length + frame->length + 5)) {
            return decodeByteData(&buf[5], frame->length * 2, frame->data);
        }
        return false;
    } else if ((buf[0] == 'r')  && (length >= 5)) {
        frame->ext = false;
        frame->rtr = true;
        frame->id = decodeNumber(&buf[1], 3) & ID_MASK;
        frame->length = (uint8_t)decodeNumber(&buf[4], 1);
        return true;
    }
    return false;
}

/**
 * 
 * This decodes a SL Command
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param cmd    The command struct to put the data into
 * 
 * @return True if this was successfully decoded, false otherwise
 */
bool decodeSLPacket(uint8_t *buf, uint8_t length, SLPacket *pkt)
{
    if ((length == 0) || (buf == NULL) || (pkt == NULL)) {
        // Packet Too Short
        return false;
    }
    memset(pkt, 0, sizeof(SLPacket));
    pkt->cmd = buf[0];
    pkt->type = Bad;
    pkt->data = 0;
    switch (buf[0]) {
        case 'O':    // Open
            pkt->type = Open;
            pkt->data = 0;
            break;
        case 'C':    // Close
            pkt->type = Close;
            pkt->data = 0;
            break;
        case 'L':    // Listen
            pkt->type = Listen;
            pkt->data = 0;
            break;
        case 'S':    // Speed
            if ((length > 1) && (buf[1] != '\r')) {
                pkt->type = Speed;
                pkt->data = decodeNumber(&buf[1], 1);
            }
            break;
        case 'T':
        case 't':
        case 'R':
        case 'r':
            if (decodeCANFrame(buf, length, &pkt->frame)) {
                pkt->type = Frame;
            }
            break;
        default:
            break;
    }
    return pkt->type != Bad;
}

