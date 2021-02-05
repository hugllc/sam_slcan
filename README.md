# sam_slcan

## Introduction
Serial Line CAN support for SAM D20, D21, and C21 pricessors

### Why C not <insert language here>
Because one of my applications for this is a bootloader written in C.  There is not enough
space in the bootloader section of the MCU to write my bootloader in C++.

That being said I may write a C++ wrapper for it.

## Microchip SAMC21 Example

### GCC Setup
The following sets the number of bytes in the circular buffers (default 256):
~~~~~~~~
-DCIRC_BUFFER_SIZE=x
~~~~~~~~

### Code

This assusme SLCAN is being used on SERCOM3 pins PA22 and PA23 at 115200 baud.

~~~~~~~~.c

#include <slcan.h>
#include <slcan_samc21.h>

int main(void)
{
    uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7 };
    SLCANFrame frame;

    // Set up the serial port and IRQ
    // slcan_usart_init(Sercom *usart, uint32_t baud, slcan_uart_pads pads, bool pin_mux_alt, uint8_t group, uint8_t txpin, uint8_t rxpin)
    slcan_usart_init(SERCOM3, 115200, UART_RX_PAD1_TX_PAD0, false, 0, PA22, PA23);
    SLCAN_NVIC_EnableIRQ(SERCOM3_IRQn, 2)

    // Send out a CAN frame
    // bool slcan_send_frame(uint32_t id, uint8_t length, uint8_t *data, bool ext);
    slcan_send_frame(5, sizeof(data), data, true);

    while (1) {
        // This will start the transmission if there is something in the buffer
        slcan_driver_main(SERCOM3);

        if (slcan_frame_rx(&frame)) {
            // Do something with the frame
            //
            // uint32_t frame.id      CAN Id
            // uint8_t frame.length   Data length
            // bool frame.ext         True if this is an extended packet
            // bool frame.rtr         True if the is a rtr packet
            // uint8_t *frame.data    Data buffer
        }
    }

}

void SERCOM3_Handler()
{
    // This function will only deal with RXC and DRE interrupts.  Anything else you want will have to be handled separately
    SLCAN_Handler(SERCOM3);
}
~~~~~~~~

## Other MCUs
This can be used generically as long as slcan_samc21.h or any other specific mcu include file are not included.
The serial port will just have to be setup manually.

## Contributions

I welcome pull requests and bugs.
