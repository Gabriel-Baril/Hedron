#include "core/core.h"
#include <iostream>

#include "hdef.h"

int main()
{
	using namespace hdn;
	Log_Init();

	hdef_id_t id = build_hdef_id("/lightthorned/root/base_map.png", hdn::hdef_t::component);
	std::cout << std::hex << id.hash() << std::endl;
}