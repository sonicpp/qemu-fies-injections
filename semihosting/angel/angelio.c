/*
 * angelio.c - I/O calls over Angel interface.
 *
 * Copyright (C) 2016 Jan Havran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <stdarg.h>

#include "angel.h"
#include "angelio.h"

#define BUF_SIZE	512

int aputchar(int c)
{
	ANGEL_CALL(SYS_WRITEC, (void *) &c);

	return 0;
}

int aputs(const char *s)
{
	char nl = '\n';

	ANGEL_CALL(SYS_WRITE0, (void *) s);
	ANGEL_CALL(SYS_WRITEC, (void *) &nl);

	return 0;
}

int aprintf(const char *format, ...)
{
	va_list args;
	char buf[BUF_SIZE];
	
	va_start(args, format);
	vsnprintf(buf, BUF_SIZE, format, args);
	va_end(args);

	ANGEL_CALL(SYS_WRITE0, (void *) buf);

	return 0;
}

