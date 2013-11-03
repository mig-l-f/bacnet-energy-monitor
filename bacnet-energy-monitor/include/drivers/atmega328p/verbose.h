/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef VERBOSE_H
#define VERBOSE_H

#include <avr/pgmspace.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	void VERBOSE_STRING(PGM_P str);
	void VERBOSE_STRING_INT(PGM_P str, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
