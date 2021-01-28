
#include "slcan.h"


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
bool parseDataFrame(uint8_t *buf, uint8_t length, CANFrame *frame)
{
    return false;
}
