#pragma once

#include "core/core.h"
#include "r_vk_model.h"

namespace hdn
{
	struct ModelComponent
	{
		Ref<VulkanModel> model{};
	};
}
