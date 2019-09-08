/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/graphics/emoji.h"

#include "slib/core/hash_map.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace emoji
		{
			
			#include "emoji.inc"
			
			class StaticContext
			{
			public:
				CHashMap<sl_char32, sl_bool> emojis;
				
			public:
				StaticContext()
				{
					sl_uint32 index = 0;
					for (;;) {
						sl_char32 ch = (sl_char32)(list[index]);
						if (ch) {
							emojis.add_NoLock(ch, sl_true);
						} else {
							break;
						}
						index++;
					}
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
			
		}
	}
	
	using namespace priv::emoji;
	
	sl_bool Emoji::isEmoji(sl_char32 ch)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			return context->emojis.getValue_NoLock(ch);
		}
		return sl_false;
	}

}
