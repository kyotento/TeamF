/*
Copyright (c) 2009-2020 Petri Lehtinen <petri@digip.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#ifndef JANSSON_CONFIG_H
#define JANSSON_CONFIG_H

/* Define this so that we can disable scattered automake configuration in source files */
#ifndef JANSSON_USING_CMAKE
#define JANSSON_USING_CMAKE
#endif

/* Note: when using cmake, JSON_INTEGER_IS_LONG_LONG is not defined nor used,
 * as we will also check for __int64 etc types.
 * (the definition was used in the automake system) */

/* Bring in the cmake-detected defines */
#define HAVE_STDINT_H 1
/* #undef HAVE_INTTYPES_H */
#define HAVE_SYS_TYPES_H 1

/* Include our standard type header for the integer typedef */

#if defined(HAVE_STDINT_H)
#  include <stdint.h>
#elif defined(HAVE_INTTYPES_H)
#  include <inttypes.h>
#elif defined(HAVE_SYS_TYPES_H)
#  include <sys/types.h>
#endif


/* If your compiler supports the inline keyword in C, JSON_INLINE is
   defined to `inline', otherwise empty. In C++, the inline is always
   supported. */
#ifdef __cplusplus
#define JSON_INLINE inline
#else
#define JSON_INLINE inline
#endif


#define json_int_t long long
#define json_strtoint strtoll
#define JSON_INTEGER_FORMAT "I64d"


/* If locale.h and localeconv() are available, define to 1, otherwise to 0. */
#define JSON_HAVE_LOCALECONV 1

/* If __atomic builtins are available they will be used to manage
   reference counts of json_t. */
#define JSON_HAVE_ATOMIC_BUILTINS 0

/* If __atomic builtins are not available we try using __sync builtins
   to manage reference counts of json_t. */
#define JSON_HAVE_SYNC_BUILTINS 0

/* Maximum recursion depth for parsing JSON input.
   This limits the depth of e.g. array-within-array constructions. */
#define JSON_PARSER_MAX_DEPTH 2048

#endif
