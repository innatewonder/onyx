#ifndef Filesystem_DEFINES_H
#define Filesystem_DEFINES_H

#include <stdint.h>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>
typedef std::vector<std::string> StringVec;
typedef StringVec::iterator      StringVecIt;

#include <map>
#include <iostream>
#include <sys/stat.h>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>

namespace Filesystem
{
typedef char      c08;

typedef int8_t    s08;
typedef uint8_t   u08;

typedef int16_t   s16;
typedef uint16_t  u16;

typedef int32_t   s32;
typedef uint32_t  u32;

typedef int64_t   s64;
typedef uint64_t  u64;

typedef float     f32;
typedef double    f64;

const s32 READ_RAW = 0;
const s32 READ_PACKAGE = 1;

}

#define Filesystem_assertion(x) {}

#endif
