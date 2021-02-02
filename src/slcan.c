
#include "slcan_defines.h"
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
