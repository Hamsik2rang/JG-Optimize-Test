//
//  Predefine.h
//  OptTest
//
//  Created by Yongsik Im on 7/14/25.
//
#pragma once
#ifndef __PREDEFINE_H__
#define __PREDEFINE_H__

#include <vector>
#include <array>
#include <list>
#include <cstdint>
#include <cassert>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #include <immintrin.h>
    #define USE_SSE 1
#elif defined(__aarch64__) || defined(_M_ARM64)
    #include <arm_neon.h>

    #define USE_NEON 1
#else
    #error This platform cannot support SIMD instruction. // SIMD지원이 안되면...못합니다.
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #define FORCEINLINE __forceinline

#elif defined(__aarch64__) || defined(_M_ARM64)
    #define FORCEINLINE __attribute__((__always_inline__))
#else
    #define FORCEINLINE inline
#endif

#define FLT_EPSILON (1.09e-07)

#endif
