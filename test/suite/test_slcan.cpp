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

#define CheckFrame(got, expect, iter) \
    fct_xchk(got.rtr == expect.rtr, "rtr: Expected %s got %s", expect.rtr ? "TRUE" : "FALSE", got.rtr ? "TRUE" : "FALSE"); \
    fct_xchk(got.ext == expect.ext, "ext: Expected %s got %s", expect.ext ? "TRUE" : "FALSE", got.ext ? "TRUE" : "FALSE"); \
    fct_xchk(got.id == expect.id, "id: Expected %u got %u", expect.id, got.id); \
    fct_xchk(got.length == expect.length, "length: Expected %u got %u", expect.length, got.length); \
    fct_xchk(got.timestamp == expect.timestamp, "id: Expected %u got %u", expect.timestamp, got.timestamp); \
    for (iter = 0; iter < got.length; iter++) { \
        fct_xchk(got.data[iter] == expect.data[iter], "data[%u]: Expected %u got %u", iter, expect.data[iter], got.data[iter]); \
    }

#define CheckSLCommand(got, expect) \
    fct_xchk(got.type == expect.type, "type: Expected %u got %u", expect.type, got.type); \
    fct_xchk(got.data == expect.data, "data: Expected %u got %u", expect.data, got.data); \
    fct_xchk(got.cmd == expect.cmd, "cmd: Expected %u got %u", expect.cmd, got.cmd);

#define CheckBuffer(got, expect, iter) \
    for (iter = 0; iter < sizeof(expect); iter++) { \
        fct_xchk(expect[iter] == got[iter], "Index %u: Expected %u got %u", iter, expect[iter], got[iter]); \
    }

uint8_t parseDigit(uint8_t digit);
uint8_t encodeDigit(uint8_t digit);
uint32_t parseNumber(uint8_t *buf, uint8_t length);
bool parseByteData(uint8_t *buf, uint8_t length, uint8_t *data);

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
    /************************************************** parseDigit() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '0') {
        uint8_t ret, expect;
        ret = parseDigit('0');
        expect = 0;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '1') {
        uint8_t ret, expect;
        ret = parseDigit('1');
        expect = 1;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '2') {
        uint8_t ret, expect;
        ret = parseDigit('2');
        expect = 2;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '3') {
        uint8_t ret, expect;
        ret = parseDigit('3');
        expect = 3;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '4') {
        uint8_t ret, expect;
        ret = parseDigit('4');
        expect = 4;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '5') {
        uint8_t ret, expect;
        ret = parseDigit('5');
        expect = 5;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '6') {
        uint8_t ret, expect;
        ret = parseDigit('6');
        expect = 6;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '7') {
        uint8_t ret, expect;
        ret = parseDigit('7');
        expect = 7;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '8') {
        uint8_t ret, expect;
        ret = parseDigit('8');
        expect = 8;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for '9') {
        uint8_t ret, expect;
        ret = parseDigit('9');
        expect = 9;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'A') {
        uint8_t ret, expect;
        ret = parseDigit('A');
        expect = 0xA;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'B') {
        uint8_t ret, expect;
        ret = parseDigit('B');
        expect = 0xB;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'C') {
        uint8_t ret, expect;
        ret = parseDigit('C');
        expect = 0xC;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'D') {
        uint8_t ret, expect;
        ret = parseDigit('D');
        expect = 0xD;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'E') {
        uint8_t ret, expect;
        ret = parseDigit('E');
        expect = 0xE;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'F') {
        uint8_t ret, expect;
        ret = parseDigit('F');
        expect = 0xF;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'a') {
        uint8_t ret, expect;
        ret = parseDigit('a');
        expect = 0xA;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'b') {
        uint8_t ret, expect;
        ret = parseDigit('b');
        expect = 0xB;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'c') {
        uint8_t ret, expect;
        ret = parseDigit('c');
        expect = 0xC;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'd') {
        uint8_t ret, expect;
        ret = parseDigit('d');
        expect = 0xD;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'e') {
        uint8_t ret, expect;
        ret = parseDigit('e');
        expect = 0xE;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns correctly for 'f') {
        uint8_t ret, expect;
        ret = parseDigit('f');
        expect = 0xF;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns 0 for '\r') {
        uint8_t ret, expect;
        ret = parseDigit('\r');
        expect = 0;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseDigit: returns 0 for '\0') {
        uint8_t ret, expect;
        ret = parseDigit('\0');
        expect = 0;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
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
    /************************************************** parseNumber() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseNumber: returns correctly for '123') {
        uint8_t have[] = { '1', '2', '3' };
        uint32_t ret, expect;
        ret = parseNumber((uint8_t *)have, sizeof(have));
        expect = 0x123;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseNumber: returns correctly for a buffer too big) {
        uint8_t have[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        uint32_t ret, expect;
        ret = parseNumber((uint8_t *)have, sizeof(have));
        expect = 0x12345678;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseNumber: returns correctly for a buffer length 0) {
        uint8_t have[] = {  };
        uint32_t ret, expect;
        ret = parseNumber((uint8_t *)have, sizeof(have));
        expect = 0;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseNumber: returns correctly for a NULL buffer) {
        uint32_t ret, expect;
        ret = parseNumber(NULL, 5);
        expect = 0;
        fct_xchk(ret == expect, "Expected %d got %d", expect, ret);
    }
    FCT_TEST_END()

    /************************************************** parseByteData() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseByteData: returns correctly for '1234') {
        uint8_t have[] = { '1', '2', '3', '4' };
        uint8_t expect[] = { 0x12, 0x34 };
        uint8_t got[sizeof(have)];
        bool ret, retexpect = true;
        uint8_t i;
        memset(got, 0, sizeof(got));
        ret = parseByteData((uint8_t *)have, sizeof(have), got);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        for (i = 0; i < sizeof(expect); i++) {
            fct_xchk(got[i] == expect[i], "Element %u: Expected %u got %u", i, expect[i], got[i]);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseByteData: deals with an odd number of characters) {
        uint8_t have[] = { '1', '2', '3', '4', '5' };
        uint8_t expect[] = { 0x12, 0x34 };
        uint8_t got[sizeof(have)];
        bool ret, retexpect = true;
        uint8_t i;
        memset(got, 0, sizeof(got));
        ret = parseByteData((uint8_t *)have, sizeof(have), got);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        for (i = 0; i < sizeof(expect); i++) {
            fct_xchk(got[i] == expect[i], "Element %u: Expected %u got %u", i, expect[i], got[i]);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseByteData: deals with a NULL buffer) {
        uint8_t got[10];
        bool ret, retexpect = false;
        ret = parseByteData(NULL, sizeof(got), got);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseByteData: deals with a NULL data array) {
        uint8_t got[] = { '1', '2', '3', '4' };
        bool ret, retexpect = false;
        ret = parseByteData(got, sizeof(got), NULL);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()

    /************************************************** parseCANFrame() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when length is 0) {
        uint8_t buffer[] = {};
        CANFrame frame;
        bool ret, expect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when length is 4) {
        uint8_t buffer[] = { 'T', '1', '2', '3' };
        CANFrame frame;
        bool ret, expect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when buffer is NULL) {
        CANFrame frame;
        bool ret, expect;
        ret = parseCANFrame(NULL, 5, &frame);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when frame is NULL) {
        uint8_t buffer[] = { '1', '2' };
        bool ret, expect;
        ret = parseCANFrame(buffer, sizeof(buffer), NULL);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: normal packet parses properly) {
        uint8_t buffer[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        CANFrame expect = {
            .id = 0x123,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = false,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: extended packet parses properly) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        CANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: normal RTR packet parses properly) {
        uint8_t buffer[] = { 'r', '1', '2', '3', '8', '\r' };
        CANFrame expect = {
            .id = 0x123,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = false,
            .rtr = true,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: extended RTR packet parses properly) {
        uint8_t buffer[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8', '8', '\r' };
        CANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = true,
            .rtr = true,
            .timestamp = 0
        };
        CANFrame frame;
        bool ret, retexpect;
        uint8_t i;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with extended packets that are too short) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7' };
        CANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with normal packets that are too short) {
        uint8_t buffer[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7' };
        CANFrame expect = {
            .id = 0x123,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = false,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with extended packets that are too short) {
        uint8_t buffer[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8' };
        CANFrame frame;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with normal packets that are too short) {
        uint8_t buffer[] = { 'r', '1', '2', '3' };
        CANFrame frame;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with extended packets that are too short w/ \\r) {
        uint8_t buffer[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '\r' };
        CANFrame expect = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with normal packets that are too short w/ \\r) {
        uint8_t buffer[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '\r' };
        CANFrame expect = {
            .id = 0x123,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = false,
            .rtr = false,
            .timestamp = 0
        };
        CANFrame frame;
        uint8_t i;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckFrame(frame, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with extended packets that are too short w/ \\r) {
        uint8_t buffer[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8', '\r' };
        CANFrame frame;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: deals with normal packets that are too short w/ \\r) {
        uint8_t buffer[] = { 'r', '1', '2', '3', '\r' };
        CANFrame frame;
        bool ret, retexpect;
        ret = parseCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /************************************************** parseSLCommand() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when length is 0) {
        uint8_t buffer[] = {};
        SLCommand cmd;
        bool ret, expect;
        ret = parseSLCommand(buffer, sizeof(buffer), &cmd);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when buffer is NULL) {
        SLCommand cmd;
        bool ret, expect;
        ret = parseSLCommand(NULL, 5, &cmd);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when cmd is NULL) {
        uint8_t buffer[] = {};
        bool ret, expect;
        ret = parseSLCommand(buffer, sizeof(buffer), NULL);
        expect = false;
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when bad command is given) {
        uint8_t buffer[] = { 'Q', '\r' };
        SLCommand expect = {
            .type = Bad,
            .data = 0,
            .cmd = 'Q'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns true when open command is given) {
        uint8_t buffer[] = { 'O', '\r' };
        SLCommand expect = {
            .type = Open,
            .data = 0,
            .cmd = 'O'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns true when close command is given) {
        uint8_t buffer[] = { 'C', '\r' };
        SLCommand expect = {
            .type = Close,
            .data = 0,
            .cmd = 'C'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns true when listen command is given) {
        uint8_t buffer[] = { 'L', '\r' };
        SLCommand expect = {
            .type = Listen,
            .data = 0,
            .cmd = 'L'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns false when bad speed command is given) {
        uint8_t buffer[] = { 'S', '\r' };
        SLCommand expect = {
            .type = Bad,
            .data = 0,
            .cmd = 'S'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = false;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(parseCANFrame: returns true when speed command is given) {
        uint8_t buffer[] = { 'S', '1', '\r' };
        SLCommand expect = {
            .type = Speed,
            .data = 1,
            .cmd = 'S'
        };
        SLCommand got;
        bool ret, retexpect;
        ret = parseSLCommand(buffer, sizeof(buffer), &got);
        retexpect = true;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckSLCommand(got, expect);
    }
    FCT_TEST_END()
    /************************************************** encodeCANFrame() ******************************************************/
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: returns 0 when length is 0) {
        uint8_t buffer[] = {};
        CANFrame frame;
        uint8_t ret, retexpect;
        ret = encodeCANFrame(buffer, sizeof(buffer), &frame);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: returns 0 when buffer is NULL) {
        CANFrame frame;
        uint8_t ret, retexpect;
        ret = encodeCANFrame(NULL, 5, &frame);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: returns 0 when frame is NULL) {
        uint8_t buffer[] = {};
        uint8_t ret, retexpect;
        ret = encodeCANFrame(buffer, sizeof(buffer), NULL);
        retexpect = 0;
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: normal packet parses properly) {
        uint8_t expect[] = { 't', '1', '2', '3', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        CANFrame have = {
            .id = 0x123,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = false,
            .rtr = false,
            .timestamp = 0
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint8_t ret, retexpect;
        ret = encodeCANFrame(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: extended packet parses properly) {
        uint8_t expect[] = { 'T', '1', '2', '3', '4', '5', '6', '7', '8', '8', '0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '\r' };
        CANFrame have = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0x0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 },
            .ext = true,
            .rtr = false,
            .timestamp = 0
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint8_t ret, retexpect;
        ret = encodeCANFrame(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: normal RTR packet parses properly) {
        uint8_t expect[] = { 'r', '1', '2', '3', '8', '\r' };
        CANFrame have = {
            .id = 0x123,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = false,
            .rtr = true,
            .timestamp = 0
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t i;
        uint8_t ret, retexpect;
        ret = encodeCANFrame(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(encodeCANFrame: extended RTR packet parses properly) {
        uint8_t expect[] = { 'R', '1', '2', '3', '4', '5', '6', '7', '8', '8', '\r' };
        CANFrame have = {
            .id = 0x12345678,
            .length = 8,
            .data = { 0, 0, 0, 0, 0, 0, 0, 0 },
            .ext = true,
            .rtr = true,
            .timestamp = 0
        };
        uint8_t got[sizeof(expect) * 2];
        uint8_t ret, retexpect;
        uint8_t i;
        ret = encodeCANFrame(got, sizeof(got), &have);
        retexpect = sizeof(expect);
        fct_xchk(ret == retexpect, "Expected %s got %s", retexpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        CheckBuffer(got, expect, i);
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
