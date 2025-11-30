#pragma once

#include "core/core.h"
#include "gfx/r_vk_model.h"

namespace dm
{
	struct ModelComponent
	{
		Ref<VulkanModel> model{};
	};
}
