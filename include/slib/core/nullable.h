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

#ifndef CHECKHEADER_SLIB_CORE_NULLABLE
#define CHECKHEADER_SLIB_CORE_NULLABLE

#include "definition.h"

#include "null_value.h"

#include "hash.h"

namespace slib
{
	
	template <class T>
	class Nullable
	{
	public:
		SLIB_INLINE Nullable(): flagNull(sl_true)
		{
			NullValue<T>::get(&value);
		}
		
		SLIB_INLINE Nullable(const Nullable& other): flagNull(other.flagNull), value(other.value) {}
		
		SLIB_INLINE Nullable(Nullable&& other): flagNull(Move(other.flagNull)), value(Move(other.value)) {}
		
		SLIB_INLINE Nullable(sl_null_t): flagNull(sl_true)
		{
			NullValue<T>::get(&value);
		}
		
		template <class OTHER>
		SLIB_INLINE Nullable(const Nullable<OTHER>& other) : flagNull(other.flagNull), value(other.value) {}
		
		template <class... ARGS>
		SLIB_INLINE Nullable(ARGS... args) : flagNull(sl_false), value(Forward<ARGS...>(args...)) {}

		SLIB_INLINE constexpr operator T const&() const noexcept
		{
			return value;
		}

		SLIB_INLINE operator T&() noexcept
		{
			return value;
		}
		
		SLIB_INLINE Nullable& operator=(const Nullable& other)
		{
			flagNull = other.flagNull;
			value = other.value;
			return *this;
		}
		
		SLIB_INLINE Nullable& operator=(Nullable&& other)
		{
			flagNull = other.flagNull;
			value = Move(other.value);
			return *this;
		}
		
		SLIB_INLINE Nullable& operator=(sl_null_t)
		{
			flagNull = sl_true;
			NullValue<T>::get(&value);
			return *this;
		}
		
		template <class OTHER>
		SLIB_INLINE Nullable& operator=(const Nullable<OTHER>& other)
		{
			flagNull = other.flagNull;
			value = other.value;
			return *this;
		}
		
		template <class ARG>
		SLIB_INLINE Nullable& operator=(ARG&& arg)
		{
			flagNull = sl_false;
			value = Forward<ARG>(arg);
			return *this;
		}
		
		SLIB_INLINE constexpr sl_bool isNull() const noexcept
		{
			return flagNull;
		}
		
		SLIB_INLINE constexpr sl_bool isNotNull() const noexcept
		{
			return !flagNull;
		}
		
		SLIB_INLINE void setNull()
		{
			flagNull = sl_true;
			NullValue<T>::get(&value);
		}

		SLIB_INLINE constexpr T const& get() const noexcept
		{
			return value;
		}
		
		SLIB_INLINE T& get() noexcept
		{
			return value;
		}
		
		SLIB_INLINE sl_bool operator==(const Nullable<T>& other) const
		{
			if (flagNull == other.flagNull) {
				if (flagNull) {
					return sl_true;
				} else {
					return value == other.value;
				}
			} else {
				return sl_false;
			}
		}
		
		SLIB_INLINE sl_bool operator!=(const Nullable<T>& other) const
		{
			if (flagNull == other.flagNull) {
				if (flagNull) {
					return sl_false;
				} else {
					return value != other.value;
				}
			} else {
				return sl_true;
			}
		}
		
		SLIB_INLINE sl_bool operator>(const Nullable<T>& other) const
		{
			if (flagNull == other.flagNull) {
				if (flagNull) {
					return sl_false;
				} else {
					return value > other.value;
				}
			} else {
				return other.flagNull;
			}
		}

		SLIB_INLINE sl_bool operator>=(const Nullable<T>& other) const
		{
			if (other.flagNull) {
				return sl_true;
			} else {
				if (flagNull) {
					return sl_false;
				} else {
					return value >= other.value;
				}
			}
		}
		
		SLIB_INLINE sl_bool operator<(const Nullable<T>& other) const
		{
			if (flagNull == other.flagNull) {
				if (flagNull) {
					return sl_false;
				} else {
					return value < other.value;
				}
			} else {
				return flagNull;
			}
		}
		
		SLIB_INLINE sl_bool operator<=(const Nullable<T>& other) const
		{
			if (flagNull) {
				return sl_true;
			} else {
				if (other.flagNull) {
					return sl_false;
				} else {
					return value <= other.value;
				}
			}
		}
		
	public:
		T value;
		sl_bool flagNull;
		
	};
	
	template <class T>
	class Hash< Nullable<T> >
	{
	public:
		SLIB_INLINE sl_size operator()(const Nullable<T>& v) const
		{
			if (v.flagNull) {
				return 0;
			} else {
				return Hash<T>()(v.value);
			}
		}
	};

}

#endif
