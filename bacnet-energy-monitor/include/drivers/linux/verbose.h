/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdio.h>
#include <stdarg.h>

#define PSTR(s)((const char *)(s))

#ifdef __cplusplus
extern "C" {
#endif

	void VERBOSE_STRING(const char * str);
	void VERBOSE_STRING_INT(const char * str, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
