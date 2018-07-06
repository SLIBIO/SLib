/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
	
	template <>
	struct StringTypeFromCharType<sl_char16> { typedef String16 Type; };
	
	template <>
	struct StringTypeFromCharType<sl_char8> { typedef String Type; };
	
	
	template <class StringType>
	struct CharTypeFromStringType;

	template <>
	struct CharTypeFromStringType<String> { typedef sl_char8 Type; };

	template <>
	struct CharTypeFromStringType<AtomicString> { typedef sl_char8 Type; };

	template <>
	struct CharTypeFromStringType<String16> { typedef sl_char16 Type; };
	
	template <>
	struct CharTypeFromStringType<AtomicString16> { typedef sl_char16 Type; };
	
}

#include "detail/string8.inc"
#include "detail/string16.inc"

#endif
