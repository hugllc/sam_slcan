#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#ifndef CIRC_BUFFER_SIZE
#define CIRC_BUFFER_SIZE 256
#endif

typedef struct {
    uint16_t head;
    uint16_t tail;
    uint8_t packets;
    uint8_t data[CIRC_BUFFER_SIZE];
} CircBuf;

void circbuf_incTail(volatile CircBuf *buf);
void circbuf_incHead(volatile CircBuf *buf);
void circbuf_init(volatile CircBuf *buf);
bool circbuf_isEmpty(volatile CircBuf *buf);
bool circbuf_hasPacket(volatile CircBuf *buf);
void circbuf_push(volatile CircBuf *buf, uint8_t byte);
uint8_t circbuf_pop(volatile CircBuf *buf);
uint16_t circbuf_getPacket(volatile CircBuf *cbuf, uint8_t *buffer, uint16_t length);
#endif // _CIRCBUF_H_
