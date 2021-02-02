#ifndef _CIRCBUFF_H_
#define _CIRCBUFF_H_

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

static inline void slcanbuf_incTail(SLCanBuf *buf)
{
    buf->tail++;
    if (buf->tail >= SLCAN_BUFFER_SIZE) {
        buf->tail = 0;
    };
}
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

static inline void slcanbuf_init(SLCanBuf *buf)
{
    memset(buf, 0, sizeof(SLCanBuf));
}

static inline bool slcanbuf_isEmpty(SLCanBuf *buf)
{
    return buf->head == buf->tail;
}

static inline void slcanbuf_push(SLCanBuf *buf, uint8_t byte)
{
    buf->data[buf->head] = byte;
    if (byte == '\r') {
        buf->packets++;
    }
    slcanbuf_incHead(buf);
}
static inline uint8_t slcanbuf_pop(SLCanBuf *buf)
{
    if (slcanbuf_isEmpty(buf)) {
        return 0;
    }
    uint16_t tail = buf->tail;
    slcanbuf_incTail(buf);
    return buf->data[tail];
}

static inline uint16_t slcanbuf_getPacket(SLCanBuf *buf, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    uint8_t byte;
    if (buf->packets == 0) {
        return 0;
    }
    for (i = 0; i < length; i++) {
        byte = slcanbuf_pop(buf);
        buffer[i] = byte;
        if (byte == '\r') {
            i++;
            break;
        }
    }
    return i;
}
#endif // _CIRCBUFF_H_
