#pragma once

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define IN_USE &&
#define NOT_IN_USE &&!
#define USE_IF(x) &&((x) ? 1 : 0) &&
#define USING(x) (1 x 1)

#define DM_MAYBE_UNUSED(x) ((void)x)
#define DM_BIT(x) (1 << x)
#define DM_ARRLEN(arr) (sizeof(arr) / sizeof(*arr))
#define DM_NAMEOF(type) #type

#ifdef _DM_DEBUG
#define DM_DEBUG IN_USE
#else
#define DM_DEBUG NOT_IN_USE
#endif

#ifdef _DM_RELEASE
#define DM_RELEASE IN_USE
#else
#define DM_RELEASE NOT_IN_USE
#endif

#ifdef _DM_RETAIL
#define DM_RETAIL IN_USE
#else
#define DM_RETAIL NOT_IN_USE
#endif

#define DM_PLATFORM_WINDOWS NOT_IN_USE
#define DM_PLATFORM_IOS NOT_IN_USE
#define DM_PLATFORM_MACOS NOT_IN_USE
#define DM_PLATFORM_ANDROID NOT_IN_USE
#define DM_PLATFORM_LINUX NOT_IN_USE

#if defined(_WIN32)
#if defined(_WIN64)
#undef DM_PLATFORM_WINDOWS
#define DM_PLATFORM_WINDOWS IN_USE
#else
#error "x86 builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(_MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#undef DM_PLATFORM_IOS
#define DM_PLATFORM_IOS IN_USE
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#undef DM_PLATFORM_MACOS
#define DM_PLATFORM_MACOS IN_USE
#error "MaxOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
#undef DM_PLATFORM_ANDROID
#define DM_PLATFORM_ANDROID IN_USE
#error "Android is not supported!"
#elif defined(__linux__)
#undef DM_PLATFORM_LINUX
#define DM_PLATFORM_LINUX IN_USE
#error "Linux is not supported!"
#else
#error "Unknown platform!"
#endif

#ifdef _MSC_VER
#define DM_DISABLE_OPTIMIZATION __pragma(optimize("", off))
#define DM_ENABLE_OPTIMIZATION __pragma(optimize("", on))
#elif defined(__GNUC__) || defined(__clang__)
#define DM_DISABLE_OPTIMIZATION __attribute__((optimize("O0")))
#define DM_ENABLE_OPTIMIZATION
#else
#define DM_DISABLE_OPTIMIZATION
#define DM_ENABLE_OPTIMIZATION
#endif

#define DM_DEV USE_IF(USING(DM_DEBUG))

#define DM_LOG_ENABLE USE_IF(USING(DM_DEV))
#define DM_LOG_CONSOLE_ENABLE USE_IF(USING(DM_LOG_ENABLE))
#define DM_LOG_FILE_ENABLE USE_IF(USING(DM_LOG_ENABLE))
#define DM_LOG_ALWAYS_FLUSH USE_IF(USING(DM_LOG_ENABLE))

#define DM_PIPELINE_MODULE NOT_IN_USE
#define DM_CACHE_VERBOSE_WARN USE_IF(USING(DM_LOG_CONSOLE_ENABLE))
#define DM_SYM_BUILDCONFIG IN_USE
#define DM_SYM_FEATURE IN_USE
#define DM_DREAMLIKE_BUILD IN_USE

#define DM_ASSERT_ENABLE USE_IF(USING(DM_DEV))
#define DM_THROW_ENABLE NOT_IN_USE // USE_IF( USING(DEV) )

#define DM_PERF_PROFILE NOT_IN_USE
