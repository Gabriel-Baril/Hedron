#pragma once
#include "core/core_define.h"

#define DM_MODULE_CONFIG_API

#if USING(DM_PLATFORM_WINDOWS)
#if _DM_MODULE_CONFIG_DYNAMIC_LINK
#if defined(_DM_MODULE_CONFIG_BUILD_DLL)
#define DM_MODULE_CONFIG_API __declspec(dllexport)
#else
#define DM_MODULE_CONFIG_API __declspec(dllimport)
#endif
#else
#define DM_MODULE_CONFIG_API
#endif
#endif
