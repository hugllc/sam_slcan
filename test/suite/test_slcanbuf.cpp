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
#include "slcanbuf.h"

FCTMF_FIXTURE_SUITE_BGN(test_slcanbuf)
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
    FCT_TEST_BGN(slcanbuf: initialized buffer is empty) {
        SLCanBuf cbuf;
        bool ret, expect = true;
        slcanbuf_init(&cbuf);
        ret = slcanbuf_isEmpty(&cbuf);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcanbuf: takes bytes in and gives them out) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '\r' };
        SLCanBuf cbuf;
        uint8_t ret, expect;
        uint16_t i;
        slcanbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            slcanbuf_push(&cbuf, buffer[i]);
        }
        for (i = 0; i < sizeof(buffer); i++) {
            ret = slcanbuf_pop(&cbuf);
            expect = buffer[i];
            fct_xchk(ret == expect, "Iteration %d:  Expected %c got %c", i, expect, ret);
        }
    }
    FCT_TEST_END()
     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcanbuf: buffer can be overfilled) {
        SLCanBuf cbuf;
        uint8_t ret, expect;
        uint32_t i;
        uint16_t count = (sizeof(cbuf) * 4);
        uint32_t start = count - sizeof(cbuf.data) + 1;
        slcanbuf_init(&cbuf);
        for (i = 0; i < count; i++) {
            slcanbuf_push(&cbuf, i);
        }
        for (i = start; i < count; i++) {
            ret = slcanbuf_pop(&cbuf);
            expect = (uint8_t)i;
            fct_xchk(ret == expect, "Iteration %d:  Expected %u got %u", i, expect, ret);
        }
    }
    FCT_TEST_END()
     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcanbuf: popping an empty buffer returns 0) {
        SLCanBuf cbuf;
        uint8_t ret, expect;
        uint32_t i;
        slcanbuf_init(&cbuf);
        for (i = 0; i < sizeof(cbuf.data); i++) {
            slcanbuf_push(&cbuf, i);
        }
        cbuf.tail = cbuf.head; // Force the buffer empty
        for (i = 0; i < sizeof(cbuf.data); i++) {
            ret = slcanbuf_pop(&cbuf);
            expect = 0;
            fct_xchk(ret == expect, "Iteration %d:  Expected %u got %u", i, expect, ret);
        }
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcanbuf: retrieves a packet from the buffer) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '\r' };
        uint8_t extra[] = { 'T', '1', '2' };
        uint8_t got[sizeof(buffer) * 2];
        SLCanBuf cbuf;
        uint16_t ret;
        uint16_t i;
        bool bret, bexpect;
        slcanbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            slcanbuf_push(&cbuf, buffer[i]);
        }
        for (i = 0; i < sizeof(extra); i++) {
            slcanbuf_push(&cbuf, extra[i]);
        }
        bret = slcanbuf_hasPacket(&cbuf);
        bexpect = true;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");

        ret = slcanbuf_getPacket(&cbuf, got, sizeof(got));
        fct_xchk(ret == sizeof(buffer), "Expected %u bytes returned got %u", sizeof(buffer), ret);
        for (i = 0; i < ret; i++) {
            fct_xchk(got[i] == buffer[i], "Iteration %d:  Expected %c got %c", i, buffer[i], got[i]);
        }
        bret = slcanbuf_hasPacket(&cbuf);
        bexpect = true;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(slcanbuf: returns 0 length if no packet is in the buffer) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6' };
        uint8_t got[sizeof(buffer) * 2];
        SLCanBuf cbuf;
        uint16_t ret, expect = 0;
        uint16_t i;
        bool bret, bexpect;
        slcanbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            slcanbuf_push(&cbuf, buffer[i]);
        }
        bret = slcanbuf_hasPacket(&cbuf);
        bexpect = false;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = slcanbuf_getPacket(&cbuf, got, sizeof(got));
        fct_xchk(ret == expect, "Expected %u bytes returned got %u", expect, ret);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
