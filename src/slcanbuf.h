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

void slcanbuf_incTail(volatile SLCanBuf *buf);
void slcanbuf_incHead(volatile SLCanBuf *buf);
void slcanbuf_init(volatile SLCanBuf *buf);
bool slcanbuf_isEmpty(volatile SLCanBuf *buf);
bool slcanbuf_hasPacket(volatile SLCanBuf *buf);
void slcanbuf_push(volatile SLCanBuf *buf, uint8_t byte);
uint8_t slcanbuf_pop(volatile SLCanBuf *buf);
uint16_t slcanbuf_getPacket(volatile SLCanBuf *cbuf, uint8_t *buffer, uint16_t length);
#endif // _SLCANBUF_H_
