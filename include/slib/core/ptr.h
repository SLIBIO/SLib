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
		
	template <class T>
	class SLIB_EXPORT Ptr
	{
	public:
		T* _ptr;
		Ref<Referable> ref;
	
	public:
		Ptr() noexcept;

		Ptr(Ptr<T>&& other) noexcept;
	
		Ptr(const Ptr<T>& other) noexcept;

		template <class O>
		Ptr(Ptr<O>&& other) noexcept;
	
		template <class O>
		Ptr(const Ptr<O>& other) noexcept;

		Ptr(AtomicPtr<T>&& other) noexcept;

		Ptr(const AtomicPtr<T>& other) noexcept;

		template <class O>
		Ptr(AtomicPtr<O>&& other) noexcept;

		template <class O>
		Ptr(const AtomicPtr<O>& other) noexcept;

		Ptr(sl_null_t) noexcept;

		Ptr(T* pointer) noexcept;

		template <class O>
		Ptr(const Ref<O>& reference) noexcept;

		template <class O>
		Ptr(T* pointer, const Ref<O>& reference) noexcept;
	
		template <class O>
		Ptr(const AtomicRef<O>& reference) noexcept;

		template <class O>
		Ptr(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		Ptr(const WeakRef<O>& weak) noexcept;

		template <class O>
		Ptr(T* pointer, const WeakRef<O>& weak) noexcept;

		template <class O>
		Ptr(const AtomicWeakRef<O>& weak) noexcept;

		template <class O>
		Ptr(T* pointer, const AtomicWeakRef<O>& weak) noexcept;

	public:
		static Ptr<T> fromPointer(T* pointer) noexcept;

		template <class O>
		static Ptr<T> fromRef(O* reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(T* pointer, O* reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(const Ref<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(const AtomicRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(const WeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromRef(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(O* reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(T* pointer, O* reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(const Ref<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(const AtomicRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(const WeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		static Ptr<T> fromWeak(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

	public:
		static const Ptr<T>& null() noexcept;
	
		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		void setNull() noexcept;

		template <class O>
		static const Ptr<T>& from(const Ptr<O>& other) noexcept;

		template <class O>
		static Ptr<T>& from(Ptr<O>& other) noexcept;

		template <class O>
		static Ptr<T>&& from(Ptr<O>&& other) noexcept;
	
		sl_bool isWeak() const noexcept;

		Ptr<T> lock() const noexcept;
		
		Ptr<T> toWeak() const noexcept;

	public:
		T* get() const noexcept;
		
		void setPointer(T* pointer) noexcept;

		template <class O>
		void setRef(O* reference) noexcept;

		template <class O>
		void setRef(T* pointer, O* reference) noexcept;

		template <class O>
		void setRef(const Ref<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		void setRef(const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setRef(const WeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(O* reference) noexcept;

		template <class O>
		void setWeak(T* pointer, O* reference) noexcept;

		template <class O>
		void setWeak(const Ref<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		void setWeak(const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setWeak(const WeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

	public:
		Ptr<T>& operator=(Ptr<T>&& other) noexcept;

		Ptr<T>& operator=(const Ptr<T>& other) noexcept;

		template <class O>
		Ptr<T>& operator=(Ptr<O>&& other) noexcept;
	
		template <class O>
		Ptr<T>& operator=(const Ptr<O>& other) noexcept;

		Ptr<T>& operator=(AtomicPtr<T>&& other) noexcept;

		Ptr<T>& operator=(const AtomicPtr<T>& other) noexcept;

		template <class O>
		Ptr<T>& operator=(AtomicPtr<O>&& other) noexcept;

		template <class O>
		Ptr<T>& operator=(const AtomicPtr<O>& other) noexcept;

		Ptr<T>& operator=(sl_null_t) noexcept;

		Ptr<T>& operator=(T* pointer) noexcept;

		template <class O>
		Ptr<T>& operator=(const Ref<O>& reference) noexcept;

		template <class O>
		Ptr<T>& operator=(const AtomicRef<O>& reference) noexcept;

		template <class O>
		Ptr<T>& operator=(const WeakRef<O>& weak) noexcept;

		template <class O>
		Ptr<T>& operator=(const AtomicWeakRef<O>& weak) noexcept;


		sl_bool operator==(const Ptr<T>& other) const noexcept;

		template <class O>
		sl_bool operator==(const Ptr<O>& other) const noexcept;
	
		sl_bool operator==(const AtomicPtr<T>& other) const noexcept;

		template <class O>
		sl_bool operator==(const AtomicPtr<O>& other) const noexcept;

		sl_bool operator!=(const Ptr<T>& other) const noexcept;

		template <class O>
		sl_bool operator!=(const Ptr<O>& other) const noexcept;

		sl_bool operator!=(const AtomicPtr<T>& other) const noexcept;

		template <class O>
		sl_bool operator!=(const AtomicPtr<O>& other) const noexcept;


		T& operator*() const noexcept;

		T* operator->() const noexcept;
	

	public:
		void _move_init(void* _other) noexcept;

		void _move_assign(void* _other) noexcept;

	};
	
	
	template <class T>
	class SLIB_EXPORT Atomic< Ptr<T> >
	{
	public:
		T* _ptr;
		Ref<Referable> _ref;
	
	public:
		Atomic() noexcept;

		Atomic(AtomicPtr<T>&& other) noexcept;

		Atomic(const AtomicPtr<T>& other) noexcept;

		template <class O>
		Atomic(AtomicPtr<O>&& other) noexcept;

		template <class O>
		Atomic(const AtomicPtr<O>& other) noexcept;

		Atomic(Ptr<T>&& other) noexcept;

		Atomic(const Ptr<T>& other) noexcept;

		template <class O>
		Atomic(Ptr<O>&& other) noexcept;
	
		template <class O>
		Atomic(const Ptr<O>& other) noexcept;

		Atomic(sl_null_t) noexcept;

		Atomic(T* pointer) noexcept;

		template <class O>
		Atomic(const Ref<O>& reference) noexcept;

		template <class O>
		Atomic(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		Atomic(const AtomicRef<O>& reference) noexcept;

		template <class O>
		Atomic(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		Atomic(const WeakRef<O>& weak) noexcept;

		template <class O>
		Atomic(T* pointer, const WeakRef<O>& weak) noexcept;

		template <class O>
		Atomic(const AtomicWeakRef<O>& weak) noexcept;

		template <class O>
		Atomic(T* pointer, const AtomicWeakRef<O>& weak) noexcept;

	public:
		static const AtomicPtr<T>& null() noexcept;

		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		void setNull() noexcept;

		template <class O>
		static const AtomicPtr<T>& from(const AtomicPtr<O>& other) noexcept;

		template <class O>
		static AtomicPtr<T>& from(AtomicPtr<O>& other) noexcept;

		template <class O>
		static AtomicPtr<T>&& from(AtomicPtr<O>&& other) noexcept;

		Ptr<T> lock() const noexcept;

	public:
		void setPointer(T* pointer) noexcept;

		template <class O>
		void setRef(O* reference) noexcept;

		template <class O>
		void setRef(T* pointer, O* reference) noexcept;

		template <class O>
		void setRef(const Ref<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		void setRef(const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setRef(const WeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setRef(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(O* reference) noexcept;

		template <class O>
		void setWeak(T* pointer, O* reference) noexcept;

		template <class O>
		void setWeak(const Ref<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const Ref<O>& reference) noexcept;

		template <class O>
		void setWeak(const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const AtomicRef<O>& reference) noexcept;

		template <class O>
		void setWeak(const WeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const WeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(const AtomicWeakRef<O>& reference) noexcept;

		template <class O>
		void setWeak(T* pointer, const AtomicWeakRef<O>& reference) noexcept;

	public:
		AtomicPtr<T>& operator=(AtomicPtr<T>&& other) noexcept;

		AtomicPtr<T>& operator=(const AtomicPtr<T>& other) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(AtomicPtr<O>&& other) noexcept;
	
		template <class O>
		AtomicPtr<T>& operator=(const AtomicPtr<O>& other) noexcept;

		AtomicPtr<T>& operator=(Ptr<T>&& other) noexcept;
	
		AtomicPtr<T>& operator=(const Ptr<T>& other) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(Ptr<O>&& other) noexcept;
	
		template <class O>
		AtomicPtr<T>& operator=(const Ptr<O>& other) noexcept;

		AtomicPtr<T>& operator=(sl_null_t) noexcept;

		AtomicPtr<T>& operator=(T* pointer) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(const Ref<O>& reference) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(const AtomicRef<O>& reference) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(const WeakRef<O>& weak) noexcept;

		template <class O>
		AtomicPtr<T>& operator=(const AtomicWeakRef<O>& weak) noexcept;


		sl_bool operator==(const AtomicPtr<T>& other) const noexcept;

		template <class O>
		sl_bool operator==(const AtomicPtr<O>& other) const noexcept;

		sl_bool operator==(const Ptr<T>& other) const noexcept;

		template <class O>
		sl_bool operator==(const Ptr<O>& other) const noexcept;

		sl_bool operator!=(const AtomicPtr<T>& other) const noexcept;

		template <class O>
		sl_bool operator!=(const AtomicPtr<O>& other) const noexcept;

		sl_bool operator!=(const Ptr<T>& other) const noexcept;

		template <class O>
		sl_bool operator!=(const Ptr<O>& other) const noexcept;

	public:
		T* _retain(Ref<Referable>& reference) const noexcept;

		void _replace(T* pointer, const Ref<Referable>& reference) noexcept;

		void _move_init(void* _other) noexcept;

		void _move_assign(void* _other) noexcept;
	
	private:
		SpinLock m_lock;
	
	};
	
	
	template <class T>
	class SLIB_EXPORT PtrLocker
	{
	private:
		Ptr<T> m_ptr;

	public:
		PtrLocker(const Ptr<T>& ptr) noexcept;

		PtrLocker(const AtomicPtr<T>& ptr) noexcept;

	public:
		void unlock() noexcept;
	
		T* get() noexcept;

		sl_bool isNull() noexcept;
	
		sl_bool isNotNull() noexcept;
	
		T& operator*() const noexcept;
	
		T* operator->() const noexcept;

	};
	
	template <class T>
	Ptr<T> SharedPtr(T* ptr);
	
	template <class T, class Deleter>
	Ptr<T> SharedPtr(T* ptr, const Deleter& deleter);

	template <class T, class... Args>
	Ptr<T> MakeShared(Args&&... args);

}

#include "detail/ptr.inc"

#endif
