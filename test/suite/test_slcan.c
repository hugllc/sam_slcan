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
bool slcan_send_reply(SLPacket *pkt);

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
    /******************************************** slcan_read_tx_buf()/slcan_send()/slcan_send_reply() *******************************************/
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
    FCT_TEST_BGN(slcan_read_tx_byte: can read out a packet that was sent) {
        uint8_t got;
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
        uint16_t i;
        bret = slcan_send(&have);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        for (i = 0; i < sizeof(expect); i++) {
            bret = slcan_read_tx_byte(&got);
            fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
            fct_xchk(got == expect[i], "Iteration %u: Expected %u got %u", i, expect[i], got);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_send_frame: can read out a packet that was sent) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        uint8_t have[] = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        bret = slcan_send_frame(0x12345678, 8, have, true);
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
    FCT_TEST_BGN(slcan_tx_has_byte: returns false with an empty buffer) {
        bool got, expect = false;
        got = slcan_tx_has_byte();
        fct_xchk(got == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", got ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_tx_has_byte: returns true with an non empty buffer) {
        uint8_t buf[10];
        bool got, expect = true;
        slcan_send_frame(0x12345678, 0, buf, true);
        got = slcan_tx_has_byte();
        fct_xchk(got == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", got ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_send_frame: takes NULL data) {
        bool expect = true;
        // This will segfault if it fails
        bool got = slcan_send_frame(0x12345678, 8, NULL, true);
        fct_xchk(got == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", got ? "TRUE" : "FALSE");
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
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_send_reply: takes a null) {
        bool bret, bexpect = false;
        bret = slcan_send_reply(NULL);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a T frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'z', '\r' };
        uint8_t have[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a t frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'z', '\r' };
        uint8_t have[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a R frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'z', '\r' };
        uint8_t have[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8', '8', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a r frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 'z', '\r' };
        uint8_t have[] = { 'r', '1', '2', '3', '8', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad T frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 'T', '1', '2', '3', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad t frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 't', '1', '2', '3', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad R frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 'R', '1', '2', '3', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad r frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 'r', '1', '2', '3', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a O frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'O', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a L frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'L', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a C frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'C', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S0 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '0', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S1 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '1', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S2 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '2', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S3 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '3', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S4 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '4', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S5 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '5', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S6 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '6', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S7 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '7', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a S8 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { '\r' };
        uint8_t have[] = { 'S', '8', '\r' };
        SLPacket pkt;
        bool bret, bexpect = true;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad S9 frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 'S', '9', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
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
    FCT_TEST_BGN(slcan_send_reply: sends an slcan reply to a bad S frame) {
        uint8_t got[BUF_SIZE];
        uint8_t expect[] = { 7 };
        uint8_t have[] = { 'S', '\r' };
        SLPacket pkt;
        bool bret, bexpect = false;
        uint16_t ret, retexpect = sizeof(expect), i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
        }
        bret = slcan_packet_rx(&pkt);
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = slcan_read_tx_buf(got, sizeof(got));
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);    
    }
    FCT_TEST_END()
     /******************************************** slcan_add_rx_byte()/slcan_packet_rx()/slcan_frame_rx() *******************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_packet_rx: Can read a frame packet out) {
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
            fct_xchk(bret == bexpect, "Iteration %u: Expected %s got %s", i, bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckSLPacket(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_packet_rx: Can read a open packet out) {
        uint8_t have[] = { 'O', '\r' };
        SLPacket expect = {
            .type = Open,
            .data = 0,
            .cmd = 'O',
            .frame = {
                .id = 0,
                .length = 0,
                .data = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        SLPacket got;
        uint8_t txexpect[] = { '\r' };
        uint8_t txgot[BUF_SIZE];
        bool bret, bexpect;
        uint16_t countexpect = sizeof(have);
        uint16_t ret, retexpect = sizeof(txexpect);
        uint16_t i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
            bret = slcan_packet_rx(&got);
            bexpect = (i == (sizeof(have) - 1));
            fct_xchk(bret == bexpect, "Iteration %u: Expected %s got %s", i, bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckSLPacket(got, expect, i);


        ret = slcan_read_tx_buf(txgot, sizeof(txgot));
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(txgot, txexpect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_frame_rx: Can read a packet out) {
        uint8_t have[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLCANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        SLCANFrame got;
        bool bret, bexpect;
        uint16_t countexpect = sizeof(have);
        uint16_t i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
            bret = slcan_frame_rx(&got);
            bexpect = (i == (sizeof(have) - 1));
            fct_xchk(bret == bexpect, "Iteration %u: Expected %s got %s", i, bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckFrame(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_frame_rx: Can read an open packet out) {
        uint8_t have[] = { 'O', '\r' };
        SLCANFrame expect = {
            .id = 0,
            .length = 0,
            .data = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
            .ext = false,
            .rtr = false,
            .timestamp = 0
        };
        SLCANFrame got;
        uint8_t txexpect[] = { '\r' };
        uint8_t txgot[BUF_SIZE];
        bool bret, bexpect;
        uint16_t countexpect = sizeof(have);
        uint16_t ret, retexpect = sizeof(txexpect);
        uint16_t i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
            bret = slcan_frame_rx(&got);
            bexpect = false;
            fct_xchk(bret == bexpect, "Iteration %u: Expected %s got %s", i, bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckFrame(got, expect, i);


        ret = slcan_read_tx_buf(txgot, sizeof(txgot));
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(txgot, txexpect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcan_frame_rx: Can read a packet out with a command in front of it) {
        uint8_t have[] = { 'O', '\r', 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLCANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        SLCANFrame got;
        bool bret, bexpect;
        uint16_t countexpect = sizeof(have);
        uint16_t i;
        for (i = 0; i < sizeof(have); i++) {
            slcan_add_rx_byte(have[i]);
            bret = slcan_frame_rx(&got);
            bexpect = (i == (sizeof(have) - 1));
            fct_xchk(bret == bexpect, "Iteration %u: Expected %s got %s", i, bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        }

        fct_xchk(i == countexpect, "Expected %u got %u", countexpect, i);
        CheckFrame(got, expect, i);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
