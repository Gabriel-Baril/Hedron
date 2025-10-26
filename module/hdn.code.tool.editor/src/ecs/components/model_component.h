#pragma once

#include "core/core.h"
#include "gfx/r_vk_model.h"

namespace hdn
{
	struct ModelComponent
	{
		Ref<VulkanModel> model{};
	};
}
