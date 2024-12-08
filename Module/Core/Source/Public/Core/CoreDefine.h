#pragma once

#include <type_traits>

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define IN_USE		&&
#define NOT_IN_USE	&&!
#define USE_IF( x )	&&((x) ? 1 : 0)&&
#define USING( x )	(1 x 1)

#define HDN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)
#define ARRLEN(arr) (sizeof(arr) / sizeof(*arr))

#ifdef _HDN_DEBUG
#define HDN_DEBUG IN_USE
#else
#define HDN_DEBUG NOT_IN_USE
#endif

#ifdef _HDN_RELEASE
#define HDN_RELEASE IN_USE
#else
#define HDN_RELEASE NOT_IN_USE
#endif

#ifdef _HDN_RETAIL
#define HDN_RETAIL IN_USE
#else
#define HDN_RETAIL NOT_IN_USE
#endif

#define DEV					USE_IF( USING(HDN_DEBUG) )
#define ENABLE_LOG			USE_IF( USING(DEV) )
#define ENABLE_ASSERT		USE_IF( USING(DEV) )
