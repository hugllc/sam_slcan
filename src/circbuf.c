
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "circbuf.h"
#include "slcan_defines.h"
/**
 * Increments the tail
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
void circbuf_incTail(volatile CircBuf *buf)
{
    buf->tail++;
    if (buf->tail >= CIRC_BUFFER_SIZE) {
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
void circbuf_incHead(volatile CircBuf *buf)
{
    buf->head++;
    if (buf->head >= CIRC_BUFFER_SIZE) {
        buf->head = 0;
    };
    // Increment the tail if we just overfilled the buffer.  This erases the oldest byte in the buffer
    if (buf->tail == buf->head) {
        circbuf_incTail(buf);
    }
}

/**
 * Initializes the given buffer
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
void circbuf_init(volatile CircBuf *buf)
{
    memset((uint8_t *)buf, 0, sizeof(CircBuf));
}

/**
 * Checks to see if the buffer is empty
 * 
 * @param buf The buffer to use
 * 
 * @return true if the buffer is empty
 */
bool circbuf_isEmpty(volatile CircBuf *buf)
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
bool circbuf_hasPacket(volatile CircBuf *buf)
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
void circbuf_push(volatile CircBuf *buf, uint8_t byte)
{
    buf->data[buf->head] = byte;
    if (byte == '\r') {
        buf->packets++;
    }
    circbuf_incHead(buf);
}

/**
 * Pops a byte off the buffer.
 * 
 * @param buf The buffer to use
 * 
 * @return The byte popped off the buffer.  Returns 0 if the buffer is empty.
 */
uint8_t circbuf_pop(volatile CircBuf *buf)
{
    if (circbuf_isEmpty(buf)) {
        return 0;
    }
    uint16_t tail = buf->tail;
    circbuf_incTail(buf);
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
uint16_t circbuf_getPacket(volatile CircBuf *cbuf, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    uint8_t byte;
    if (cbuf->packets == 0) {
        return 0;
    }
    i = 0;
    while (!circbuf_isEmpty(cbuf) && (i < length)) {
        byte = circbuf_pop(cbuf);
        buffer[i] = byte;
        i++;
        if (byte == '\r') {
            cbuf->packets--;
            break;
        }
    }
    return i;
}
