#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#undef NULL_PTR
#define NULL_PTR  ((void *)0)

#ifndef FALSE
#define FALSE    (0)
#endif

#ifndef TRUE
#define TRUE     (1)
#endif

typedef unsigned char  bool_t, * pbool_t;
typedef unsigned char  uint8_t, u8_t, * pu8_t;
typedef signed   char  int8_t, s8_t, * ps8_t;
typedef unsigned short uint16_t, u16_t, * pu16_t;
typedef signed   short int16_t, s16_t, * ps16_t;
typedef unsigned int   uint32_t, u32_t, * pu32_t;
typedef signed   int   int32_t, s32_t, * ps32_t;

typedef signed   long long   s64_t, * ps64_t;
typedef unsigned long long   u64_t, * pu64_t;

typedef float  f32_t, * pf32_t;
typedef double f64_t, * pf64_t;

typedef struct
{
    s16_t s_im;
    s16_t s_re;
}s16cx_t, * ps16cx_t;

typedef struct
{
    s32_t s_im;
    s32_t s_re;
}s32cx_t, * ps32cx_t;

typedef struct
{
    s64_t s_im;
    s64_t s_re;
}s64cx_t, * ps64cx_t;

typedef struct
{
    f32_t s_im;
    f32_t s_re;
}fcx_t, * pfcx_t;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif
