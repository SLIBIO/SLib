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

#ifndef CHECKHEADER_SLIB_CORE_NULL_VALUE
#define CHECKHEADER_SLIB_CORE_NULL_VALUE

namespace slib
{
	
	template <class T>
	struct NullValue
	{
		SLIB_INLINE static T get()
		{
			return T();
		}
	};
	
#define SLIB_DEFINE_NULL_VALUE(T, VALUE) \
	template <> \
	struct NullValue<T> \
	{ \
		SLIB_INLINE static T get() \
		{ \
			return VALUE; \
		} \
	};
	
	
	SLIB_DEFINE_NULL_VALUE(char, 0)
	SLIB_DEFINE_NULL_VALUE(signed char, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned char, 0)
	SLIB_DEFINE_NULL_VALUE(char16_t, 0)
	SLIB_DEFINE_NULL_VALUE(char32_t, 0)
	SLIB_DEFINE_NULL_VALUE(short, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned short, 0)
	SLIB_DEFINE_NULL_VALUE(int, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned int, 0)
	SLIB_DEFINE_NULL_VALUE(long, 0)
	SLIB_DEFINE_NULL_VALUE(unsigned long, 0)
	SLIB_DEFINE_NULL_VALUE(sl_int64, 0)
	SLIB_DEFINE_NULL_VALUE(sl_uint64, 0)
	SLIB_DEFINE_NULL_VALUE(bool, false)
	SLIB_DEFINE_NULL_VALUE(float, 0)
	SLIB_DEFINE_NULL_VALUE(double, 0)
	
	template <class T>
	struct NullValue<T*>
	{
		SLIB_INLINE static T* get()
		{
			return sl_null;
		}
	};
	
}

#endif
