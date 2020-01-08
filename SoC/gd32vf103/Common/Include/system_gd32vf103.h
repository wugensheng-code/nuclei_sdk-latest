/**************************************************************************//**
 * @file     system_<Device>.h
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Header File for
 *           Device <Device>
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYSTEM_GD32VF103_H__   /* ToDo: replace '<Device>' with your device name */
#define __SYSTEM_GD32VF103_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

/** 
 * \brief       Register an exception handler for exception code EXCn
 */
extern void register_core_exception_handler(uint32_t EXCn, unsigned long exc_handler);

/** 
 * \brief       Get current exception handler for exception code EXCn
 */
extern unsigned long get_core_exception_handler(uint32_t EXCn);

/**
 * \brief initialize eclic config
 */
extern void eclic_init(void);

/**
 * \brief  initialize a specific IRQ and register the handler
 * \details
 * This function set vector mode, trigger mode and polarity, interrupt level and priority,
 * assign handler for specific IRQn.
 */
extern int ECLIC_Register_IRQ(IRQn_Type IRQn, uint8_t shv, ECLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, void *handler);

/**
 *  \brief      delay a time in milliseconds
 *  \param[in]  count: count in milliseconds
 *  \param[out] none
 *  \retval     none
 */
extern void delay_1ms(uint32_t count);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_GD32VF103_H__ */
