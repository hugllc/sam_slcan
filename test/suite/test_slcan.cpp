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
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(Returns true with a good packet) {
        uint8_t buffer[16];
        const char *str = "T123456\r";
        bool ret, expect;
        uint8_t i;
        uint8_t len = strlen(str);
        memset(buffer, 0, sizeof(buffer));
        for (i = 0; i < len; i++) {
            buffer[i] = str[i];
            ret = isPacket(buffer, i + 1);
            expect = (i == (len - 1));
            fct_xchk(ret == expect, "Iteration %d:  Expected %s got %s", i, expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(Deals with an empty buffer) {
        uint8_t buffer[16];
        bool ret, expect;
        memset(buffer, 0, sizeof(buffer));
        ret = isPacket(buffer, 0);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
}
FCTMF_FIXTURE_SUITE_END();
