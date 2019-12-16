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

#ifndef CHECKHEADER_SLIB_DB_PARAMETER
#define CHECKHEADER_SLIB_DB_PARAMETER

#include "definition.h"

#include "../core/variant.h"

namespace slib
{
	
	template <class TYPE>
	class SLIB_EXPORT DatabaseParametersLocker
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const TYPE& map, const List<String>& _names)
		{
			data = sl_null;
			count = 0;
			ListElements<String> names(_names);
			if (names.count) {
				List<Variant> list = List<Variant>::create(0, names.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < names.count; i++) {
						list.add_NoLock(map.getValue_NoLock(names[i]));
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <class T>
	class SLIB_EXPORT DatabaseParametersLocker< List<T> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const List<T>& _src, const List<String>& names)
		{
			data = sl_null;
			count = 0;
			ListLocker<T> src(_src);
			if (src.count) {
				List<Variant> list = List<Variant>::create(0, src.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < src.count; i++) {
						list.add_NoLock(src[i]);
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <class T>
	class SLIB_EXPORT DatabaseParametersLocker< Atomic< List<T> > >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const Atomic< List<T> >& _src, const List<String>& names)
		{
			data = sl_null;
			count = 0;
			ListLocker<T> src(_src);
			if (src.count) {
				List<Variant> list = List<Variant>::create(0, src.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < src.count; i++) {
						list.add_NoLock(src[i]);
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <class T>
	class SLIB_EXPORT DatabaseParametersLocker< ListParam<T> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListParam<T>& _src, const List<String>& names)
		{
			data = sl_null;
			count = 0;
			ListLocker<T> src(_src);
			if (src.count) {
				List<Variant> list = List<Variant>::create(0, src.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < src.count; i++) {
						list.add_NoLock(src[i]);
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <class T>
	class SLIB_EXPORT DatabaseParametersLocker< ListElements<T> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListElements<T>& src, const List<String>& names)
		{
			data = sl_null;
			count = 0;
			if (src.count) {
				List<Variant> list = List<Variant>::create(0, src.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < src.count; i++) {
						list.add_NoLock(src[i]);
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <class T>
	class SLIB_EXPORT DatabaseParametersLocker< ListLocker<T> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListLocker<T>& src, const List<String>& names)
		{
			data = sl_null;
			count = 0;
			if (src.count) {
				List<Variant> list = List<Variant>::create(0, src.count);
				if (list.isNotNull()) {
					for (sl_size i = 0; i < src.count; i++) {
						list.add_NoLock(src[i]);
					}
					m_list = list;
					data = list.getData();
					count = list.getCount();
				}
			}
		}
		
	private:
		List<Variant> m_list;

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker< List<Variant> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const List<Variant>& src, const List<String>& names): m_locker(src)
		{
			data = m_locker.data;
			count = m_locker.count;
		}
		
	private:
		ListLocker<Variant> m_locker;

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker< Atomic< List<Variant> > >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const Atomic< List<Variant> >& src, const List<String>& names): m_locker(src)
		{
			data = m_locker.data;
			count = m_locker.count;
		}
		
	private:
		ListLocker<Variant> m_locker;

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker< ListParam<Variant> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListParam<Variant>& src, const List<String>& names): m_locker(src)
		{
			data = m_locker.data;
			count = m_locker.count;
		}
		
	public:
		ListLocker<Variant> m_locker;

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker< ListElements<Variant> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListElements<Variant>& src, const List<String>& names)
		{
			data = src.data;
			count = src.count;
		}

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker< ListLocker<Variant> >
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(const ListLocker<Variant>& src, const List<String>& names)
		{
			data = src.data;
			count = src.count;
		}

	};
	
	template <>
	class SLIB_EXPORT DatabaseParametersLocker<sl_null_t>
	{
	public:
		Variant* data;
		sl_size count;
		
	public:
		DatabaseParametersLocker(sl_null_t, const List<String>& names)
		{
			data = sl_null;
			count = 0;
		}

	};
	
}

#endif
