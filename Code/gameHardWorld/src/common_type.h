#pragma once
#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <string>

#define MAX_BYTES_OF_CHAR                 20u
#define HALF_MAX_BYTES_OF_CHAR            10u

#define MAX_LENGTH_OF_NAME                            HALF_MAX_BYTES_OF_CHAR
#define MAX_LENGTH_OF_DESC                            100U

#define WAPPED(x, lower, upper)              ((x)>=(upper) ? ((x) - ((upper) - (lower))):((x) < (lower) ? ((x) + ((upper) - (lower))):(x)))
#define MAX(a,b)                             ((a) > (b)?(a):(b))
#define MIN(a,b)                             ((a) < (b)?(a):(b))

#if 0
#define _BIT_MASK_COMMAND
#endif

typedef unsigned char           bool_t;


typedef unsigned char           u8_t;
typedef unsigned short          u16_t;
typedef unsigned int            u32_t;
typedef unsigned long long      u64_t;
typedef signed char             s8_t;
typedef signed short            s16_t;
typedef signed int              s32_t;
typedef signed long long        s64_t;
typedef float                   f32_t;
typedef double                  f64_t;


#ifdef _UNICODE
typedef WCHAR                   char_t;
#else
typedef CHAR                    char_t;
#endif





#endif // !COMMON_TYPE_H
