#include "ideation_parse.h"
#include "ideation_database.h"

#include "core/Core.h"

int main()
{
#if USING( HDN_DEBUG )
	hdn::Log_Init();
#endif

	IdeationNode node;
	parse_ideation_file("../data/ideation.json", node);
	return 0;
}