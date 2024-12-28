#include "ideation_parse.h"
#include "ideation_database.h"

#include "core/core.h"

int main()
{
	using namespace hdn;
	Log_Init();

	IdeationNode node;
	parse_ideation_file("../data/ideation.json", node);
	return 0;
}