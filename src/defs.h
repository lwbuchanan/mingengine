#ifndef MING_DEFS_H
#define MING_DEFS_H

#include <cstddef>
#include <cstdint>

// Storage class for names with internal linkage
// static is a stupid name for this
// why not internal or 'intern' as opposed to 'extern'
#define internal static

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef size_t usize;

typedef uint32 rgba;

#endif
