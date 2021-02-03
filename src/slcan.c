
#include "slcan_defines.h"
#include "slcan_decode.h"
#include "slcan_encode.h"
#include "slcanbuf.h"
#include "slcan.h"
#include <stdio.h>

volatile SLCanBuf slcan_txbuf;
volatile SLCanBuf slcan_rxbuf;

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
    bool ret = false;
    if (pkt != NULL) {
        length = slcanbuf_getPacket(&slcan_rxbuf, buf, sizeof(buf));
        if (length > 0) {
            ret = decodeSLPacket(buf, sizeof(buf), pkt);
            if (ret && (pkt->type != Frame)) {
                slcan_send(pkt); // Send a reply
            }
        }
    }
    return ret;
}
/**
 * Reads a packet out of the receive buffer
 * 
 * @param pkt The packet to write it in.
 * 
 * @return True if a packet was received
 */
bool slcan_frame_rx(SLCANFrame *frame)
{
    uint16_t length;
    uint8_t buf[64];
    SLPacket pkt;
    bool ret = false;
    if (frame != NULL) {
        length = slcanbuf_getPacket(&slcan_rxbuf, buf, sizeof(buf));
        if (length > 0) {
            ret = decodeSLCANFrame(buf, sizeof(buf), frame);
            if (ret == false) {
                if (decodeSLPacket(buf, sizeof(buf), &pkt)) {
                    slcan_send(&pkt);
                }

            }
        }
    }
    return ret;
}

/**
 * Adds a byte to the rx buffer
 * 
 * @param pkt The packet to write it in.
 * 
 * @return void
 */
void slcan_add_rx_byte(uint8_t byte)
{
    slcanbuf_push(&slcan_rxbuf, byte);
}

/**
 * Sends a packet
 * 
 * @param pkt The packet to write it in.
 * 
 * @return True if a packet was sent
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
/**
 * Sends a packet
 * 
 * @param id     The CAN ID to send
 * @param length The length of the data
 * @param data   The data to send out
 * @param ext    True if this is an extended packet
 * 
 * @return True if a packet was sent
 */
bool slcan_send_frame(uint32_t id, uint8_t length, uint8_t *data, bool ext)
{
    SLPacket pkt;
    pkt.type = Frame;
    pkt.frame.id = id;
    pkt.frame.length = length;
    pkt.frame.rtr = false;
    pkt.frame.ext = ext;
    memcpy (pkt.frame.data, data, length);
    return slcan_send(&pkt);
}
