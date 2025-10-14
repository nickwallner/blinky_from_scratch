#include "drivers/include/nvic.h"

/* enable a irq in the nvic */
/* same implementation as ARM CMSIS for cortex m4 */
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    }
}