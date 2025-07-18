//
//  Predefine.h
//  JungleOptimizeTest
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
#define PASTE(a, b) a##b


#define FUNC_NAME(func) #func
#define ELAPSE_START(n) auto PASTE(start, n) = std::chrono::high_resolution_clock::now();
#define ELAPSE_END(n, result) auto PASTE(end, n)= std::chrono::high_resolution_clock::now(); \
auto PASTE(duration, n) = std::chrono::duration_cast<std::chrono::microseconds>(end##n - start##n); \
result = duration##n.count();

// 10개 이상의 인수는 없을 걸로 예상...
#define NARGS(...) NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define CALL_FUNC_0(func) func()
#define CALL_FUNC_N(func, ...) func(__VA_ARGS__)

#define TEST_FUNC_DISPATCH(func, n, ...) \
TEST_FUNC_DISPATCH_IMPL(func, n, __VA_ARGS__)

#define TESE_FUNC_DISPATCH_IMPL(func, n, ...) \
CALL_FUNC_##n(func, __VA_ARGS__)

#define TEST_FUNC(func, ...) \
TEST_FUNC_DISPATCH(func, \
NARGS(__VA_ARGS__), \
__VA_ARGS__)

#define WARM_UP_CACHE_START() \
int index = 3; \
while(index-- > 0) \
{\

#define WARM_UP_CACHE_END() }

#endif
