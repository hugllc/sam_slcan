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
#include "circbuf.h"

FCTMF_FIXTURE_SUITE_BGN(test_circbuf)
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
    FCT_TEST_BGN(circbuf: initialized buffer is empty) {
        CircBuf cbuf;
        bool ret, expect = true;
        circbuf_init(&cbuf);
        ret = circbuf_isEmpty(&cbuf);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(circbuf: takes bytes in and gives them out) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '\r' };
        CircBuf cbuf;
        uint8_t ret, expect;
        uint16_t i;
        circbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            circbuf_push(&cbuf, buffer[i]);
        }
        for (i = 0; i < sizeof(buffer); i++) {
            ret = circbuf_pop(&cbuf);
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
    FCT_TEST_BGN(circbuf: buffer can be overfilled) {
        CircBuf cbuf;
        uint8_t ret, expect;
        uint32_t i;
        uint16_t count = (sizeof(cbuf) * 4);
        uint32_t start = count - sizeof(cbuf.data) + 1;
        circbuf_init(&cbuf);
        for (i = 0; i < count; i++) {
            circbuf_push(&cbuf, i);
        }
        for (i = start; i < count; i++) {
            ret = circbuf_pop(&cbuf);
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
    FCT_TEST_BGN(circbuf: head pushes tail when the buffer is overfilled) {
        CircBuf cbuf;
        uint8_t expect;
        uint32_t i;
        uint16_t count = sizeof(cbuf.data);
        circbuf_init(&cbuf);
        for (i = 0; i < count; i++) {
            circbuf_push(&cbuf, i);
        }
        expect = 21;
        circbuf_push(&cbuf, expect);
        fct_xchk((cbuf.head + 1) == cbuf.tail, "Pointer: Expected %u got %u", (cbuf.head + 1), cbuf.tail);
        fct_xchk(cbuf.data[cbuf.head - 1] == expect, "Expected %u got %u", expect, cbuf.data[cbuf.head - 1]);
        fct_xchk(cbuf.data[cbuf.tail] == cbuf.tail, "Expected %u got %u", cbuf.tail, cbuf.data[cbuf.tail]);
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(circbuf: popping an empty buffer returns 0) {
        CircBuf cbuf;
        uint8_t ret, expect;
        uint32_t i;
        circbuf_init(&cbuf);
        for (i = 0; i < sizeof(cbuf.data); i++) {
            circbuf_push(&cbuf, i);
        }
        cbuf.tail = cbuf.head; // Force the buffer empty
        for (i = 0; i < sizeof(cbuf.data); i++) {
            ret = circbuf_pop(&cbuf);
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
    FCT_TEST_BGN(circbuf: retrieves a packet from the buffer) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '\r' };
        uint8_t extra[] = { 'T', '1', '2' };
        uint8_t got[sizeof(buffer) * 2];
        CircBuf cbuf;
        uint16_t ret;
        uint16_t i;
        bool bret, bexpect;
        circbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            circbuf_push(&cbuf, buffer[i]);
        }
        for (i = 0; i < sizeof(extra); i++) {
            circbuf_push(&cbuf, extra[i]);
        }
        bret = circbuf_hasPacket(&cbuf);
        bexpect = true;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");

        ret = circbuf_getPacket(&cbuf, got, sizeof(got));
        fct_xchk(ret == sizeof(buffer), "Expected %u bytes returned got %u", sizeof(buffer), ret);
        for (i = 0; i < ret; i++) {
            fct_xchk(got[i] == buffer[i], "Iteration %d:  Expected %c got %c", i, buffer[i], got[i]);
        }
        bret = circbuf_hasPacket(&cbuf);
        bexpect = false;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(circbuf: returns 0 length if no packet is in the buffer) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6' };
        uint8_t got[sizeof(buffer) * 2];
        CircBuf cbuf;
        uint16_t ret, expect = 0;
        uint16_t i;
        bool bret, bexpect;
        circbuf_init(&cbuf);
        for (i = 0; i < sizeof(buffer); i++) {
            circbuf_push(&cbuf, buffer[i]);
        }
        bret = circbuf_hasPacket(&cbuf);
        bexpect = false;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = circbuf_getPacket(&cbuf, got, sizeof(got));
        fct_xchk(ret == expect, "Expected %u bytes returned got %u", expect, ret);
    }
    FCT_TEST_END()

     /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(circbuf: returns 0 length if buffer is empty) {
        uint8_t got[256];
        CircBuf cbuf;
        uint16_t ret, expect = 0;
        bool bret, bexpect;
        circbuf_init(&cbuf);
        bret = circbuf_hasPacket(&cbuf);
        bexpect = false;
        fct_xchk(bret == bexpect, "Expected %s got %s", bexpect ? "TRUE" : "FALSE", bret ? "TRUE" : "FALSE");
        ret = circbuf_getPacket(&cbuf, got, sizeof(got));
        fct_xchk(ret == expect, "Expected %u bytes returned got %u", expect, ret);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
