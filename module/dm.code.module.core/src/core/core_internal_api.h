#pragma once
#include "core_define.h"

#define DM_MODULE_CORE_API

#if USING(DM_PLATFORM_WINDOWS)
#if _DM_MODULE_CORE_DYNAMIC_LINK
#if defined(_DM_MODULE_CORE_BUILD_DLL)
#define DM_MODULE_CORE_API __declspec(dllexport)
#else
#define DM_MODULE_CORE_API __declspec(dllimport)
#endif
#else
#define DM_MODULE_CORE_API
#endif
#endif
