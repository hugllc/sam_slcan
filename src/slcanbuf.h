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

void slcanbuf_incTail(SLCanBuf *buf);
void slcanbuf_incHead(SLCanBuf *buf);
void slcanbuf_init(SLCanBuf *buf);
bool slcanbuf_isEmpty(SLCanBuf *buf);
bool slcanbuf_hasPacket(SLCanBuf *buf);
void slcanbuf_push(SLCanBuf *buf, uint8_t byte);
uint8_t slcanbuf_pop(SLCanBuf *buf);
uint16_t slcanbuf_getPacket(SLCanBuf *cbuf, uint8_t *buffer, uint16_t length);
#endif // _SLCANBUF_H_
