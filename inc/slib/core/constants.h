#ifndef CHECKHEADER_SLIB_CORE_CONSTANTS
#define CHECKHEADER_SLIB_CORE_CONSTANTS

#include "definition.h"

namespace slib
{

	enum class MapPutMode
	{
		AddOrReplace = 0,
		ReplaceExisting = 1,
		AddNew = 2,
		AddAlways = 3,
		Default = AddOrReplace
	};

}

#endif
