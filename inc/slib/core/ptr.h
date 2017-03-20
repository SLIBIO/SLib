/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PTR
#define CHECKHEADER_SLIB_CORE_PTR

#include "definition.h"

#include "ref.h"

namespace slib
{
	
	template <class T>
	class Ptr;
	
	template <class T>
	using AtomicPtr = Atomic< Ptr<T> >;
	
	struct _Ptr_Const
	{
		void* _ptr;
		void* ref;
		sl_int32 lock;
	};
	
	extern const _Ptr_Const _Ptr_Null;
	
	template <class T>
	class SLIB_EXPORT Ptr
	{
	public:
		T* _ptr;
		Ref<Referable> ref;
	
	public:
		Ptr();

		Ptr(Ptr<T>&& other);
	
		Ptr(const Ptr<T>& other);

		template <class O>
		Ptr(Ptr<O>&& other);
	
		template <class O>
		Ptr(const Ptr<O>& other);

		Ptr(AtomicPtr<T>&& other);

		Ptr(const AtomicPtr<T>& other);

		template <class O>
		Ptr(AtomicPtr<O>&& other);

		template <class O>
		Ptr(const AtomicPtr<O>& other);

		Ptr(sl_null_t);

		Ptr(T* pointer);

		template <class O>
		Ptr(const Ref<O>& reference);

		template <class O>
		Ptr(T* pointer, const Ref<O>& reference);
	
		template <class O>
		Ptr(const AtomicRef<O>& reference);

		template <class O>
		Ptr(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		Ptr(const WeakRef<O>& weak);

		template <class O>
		Ptr(T* pointer, const WeakRef<O>& weak);

		template <class O>
		Ptr(const AtomicWeakRef<O>& weak);

		template <class O>
		Ptr(T* pointer, const AtomicWeakRef<O>& weak);

	public:
		static Ptr<T> fromPointer(T* pointer);

		template <class O>
		static Ptr<T> fromRef(O* reference);

		template <class O>
		static Ptr<T> fromRef(T* pointer, O* reference);

		template <class O>
		static Ptr<T> fromRef(const Ref<O>& reference);

		template <class O>
		static Ptr<T> fromRef(T* pointer, const Ref<O>& reference);

		template <class O>
		static Ptr<T> fromRef(const AtomicRef<O>& reference);

		template <class O>
		static Ptr<T> fromRef(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		static Ptr<T> fromRef(const WeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromRef(T* pointer, const WeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromRef(const AtomicWeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromRef(T* pointer, const AtomicWeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(O* reference);

		template <class O>
		static Ptr<T> fromWeak(T* pointer, O* reference);

		template <class O>
		static Ptr<T> fromWeak(const Ref<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const Ref<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(const AtomicRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(const WeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const WeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(const AtomicWeakRef<O>& reference);

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const AtomicWeakRef<O>& reference);

	public:
		static const Ptr<T>& null();
	
		sl_bool isNull() const;

		sl_bool isNotNull() const;

		void setNull();

		template <class O>
		static const Ptr<T>& from(const Ptr<O>& other);
	
		sl_bool isWeak() const;

		Ptr<T> lock() const;

	public:
		void setPointer(T* pointer);

		template <class O>
		void setRef(O* reference);

		template <class O>
		void setRef(T* pointer, O* reference);

		template <class O>
		void setRef(const Ref<O>& reference);

		template <class O>
		void setRef(T* pointer, const Ref<O>& reference);

		template <class O>
		void setRef(const AtomicRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		void setRef(const WeakRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const WeakRef<O>& reference);

		template <class O>
		void setRef(const AtomicWeakRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const AtomicWeakRef<O>& reference);

		template <class O>
		void setWeak(O* reference);

		template <class O>
		void setWeak(T* pointer, O* reference);

		template <class O>
		void setWeak(const Ref<O>& reference);

		template <class O>
		void setWeak(T* pointer, const Ref<O>& reference);

		template <class O>
		void setWeak(const AtomicRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		void setWeak(const WeakRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const WeakRef<O>& reference);

		template <class O>
		void setWeak(const AtomicWeakRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const AtomicWeakRef<O>& reference);

	public:
		Ptr<T>& operator=(Ptr<T>&& other);

		Ptr<T>& operator=(const Ptr<T>& other);

		template <class O>
		Ptr<T>& operator=(Ptr<O>&& other);
	
		template <class O>
		Ptr<T>& operator=(const Ptr<O>& other);

		Ptr<T>& operator=(AtomicPtr<T>&& other);

		Ptr<T>& operator=(const AtomicPtr<T>& other);

		template <class O>
		Ptr<T>& operator=(AtomicPtr<O>&& other);

		template <class O>
		Ptr<T>& operator=(const AtomicPtr<O>& other);

		Ptr<T>& operator=(sl_null_t);

		Ptr<T>& operator=(T* pointer);

		template <class O>
		Ptr<T>& operator=(const Ref<O>& reference);

		template <class O>
		Ptr<T>& operator=(const AtomicRef<O>& reference);

		template <class O>
		Ptr<T>& operator=(const WeakRef<O>& weak);

		template <class O>
		Ptr<T>& operator=(const AtomicWeakRef<O>& weak);


		sl_bool operator==(const Ptr<T>& other) const;

		template <class O>
		sl_bool operator==(const Ptr<O>& other) const;
	
		sl_bool operator==(const AtomicPtr<T>& other) const;

		template <class O>
		sl_bool operator==(const AtomicPtr<O>& other) const;

		sl_bool operator!=(const Ptr<T>& other) const;

		template <class O>
		sl_bool operator!=(const Ptr<O>& other) const;

		sl_bool operator!=(const AtomicPtr<T>& other) const;

		template <class O>
		sl_bool operator!=(const AtomicPtr<O>& other) const;


		T& operator*() const;

		T* operator->() const;
	

	public:
		void _move_init(void* _other);

		void _move_assign(void* _other);

	};
	
	
	template <class T>
	class SLIB_EXPORT Atomic< Ptr<T> >
	{
	public:
		T* _ptr;
		Ref<Referable> _ref;
	
	public:
		Atomic();

		Atomic(AtomicPtr<T>&& other);

		Atomic(const AtomicPtr<T>& other);

		template <class O>
		Atomic(AtomicPtr<O>&& other);

		template <class O>
		Atomic(const AtomicPtr<O>& other);

		Atomic(Ptr<T>&& other);

		Atomic(const Ptr<T>& other);

		template <class O>
		Atomic(Ptr<O>&& other);
	
		template <class O>
		Atomic(const Ptr<O>& other);

		Atomic(sl_null_t);

		Atomic(T* pointer);

		template <class O>
		Atomic(const Ref<O>& reference);

		template <class O>
		Atomic(T* pointer, const Ref<O>& reference);

		template <class O>
		Atomic(const AtomicRef<O>& reference);

		template <class O>
		Atomic(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		Atomic(const WeakRef<O>& weak);

		template <class O>
		Atomic(T* pointer, const WeakRef<O>& weak);

		template <class O>
		Atomic(const AtomicWeakRef<O>& weak);

		template <class O>
		Atomic(T* pointer, const AtomicWeakRef<O>& weak);

	public:
		static const AtomicPtr<T>& null();

		sl_bool isNull() const;

		sl_bool isNotNull() const;

		void setNull();

		template <class O>
		static const AtomicPtr<T>& from(const AtomicPtr<O>& other);

		Ptr<T> lock() const;

	public:
		void setPointer(T* pointer);

		template <class O>
		void setRef(O* reference);

		template <class O>
		void setRef(T* pointer, O* reference);

		template <class O>
		void setRef(const Ref<O>& reference);

		template <class O>
		void setRef(T* pointer, const Ref<O>& reference);

		template <class O>
		void setRef(const AtomicRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		void setRef(const WeakRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const WeakRef<O>& reference);

		template <class O>
		void setRef(const AtomicWeakRef<O>& reference);

		template <class O>
		void setRef(T* pointer, const AtomicWeakRef<O>& reference);

		template <class O>
		void setWeak(O* reference);

		template <class O>
		void setWeak(T* pointer, O* reference);

		template <class O>
		void setWeak(const Ref<O>& reference);

		template <class O>
		void setWeak(T* pointer, const Ref<O>& reference);

		template <class O>
		void setWeak(const AtomicRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const AtomicRef<O>& reference);

		template <class O>
		void setWeak(const WeakRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const WeakRef<O>& reference);

		template <class O>
		void setWeak(const AtomicWeakRef<O>& reference);

		template <class O>
		void setWeak(T* pointer, const AtomicWeakRef<O>& reference);

	public:
		AtomicPtr<T>& operator=(AtomicPtr<T>&& other);

		AtomicPtr<T>& operator=(const AtomicPtr<T>& other);

		template <class O>
		AtomicPtr<T>& operator=(AtomicPtr<O>&& other);
	
		template <class O>
		AtomicPtr<T>& operator=(const AtomicPtr<O>& other);

		AtomicPtr<T>& operator=(Ptr<T>&& other);
	
		AtomicPtr<T>& operator=(const Ptr<T>& other);

		template <class O>
		AtomicPtr<T>& operator=(Ptr<O>&& other);
	
		template <class O>
		AtomicPtr<T>& operator=(const Ptr<O>& other);

		AtomicPtr<T>& operator=(sl_null_t);

		AtomicPtr<T>& operator=(T* pointer);

		template <class O>
		AtomicPtr<T>& operator=(const Ref<O>& reference);

		template <class O>
		AtomicPtr<T>& operator=(const AtomicRef<O>& reference);

		template <class O>
		AtomicPtr<T>& operator=(const WeakRef<O>& weak);

		template <class O>
		AtomicPtr<T>& operator=(const AtomicWeakRef<O>& weak);


		sl_bool operator==(const AtomicPtr<T>& other) const;

		template <class O>
		sl_bool operator==(const AtomicPtr<O>& other) const;

		sl_bool operator==(const Ptr<T>& other) const;

		template <class O>
		sl_bool operator==(const Ptr<O>& other) const;

		sl_bool operator!=(const AtomicPtr<T>& other) const;

		template <class O>
		sl_bool operator!=(const AtomicPtr<O>& other) const;

		sl_bool operator!=(const Ptr<T>& other) const;

		template <class O>
		sl_bool operator!=(const Ptr<O>& other) const;

	public:
		T* _retain(Ref<Referable>& reference) const;

		void _replace(T* pointer, const Ref<Referable>& reference);

		void _move_init(void* _other);

		void _move_assign(void* _other);
	
	private:
		SpinLock m_lock;
	
	};
	
	
	template <class T>
	class SLIB_EXPORT PtrLocker
	{
	private:
		Ptr<T> m_ptr;

	public:
		PtrLocker(const Ptr<T>& ptr);

		PtrLocker(const AtomicPtr<T>& ptr);

	public:
		void unlock();
	
		T* get();

		sl_bool isNull();
	
		sl_bool isNotNull();
	
		T& operator*() const;
	
		T* operator->() const;

	};

}

#include "detail/ptr.h"

#endif
