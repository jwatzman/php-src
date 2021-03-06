/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2014 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Sascha Schumann <sascha@schumann.cx>                        |
   |          Ard Biesheuvel <ard.biesheuvel@linaro.org>                  |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef ZEND_MULTIPLY_H
#define ZEND_MULTIPLY_H

#if defined(__i386__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__ ("imul %3,%0\n"											\
		"adc $0,%1" 												\
			: "=r"(__tmpvar),"=r"(usedval) 							\
			: "0"(a), "r"(b), "1"(0));								\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__x86_64__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__ ("imul %3,%0\n"											\
		"adc $0,%1" 												\
			: "=r"(__tmpvar),"=r"(usedval) 							\
			: "0"(a), "r"(b), "1"(0));								\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__arm__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__("smull %0, %1, %2, %3\n"								\
		"sub %1, %1, %0, asr #31"									\
			: "=r"(__tmpvar), "=r"(usedval)							\
			: "r"(a), "r"(b));										\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__aarch64__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__("mul %0, %2, %3\n"										\
		"smulh %1, %2, %3\n"										\
		"sub %1, %1, %0, asr #63\n"									\
			: "=X"(__tmpvar), "=X"(usedval)							\
			: "X"(a), "X"(b));										\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(ZEND_WIN32)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long   __lres  = (a) * (b);										\
	long double __dres  = (long double)(a) * (long double)(b);		\
	long double __delta = (long double) __lres - __dres;			\
	if ( ((usedval) = (( __dres + __delta ) != __dres))) {			\
		(dval) = __dres;											\
	} else {														\
		(lval) = __lres;											\
	}																\
} while (0)

#elif SIZEOF_ZEND_LONG == 4 && defined(HAVE_ZEND_LONG64)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long64 __result = (zend_long64) (a) * (zend_long64) (b);	\
	if (__result > ZEND_LONG_MAX || __result < ZEND_LONG_MIN) {		\
		(dval) = (double) __result;									\
		(usedval) = 1;												\
	} else {														\
		(lval) = (long) __result;									\
		(usedval) = 0;												\
	}																\
} while (0)

#else

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	long   __lres  = (a) * (b);										\
	long double __dres  = (long double)(a) * (long double)(b);		\
	long double __delta = (long double) __lres - __dres;			\
	if ( ((usedval) = (( __dres + __delta ) != __dres))) {			\
		(dval) = __dres;											\
	} else {														\
		(lval) = __lres;											\
	}																\
} while (0)

#endif

#if defined(__GNUC__) && (defined(__native_client__) || defined(i386))

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	size_t res = nmemb;
	zend_ulong m_overflow = 0;

	__asm__ ("mull %3\n\taddl %4,%0\n\tadcl $0,%1"
	     : "=&a"(res), "=&d" (m_overflow)
	     : "%0"(res),
	       "rm"(size),
	       "rm"(offset));

	if (UNEXPECTED(m_overflow)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return res;
}

#elif defined(__GNUC__) && defined(__x86_64__)

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	size_t res = nmemb;
	zend_ulong m_overflow = 0;

#ifdef __ILP32__ /* x32 */
# define LP_SUFF "l"
#else /* amd64 */
# define LP_SUFF "q"
#endif

	__asm__ ("mul" LP_SUFF  " %3\n\t"
		"add %4,%0\n\t"
		"adc $0,%1"
		: "=&a"(res), "=&d" (m_overflow)
		: "%0"(res),
		  "rm"(size),
		  "rm"(offset));

#undef LP_SUFF
	if (UNEXPECTED(m_overflow)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return res;
}

#elif defined(__GNUC__) && defined(__arm__)

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	size_t res;
	zend_ulong m_overflow;

	__asm__ ("umlal %0,%1,%2,%3"
		: "=r"(res), "=r"(m_overflow)
		: "r"(nmemb),
		  "r"(size),
		  "0"(offset),
		  "1"(0));

	if (UNEXPECTED(m_overflow)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return res;
}

#elif defined(__GNUC__) && defined(__aarch64__)

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	size_t res;
	zend_ulong m_overflow;

	__asm__ ("mul %0,%2,%3\n\tumulh %1,%2,%3\n\tadds %0,%0,%4\n\tadc %1,%1,xzr"
		: "=&r"(res), "=&r"(m_overflow)
		: "r"(nmemb),
		  "r"(size),
		  "r"(offset));

	if (UNEXPECTED(m_overflow)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return res;
}

#elif SIZEOF_SIZE_T == 4 && defined(HAVE_ZEND_LONG64)

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	zend_ulong64 res = (zend_ulong64)nmemb * (zend_ulong64)size + (zend_ulong64)offset;

	if (UNEXPECTED(res > (zend_ulong64)0xFFFFFFFFL)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return (size_t) res;
}

#else

static zend_always_inline size_t zend_safe_address(size_t nmemb, size_t size, size_t offset, int *overflow)
{
	size_t res = nmemb * size + offset;
	double _d  = (double)nmemb * (double)size + (double)offset;
	double _delta = (double)res - _d;

	if (UNEXPECTED((_d + _delta ) != _d)) {
		*overflow = 1;
		return 0;
	}
	*overflow = 0;
	return res;
}
#endif

#endif /* ZEND_MULTIPLY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
