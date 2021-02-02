#ifndef TEST_H
#define TEST_H

#include "fct.h"

void TestInit(void);

#define CheckFrame(got, expect, iter) \
    fct_xchk(got.rtr == expect.rtr, "rtr: Expected %s got %s", expect.rtr ? "TRUE" : "FALSE", got.rtr ? "TRUE" : "FALSE"); \
    fct_xchk(got.ext == expect.ext, "ext: Expected %s got %s", expect.ext ? "TRUE" : "FALSE", got.ext ? "TRUE" : "FALSE"); \
    fct_xchk(got.id == expect.id, "id: Expected %u got %u", expect.id, got.id); \
    fct_xchk(got.length == expect.length, "length: Expected %u got %u", expect.length, got.length); \
    fct_xchk(got.timestamp == expect.timestamp, "id: Expected %u got %u", expect.timestamp, got.timestamp); \
    for (iter = 0; iter < got.length; iter++) { \
        fct_xchk(got.data[iter] == expect.data[iter], "data[%u]: Expected %u got %u", iter, expect.data[iter], got.data[iter]); \
    }

#define CheckSLPacket(got, expect, iter) \
    fct_xchk(got.type == expect.type, "type: Expected %u got %u", expect.type, got.type); \
    fct_xchk(got.data == expect.data, "data: Expected %u got %u", expect.data, got.data); \
    fct_xchk(got.cmd == expect.cmd, "cmd: Expected %c got %c", expect.cmd, got.cmd); \
    CheckFrame(got.frame, expect.frame, iter)

#define CheckBuffer(got, expect, iter) \
    for (iter = 0; iter < sizeof(expect); iter++) { \
        fct_xchk(expect[iter] == got[iter], "Index %u: Expected %u got %u", iter, expect[iter], got[iter]); \
    }

#endif

