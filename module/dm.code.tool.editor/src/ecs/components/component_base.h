#pragma once

namespace dm
{
	struct ComponentBase
	{
		int ComponentUUID = 0; // TODO: Populate
		bool enabled = true;

		ComponentBase();

		ComponentBase(const ComponentBase &) = default;
	};
}
