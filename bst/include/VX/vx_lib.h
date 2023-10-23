/*
 * Copyright (c) 2017 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.
 *
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Cadence Design
 * Systems Inc. They may be adapted and modified by bona fide purchasers for
 * internal use, but neither the original nor any adapted or modified version
 * may be disclosed or distributed to third parties in any manner, medium, or
 * form, in whole or in part, without the prior written consent of Cadence 
 * Design Systems Inc or in accordance with the terms of your Design Technology
 * License Agreement with Cadence Design Systems. This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */
 
#ifndef _VX_LIB_H_
#define _VX_LIB_H_


#ifdef _MSC_VER

#define CCALL __cdecl
#pragma section(".CRT$XCU",read)
#define STATIC_LIB_INITIALIZER(f) \
   static void __cdecl f(void); \
   __declspec(allocate(".CRT$XCU")) static void (__cdecl*f##_)(void) = f; \
   static void __cdecl f(void)

#elif defined(__GNUC__) || defined(__XTENSA__)

#define CCALL
#define STATIC_LIB_INITIALIZER(f) \
		static void f(void) __attribute__((constructor)); \
		static void f(void)

#endif



#endif /* _VX_LIB_H_ */
