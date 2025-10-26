#include "nxsrc.h"
#include "symdb.h"

namespace hdn
{
	bool nxsrc_agnostic_parse(const fspath& path)
	{
		fspath name = filesystem_filename(path);
		ESymbolType type = symdb_str_to_sym(filesystem_extension(path).string().c_str() + 1);
		HINFO("nxsymbol ({0}) '{1}' registered", symdb_sym_to_str(type), get_symbol_from_name(name.string().c_str()));
		return true;
	}
}
