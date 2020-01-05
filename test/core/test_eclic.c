#include <stdlib.h>
#include "ctest.h"
#include "nuclei_sdk_soc.h"


static volatile uint32_t irqTaken = 0;
/**
 * \brief Test case: TC_CoreFunc_EnDisIRQ
 * \details
 * Check expected behavior of interrupt related control functions:
 * - __disable_irq() and __enable_irq()
 * - ECLIC_EnableIRQ, ECLIC_DisableIRQ,  and ECLIC_GetEnableIRQ
 * - ECLIC_SetPendingIRQ, ECLIC_ClearPendingIRQ, and ECLIC_GetPendingIRQ
 */
void eclic_irq19_handler() {
    irqTaken++;
}

CTEST(eclic, en_dis_irq)
{
    // Globally disable all interrupt servicing
    __disable_irq();

    ECLIC_SetShvIRQ(SOC_INT19_IRQn, ECLIC_NON_VECTOR_INTERRUPT);
    ASSERT_EQUAL(ECLIC_GetShvIRQ(SOC_INT19_IRQn), ECLIC_NON_VECTOR_INTERRUPT);

    ECLIC_SetTrigIRQ(SOC_INT19_IRQn, ECLIC_POSTIVE_EDGE_TRIGGER);
    ASSERT_EQUAL(ECLIC_GetTrigIRQ(SOC_INT19_IRQn), ECLIC_POSTIVE_EDGE_TRIGGER);

    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, 1);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn), 1);

    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, 0);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn), 0);

    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, 0);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn), 0);

    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, 1);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn), 1);

    // Clear its pending state
    ECLIC_ClearPendingIRQ(SOC_INT19_IRQn);
    ASSERT_EQUAL(ECLIC_GetPendingIRQ(SOC_INT19_IRQn), 0);

    // Register test interrupt handler.
    ECLIC_SetVector(SOC_INT19_IRQn, (rv_csr_t)eclic_irq19_handler);
    CTEST_LOG("Register irq vector 0x%x vs 0x%x", ECLIC_GetVector(SOC_INT19_IRQn), (rv_csr_t)eclic_irq19_handler);
    ASSERT_EQUAL(ECLIC_GetVector(SOC_INT19_IRQn), (rv_csr_t)eclic_irq19_handler);

    // Enable the interrupt
    ECLIC_EnableIRQ(SOC_INT19_IRQn);
    ASSERT_EQUAL(ECLIC_GetEnableIRQ(SOC_INT19_IRQn), 1);

    // Set the interrupt pending state
    ECLIC_SetPendingIRQ(SOC_INT19_IRQn);
    for (uint32_t i = 10; i > 0; --i) {}

    // Interrupt is not taken
    ASSERT_EQUAL(irqTaken, 0);
    ASSERT_EQUAL(ECLIC_GetPendingIRQ(SOC_INT19_IRQn), 1);

    // Globally enable interrupt servicing
    __enable_irq();

    for (uint32_t i = 1000; i > 0; --i) {}

    // Interrupt was taken
    ASSERT_EQUAL(irqTaken, 1);

    // Interrupt it not pending anymore.
    ASSERT_EQUAL(ECLIC_GetPendingIRQ(SOC_INT19_IRQn), 0);

    // Disable interrupt
    ECLIC_DisableIRQ(SOC_INT19_IRQn);
    ASSERT_EQUAL(ECLIC_GetEnableIRQ(SOC_INT19_IRQn), 0);

    // Set interrupt pending
    ECLIC_SetPendingIRQ(SOC_INT19_IRQn);
    for (uint32_t i = 10; i > 0; --i) {
    }

    // Interrupt is not taken again
    ASSERT_EQUAL(irqTaken, 1);
    ASSERT_EQUAL(ECLIC_GetPendingIRQ(SOC_INT19_IRQn), 1);

    // Clear interrupt pending
    ECLIC_ClearPendingIRQ(SOC_INT19_IRQn);
    for (uint32_t i = 10; i > 0; --i) {
    }

    // Interrupt it not pending anymore.
    ASSERT_EQUAL(ECLIC_GetPendingIRQ(SOC_INT19_IRQn), 0);

    // Globally disable interrupt servicing
    __disable_irq();
}

void get_max_lvl_pri(uint8_t *maxpri, uint8_t *maxlvl)
{
    uint8_t nlbits = __ECLIC_GetCfgNlbits();
    uint8_t intctlbits = (uint8_t)__ECLIC_INTCTLBITS;
    *maxpri = 0;
    *maxlvl = 0;

    if (nlbits < intctlbits) {
        *maxpri = ((1 << (intctlbits - nlbits)) - 1);
    }
    if (nlbits > 0) {
        *maxlvl = (1 << nlbits) - 1;
    }
}
/**
 * \brief Test case: eclic_LvlPri
 * \details
 * Check expected behavior of interrupt level and priority control functions:
 * - ECLIC_SetPriorityIRQ, ECLIC_GetPriorityIRQ,
 * - ECLIC_SetLevelIRQ, ECLIC_GetLevelIRQ
 */
CTEST(eclic, lvl_pri_irq)
{
    uint8_t oldnlbits = __ECLIC_GetCfgNlbits();
    uint8_t maxlvl, maxpri;
    uint8_t lvlnew, prinew;

    get_max_lvl_pri(&maxpri, &maxlvl);

    if (maxlvl == 0) {
        lvlnew = 0;
    } else {
        lvlnew = rand() % maxlvl;
    }
    if (maxpri == 0) {
        prinew = 0;
    } else {
        prinew = rand() % maxpri;
    }

    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, prinew);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn), prinew);

    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, lvlnew);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn), lvlnew);

    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, 255);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn), maxpri);

    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, 255);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn), maxlvl);

    ECLIC_SetCfgNlbits(3);
    get_max_lvl_pri(&maxpri, &maxlvl);
    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, 15);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn), maxlvl);
    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, 31);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn), maxpri);
    ECLIC_SetCfgNlbits(2);
    get_max_lvl_pri(&maxpri, &maxlvl);
    ECLIC_SetLevelIRQ(SOC_INT19_IRQn, 2);
    ECLIC_SetPriorityIRQ(SOC_INT19_IRQn, 5);
    ASSERT_EQUAL(ECLIC_GetLevelIRQ(SOC_INT19_IRQn) <= maxlvl, 1);
    ASSERT_EQUAL(ECLIC_GetPriorityIRQ(SOC_INT19_IRQn) <= maxpri, 1);

    ECLIC_SetCfgNlbits(oldnlbits);
    ASSERT_EQUAL(ECLIC_GetCfgNlbits(), oldnlbits);
}

/**
 * \brief Test case: eclic_register
 * \details
 * Check expected behavior of eclic register read/write:
 * - ECLIC_SetPriorityIRQ, ECLIC_GetPriorityIRQ,
 * - ECLIC_SetLevelIRQ, ECLIC_GetLevelIRQ
 */
CTEST(eclic, reg_read_write)
{
    uint32_t orig = ECLIC_GetCfgNlbits();
    ECLIC_SetCfgNlbits(__ECLIC_INTCTLBITS - 1);
    ASSERT_EQUAL(ECLIC_GetCfgNlbits(), __ECLIC_INTCTLBITS - 1);
    ECLIC_SetCfgNlbits(orig);

    orig = ECLIC_GetMth();
    ECLIC_SetMth(orig + 1);
    ASSERT_EQUAL(ECLIC_GetMth(), orig);
    ECLIC_SetMth(orig);

    ECLIC_GetTrigIRQ(SOC_INT19_IRQn);
    ECLIC_SetTrigIRQ(SOC_INT19_IRQn, ECLIC_NEGTIVE_EDGE_TRIGGER);
    ASSERT_EQUAL(ECLIC_GetTrigIRQ(SOC_INT19_IRQn), ECLIC_NEGTIVE_EDGE_TRIGGER);

    CTEST_LOG("CLICINTCTLBITS : %d", ECLIC_GetInfoCtlbits());
    CTEST_LOG("CLIC VERSION : 0x%x", ECLIC_GetInfoVer());
    CTEST_LOG("NUM_INTERRUPT : %d", ECLIC_GetInfoNum());
}

CTEST(eclic, nmi_entry) {
    ASSERT_EQUAL(__get_nmi_entry(), __RV_CSR_READ(CSR_MTVEC));
}

void new_nonvec_entry(void)
{

}

CTEST(eclic, nonvec_entry) {
    rv_csr_t entry = __get_nonvec_entry();
    CTEST_LOG("Non vector entry is 0x%x", entry);

    __disable_irq();
    __set_nonvec_entry((rv_csr_t)new_nonvec_entry);
    //CTEST_LOG("Non vector entry is 0x%x, 0x%x", __Get_Nonvec_Entry(), new_nonvec_entry);
    ASSERT_EQUAL(__get_nonvec_entry(), (rv_csr_t)new_nonvec_entry & (rv_csr_t)(~0x3));

    __set_nonvec_entry((rv_csr_t)entry);
    ASSERT_EQUAL(__get_nonvec_entry(), entry);
    //__enable_irq();

}

void new_exc_entry(void)
{

}

CTEST(eclic, exc_entry) {
    rv_csr_t entry = __get_exc_entry();
    CTEST_LOG("Exception entry is 0x%x", entry);
    __set_exc_entry((rv_csr_t)new_exc_entry);
    CTEST_LOG("Exception set entry is 0x%x, 0x%x", __get_exc_entry(), (rv_csr_t)new_exc_entry);

    ASSERT_EQUAL(__get_exc_entry(), (rv_csr_t)new_exc_entry & (rv_csr_t)(~0x3F));

    __set_exc_entry((rv_csr_t)entry);

    ASSERT_EQUAL(__get_exc_entry(), entry);
}
