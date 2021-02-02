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
#include <cmath>
#include <stdio.h>
#include "main.h"
#include "fct.h"
#include "slcan.h"
#include "slcan_encode.h"
#include "slcan_decode.h"

uint8_t encodeDigit(uint8_t digit);

FCTMF_FIXTURE_SUITE_BGN(test_slcan_encode)
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
    /************************************************** encodeDigit() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0) {
        uint8_t ret, expect;
        ret = encodeDigit(0);
        expect = '0';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 1) {
        uint8_t ret, expect;
        ret = encodeDigit(1);
        expect = '1';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 2) {
        uint8_t ret, expect;
        ret = encodeDigit(2);
        expect = '2';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 3) {
        uint8_t ret, expect;
        ret = encodeDigit(3);
        expect = '3';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 4) {
        uint8_t ret, expect;
        ret = encodeDigit(4);
        expect = '4';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 5) {
        uint8_t ret, expect;
        ret = encodeDigit(5);
        expect = '5';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 6) {
        uint8_t ret, expect;
        ret = encodeDigit(6);
        expect = '6';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 7) {
        uint8_t ret, expect;
        ret = encodeDigit(7);
        expect = '7';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 8) {
        uint8_t ret, expect;
        ret = encodeDigit(8);
        expect = '8';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 9) {
        uint8_t ret, expect;
        ret = encodeDigit(9);
        expect = '9';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xA) {
        uint8_t ret, expect;
        ret = encodeDigit(0xA);
        expect = 'A';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xB) {
        uint8_t ret, expect;
        ret = encodeDigit(0xB);
        expect = 'B';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xC) {
        uint8_t ret, expect;
        ret = encodeDigit(0xC);
        expect = 'C';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xD) {
        uint8_t ret, expect;
        ret = encodeDigit(0xD);
        expect = 'D';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xE) {
        uint8_t ret, expect;
        ret = encodeDigit(0xE);
        expect = 'E';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xF) {
        uint8_t ret, expect;
        ret = encodeDigit(0xF);
        expect = 'F';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xFF) {
        uint8_t ret, expect;
        ret = encodeDigit(0xFF);
        expect = 'F';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeDigit: returns correctly for 0xF0) {
        uint8_t ret, expect;
        ret = encodeDigit(0xF0);
        expect = '0';
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /************************************************** encodeSLPacket() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: returns 0 when length is 0) {
        uint8_t buffer[] = {};
        SLPacket frame;
        uint8_t ret, retexpect;
        ret = encodeSLPacket(buffer, sizeof(buffer), &frame);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: returns 0 when buffer is NULL) {
        SLPacket frame;
        uint8_t ret, retexpect;
        ret = encodeSLPacket(NULL, 5, &frame);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: returns 0 when frame is NULL) {
        uint8_t buffer[] = {};
        uint8_t ret, retexpect;
        ret = encodeSLPacket(buffer, sizeof(buffer), NULL);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: normal packet decodes properly) {
        uint8_t expect[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        SLPacket have = {
            .type = Frame,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0x123,
                .length = 8,
                .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint16_t ret, retexpect;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: extended packet decodes properly) {
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
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint16_t ret, retexpect;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: normal RTR packet decodes properly) {
        uint8_t expect[] = { 'r', '1', '2', '3', '8', '\r' };
        SLPacket have = {
            .type = Frame,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0x123,
                .length = 8,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = false,
                .rtr = true,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint16_t ret, retexpect;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: extended RTR packet decodes properly) {
        uint8_t expect[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8', '8', '\r' };
        SLPacket have = {
            .type = Frame,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0x12345678,
                .length = 8,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = true,
                .rtr = true,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint16_t ret, retexpect;
        uint8_t i;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: encodes bad packets correctly) {
        uint8_t expect[] = { 7 };
        SLPacket have = {
            .type = Bad,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0,
                .length = 0,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint16_t ret, retexpect;
        uint8_t i;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: encodes open packets correctly) {
        uint8_t expect[] = { '\r' };
        SLPacket have = {
            .type = Open,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0,
                .length = 0,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint16_t ret, retexpect;
        uint8_t i;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: encodes close packets correctly) {
        uint8_t expect[] = { '\r' };
        SLPacket have = {
            .type = Close,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0,
                .length = 0,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint16_t ret, retexpect;
        uint8_t i;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeSLPacket: encodes listen packets correctly) {
        uint8_t expect[] = { '\r' };
        SLPacket have = {
            .type = Listen,
            .data = 0,
            .cmd = 'S',
            .frame = {
                .id = 0,
                .length = 0,
                .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
                .ext = false,
                .rtr = false,
                .timestamp = 0
            }
        };
        uint8_t got[sizeof(expect) * 2];
        uint16_t ret, retexpect;
        uint8_t i;
        ret = encodeSLPacket(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %u got %u", retexpect, ret);
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
