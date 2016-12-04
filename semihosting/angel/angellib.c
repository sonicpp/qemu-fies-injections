/*
 * angellib.c - Angel interface library.
 *
 * Copyright (C) 2016 Jan Havran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "angel.h"
#include "angellib.h"

void aexit(int status)
{
	ANGEL_CALL(angel_SWIreason_ReportException,
		(void *) ADP_Stopped_ApplicationExit);
}

