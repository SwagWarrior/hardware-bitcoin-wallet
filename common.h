/** \file common.h
  *
  * \brief Defines things which are common to most of the source distribution.
  *
  * If porting to another platform, please check for the presence of stdint.h
  * and if it isn't available, define NO_STDINT_H and check that the typedefs
  * below refer to appropriate types.
  *
  * This file is licensed as described by the file LICENCE.
  */

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

// Some platforms or toolchains lack stdint.h. Please define NO_STDINT_H and
// modify the three typedefs below if this is the case.
#ifdef NO_STDINT_H

// These typedefs are platform-dependent. Change them if they don't match
// the comments.
/** Unsigned 8 bit integer. */
typedef unsigned char uint8_t;
/** Unsigned 16 bit integer. */
typedef unsigned short uint16_t;
/** Unsigned 32 bit integer. */
typedef unsigned long uint32_t;

#else

#include <stdint.h>

#endif // #ifdef NO_STDINT_H

// The only functions from string.h used by hardware Bitcoin wallet
// are memcpy() and memset(). If string.h is not available on the target
// platform/toolchain, these functions will have to be implemented somewhere.
#include <string.h>

/** In certain situations, inlining can cause an overall increase in stack
  * space. For example, let foo() use 100 bytes of stack space, bar() 104 bytes
  * and sno() 50 bytes. If sno() calls foo() and then (after foo() returns)
  * calls bar(), the maximum stack space used is 154 bytes. But if an
  * enthusiastic compiler decides to inline foo() and bar() into sno(), the
  * maximum stack space used is now 254 bytes, because all the functions'
  * frames are combined.
  * NOINLINE is supposed to tell compilers to not inline the associated
  * function. Careful use of NOINLINE can decrease the amount of maximum stack
  * space used. */
#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif // #if defined(__GNUC__)

/** On certain platforms, unchanging, read-only data (eg. lookup tables) needs
  * to be marked and accessed in a way that is different to read/write data.
  * Marking this data with PROGMEM saves valuable RAM space. However, any data
  * marked with PROGMEM needs to be accessed using
  * the #LOOKUP_BYTE and #LOOKUP_DWORD macros. */
#if defined(AVR) && defined(__GNUC__)
#include <avr/io.h>
#include <avr/pgmspace.h>
#define LOOKUP_DWORD(x)		(pgm_read_dword_near(&(x)))
#define LOOKUP_BYTE(x)		(pgm_read_byte_near(&(x)))
#else
#define PROGMEM
/** Use this to access #PROGMEM lookup tables which have dword (32 bit)
  * entries. For example, normally you would use `r = dword_table[i];` but
  * for a #PROGMEM table, use `r = LOOKUP_DWORD(dword_table[i]);`. */
#define LOOKUP_DWORD(x)		(x)
/** Use this to access #PROGMEM lookup tables which have byte (8 bit)
  * entries. For example, normally you would use `r = byte_table[i];` but
  * for a #PROGMEM table, use `r = LOOKUP_BYTE(byte_table[i]);`. */
#define LOOKUP_BYTE(x)		(x)
#endif // #if defined(AVR) && defined(__GNUC__)

#endif // #ifndef COMMON_H_INCLUDED
