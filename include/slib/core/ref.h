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

#ifndef CHECKHEADER_SLIB_CORE_REF
#define CHECKHEADER_SLIB_CORE_REF

#include "definition.h"

#include "base.h"
#include "atomic.h"
#include "macro.h"

#ifdef SLIB_DEBUG
#define SLIB_DEBUG_REFERENCE
#endif

typedef const void* sl_object_type;

namespace slib
{
	
	class CWeakRef;
	
	class SLIB_EXPORT Referable
	{
	public:
		Referable() noexcept;

		Referable(const Referable& other) noexcept;
		
		Referable(Referable&& other) noexcept;

		virtual ~Referable() noexcept;

	public:
		sl_reg increaseReference() noexcept;

		sl_reg decreaseReference() noexcept;

		sl_reg decreaseReferenceNoFree() noexcept;
		
		sl_reg getReferenceCount() noexcept;

		void makeNeverFree() noexcept;

	public:
		static sl_object_type ObjectType() noexcept;
		
		static sl_bool isDerivedFrom(sl_object_type type) noexcept;
		
		virtual sl_object_type getObjectType() const noexcept;

		virtual sl_bool isInstanceOf(sl_object_type type) const noexcept;

	private:
		void _clearWeak() noexcept;

	public:
		sl_bool _isWeakRef() const noexcept;

		CWeakRef* _getWeakObject() noexcept;

		void _free() noexcept;
		
	public:
		Referable& operator=(const Referable& other) = delete;
		
		Referable& operator=(Referable&& other) = delete;

	private:
		sl_reg m_signature;
#if defined(SLIB_DEBUG_REFERENCE)
		void _checkValid() noexcept;
#endif

	private:
		sl_reg m_nRefCount;
		sl_bool m_flagWeakRef;
		CWeakRef* m_weak;
		SpinLock m_lockWeak;

		friend class CWeakRef;
		
	};

	
	template <class T>
	class Ref;
	
	template <class T>
	class WeakRef;
	
	template <class T>
	using AtomicRef = Atomic< Ref<T> >;
	
	template <class T>
	using AtomicWeakRef = Atomic< WeakRef<T> >;

	
	template <class T>
	class SLIB_EXPORT Ref
	{
	public:
		constexpr Ref() noexcept : _ptr(sl_null) {}

		constexpr Ref(sl_null_t) noexcept : _ptr(sl_null) {}

		Ref(T* _other) noexcept;

		Ref(Ref<T>&& other) noexcept;

		Ref(const Ref<T>& other) noexcept;

		template <class OTHER>
		Ref(Ref<OTHER>&& other) noexcept;

		template <class OTHER>
		Ref(const Ref<OTHER>& other) noexcept;

		Ref(AtomicRef<T>&& other) noexcept;

		Ref(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		Ref(AtomicRef<OTHER>&& other) noexcept;

		template <class OTHER>
		Ref(const AtomicRef<OTHER>& other) noexcept;

		Ref(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		Ref(const WeakRef<OTHER>& other) noexcept;

		Ref(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		Ref(const AtomicWeakRef<OTHER>& other) noexcept;

		~Ref() noexcept;
	
	public:
		static const Ref<T>& null() noexcept;

		sl_bool isNull() const noexcept;
	
		sl_bool isNotNull() const noexcept;
	
		void setNull() noexcept;

		T* get() const& noexcept;

		const Ref<Referable>& getReference() const noexcept;

		template <class OTHER>
		static const Ref<T>& from(const Ref<OTHER>& other) noexcept;

		template <class OTHER>
		static Ref<T>& from(Ref<OTHER>& other) noexcept;

		template <class OTHER>
		static Ref<T>&& from(Ref<OTHER>&& other) noexcept;

	public:
		Ref<T>& operator=(sl_null_t) noexcept;

		Ref<T>& operator=(T* other) noexcept;
	
		Ref<T>& operator=(Ref<T>&& other) noexcept;

		Ref<T>& operator=(const Ref<T>& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(Ref<OTHER>&& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(const Ref<OTHER>& other) noexcept;

		Ref<T>& operator=(AtomicRef<T>&& other) noexcept;

		Ref<T>& operator=(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(AtomicRef<OTHER>&& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(const AtomicRef<OTHER>& other) noexcept;

		Ref<T>& operator=(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(const WeakRef<OTHER>& other) noexcept;

		Ref<T>& operator=(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		Ref<T>& operator=(const AtomicWeakRef<OTHER>& other) noexcept;
	
	public:
		sl_bool operator==(sl_null_t) const noexcept;

		sl_bool operator==(T* other) const noexcept;

		sl_bool operator==(const Ref<T>& other) const noexcept;
	
		template <class OTHER>
		sl_bool operator==(const Ref<OTHER>& other) const noexcept;

		sl_bool operator==(const AtomicRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const AtomicRef<OTHER>& other) const noexcept;

		sl_bool operator!=(sl_null_t) const noexcept;
	
		sl_bool operator!=(T* other) const noexcept;

		sl_bool operator!=(const Ref<T>& other) const noexcept;
	
		template <class OTHER>
		sl_bool operator!=(const Ref<OTHER>& other) const noexcept;

		sl_bool operator!=(const AtomicRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const AtomicRef<OTHER>& other) const noexcept;
	
	public:
		T& operator*() const& noexcept;
	
		T* operator->() const& noexcept;
		
		explicit operator sl_bool() const noexcept;
	
	public:
		void _replaceObject(T* other) noexcept;

		void _move_init(void* other) noexcept;
	
		void _move_assign(void* other) noexcept;

	public:
		T* _ptr;

	};
	
	template <class T>
	class Atomic< Ref<T> >
	{
	public:
		constexpr Atomic() noexcept : _ptr(sl_null) {}

		constexpr Atomic(sl_null_t) noexcept : _ptr(sl_null) {}

		Atomic(T* other) noexcept;

		Atomic(AtomicRef<T>&& other) noexcept;
	
		Atomic(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(AtomicRef<OTHER>&& other) noexcept;
	
		template <class OTHER>
		Atomic(const AtomicRef<OTHER>& other) noexcept;

		Atomic(Ref<T>&& other) noexcept;

		Atomic(const Ref<T>& other) noexcept;

		template <class OTHER>
		Atomic(Ref<OTHER>&& other) noexcept;
	
		template <class OTHER>
		Atomic(const Ref<OTHER>& other) noexcept;

		Atomic(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(const WeakRef<OTHER>& other) noexcept;

		Atomic(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(const AtomicWeakRef<OTHER>& other) noexcept;
	
		~Atomic() noexcept;
	
	public:
		static const AtomicRef<T>& null() noexcept;
	
		sl_bool isNull() const noexcept;
	
		sl_bool isNotNull() const noexcept;
	
		void setNull() noexcept;
	
		template <class OTHER>
		static const AtomicRef<T>& from(const AtomicRef<OTHER>& other) noexcept;

		template <class OTHER>
		static AtomicRef<T>& from(AtomicRef<OTHER>& other) noexcept;

		template <class OTHER>
		static AtomicRef<T>&& from(AtomicRef<OTHER>&& other) noexcept;
	
	public:
		AtomicRef<T>& operator=(sl_null_t) noexcept;

		AtomicRef<T>& operator=(T* other) noexcept;

		AtomicRef<T>& operator=(AtomicRef<T>&& other) noexcept;

		AtomicRef<T>& operator=(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		AtomicRef<T>& operator=(AtomicRef<OTHER>&& other) noexcept;
	
		template <class OTHER>
		AtomicRef<T>& operator=(const AtomicRef<OTHER>& other) noexcept;

		AtomicRef<T>& operator=(Ref<T>&& other) noexcept;
	
		AtomicRef<T>& operator=(const Ref<T>& other) noexcept;

		template <class OTHER>
		AtomicRef<T>& operator=(Ref<OTHER>&& other) noexcept;
	
		template <class OTHER>
		AtomicRef<T>& operator=(const Ref<OTHER>& other) noexcept;

		AtomicRef<T>& operator=(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		AtomicRef<T>& operator=(const WeakRef<OTHER>& other) noexcept;

		AtomicRef<T>& operator=(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		AtomicRef<T>& operator=(const AtomicWeakRef<OTHER>& other) noexcept;
	
	public:
		sl_bool operator==(sl_null_t) const noexcept;

		sl_bool operator==(T* other) const noexcept;

		sl_bool operator==(const AtomicRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const AtomicRef<OTHER>& other) const noexcept;

		sl_bool operator==(const Ref<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const Ref<OTHER>& other) const noexcept;

		sl_bool operator!=(sl_null_t) const noexcept;

		sl_bool operator!=(T* other) const noexcept;

		sl_bool operator!=(const AtomicRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const AtomicRef<OTHER>& other) const noexcept;

		sl_bool operator!=(const Ref<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const Ref<OTHER>& other) const noexcept;

	public:
		Ref<T> operator*() const noexcept;
		
		explicit operator sl_bool() const noexcept;
	
	public:
		T* _retainObject() const noexcept;

		void _replaceObject(T* other) noexcept;

		void _move_init(void* other) noexcept;

		void _move_assign(void* other) noexcept;

	public:
		T* _ptr;
	private:
		SpinLock m_lock;
	
	};

	
	template <class T>
	class SLIB_EXPORT WeakRef
	{
	public:
		SLIB_INLINE WeakRef() noexcept = default;

		SLIB_INLINE WeakRef(sl_null_t) noexcept {}

		WeakRef(T* other) noexcept;
	
		WeakRef(WeakRef<T>&& other) noexcept;

		WeakRef(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef(WeakRef<OTHER>&& other) noexcept;
	
		template <class OTHER>
		WeakRef(const WeakRef<OTHER>& other) noexcept;

		WeakRef(AtomicWeakRef<T>&& other) noexcept;
	
		WeakRef(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef(AtomicWeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		WeakRef(const AtomicWeakRef<OTHER>& other) noexcept;

		WeakRef(const Ref<T>& other) noexcept;
	
		template <class OTHER>
		WeakRef(const Ref<OTHER>& other) noexcept;
	
		WeakRef(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef(const AtomicRef<OTHER>& other) noexcept;

		~WeakRef() noexcept;

	public:
		static const WeakRef<T>& null() noexcept;

		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		void setNull() noexcept;

		template <class OTHER>
		static const WeakRef<T>& from(const WeakRef<OTHER>& other) noexcept;

		template <class OTHER>
		static WeakRef<T>& from(WeakRef<OTHER>& other) noexcept;

		template <class OTHER>
		static WeakRef<T>&& from(WeakRef<OTHER>&& other) noexcept;

		Ref<T> lock() const noexcept;

		static WeakRef<T> fromReferable(Referable* referable) noexcept;

	public:
		WeakRef<T>& operator=(sl_null_t) noexcept;

		WeakRef<T>& operator=(T* other) noexcept;
	
		WeakRef<T>& operator=(WeakRef<T>&& other) noexcept;

		WeakRef<T>& operator=(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef<T>& operator=(WeakRef<OTHER>&& other) noexcept;
	
		template <class OTHER>
		WeakRef<T>& operator=(const WeakRef<OTHER>& other) noexcept;

		WeakRef<T>& operator=(AtomicWeakRef<T>&& other) noexcept;

		WeakRef<T>& operator=(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef<T>& operator=(AtomicWeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		WeakRef<T>& operator=(const AtomicWeakRef<OTHER>& other) noexcept;
	
		WeakRef<T>& operator=(const Ref<T>& other) noexcept;
	
		template <class OTHER>
		WeakRef<T>& operator=(const Ref<OTHER>& other) noexcept;
	
		WeakRef<T>& operator=(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		WeakRef<T>& operator=(const AtomicRef<OTHER>& other) noexcept;

	public:
		sl_bool operator==(const WeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const WeakRef<OTHER>& other) const noexcept;

		sl_bool operator==(const AtomicWeakRef<T>& other) const noexcept;
	
		template <class OTHER>
		sl_bool operator==(const AtomicWeakRef<OTHER>& other) const noexcept;
	
		sl_bool operator!=(const WeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const WeakRef<OTHER>& other) const noexcept;

		sl_bool operator!=(const AtomicWeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const AtomicWeakRef<OTHER>& other) const noexcept;
		
	public:
		explicit operator sl_bool() const noexcept;
	
	private:
		void _set(T* other) noexcept;

	public:
		Ref<CWeakRef> _weak;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< WeakRef<T> >
	{
	public:
		SLIB_INLINE Atomic() noexcept = default;

		SLIB_INLINE Atomic(sl_null_t) noexcept {}

		Atomic(T* other) noexcept;

		Atomic(AtomicWeakRef<T>&& other) noexcept;

		Atomic(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(AtomicWeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		Atomic(const AtomicWeakRef<OTHER>& other) noexcept;

		Atomic(WeakRef<T>&& other) noexcept;

		Atomic(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(WeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		Atomic(const WeakRef<OTHER>& other) noexcept;

		Atomic(const Ref<T>& other) noexcept;

		template <class OTHER>
		Atomic(const Ref<OTHER>& other) noexcept;

		Atomic(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		Atomic(const AtomicRef<OTHER>& other) noexcept;

		~Atomic() noexcept;

	public:
		static const AtomicWeakRef<T>& null() noexcept;

		sl_bool isNull() const noexcept;

		sl_bool isNotNull() const noexcept;

		void setNull() noexcept;

		template <class OTHER>
		static const AtomicWeakRef<T>& from(const AtomicWeakRef<OTHER>& other) noexcept;

		template <class OTHER>
		static AtomicWeakRef<T>& from(AtomicWeakRef<OTHER>& other) noexcept;

		template <class OTHER>
		static AtomicWeakRef<T>&& from(AtomicWeakRef<OTHER>&& other) noexcept;
	
		Ref<T> lock() const noexcept;
	
	public:
		AtomicWeakRef<T>& operator=(sl_null_t) noexcept;

		AtomicWeakRef<T>& operator=(T* other) noexcept;
	
		AtomicWeakRef<T>& operator=(AtomicWeakRef<T>&& other) noexcept;

		AtomicWeakRef<T>& operator=(const AtomicWeakRef<T>& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(AtomicWeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(const AtomicWeakRef<OTHER>& other) noexcept;

		AtomicWeakRef<T>& operator=(WeakRef<T>&& other) noexcept;

		AtomicWeakRef<T>& operator=(const WeakRef<T>& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(WeakRef<OTHER>&& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(const WeakRef<OTHER>& other) noexcept;

		AtomicWeakRef<T>& operator=(const Ref<T>& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(const Ref<OTHER>& other) noexcept;

		AtomicWeakRef<T>& operator=(const AtomicRef<T>& other) noexcept;

		template <class OTHER>
		AtomicWeakRef<T>& operator=(const AtomicRef<OTHER>& other) noexcept;

	public:
		sl_bool operator==(const AtomicWeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const AtomicWeakRef<OTHER>& other) const noexcept;

		sl_bool operator==(const WeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator==(const WeakRef<OTHER>& other) const noexcept;

		sl_bool operator!=(const AtomicWeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const AtomicWeakRef<OTHER>& other) const noexcept;

		sl_bool operator!=(const WeakRef<T>& other) const noexcept;

		template <class OTHER>
		sl_bool operator!=(const WeakRef<OTHER>& other) const noexcept;

	public:
		WeakRef<T> operator*() const noexcept;
		
		explicit operator sl_bool() const noexcept;
	
	private:
		void _set(T* other) noexcept;

	public:
		AtomicRef<CWeakRef> _weak;
	
	};
	
	template <class T>
	struct PropertyTypeHelper< WeakRef<T> >
	{
		typedef Ref<T> const& ArgType;
		typedef Ref<T> RetType;
	};

	
	class SLIB_EXPORT CWeakRef : public Referable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CWeakRef() noexcept;

		~CWeakRef() noexcept;

	public:
		Referable* m_object;
		SpinLock m_lock;

	public:
		static CWeakRef* create(Referable* object) noexcept;

	public:
		Ref<Referable> lock() noexcept;

		void release() noexcept;
		
	};
	
	class SLIB_EXPORT ReferableKeeper
	{
	public:
		ReferableKeeper(Referable* object) noexcept;

		~ReferableKeeper() noexcept;

	private:
		Referable* m_object;

	};
	
	template <class T, class... ARGS>
	Ref<T> New(ARGS&&... args) noexcept;

	template <class T, class... ARGS>
	Ref<T> Init(ARGS&&... args) noexcept;

	template <class T, class OTHER>
	sl_bool IsInstanceOf(OTHER* object) noexcept;

	template <class T, class OTHER>
	sl_bool IsInstanceOf(const Ref<OTHER>& object) noexcept;
	
	template <class T, class OTHER>
	T* CastInstance(OTHER* object) noexcept;

	template <class T, class OTHER>
	const Ref<T>& CastRef(const Ref<OTHER>& object) noexcept;

	template <class T, class OTHER>
	const Ref<T>& CastRef(const Ref<OTHER>& object, const Ref<T>& def) noexcept;

}

#include "detail/ref.inc"

#endif
