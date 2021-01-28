
#include "slcan.h"

/**
 * 
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

uint32_t parseNumber(uint8_t *buf, uint8_t length) {
    uint32_t ret = 0;
    uint8_t i;
    length = (length > 8) ? 8 : length;
    for (i = 0; i < length; i++) {
        ret <<= 4;
        ret += parseDigit(buf[i]) & 0xF;
    }
    return ret;
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
    if (length < 5) {
        // Packet Too Short
        return false;
    }
    if ((buf[0] == 'T') && (length >= 10)) {
        return true;
    } else if ((buf[0] == 'R') && (length >= 10)) {
        return true;
    } else if (buf[0] == 't') {
        return true;
    } else if (buf[0] == 'r') {
        return true;
    }
    return false;
}
