/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <kernel.h>
#include <device.h>
#include <init.h>
#include <soc.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/prcm.h>

static int ti_cc3200_init(struct device *arg)
{
	ARG_UNUSED(arg);

	PRCMCC3200MCUInit();

#ifdef CONFIG_UART_CC32XX
	/*
	 * Enable Peripheral Clocks, ensuring UART can wake the processor from
	 * idle (after ARM wfi instruction)
	 */
	MAP_PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK |
				    PRCM_SLP_MODE_CLK);
#endif

	return 0;
}

SYS_INIT(ti_cc3200_init, PRE_KERNEL_1, 0);
