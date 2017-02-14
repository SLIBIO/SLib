#ifndef CHECKHEADER_SLIB_CORE_DETAIL_NEW_HELPER
#define CHECKHEADER_SLIB_CORE_DETAIL_NEW_HELPER

#include "../new_helper.h"

namespace slib
{
	
	template <class T>
	SLIB_INLINE T* NewHelper<T>::create()
	{
		T* data = (T*)(Base::createMemory(sizeof(T)));
		if (data) {
			new (data) T();
		}
		return data;
	}

	template <class T>
	SLIB_INLINE T* NewHelper<T>::create(sl_size count)
	{
		T* data = (T*)(Base::createMemory(sizeof(T)*count));
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				new (data + i) T();
			}
		}
		return data;
	}

	template <class T>
	template <class _T>
	SLIB_INLINE T* NewHelper<T>::create(const _T* other)
	{
		T* data = (T*)(Base::createMemory(sizeof(T)));
		if (data) {
			new (data) T(*other);
		}
		return data;
	}

	template <class T>
	template <class _T>
	SLIB_INLINE T* NewHelper<T>::create(const _T* other, sl_size count)
	{
		T* data = (T*)(Base::createMemory(sizeof(T)*count));
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				new (data + i) T(other[i]);
			}
		}
		return data;
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::free(T* data)
	{
		if (data) {
			data->~T();
			Base::freeMemory((void*)data);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::free(T* data, sl_size count)
	{
		if (data) {
			for (sl_size i = 0; i < count; i++) {
				(data+i)->~T();
			}
			Base::freeMemory((void*)data);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data)
	{
		new (data) T();
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			new (data + i) T();
		}
	}

	template <class T>
	template <class _T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, const _T* other)
	{
		new (data) T(*other);
	}

	template <class T>
	template <class _T>
	SLIB_INLINE void NewHelper<T>::constructor(T* data, const _T* other, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			new (data + i) T(other[i]);
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::destructor(T* data)
	{
		if (data) {
			data->~T();
		}
	}

	template <class T>
	SLIB_INLINE void NewHelper<T>::destructor(T* data, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			(data+i)->~T();
		}
	}

}

#endif

