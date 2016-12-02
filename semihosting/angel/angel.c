/*
 * angel.c - Angel interface.
 *
 * Copyright (C) 2016 Jan Havran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "angel.h"

void ANGEL_CALL(enum ANGEL_SWI type, void *param)
{
	__attribute__ ((unused)) register int reg0 asm("r0");
	__attribute__ ((unused)) register int reg1 asm("r1");

	reg0 = type;
	reg1 = (int) param;

	asm("svc 0x00123456");
}

