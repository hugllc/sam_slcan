#ifndef _SLCANBUF_H_
#define _SLCANBUF_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#ifndef SLCAN_BUFFER_SIZE
#define SLCAN_BUFFER_SIZE 256
#endif

typedef struct {
    uint16_t head;
    uint16_t tail;
    uint8_t packets;
    uint8_t data[SLCAN_BUFFER_SIZE];
} SLCanBuf;


/**
 * Increments the tail
 * 
 * @param buf The buffer to use
 * 
 * @return void
 */
static inline void slcanbuf_incTail(SLCanBuf *buf)
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
static inline void slcanbuf_incHead(SLCanBuf *buf)
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
static inline void slcanbuf_init(SLCanBuf *buf)
{
    memset(buf, 0, sizeof(SLCanBuf));
}

/**
 * Checks to see if the buffer is empty
 * 
 * @param buf The buffer to use
 * 
 * @return true if the buffer is empty
 */
static inline bool slcanbuf_isEmpty(SLCanBuf *buf)
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
static inline bool slcanbuf_hasPacket(SLCanBuf *buf)
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
static inline void slcanbuf_push(SLCanBuf *buf, uint8_t byte)
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
static inline uint8_t slcanbuf_pop(SLCanBuf *buf)
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
static inline uint16_t slcanbuf_getPacket(SLCanBuf *cbuf, uint8_t *buffer, uint16_t length)
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
#endif // _SLCANBUF_H_