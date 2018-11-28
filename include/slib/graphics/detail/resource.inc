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

namespace slib
{
	
	class _priv_ImageResourceEntry
	{
	public:
		sl_bool flagValid;

		sl_uint32 width;
		sl_uint32 height;

		const sl_uint8* source_bytes;
		sl_uint32 source_size;

		sl_int32 lock;
		void* image;
		sl_bool flag_load;

	public:
		Ref<Image> getImage();

		Ref<Image> getMatchingImage(sl_uint32 width, sl_uint32 height);

	};
	
	class _priv_ImageResourceFreeStatic
	{
	public:
		_priv_ImageResourceFreeStatic(_priv_ImageResourceEntry* entries);

		~_priv_ImageResourceFreeStatic();

	private:
		_priv_ImageResourceEntry* m_entries;

	};
	
	Ref<Image> _priv_ImageResource_getImage(_priv_ImageResourceEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight);
	
	List< Ref<Image> > _priv_ImageResource_getImages(_priv_ImageResourceEntry* entries);
	
	Ref<Drawable> _priv_ImageResource_get(_priv_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height);

}
