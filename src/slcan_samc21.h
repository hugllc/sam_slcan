#ifndef _SLCAN_MCU_H_
#define _SLCAN_MCU_H_

#include <sam.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "slcan.h"

typedef enum {
    SLCAN_RX_PAD0_TX_PAD2 = SERCOM_USART_CTRLA_RXPO(0) | SERCOM_USART_CTRLA_TXPO(1),
    SLCAN_RX_PAD1_TX_PAD2 = SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_TXPO(1),
    SLCAN_RX_PAD2_TX_PAD0 = SERCOM_USART_CTRLA_RXPO(2),
    SLCAN_RX_PAD3_TX_PAD0 = SERCOM_USART_CTRLA_RXPO(3),
    SLCAN_RX_PAD1_TX_PAD0 = SERCOM_USART_CTRLA_RXPO(1),
    SLCAN_RX_PAD3_TX_PAD2 = SERCOM_USART_CTRLA_RXPO(3) | SERCOM_USART_CTRLA_TXPO(1),
} slcan_uart_pads;

#define slcan_calcBaudNumber(rate) (uint16_t)(65536.0 * (1.0 - (16.0 * ((float)rate/(float)CPU_FREQUENCY))))

#define SLCAN_NVIC_EnableIRQ(irq, priority) \
    NVIC_DisableIRQ(irq); \
    NVIC_ClearPendingIRQ(irq); \
    NVIC_SetPriority(irq, priority);  \
    NVIC_EnableIRQ(irq)


static inline void slcan_usart_init(Sercom *usart, uint32_t baud, slcan_uart_pads pads, bool pin_mux_alt, uint8_t group, uint8_t txpin, uint8_t rxpin)
{
    uint32_t mclk_mask;
    uint32_t gclk_id;
    if (usart == SERCOM0) {
        mclk_mask = MCLK_APBCMASK_SERCOM0;
        gclk_id = SERCOM0_GCLK_ID_CORE;
    } else if (usart == SERCOM1) {
        mclk_mask = MCLK_APBCMASK_SERCOM1;
        gclk_id = SERCOM1_GCLK_ID_CORE;
    } else if (usart == SERCOM2) {
        mclk_mask = MCLK_APBCMASK_SERCOM2;
        gclk_id = SERCOM2_GCLK_ID_CORE;
    } else if (usart == SERCOM3) {
        mclk_mask = MCLK_APBCMASK_SERCOM3;
        gclk_id = SERCOM3_GCLK_ID_CORE;
    } else if (usart == SERCOM4) {
        mclk_mask = MCLK_APBCMASK_SERCOM4;
        gclk_id = SERCOM4_GCLK_ID_CORE;
    } else if (usart == SERCOM5) {
        mclk_mask = MCLK_APBCMASK_SERCOM5;
        gclk_id = SERCOM5_GCLK_ID_CORE;
    }

    MCLK->APBCMASK.reg |= mclk_mask;
    GCLK->PCHCTRL[gclk_id].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_MASK);

    PORT->Group[group].PINCFG[txpin].bit.PMUXEN = 1;
    PORT->Group[group].PINCFG[rxpin].bit.PMUXEN = 1;
    PORT->Group[group].PMUX[txpin / 2].reg = PORT_PMUX_PMUXO(pin_mux_alt ? 3 : 2) |  PORT_PMUX_PMUXE(pin_mux_alt ? 3 : 2);

    while (usart->USART.SYNCBUSY.bit.ENABLE);
    usart->USART.CTRLA.bit.ENABLE = 0;
    while (usart->USART.SYNCBUSY.bit.SWRST);
    usart->USART.CTRLA.bit.SWRST = 1;
    while (usart->USART.CTRLA.bit.SWRST);
    while (usart->USART.SYNCBUSY.bit.SWRST || usart->USART.SYNCBUSY.bit.ENABLE);
    usart->USART.CTRLA.reg = pads | SERCOM_USART_CTRLA_MODE(1) | SERCOM_USART_CTRLA_DORD;
    while (usart->USART.SYNCBUSY.bit.CTRLB);
    usart->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0);
    usart->USART.BAUD.reg = slcan_calcBaudNumber(baud);

    while (usart->USART.SYNCBUSY.bit.ENABLE);
    usart->USART.CTRLA.bit.ENABLE = 1;

    slcan_init();
    usart->USART.INTENSET.reg = SERCOM_USART_INTENCLR_RXC;

}
static inline void slcan_driver_main(Sercom *usart)
{
    if (slcan_tx_has_byte() && (usart->USART.INTENSET.bit.DRE == 0)) {
        usart->USART.INTENSET.reg = SERCOM_USART_INTENCLR_DRE;
    }
}
static inline void SLCAN_Handler(Sercom *usart)
{
    uint8_t byte;
    uint8_t flags = usart->USART.INTFLAG.reg; // Clear the flags
    usart->USART.INTFLAG.reg = flags;
    if ((flags & SERCOM_USART_INTFLAG_RXC) == SERCOM_USART_INTFLAG_RXC) {
        slcan_add_rx_byte(usart->USART.DATA.reg);
    }
    if ((flags & SERCOM_USART_INTFLAG_DRE) == SERCOM_USART_INTFLAG_DRE) {
        if (slcan_read_tx_byte(&byte)) {
            usart->USART.DATA.reg = byte;
        } else {
            // Disable this interrupt
            usart->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_DRE;
        }

    }

}


#endif // _SLCAN_MCU_H_