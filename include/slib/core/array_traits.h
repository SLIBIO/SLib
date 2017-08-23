/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ARRAY_TRAITS
#define CHECKHEADER_SLIB_CORE_ARRAY_TRAITS

#include "definition.h"

#include "cpp.h"
#include "base.h"
#include "compare.h"

#include <new>

namespace slib
{
	
	
	struct SLIB_EXPORT ArrayTraits_Memory
	{
		
		template <class T>
		SLIB_INLINE static void construct(T* dst, sl_reg count) noexcept
		{
			while (count-- > 0) {
				new (dst++) T();
			}
		}
		
		template <class T, class TYPE>
		SLIB_INLINE static void copy_construct(T* dst, const TYPE* src, sl_reg count) noexcept
		{
			while (count-- > 0) {
				new (dst++) T(*(src++));
			}
		}
		
		template <class T, class TYPE>
		SLIB_INLINE static void copy(T* dst, const TYPE* src, sl_reg count) noexcept
		{
			while (count-- > 0) {
				*(dst++) = *(src++);
			}
		}
		
		template <class T>
		SLIB_INLINE static void free(T* dst, sl_reg count) noexcept
		{
			while (count-- > 0) {
				(dst++)->~T();
			}
		}

	};
	
	
	struct SLIB_EXPORT ArrayTraits_IndexOf
	{
		
		template <class T, class VALUE, class EQUALS>
		static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, const EQUALS& equals, sl_reg startIndex) noexcept
		{
			sl_reg ret = -1;
			if (startIndex < 0) {
				startIndex = 0;
			}
			for (sl_size i = startIndex; i < count; i++) {
				if (equals(data[i], value)) {
					ret = i;
					break;
				}
			}
			return ret;
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, char startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, signed char startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, unsigned char startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, short startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, unsigned short startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, int startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, unsigned int startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}

		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, long startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, unsigned long startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, sl_int64 startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, sl_uint64 startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, float startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg indexOf(T* data, sl_reg count, const VALUE& value, double startIndex) noexcept
		{
			return indexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}

		template <class T, class TYPE, class EQUALS>
		static sl_reg indexOf(T* data, sl_reg count, const TYPE& value, const EQUALS& equals) noexcept
		{
			sl_reg ret = -1;
			for (sl_size i = 0; i < count; i++) {
				if (equals(data[i], value)) {
					ret = i;
					break;
				}
			}
			return ret;
		}
		
		
		template <class T, class VALUE, class EQUALS>
		sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, const EQUALS& equals, sl_reg startIndex) noexcept
		{
			sl_reg ret = -1;
			if (startIndex < 0 || startIndex >= (sl_reg)count) {
				startIndex = count - 1;
			}
			for (sl_reg i = startIndex; i >= 0; i--) {
				if (equals(data[i], value)) {
					ret = i;
					break;
				}
			}
			return ret;
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, char startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, signed char startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, unsigned char startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, short startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, unsigned short startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, int startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, unsigned int startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, long startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, unsigned long startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, sl_int64 startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, sl_uint64 startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, float startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE>
		SLIB_INLINE static sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, double startIndex) noexcept
		{
			return lastIndexOf(data, count, value, Equals<T, VALUE>(), (sl_reg)startIndex);
		}
		
		template <class T, class VALUE, class EQUALS>
		sl_reg lastIndexOf(T* data, sl_reg count, const VALUE& value, const EQUALS& equals) noexcept
		{
			sl_reg ret = -1;
			for (sl_reg i = count - 1; i >= 0; i--) {
				if (equals(data[i], value)) {
					ret = i;
					break;
				}
			}
			return ret;
		}
		
	};
	
	
	struct SLIB_EXPORT ArrayTraits_Reverse
	{
		
		template <class T>
		static void reverse(T* data, sl_size count) noexcept
		{
			T* end = data + count;
			sl_reg n = count >> 1;
			while (n-- > 0) {
				T temp(Move(*(--end)));
				*(end) = Move(*data);
				*(data++) = Move(temp);
			}
		}

	};
	
	template <class ELEMENT_TYPE>
	struct SLIB_EXPORT ArrayTraits : public ArrayTraits_Memory, public ArrayTraits_IndexOf, public ArrayTraits_Reverse
	{
	};
	
	
	struct SLIB_EXPORT ArrayTraits_Primitive : public ArrayTraits_IndexOf
	{
		
		template <class T>
		SLIB_INLINE static void construct(T* dst, sl_reg count) noexcept
		{
		}
		
		template <class T>
		SLIB_INLINE static void copy_construct(T* dst, const T* src, sl_reg count) noexcept
		{
			Base::copyMemory(dst, src, ((sl_size)count)*sizeof(T));
		}

		template <class T, class TYPE>
		SLIB_INLINE static void copy_construct(T* dst, const TYPE* src, sl_reg count) noexcept
		{
			while (count-- > 0) {
				*(dst++) = *(src++);
			}
		}
		
		template <class T>
		SLIB_INLINE static void copy(T* dst, const T* src, sl_reg count) noexcept
		{
			Base::copyMemory(dst, src, ((sl_size)count)*sizeof(T));
		}
		
		template <class T, class TYPE>
		SLIB_INLINE static void copy(T* dst, const TYPE* src, sl_reg count) noexcept
		{
			while (count-- > 0) {
				*(dst++) = *(src++);
			}
		}
		
		template <class T>
		SLIB_INLINE static void free(T* dst, sl_reg count) noexcept
		{
		}
		
		template <class T>
		static void reverse(T* data, sl_size count) noexcept
		{
			T temp;
			T* end = data + count;
			sl_reg n = count >> 1;
			while (n-- > 0) {
				temp = *(--end);
				*end = *data;
				*(data++) = temp;
			}
		}

	};
	
	template <> struct ArrayTraits<char> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<signed char> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<unsigned char> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<short> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<unsigned short> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<int> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<unsigned int> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<long> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<unsigned long> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<sl_int64> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<sl_uint64> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<float> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<double> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<sl_char16> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<sl_char32> : public ArrayTraits_Primitive {};
	template <> struct ArrayTraits<sl_bool> : public ArrayTraits_Primitive {};

	
	struct SLIB_EXPORT ArrayTraits_ZeroInit : public ArrayTraits_Memory, public ArrayTraits_IndexOf, public ArrayTraits_Reverse
	{
		
		template <class T>
		SLIB_INLINE static void construct(T* dst, sl_reg count) noexcept
		{
			Base::zeroMemory(dst, ((sl_size)count) * sizeof(T));
		}

	};
	
	class String;
	template <> struct ArrayTraits<String> : public ArrayTraits_ZeroInit {};
	
	class String16;
	template <> struct ArrayTraits<String16> : public ArrayTraits_ZeroInit {};
	
	template <class T> class Ref;
	template <class T> struct ArrayTraits< Ref<T> > : public ArrayTraits_ZeroInit {};
	
	template <class T> class WeakRef;
	template <class T> struct ArrayTraits< WeakRef<T> > : public ArrayTraits_ZeroInit {};
	
	template <class T> class Array;
	template <class T> struct ArrayTraits< Array<T> > : public ArrayTraits_ZeroInit {};
	
	template <class T> class List;
	template <class T> struct ArrayTraits< List<T> > : public ArrayTraits_ZeroInit {};
	
	template <class KT, class VT> class Map;
	template <class KT, class VT> struct ArrayTraits< Map<KT, VT> > : public ArrayTraits_ZeroInit {};

	class Variant;
	template <> struct ArrayTraits<Variant> : public ArrayTraits_ZeroInit {};
	
}

#endif
