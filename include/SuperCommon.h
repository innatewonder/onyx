/////////////////
//IT IS HIGHLY SUGGESTED THAT THIS FILE IS IN A PRECOMPILED HEADER
/////////////////

#pragma once
/////////////////
//Some Default Includes
#include <iostream>

#define PLAT_WINDOWS  1
#define PLAT_MAC      2
#define PLAT_UNIX     3

#if defined(_WIN32)
  #define PLATFORM PLAT_WINDOWS
#elif defined(__APPLE__)
  #define PLATFORM PLAT_MAC
#else
  #define PLATFORM PLAT_UNIX
#endif

/////////////////
//WARNING SUPRESSIONS
#if PLATFORM == PLAT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif

/////////////////
//ASSERTIONS
#include <cassert>

#ifdef _USE_CONTROL_ADV_ASSERT_
#ifdef _DEBUG
#define assertion(x) \
{ \
  if(!(x)) \
  { \
    std::cout << "Assert " << __FILE__ << ":" << __LINE__ << "(" << #x << ")\n"; \
    __debugbreak(); \
  } \
}
#else
#define assertion(x) {}
#endif//_DEBUG
#else
#define assertion(x) assert(x)
#endif //_USE_CONTROL_ADV_ASSERT_

#define SOFTASSERT(statement, msg, retval) \
  if(!(statement)) \
  { \
    std::cerr << "::SoftAssert::" << __FILE__ << ":" << __LINE__ << " " << msg << std::endl; \
    return retval; \
  }

/////////////////
//SUPER HELPFUL FOR VS TODO OUTPUT
#define STRINGIZE_(X) #X
#define STRINGIZE(X) STRINGIZE_(X)

#if PLATFORM == PLAT_WINDOWS
  #define TODO(X) \
    __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) "): TODO " X))
#else
  #define TODO_MSG(X) _Pragma(#X)
  #define TODO(X) TODO_MSG(message("TODO " X))
#endif
/////////////////
//DEFAULT TYPE DEFINITIONS
#include <stdint.h>

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

/////////////////
//WRAPPERS FOR CASTS
#define DYNCAST(newClass, var) dynamic_cast<newClass>(var)
#define RECAST(newClass, var) reinterpret_cast<newClass>(var)
#define SCAST(newType, var) static_cast<newType>(var)
