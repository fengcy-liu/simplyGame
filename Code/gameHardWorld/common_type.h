#pragma once
#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <string>

#define WAIP(x, lower, upper)              ((x)>=(upper) ? ((x) - ((upper) - (lower))):((x) < (lower) ? ((x) + ((upper) - (lower))):(x)))

typedef unsigned char			u8_t;
typedef unsigned short			u16_t;
typedef unsigned int			u32_t;
typedef unsigned long long      u64_t;
typedef signed char				s8_t;
typedef signed short			s16_t;
typedef signed int				s32_t;
typedef signed long long        s64_t;
typedef float					f32_t;
typedef double					f64_t;


#ifdef _UNICODE
typedef WCHAR					char_t;
#else
typedef CHAR					char_t;
#endif





#endif // !COMMON_TYPE_H
