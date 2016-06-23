/*
 * Copyright (c) 2015 Wind River Systems, Inc.
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

#include <zephyr.h>
#include <tc_util.h>


void main(void)
{
	/* Declaring volatile so the compiler doesn't try to optimize any
	 * of the math away at build time
	 */
	volatile uint64_t bignum, ba, bb;
	volatile uint32_t num, a, b;

	int rv = TC_PASS;

	TC_START("Integer math functions");

	ba = 0x00000012ABCDEF12ULL;
	bb = 0x0000001000000111ULL;
	bignum = ba * bb;
	if (bignum != 0xbcdf0509369bf232ULL) {
		TC_PRINT("64-bit multiplication failed\n");
		rv = TC_FAIL;
	}

	a = 30000;
	b = 5872;
	num = a * b;
	if (num != 176160000) {
		TC_PRINT("32-bit multiplication failed\n");
		rv = TC_FAIL;
	}

	a = 234424432;
	b = 98982;
	num = a / b;
	if (num != 2368) {
		TC_PRINT("32-bit division failed\n");
		rv = TC_FAIL;
	}

	TC_END_RESULT(rv);
	TC_END_REPORT(rv);
}
