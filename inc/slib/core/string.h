#ifndef CHECKHEADER_SLIB_CORE_STRING
#define CHECKHEADER_SLIB_CORE_STRING

#include "definition.h"

#define SLIB_CHAR_IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_ALPHA_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define SLIB_CHAR_IS_ALPHA_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define SLIB_CHAR_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define SLIB_CHAR_IS_ALNUM(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define SLIB_CHAR_IS_HEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define SLIB_CHAR_IS_WHITE_SPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define SLIB_CHAR_IS_SPACE_TAB(c) (c == ' ' || c == '\t')
#define SLIB_CHAR_IS_C_NAME(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || c == '_')

#define SLIB_CHAR_DIGIT_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : 10)
#define SLIB_CHAR_HEX_TO_INT(c) (((c) >= '0' && (c) <= '9') ? ((c) - '0') : (((c) >= 'A' && (c) <= 'F') ? ((c) -  55) : ((c) >= 'a' && (c) <= 'f') ? ((c) -  87) : 16))
#define SLIB_CHAR_UPPER_TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z')?((c) + ('a' - 'A')):(c))
#define SLIB_CHAR_LOWER_TO_UPPER(c) (((c) >= 'a' && (c) <= 'z')?((c) - ('a' - 'A')):(c))

#include "string8.h"
#include "string16.h"

namespace slib
{

	class SLIB_EXPORT StringData
	{
	public:
		union {
			const sl_char8* sz8;
			const sl_char16* sz16;
			const sl_char32* sz32;
		};
		sl_size len;
		Ref<Referable> refer;
		String str8;
		String16 str16;
	};
	
	
	template <class CharType>
	struct StringTypeFromCharType;

}

#include "detail/string8.h"
#include "detail/string16.h"

#endif
