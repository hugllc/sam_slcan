#include "main.h"
#include <stdint.h>
#include "slcan.h"

uint32_t reboot;

FCT_BGN()
{
    FCTMF_SUITE_CALL(test_circbuf);
    FCTMF_SUITE_CALL(test_slcan_decode);
    FCTMF_SUITE_CALL(test_slcan_encode);
    FCTMF_SUITE_CALL(test_extra);
    FCTMF_SUITE_CALL(test_slcan);
}
FCT_END();


void TestInit(void)
{
    reboot = 0;
    slcan_init();
}
