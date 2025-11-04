#include "xsrc_request.h"
#include "xsrc.h"

namespace hdn
{
	RequestResult xsrc_object_request(sym_t symbol, u64 id)
	{
		const SymbolMetadata* meta = symdb_get_meta(symbol);
		if (!meta)
		{
			return RequestResult::NOT_FOUND;
		}

		const fspath& path = meta->path;
		xsrc_agnostic_parse(path);
		if (cache_obj_exist(id))
		{
			return RequestResult::NOT_FOUND;
		}
		return RequestResult::SUCCESS;
	}
}