
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "slcanbuf.h"

/**
 * Increments the tail
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
void slcanbuf_incTail(volatile SLCanBuf *buf)
{
    buf->tail++;
    if (buf->tail >= SLCAN_BUFFER_SIZE) {
        buf->tail = 0;
    };
}
/**
 * Increments the head
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
void slcanbuf_incHead(volatile SLCanBuf *buf)
{
    buf->head++;
    if (buf->head >= SLCAN_BUFFER_SIZE) {
        buf->head = 0;
    };
    // Increment the tail if we just overfilled the buffer.  This erases the oldest byte in the buffer
    if (buf->tail == buf->head) {
        slcanbuf_incTail(buf);
    }
}

/**
 * Initializes the given buffer
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
void slcanbuf_init(volatile SLCanBuf *buf)
{
    memset((uint8_t *)buf, 0, sizeof(SLCanBuf));
}

/**
 * Checks to see if the buffer is empty
 * 
 * @param buf The buffer to use
 * 
 * @return true if the buffer is empty
 */
bool slcanbuf_isEmpty(volatile SLCanBuf *buf)
{
    return buf->head == buf->tail;
}
/**
 * Checks to see if the buffer is empty
 * 
 * @param buf The buffer to use
 * 
 * @return true if the buffer contains at least one packet
 */
bool slcanbuf_hasPacket(volatile SLCanBuf *buf)
{
    return buf->packets > 0;
}
/**
 * Adds the given byte to the buffer.  It will remove the
 * last byte in the buffer if the buffer is full.
 * 
 * @param buf  The buffer to use
 * @param byte The byte to add
 * 
 * @return void
 */
void slcanbuf_push(volatile SLCanBuf *buf, uint8_t byte)
{
    buf->data[buf->head] = byte;
    if (byte == '\r') {
        buf->packets++;
    }
    slcanbuf_incHead(buf);
}

/**
 * Pops a byte off the buffer.
 * 
 * @param buf The buffer to use
 * 
 * @return The byte popped off the buffer.  Returns 0 if the buffer is empty.
 */
uint8_t slcanbuf_pop(volatile SLCanBuf *buf)
{
    if (slcanbuf_isEmpty(buf)) {
        return 0;
    }
    uint16_t tail = buf->tail;
    slcanbuf_incTail(buf);
    return buf->data[tail];
}
/**
 * Gets a packet out of the buffer
 * 
 * @param cbuf   The circular buffer to use
 * @param buffer The buffer to put the packet into
 * @param length The size of the buffer to put the packet into
 * 
 * @return The number of bytes put into the buffer
 */
uint16_t slcanbuf_getPacket(volatile SLCanBuf *cbuf, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    uint8_t byte;
    if (cbuf->packets == 0) {
        return 0;
    }
    for (i = 0; i < length; i++) {
        byte = slcanbuf_pop(cbuf);
        buffer[i] = byte;
        if (byte == '\r') {
            i++;
            break;
        }
        cbuf->packets--;
    }
    return i;
}
