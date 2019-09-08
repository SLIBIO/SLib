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
			extern unsigned long list[];
			
			#include "emoji_png_data0.inc"
			#include "emoji_png_data1.inc"
			#include "emoji_png_data2.inc"
			#include "emoji_png_data3.inc"
			#include "emoji_png_data4.inc"
			#include "emoji_png_data5.inc"
			#include "emoji_png_data6.inc"
			#include "emoji_png_data7.inc"
			#include "emoji_png_data8.inc"
			#include "emoji_png_data9.inc"
			#include "emoji_png_data10.inc"
			#include "emoji_png_data11.inc"
			#include "emoji_png_data12.inc"
			#include "emoji_png_data13.inc"
			#include "emoji_png_data14.inc"
			#include "emoji_png_data15.inc"
			#include "emoji_png_data16.inc"
			#include "emoji_png_data17.inc"
			#include "emoji_png_data18.inc"
			#include "emoji_png_data19.inc"
			#include "emoji_png.inc"
			
			class StaticPngContext
			{
			public:
				CHashMap<sl_char32, Memory> mapPNG;
				CHashMap< sl_char32, Ref<Image> > mapImage;
				Mutex lockImage;
				
			public:
				StaticPngContext()
				{
					sl_uint32 index = 0;
					for (;;) {
						sl_char32 ch = (sl_char32)(list[index]);
						const void* data = png[index];
						unsigned long size = png_size[index];
						if (ch && data && size) {
							Memory mem = Memory::createStatic(data, size);
							if (mem.isNotNull()) {
								mapPNG.add_NoLock(ch, Move(mem));
							}
						} else {
							break;
						}
						index++;
					}
				}
				
			public:
				Ref<Image> getImage(sl_char32 ch)
				{
					MutexLocker lock(&lockImage);
					Ref<Image> image;
					if (mapImage.get_NoLock(ch, &image)) {
						return image;
					}
					Memory mem = mapPNG.getValue_NoLock(ch);
					if (mem.isNotNull()) {
						image = Image::loadFromMemory(mem);
						if (image.isNotNull()) {
							mapImage.put_NoLock(ch, image);
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
	
	Memory Emoji::getPng(sl_char32 ch)
	{
		StaticPngContext* context = GetStaticPngContext();
		if (context) {
			return context->mapPNG.getValue_NoLock(ch);
		}
		return sl_null;
	}

	Ref<Image> Emoji::getImage(sl_char32 ch)
	{
		StaticPngContext* context = GetStaticPngContext();
		if (context) {
			return context->getImage(ch);
		}
		return sl_null;
	}
	
}
