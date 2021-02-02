
#include "slcan_defines.h"
#include "slcan_decode.h"
#include "slcan_encode.h"
#include "slcanbuf.h"

SLCanBuf slcan_txbuf;
SLCanBuf slcan_rxbuf;

/**
 * Initializes the slcan framework
 * 
 * @return void
 */
void slcan_init(void)
{
    slcanbuf_init(&slcan_txbuf);
    slcanbuf_init(&slcan_rxbuf);
}

/**
 * Reads out the tx buffer.
 * 
 * @param buf    The buffer to write to
 * @param length The length of the given buffer
 * 
 * @return The number of bytes written to the buffer
 */
uint16_t slcan_read_tx_buf(uint8_t *buf, uint16_t length)
{
    uint16_t index = 0;
    if (buf != NULL) {
        while (!slcanbuf_isEmpty(&slcan_txbuf) && (index < length)) {
            buf[index] = slcanbuf_pop(&slcan_txbuf);
            index++;
        }
    }
    return index;
}

/**
 * Reads a packet out of the receive buffer
 * 
 * @param pkt The packet to write it in.
 * 
 * @return True if a packet was received
 */
bool slcan_packet_rx(SLPacket *pkt)
{
    uint16_t length;
    uint8_t buf[64];
    if (pkt != NULL) {
        length = slcanbuf_getPacket(&slcan_rxbuf, buf, sizeof(buf));
        if (length > 0) {
            return decodeSLPacket(buf, sizeof(buf), pkt);
        }
    }
    return false;
}
/**
 * Reads a packet out of the receive buffer
 * 
 * @param pkt The packet to write it in.
 * 
 * @return True if a packet was received
 */
void slcan_add_rx_byte(uint8_t byte)
{
    slcanbuf_push(&slcan_rxbuf, byte);
}

/**
 * Reads a packet out of the receive buffer
 * 
 * @param pkt The packet to write it in.
 * 
 * @return True if a packet was received
 */
bool slcan_send(SLPacket *pkt)
{
    uint16_t length;
    uint16_t index;
    uint8_t buf[64];
    if (pkt != NULL) {
        length = encodeSLPacket(buf, sizeof(buf), pkt);
        index = 0;
        while (length > index) {
            slcanbuf_push(&slcan_txbuf, buf[index]);
            index++;
        }
        return length > 0;
    }
    return false;
}
