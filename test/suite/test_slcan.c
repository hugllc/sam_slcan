/**
 * @file       test/suite/Core/mmu/test_mmu.c
 * @author     Scott L. Price <prices@hugllc.com>
 * @copyright  © 2016 Hunt Utilities Group, LLC
 * @brief   The test file for mmu.c
 * @details
 *
 *
 */
/*
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include "main.h"
#include "fct.h"
#include "slcan.h"

#define BUF_SIZE 64
uint8_t decodeDigit(uint8_t digit);
uint8_t encodeDigit(uint8_t digit);
uint32_t decodeNumber(uint8_t *buf, uint8_t length);
bool decodeByteData(uint8_t *buf, uint8_t length, uint8_t *data);

FCTMF_FIXTURE_SUITE_BGN(test_slcan)
{
    /**
    * @brief This sets up this suite
    *
    * @return 0 success, otherwise failure
    */
    FCT_SETUP_BGN() {
        TestInit();
    }
    FCT_SETUP_END();
    /**
    * @brief This tears down this suite
    *
    * @return 0 success, otherwise failure
    */
    FCT_TEARDOWN_BGN() {
    } FCT_TEARDOWN_END()
    /******************************************** slcan_read_tx_buf()/slcan_send() *******************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_read_tx_buf: Returns 0 when buffer is empty) {
        uint8_t got[BUF_SIZE];
        uint16_t ret, expect = 0;
        ret = slcan_read_tx_buf(got, sizeof(got));
        fct_xchk(ret == expect, "Expected %u got %u", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_read_tx_buf: handles a null) {
        SLPacket have = {
            .type = Frame,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0x12345678,
                .length = 8,
                .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
                .ext = true,
                .rtr = false,
                .timestamp = 0
            }
        };
        bool bret, bexpect = true;
        uint16_t ret, retexpect = 0;
        bret = slcan_send(&have);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = slcan_read_tx_buf(NULL, 10);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_read_tx_buf: can read out a packet that was sent) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLPacket have = {
            .type = Frame,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0x12345678,
                .length = 8,
                .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
                .ext = true,
                .rtr = false,
                .timestamp = 0
            }
        };
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        bret = slcan_send(&have);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = slcan_read_tx_buf(got, sizeof(got));
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_send: takes a null) {
        bool bret, bexpect = false;
        bret = slcan_send(NULL);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /******************************************** slcan_read_tx_buf()/slcan_send() *******************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_packet_rx: Can read a packet out) {
        uint8_t have[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLPacket expect = {
            .type = Frame,
            .data = 0,
            .cmd = 'T',
            .frame = {
                .id = 0x12345678,
                .length = 8,
                .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
                .ext = true,
                .rtr = false,
                .timestamp = 0
            }
        };
        SLPacket got;
        bool bret, bexpect;
        uint16_t countexpect = sizeof(have);
        uint16_t i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
            bret = slcan_packet_rx(&got);
            bexpect = (i == (sizeof(have) - 1));
            fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckSLPacket(got, expect, i);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
