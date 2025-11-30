#include "nxsrc.h"

#include "symdb.h"

namespace dm
{
	bool nxsrc_agnostic_parse(const fspath &path)
	{
		fspath name = filesystem_filename(path);
		ESymbolType type = symdb_str_to_sym(filesystem_extension(path).string().c_str() + 1);
		sym_t sym = name.string().c_str();
		DM_INFO_LOG("nxsymbol ({0}) '{1}' ({2}) registered", symdb_sym_to_str(type), name.string().c_str(), static_cast<u64>(sym));
		return true;
	}
}
