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
			extern const void* png[];
			extern unsigned int png_size[];
		}
		
		using namespace noto_emoji;
		
		namespace emoji
		{

			class StaticPngContext
			{
			public:
				CHashMap<String16, Memory> mapPNG;
				CHashMap< String16, Ref<Image> > mapImage;
				Mutex lockImage;
				
			public:
				StaticPngContext()
				{
					for (sl_uint32 index = 0; ; index++) {
						const char32_t* sz = emojis[index];
						if (sz) {
							String16 str(sz);
							sl_size len = str.getLength();
							if (len) {
								const void* data = png[index];
								sl_size size = (sl_size)(png_size[index]);
								if (data && size) {
									Memory mem = Memory::createStatic(data, size);
									if (mem.isNotNull()) {
										mapPNG.add_NoLock(str, Move(mem));
									}
								}
							}
						} else {
							break;
						}
					}
				}
				
			public:
				Ref<Image> getImage(const String16& str)
				{
					MutexLocker lock(&lockImage);
					Ref<Image> image;
					if (mapImage.get_NoLock(str, &image)) {
						return image;
					}
					Memory mem = mapPNG.getValue_NoLock(str);
					if (mem.isNotNull()) {
						image = Image::loadFromMemory(mem);
						if (image.isNotNull()) {
							mapImage.put_NoLock(str, image);
							return image;
						}
					}
					return sl_null;
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticPngContext, GetStaticPngContext)
			
		}
	}
	
	using namespace priv::emoji;
	
	Memory Emoji::getPng(const String16& str)
	{
		StaticPngContext* context = GetStaticPngContext();
		if (context) {
			return context->mapPNG.getValue_NoLock(str);
		}
		return sl_null;
	}

	Ref<Image> Emoji::getImage(const String16& str)
	{
		StaticPngContext* context = GetStaticPngContext();
		if (context) {
			return context->getImage(str);
		}
		return sl_null;
	}
	
}
