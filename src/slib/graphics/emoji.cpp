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
		
		namespace noto_emoji
		{
			extern const char32_t* emojis[];
		}
		
		using namespace noto_emoji;
		
		namespace emoji
		{
			
			class StaticContext
			{
			public:
				CMap<String16, sl_bool> mapEmojis;
				CHashMap<sl_char32, sl_bool> mapEmojiFirstChars;
				sl_size maxLength;
				
			public:
				StaticContext()
				{
					maxLength = 0;
					for (sl_uint32 index = 0; ; index++) {
						const sl_char32* sz = emojis[index];
						if (sz) {
							String16 str(sz);
							sl_size len = str.getLength();
							if (len) {
								if (len > maxLength) {
									maxLength = len;
								}
								mapEmojis.add_NoLock(Move(str), sl_true);
								mapEmojiFirstChars.emplace_NoLock(sz[0], sl_true);
							}
						} else {
							break;
						}
					}
				}
				
			public:
				sl_size getEmojiLength(const sl_char16* sz, sl_size len)
				{
					sl_char32 ch = sz[0];
					if (ch >= 0xD800 && ch < 0xE000) {
						if (len >= 2) {
							sl_uint32 ch1 = (sl_uint32)((sl_uint16)sz[1]);
							if (ch < 0xDC00 && ch1 >= 0xDC00 && ch1 < 0xE000) {
								ch = (sl_char32)(((ch - 0xD800) << 10) | (ch1 - 0xDC00)) + 0x10000;
							} else {
								return 0;
							}
						} else {
							return 0;
						}
					}
					if (mapEmojiFirstChars.getValue(ch)) {
						if (len > maxLength) {
							len = maxLength;
						}
						len = Base::getStringLength2(sz, len);
						MapNode<String16, sl_bool>* node = sl_null;
						mapEmojis.getNearest(String16(sz, len), &node);
						if (node) {
							sl_size lenKey = node->key.getLength();
							if (lenKey <= len && Base::equalsMemory2((sl_uint16*)sz, (sl_uint16*)(node->key.getData()), lenKey)) {
								return node->key.getLength();
							}
						}
					}
					return 0;
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
			
		}
		
	}
	
	using namespace priv::emoji;
	
	sl_bool Emoji::isEmoji(const String16& str)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			return context->mapEmojis.getValue_NoLock(str);
		}
		return sl_false;
	}
	
	sl_bool Emoji::isEmoji(sl_char32 ch)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			return context->mapEmojiFirstChars.getValue_NoLock(ch);
		}
		return sl_false;
	}

	sl_size Emoji::getEmojiLength(const sl_char16* sz, sl_size len)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			return context->getEmojiLength(sz, len);
		}
		return sl_false;
	}

}
