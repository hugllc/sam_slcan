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
    /************************************************** isPacket() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isPacket: Returns true with a good packet) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '\r' };
        bool ret, expect;
        uint8_t i;
        for (i = 0; i < sizeof(buffer); i++) {
            ret = isPacket(buffer, i + 1);
            expect = (i == (sizeof(buffer) - 1));
            fct_xchk(ret == expect, "Iteration %d:  Expected %s got %s", i, expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isPacket: Deals with an empty buffer) {
        uint8_t buffer[16];
        bool ret, expect;
        memset(buffer, 0, sizeof(buffer));
        ret = isPacket(buffer, 0);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isPacket: Deals with a NULL buffer) {
        bool ret, expect;
        ret = isPacket(NULL, 5);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

    /************************************************** isCANFrame() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: Deals with an empty buffer) {
        uint8_t buffer[6];
        bool ret, expect;
        memset(buffer, 0, sizeof(buffer));
        ret = isCANFrame(buffer, 0);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: Deals with a NULL buffer) {
        bool ret, expect;
        ret = isCANFrame(NULL, 5);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: returns true for a packet that starts with T) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '\r' };
        bool ret, expect;
        ret = isCANFrame(buffer, sizeof(buffer));
        expect = true;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: returns true for a packet that starts with t) {
        uint8_t buffer[] = { 't', '1', '2', '3', '4', '\r' };
        bool ret, expect;
        ret = isCANFrame(buffer, sizeof(buffer));
        expect = true;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: returns true for a packet that starts with R) {
        uint8_t buffer[] = { 'R', '1', '2', '3', '4', '\r' };
        bool ret, expect;
        ret = isCANFrame(buffer, sizeof(buffer));
        expect = true;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(isCANFrame: returns true for a packet that starts with r) {
        uint8_t buffer[] = { 'r', '1', '2', '3', '4', '\r' };
        bool ret, expect;
        ret = isCANFrame(buffer, sizeof(buffer));
        expect = true;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
