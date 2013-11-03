/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>


void VERBOSE_STRING(PGM_P str){
#if defined(VERBOSE) //Only writes if bacnet stack is in verbose mode
	fprintf_P(stderr, str);
#endif
}

void VERBOSE_STRING_INT(PGM_P str, ...){
#if defined(VERBOSE)
	va_list ap;
	va_start(ap, str);
	vfprintf_P(stderr, str, ap);
	va_end(ap);
#endif
}

