#include "main.h"
#include "stdint.h"

uint32_t reboot;

FCT_BGN()
{
    FCTMF_SUITE_CALL(test_slcanbuf);
    FCTMF_SUITE_CALL(test_slcan);
}
FCT_END();


void TestInit(void)
{
    reboot = 0;
}
