#pragma once

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;
typedef unsigned long long uint128;

typedef uint64 pde_t;

// Kernel (compact) string
typedef struct {
  union {
    uint64 u64;
    uchar  uc[sizeof(uint64)];
  };
} kstr;

// Kernel (compact) long string
typedef struct {
  union {
    uint128 u128;
    uchar   uc[sizeof(uint128)];
  };
} klstr;
