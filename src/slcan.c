
#include "slcan.h"

#define EXT_ID_MASK 0x1FFFFFFF
#define ID_MASK 0x7FF
/**
 * Parses a single text digit and returns the value
 * 
 * @param digit The character to parse
 * 
 * @return The value of the digit parsed
 */
uint8_t parseDigit(uint8_t digit)
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
 * This parses a number with an arbitrary number of digits up to 8.
 * The number is read as big endian.
 * 
 * @param buf    The buffer to use
 * @param length The number of digits to parse, max 8
 * 
 * @return The parsed number, unsigned
 */
uint32_t parseNumber(uint8_t *buf, uint8_t length) {
    uint32_t ret = 0;
    uint8_t i;
    if (buf != NULL) {
        length = (length > 8) ? 8 : length;
        for (i = 0; i < length; i++) {
            ret <<= 4;
            ret += parseDigit(buf[i]) & 0xF;
        }
    }
    return ret;
}
/**
 * 
 * This parses byte data
 * 
 * @param buf    The buffer to use
 * @param length The number of characters to parse
 * @param data   The data buffer to put the byte data in
 * 
 * @return True if this was successfully parsed, false otherwise
 */
bool parseByteData(uint8_t *buf, uint8_t length, uint8_t *data)
{
    if ((buf == NULL) || (data == NULL)) {
        // Can't have null pointers
        return false;
    }
    uint8_t i;
    uint8_t bytes = length / 2;
    for (i = 0; i < bytes; i++) {
        data[i] = (uint8_t)parseNumber(&(buf[i * 2]), 2);
    }
    return true;
}
/**
 * 
 * This parses a data packet
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param frame  The frame struct to put the data into
 * 
 * @return True if this was successfully parsed, false otherwise
 */
bool parseCANFrame(uint8_t *buf, uint8_t length, CANFrame *frame)
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
        frame->id = parseNumber(&buf[1], 8) & EXT_ID_MASK;
        frame->length = (uint8_t)parseNumber(&buf[9], 1);
        if (length >= (frame->length + frame->length + 10)) {
            return parseByteData(&buf[10], frame->length * 2, frame->data);
        }
        return false;
    } else if ((buf[0] == 'R') && (length >= 10)) {
        frame->ext = true;
        frame->rtr = true;
        frame->id = parseNumber(&buf[1], 8) & EXT_ID_MASK;
        frame->length = (uint8_t)parseNumber(&buf[9], 1);
        return true;
    } else if (buf[0] == 't') {
        frame->ext = false;
        frame->rtr = false;
        frame->id = parseNumber(&buf[1], 3) & ID_MASK;
        frame->length = (uint8_t)parseNumber(&buf[4], 1);
        if (length >= (frame->length + frame->length + 5)) {
            return parseByteData(&buf[5], frame->length * 2, frame->data);
        }
        return false;
    } else if ((buf[0] == 'r')  && (length >= 5)) {
        frame->ext = false;
        frame->rtr = true;
        frame->id = parseNumber(&buf[1], 3) & ID_MASK;
        frame->length = (uint8_t)parseNumber(&buf[4], 1);
        return true;
    }
    return false;
}

/**
 * 
 * This parses a SL Command
 * 
 * @param buf    The buffer to use
 * @param length The length of the buffer
 * @param cmd    The command struct to put the data into
 * 
 * @return True if this was successfully parsed, false otherwise
 */
bool parseSLCommand(uint8_t *buf, uint8_t length, SLCommand *cmd)
{
    if ((length == 0) || (buf == NULL) || (cmd == NULL)) {
        // Packet Too Short
        return false;
    }
    cmd->cmd = buf[0];
    cmd->type = Bad;
    cmd->data = 0;
    switch (buf[0]) {
        case 'O':    // Open
            cmd->type = Open;
            cmd->data = 0;
            break;
        case 'C':    // Close
            cmd->type = Close;
            cmd->data = 0;
            break;
        case 'L':    // Listen
            cmd->type = Listen;
            cmd->data = 0;
            break;
        case 'S':    // Speed
            if ((length > 1) && (buf[1] != '\r')) {
                cmd->type = Speed;
                cmd->data = parseNumber(&buf[1], 1);
            }
            break;
        default:
            break;
    }
    return cmd->type != Bad;
}
