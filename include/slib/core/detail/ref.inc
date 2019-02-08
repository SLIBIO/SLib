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

	SLIB_INLINE sl_bool Referable::_isWeakRef() const noexcept
	{
		return m_flagWeakRef;
	}

	struct _priv_Ref_Const;
	extern const _priv_Ref_Const _priv_Ref_Null;

	template <class T, class... ARGS>
	SLIB_INLINE Ref<T> New(ARGS&&... args) noexcept
	{
		return new T(Forward<ARGS>(args)...);
	}
	
	template <class REF, class T>
	struct _priv_InitHelper
	{
		template <class... ARGS>
		SLIB_INLINE static void init(REF* o, ARGS&&... args) noexcept { (*o) = (*o)->init(Forward<ARGS>(args)...); }
	};
	
	template <class REF>
	struct _priv_InitHelper<REF, void>
	{
		template <class... ARGS>
		SLIB_INLINE static void init(REF* o, ARGS&&... args) noexcept { (*o)->init(Forward<ARGS>(args)...); }
	};
	
	template <class T, class... ARGS>
	SLIB_INLINE Ref<T> Init(ARGS&&... args) noexcept
	{
		Ref<T> o = new T;
		if (o.isNotNull()) {
			_priv_InitHelper<Ref<T>, decltype(o->init(args...))>::init(&o, Forward<ARGS>(args)...);
			return o;
		}
		return sl_null;
	}
	
	template <class T, class OTHER>
	SLIB_INLINE sl_bool IsInstanceOf(OTHER* object) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, Referable*)
		if (object) {
			return object->isInstanceOf(T::ObjectType());
		}
		return sl_false;
	}
	
	template <class T, class OTHER>
	SLIB_INLINE sl_bool IsInstanceOf(const Ref<OTHER>& object) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, Referable*)
		if (object.isNotNull()) {
			return object->isInstanceOf(T::ObjectType());
		}
		return sl_false;
	}
	
	template <class T, class OTHER>
	SLIB_INLINE T* CastInstance(OTHER* object) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, Referable*)
		if (object) {
			if (object->isInstanceOf(T::ObjectType())) {
				return static_cast<T*>(object);
			}
		}
		return sl_null;
	}
	
	template <class T, class OTHER>
	SLIB_INLINE const Ref<T>& CastRef(const Ref<OTHER>& object) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, Referable*)
		if (object.isNotNull()) {
			if (object->isInstanceOf(T::ObjectType())) {
				return *(reinterpret_cast<Ref<T> const*>(&object));
			}
		}
		return Ref<T>::null();
	}
	
	template <class T, class OTHER>
	SLIB_INLINE const Ref<T>& CastRef(const Ref<OTHER>& object, const Ref<T>& def) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, Referable*)
		if (object.isNotNull()) {
			if (object->isInstanceOf(T::ObjectType())) {
				return *(reinterpret_cast<Ref<T> const*>(&object));
			}
		}
		return def;
	}
	
	template <class T>
	SLIB_INLINE Ref<T>::Ref(T* other) noexcept
	{
		if (other) {
			other->increaseReference();
		}
		_ptr = other;
	}
	
	template <class T>
	SLIB_INLINE Ref<T>::Ref(Ref<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	SLIB_INLINE Ref<T>::Ref(const Ref<T>& other) noexcept
	{
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE Ref<T>::Ref(Ref<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE Ref<T>::Ref(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	Ref<T>::Ref(AtomicRef<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Ref<T>::Ref(const AtomicRef<T>& other) noexcept
	{
		T* o = other._retainObject();
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>::Ref(AtomicRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>::Ref(const AtomicRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._retainObject();
		_ptr = o;
	}
	
	template <class T>
	Ref<T>::Ref(const WeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>::Ref(const WeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	Ref<T>::Ref(const AtomicWeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>::Ref(const AtomicWeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}

	template <class T>
	SLIB_INLINE Ref<T>::~Ref() noexcept
	{
		SLIB_TRY_CONVERT_TYPE(T*, Referable*)
		if (_ptr) {
			_ptr->decreaseReference();
		}
	}
	
	template <class T>
	SLIB_INLINE const Ref<T>& Ref<T>::null() noexcept
	{
		return *(reinterpret_cast<Ref<T> const*>(&_priv_Ref_Null));
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ref<T>::isNull() const noexcept
	{
		return (_ptr == sl_null);
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ref<T>::isNotNull() const noexcept
	{
		return (_ptr != sl_null);
	}
	
	template <class T>
	void Ref<T>::setNull() noexcept
	{
		_replaceObject(sl_null);
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE const Ref<T>& Ref<T>::from(const Ref<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<Ref<T> const*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE Ref<T>& Ref<T>::from(Ref<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<Ref<T>*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE Ref<T>&& Ref<T>::from(Ref<OTHER>&& other) noexcept
	{
		return static_cast<Ref<T>&&>(*(reinterpret_cast<Ref<T>*>(&other)));
	}

	template <class T>
	SLIB_INLINE T* Ref<T>::get() const& noexcept
	{
		return _ptr;
	}

	template <class T>
	const Ref<Referable>& Ref<T>::getReference() const noexcept
	{
		return *(reinterpret_cast<Ref<Referable> const*>(this));
	}

	template <class T>
	Ref<T>& Ref<T>::operator=(sl_null_t) noexcept
	{
		_replaceObject(sl_null);
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(T* other) noexcept
	{
		if (_ptr != other) {
			if (other) {
				other->increaseReference();
			}
			_replaceObject(other);
		}
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(Ref<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(const Ref<T>& other) noexcept
	{
		T* o = other._ptr;
		if (_ptr != o) {
			if (o) {
				o->increaseReference();
			}
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(Ref<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._ptr;
		if (_ptr != o) {
			if (o) {
				o->increaseReference();
			}
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(AtomicRef<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(const AtomicRef<T>& other) noexcept
	{
		T* o = other._retainObject();
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(AtomicRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_assign(&other);
		return *this;
	}

	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(const AtomicRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._retainObject();
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(const WeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(const WeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	Ref<T>& Ref<T>::operator=(const AtomicWeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	Ref<T>& Ref<T>::operator=(const AtomicWeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	sl_bool Ref<T>::operator==(sl_null_t) const noexcept
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	sl_bool Ref<T>::operator==(T* other) const noexcept
	{
		return _ptr == other;
	}
	
	template <class T>
	sl_bool operator==(sl_null_t, const Ref<T>& b) noexcept
	{
		return sl_null == b._ptr;
	}
	
	template <class T>
	sl_bool operator==(T* a, const Ref<T>& b) noexcept
	{
		return a == b._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ref<T>::operator==(const Ref<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool Ref<T>::operator==(const Ref<OTHER>& other) const noexcept
	{
		return (void*)_ptr == (void*)(other._ptr);
	}
	
	template <class T>
	sl_bool Ref<T>::operator==(const AtomicRef<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool Ref<T>::operator==(const AtomicRef<OTHER>& other) const noexcept
	{
		return (void*)_ptr == (void*)(other._ptr);
	}
	
	template <class T>
	sl_bool Ref<T>::operator!=(sl_null_t) const noexcept
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	sl_bool Ref<T>::operator!=(T* other) const noexcept
	{
		return _ptr != other;
	}
	
	template <class T>
	sl_bool operator!=(sl_null_t, const Ref<T>& b) noexcept
	{
		return sl_null != b._ptr;
	}
	
	template <class T>
	sl_bool operator!=(T* a, const Ref<T>& b) noexcept
	{
		return a != b._ptr;
	}
	
	template <class T>
	sl_bool Ref<T>::operator!=(const Ref<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool Ref<T>::operator!=(const Ref<OTHER>& other) const noexcept
	{
		return (void*)_ptr != (void*)(other._ptr);
	}
	
	template <class T>
	sl_bool Ref<T>::operator!=(const AtomicRef<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool Ref<T>::operator!=(const AtomicRef<OTHER>& other) const noexcept
	{
		return (void*)_ptr != (void*)(other._ptr);
	}
	
	template <class T>
	SLIB_INLINE T& Ref<T>::operator*() const& noexcept
	{
		return *(_ptr);
	}
	
	template <class T>
	SLIB_INLINE T* Ref<T>::operator->() const& noexcept
	{
		return _ptr;
	}

	template <class T>
	SLIB_INLINE Ref<T>::operator sl_bool() const noexcept
	{
		return _ptr != sl_null;
	}

	template <class T>
	SLIB_INLINE void Ref<T>::_replaceObject(T* other) noexcept
	{
		if (_ptr) {
			_ptr->decreaseReference();
		}
		_ptr = other;
	}
	
	template <class T>
	SLIB_INLINE void Ref<T>::_move_init(void* _other) noexcept
	{
		Ref<T>& other = *(reinterpret_cast<Ref<T>*>(_other));
		_ptr = other._ptr;
		other._ptr = sl_null;
	}
	
	template <class T>
	SLIB_INLINE void Ref<T>::_move_assign(void* _other) noexcept
	{
		if ((void*)this != _other) {
			Ref<T>& other = *(reinterpret_cast<Ref<T>*>(_other));
			_replaceObject(other._ptr);
			other._ptr = sl_null;
		}
	}
	
	
	template <class T>
	Atomic< Ref<T> >::Atomic(T* other) noexcept
	{
		if (other) {
			other->increaseReference();
		}
		_ptr = other;
	}
	
	template <class T>
	Atomic< Ref<T> >::Atomic(AtomicRef<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ref<T> >::Atomic(const AtomicRef<T>& other) noexcept
	{
		T* o = other._retainObject();
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(AtomicRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_init(&other);
	}

	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(const AtomicRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._retainObject();
		_ptr = o;
	}

	template <class T>
	Atomic< Ref<T> >::Atomic(Ref<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ref<T> >::Atomic(const Ref<T>& other) noexcept
	{
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(Ref<OTHER>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	Atomic< Ref<T> >::Atomic(const WeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(const WeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	Atomic< Ref<T> >::Atomic(const AtomicWeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	template <class OTHER>
	Atomic< Ref<T> >::Atomic(const AtomicWeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_ptr = o;
	}
	
	template <class T>
	Atomic< Ref<T> >::~Atomic() noexcept
	{
		SLIB_TRY_CONVERT_TYPE(T*, Referable*)
		T* ptr = _ptr;
		if (ptr) {
			ptr->decreaseReference();
		}
	}
	
	template <class T>
	SLIB_INLINE const AtomicRef<T>& Atomic< Ref<T> >::null() noexcept
	{
		return *(reinterpret_cast<AtomicRef<T> const*>(&_priv_Ref_Null));
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::isNull() const noexcept
	{
		return (_ptr == sl_null);
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::isNotNull() const noexcept
	{
		return (_ptr != sl_null);
	}
	
	template <class T>
	void Atomic< Ref<T> >::setNull() noexcept
	{
		_replaceObject(sl_null);
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE const AtomicRef<T>& Atomic< Ref<T> >::from(const AtomicRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<AtomicRef<T> const*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE AtomicRef<T>& Atomic< Ref<T> >::from(AtomicRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<AtomicRef<T>*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE AtomicRef<T>&& Atomic< Ref<T> >::from(AtomicRef<OTHER>&& other) noexcept
	{
		return static_cast<AtomicRef<T>&&>(*(reinterpret_cast<AtomicRef<T>*>(&other)));
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(sl_null_t) noexcept
	{
		_replaceObject(sl_null);
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(T* other) noexcept
	{
		if (_ptr != other) {
			if (other) {
				other->increaseReference();
			}
			_replaceObject(other);
		}
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(AtomicRef<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const AtomicRef<T>& other) noexcept
	{
		if (_ptr != other._ptr) {
			T* o = other._retainObject();
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(AtomicRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const AtomicRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		if (_ptr != other._ptr) {
			T* o = other._retainObject();
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(Ref<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const Ref<T>& other) noexcept
	{
		T* o = other._ptr;
		if (_ptr != o) {
			if (o) {
				o->increaseReference();
			}
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(Ref<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_move_assign(&other);
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		T* o = other._ptr;
		if (_ptr != o) {
			if (o) {
				o->increaseReference();
			}
			_replaceObject(o);
		}
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const WeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const WeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const AtomicWeakRef<T>& _other) noexcept
	{
		Ref<T> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	AtomicRef<T>& Atomic< Ref<T> >::operator=(const AtomicWeakRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other.lock());
		T* o = other._ptr;
		if (o) {
			o->increaseReference();
		}
		_replaceObject(o);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(sl_null_t) const noexcept
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(T* other) const noexcept
	{
		return _ptr == other;
	}
	
	template <class T>
	SLIB_INLINE sl_bool operator==(sl_null_t, const AtomicRef<T>& b) noexcept
	{
		return sl_null == b._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool operator==(T* a, const AtomicRef<T>& b) noexcept
	{
		return a == b._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(const AtomicRef<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(const AtomicRef<OTHER>& other) const noexcept
	{
		return (void*)_ptr == (void*)(other._ptr);
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(const Ref<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator==(const Ref<OTHER>& other) const noexcept
	{
		return (void*)_ptr == (void*)(other._ptr);
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(sl_null_t) const noexcept
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(T* other) const noexcept
	{
		return _ptr != other;
	}
	
	template <class T>
	SLIB_INLINE sl_bool operator!=(sl_null_t, const AtomicRef<T>& b) noexcept
	{
		return sl_null != b._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool operator!=(T* a, const AtomicRef<T>& b) noexcept
	{
		return a != b._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(const AtomicRef<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(const AtomicRef<OTHER>& other) const noexcept
	{
		return (void*)_ptr != (void*)(other._ptr);
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(const Ref<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< Ref<T> >::operator!=(const Ref<OTHER>& other) const noexcept
	{
		return (void*)_ptr != (void*)(other._ptr);
	}
	
	template <class T>
	SLIB_INLINE Ref<T> Atomic< Ref<T> >::operator*() const noexcept
	{
		return *this;
	}

	template <class T>
	SLIB_INLINE Atomic< Ref<T> >::operator sl_bool() const noexcept
	{
		return _ptr != sl_null;
	}

	template <class T>
	SLIB_INLINE T* Atomic< Ref<T> >::_retainObject() const noexcept
	{
		if (_ptr) {
			SpinLocker lock(&m_lock);
			T* ptr = _ptr;
			if (ptr) {
				ptr->increaseReference();
			}
			return ptr;
		} else {
			return sl_null;
		}
	}
	
	template <class T>
	SLIB_INLINE void Atomic< Ref<T> >::_replaceObject(T* other) noexcept
	{
		T* before;
		{
			SpinLocker lock(&m_lock);
			before = _ptr;
			_ptr = other;
		}
		if (before) {
			before->decreaseReference();
		}
	}
	
	template <class T>
	SLIB_INLINE void Atomic< Ref<T> >::_move_init(void* _other) noexcept
	{
		AtomicRef<T>& other = *(reinterpret_cast<AtomicRef<T>*>(_other));
		_ptr = other._ptr;
		other._ptr = sl_null;
	}
	
	template <class T>
	SLIB_INLINE void Atomic< Ref<T> >::_move_assign(void* _other) noexcept
	{
		if ((void*)this != _other) {
			AtomicRef<T>& other = *(reinterpret_cast<AtomicRef<T>*>(_other));
			_replaceObject(other._ptr);
			other._ptr = sl_null;
		}
	}
	
	
	template <class T>
	WeakRef<T>::WeakRef(T* _other) noexcept
	{
		_set(_other);
	}
	
	template <class T>
	SLIB_INLINE WeakRef<T>::WeakRef(WeakRef<T>&& other) noexcept
	{
		_weak._move_init(&other);
	}
	
	template <class T>
	SLIB_INLINE WeakRef<T>::WeakRef(const WeakRef<T>& other) noexcept : _weak(other._weak)
	{
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(WeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_init(&other);
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(const WeakRef<OTHER>& other) noexcept : _weak(other._weak)
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
	}
	
	template <class T>
	WeakRef<T>::WeakRef(AtomicWeakRef<T>&& other) noexcept
	{
		_weak._move_init(&other);
	}
	
	
	template <class T>
	WeakRef<T>::WeakRef(const AtomicWeakRef<T>& other) noexcept : _weak(other._weak)
	{
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(AtomicWeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_init(&other);
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(const AtomicWeakRef<OTHER>& other) noexcept : _weak(other._weak)
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
	}
	
	template <class T>
	WeakRef<T>::WeakRef(const Ref<T>& other) noexcept
	{
		_set(other._ptr);
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_set(other._ptr);
	}
	
	template <class T>
	WeakRef<T>::WeakRef(const AtomicRef<T>& _other) noexcept
	{
		Ref<T> other(_other);
		_set(other._ptr);
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>::WeakRef(const AtomicRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other);
		_set(other._ptr);
	}
	
	template <class T>
	WeakRef<T>::~WeakRef() noexcept
	{
		SLIB_TRY_CONVERT_TYPE(T*, Referable*)
	}
	
	template <class T>
	SLIB_INLINE const WeakRef<T>& WeakRef<T>::null() noexcept
	{
		return *(reinterpret_cast<WeakRef<T> const*>(&_priv_Ref_Null));
	}
	
	template <class T>
	sl_bool WeakRef<T>::isNull() const noexcept
	{
		return _weak.isNull();
	}
	
	template <class T>
	sl_bool WeakRef<T>::isNotNull() const noexcept
	{
		return _weak.isNotNull();
	}
	
	template <class T>
	void WeakRef<T>::setNull() noexcept
	{
		_weak.setNull();
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE const WeakRef<T>& WeakRef<T>::from(const WeakRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<WeakRef<T> const*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE WeakRef<T>& WeakRef<T>::from(WeakRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<WeakRef<T>*>(&other));
	}
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE WeakRef<T>&& WeakRef<T>::from(WeakRef<OTHER>&& other) noexcept
	{
		return static_cast<WeakRef<T>&&>(*(reinterpret_cast<WeakRef<T>*>(&other)));
	}
	
	template <class T>
	Ref<T> WeakRef<T>::lock() const noexcept
	{
		if (_weak.isNotNull()) {
			return Ref<T>::from(_weak->lock());
		}
		return sl_null;
	}
	
	template <class T>
	WeakRef<T> WeakRef<T>::fromReferable(Referable* referable) noexcept
	{
		if (referable) {
			WeakRef<T> ret;
			if (referable->_isWeakRef()) {
				ret._weak = static_cast<CWeakRef*>(referable);
			} else {
				ret._weak = referable->_getWeakObject();
			}
			return ret;
		} else {
			return sl_null;
		}
	}
	
	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(sl_null_t) noexcept
	{
		_weak.setNull();
		return *this;
	}
	
	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(T* _other) noexcept
	{
		_set(_other);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(WeakRef<T>&& other) noexcept
	{
		_weak._move_assign(&other);
		return *this;
	}
	
	template <class T>
	SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(const WeakRef<T>& other) noexcept
	{
		_weak = other._weak;
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>& WeakRef<T>::operator=(WeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>& WeakRef<T>::operator=(const WeakRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak = other._weak;
		return *this;
	}

	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(AtomicWeakRef<T>&& other) noexcept
	{
		_weak._move_assign(&other);
		return *this;
	}


	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(const AtomicWeakRef<T>& other) noexcept
	{
		_weak = other._weak;
		return *this;
	}
	
	
	template <class T>
	template <class OTHER>
	SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(AtomicWeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_assign(&other);
		return *this;
	}
	
	
	template <class T>
	template <class OTHER>
	WeakRef<T>& WeakRef<T>::operator=(const AtomicWeakRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak = other._weak;
		return *this;
	}
	
	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(const Ref<T>& other) noexcept
	{
		_set(other._ptr);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>& WeakRef<T>::operator=(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_set(other._ptr);
		return *this;
	}
	
	template <class T>
	WeakRef<T>& WeakRef<T>::operator=(const AtomicRef<T>& _other) noexcept
	{
		Ref<T> other(_other);
		_set(other._ptr);
		return *this;
	}
	
	template <class T>
	template <class OTHER>
	WeakRef<T>& WeakRef<T>::operator=(const AtomicRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other);
		_set(other._ptr);
		return *this;
	}
	
	template <class T>
	sl_bool WeakRef<T>::operator==(const WeakRef<T>& other) const noexcept
	{
		return _weak == other._weak;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool WeakRef<T>::operator==(const WeakRef<OTHER>& other) const noexcept
	{
		return _weak == other._weak;
	}

	
	template <class T>
	sl_bool WeakRef<T>::operator==(const AtomicWeakRef<T>& other) const noexcept
	{
		return _weak == other._weak;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool WeakRef<T>::operator==(const AtomicWeakRef<OTHER>& other) const noexcept
	{
		return _weak == other._weak;
	}
	
	
	template <class T>
	sl_bool WeakRef<T>::operator!=(const WeakRef<T>& other) const noexcept
	{
		return _weak != other._weak;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool WeakRef<T>::operator!=(const WeakRef<OTHER>& other) const noexcept
	{
		return _weak != other._weak;
	}
	
	
	template <class T>
	sl_bool WeakRef<T>::operator!=(const AtomicWeakRef<T>& other) const noexcept
	{
		return _weak != other._weak;
	}
	
	template <class T>
	template <class OTHER>
	sl_bool WeakRef<T>::operator!=(const AtomicWeakRef<OTHER>& other) const noexcept
	{
		return _weak != other._weak;
	}

	template <class T>
	SLIB_INLINE WeakRef<T>::operator sl_bool() const noexcept
	{
		return _weak.isNotNull();
	}

	template <class T>
	SLIB_INLINE void WeakRef<T>::_set(T* object) noexcept
	{
		if (object) {
			_weak = object->_getWeakObject();
		} else {
			_weak.setNull();
		}
	}
	
	
	template <class T>
	Atomic< WeakRef<T> >::Atomic(T* _other) noexcept
	{
		_set(_other);
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(AtomicWeakRef<T>&& other) noexcept
	{
		_weak._move_init(&other);
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(const AtomicWeakRef<T>& other) noexcept : _weak(other._weak)
	{
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(AtomicWeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_init(&other);
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(const AtomicWeakRef<OTHER>& other) noexcept : _weak(other._weak)
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(WeakRef<T>&& other) noexcept
	{
		_weak._move_init(&other);
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(const WeakRef<T>& other) noexcept : _weak(other._weak)
	{
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(WeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_init(&other);
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(const WeakRef<OTHER>& other) noexcept : _weak(other._weak)
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(const Ref<T>& other) noexcept
	{
		_set(other._ptr);
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_set(other._ptr);
	}

	template <class T>
	Atomic< WeakRef<T> >::Atomic(const AtomicRef<T>& _other) noexcept
	{
		Ref<T> other(_other);
		_set(other._ptr);
	}

	template <class T>
	template <class OTHER>
	Atomic< WeakRef<T> >::Atomic(const AtomicRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other);
		_set(other._ptr);
	}
	
	template <class T>
	Atomic< WeakRef<T> >::~Atomic() noexcept
	{
		SLIB_TRY_CONVERT_TYPE(T*, Referable*)
	}

	template <class T>
	SLIB_INLINE const AtomicWeakRef<T>& Atomic< WeakRef<T> >::null() noexcept
	{
		return *(reinterpret_cast<AtomicWeakRef<T> const*>(&_priv_Ref_Null));
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::isNull() const noexcept
	{
		return _weak.isNull();
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::isNotNull() const noexcept
	{
		return _weak.isNotNull();
	}

	template <class T>
	void Atomic< WeakRef<T> >::setNull() noexcept
	{
		_weak.setNull();
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE const AtomicWeakRef<T>& Atomic< WeakRef<T> >::from(const AtomicWeakRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<AtomicWeakRef<T> const*>(&other));
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE AtomicWeakRef<T>& Atomic< WeakRef<T> >::from(AtomicWeakRef<OTHER>& other) noexcept
	{
		return *(reinterpret_cast<AtomicWeakRef<T>*>(&other));
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE AtomicWeakRef<T>&& Atomic< WeakRef<T> >::from(AtomicWeakRef<OTHER>&& other) noexcept
	{
		return static_cast<AtomicWeakRef<T>&&>(*(reinterpret_cast<AtomicWeakRef<T>*>(&other)));
	}

	template <class T>
	Ref<T> Atomic< WeakRef<T> >::lock() const noexcept
	{
		Ref<CWeakRef> weak(_weak);
		if (weak.isNotNull()) {
			return Ref<T>::from(weak->lock());
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE WeakRef<T> Atomic< WeakRef<T> >::operator*() const noexcept
	{
		return *this;
	}

	template <class T>
	SLIB_INLINE Atomic< WeakRef<T> >::operator sl_bool() const noexcept
	{
		return _weak.isNotNull();
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(sl_null_t) noexcept
	{
		_weak.setNull();
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(T* _other) noexcept
	{
		_set(_other);
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(AtomicWeakRef<T>&& other) noexcept
	{
		_weak._move_assign(&other);
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const AtomicWeakRef<T>& other) noexcept
	{
		_weak = other._weak;
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(AtomicWeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_assign(&other);
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const AtomicWeakRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak = other._weak;
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(WeakRef<T>&& other) noexcept
	{
		_weak._move_assign(&other);
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const WeakRef<T>& other) noexcept
	{
		_weak = other._weak;
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(WeakRef<OTHER>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak._move_assign(&other);
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const WeakRef<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_weak = other._weak;
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const Ref<T>& other) noexcept
	{
		_set(other._ptr);
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const Ref<OTHER>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		_set(other._ptr);
		return *this;
	}

	template <class T>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const AtomicRef<T>& _other) noexcept
	{
		Ref<T> other(_other);
		_set(other._ptr);
		return *this;
	}

	template <class T>
	template <class OTHER>
	AtomicWeakRef<T>& Atomic< WeakRef<T> >::operator=(const AtomicRef<OTHER>& _other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(OTHER*, T*)
		Ref<OTHER> other(_other);
		_set(other._ptr);
		return *this;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator==(const AtomicWeakRef<T>& other) const noexcept
	{
		return _weak == other._weak;
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator==(const AtomicWeakRef<OTHER>& other) const noexcept
	{
		return _weak == other._weak;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator==(const WeakRef<T>& other) const noexcept
	{
		return _weak == other._weak;
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator==(const WeakRef<OTHER>& other) const noexcept
	{
		return _weak == other._weak;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator!=(const AtomicWeakRef<T>& other) const noexcept
	{
		return _weak != other._weak;
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator!=(const AtomicWeakRef<OTHER>& other) const noexcept
	{
		return _weak != other._weak;
	}

	template <class T>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator!=(const WeakRef<T>& other) const noexcept
	{
		return _weak != other._weak;
	}

	template <class T>
	template <class OTHER>
	SLIB_INLINE sl_bool Atomic< WeakRef<T> >::operator!=(const WeakRef<OTHER>& other) const noexcept
	{
		return _weak != other._weak;
	}

	template <class T>
	SLIB_INLINE void Atomic< WeakRef<T> >::_set(T* object) noexcept
	{
		if (object) {
			_weak = object->_getWeakObject();
		} else {
			_weak.setNull();
		}
	}

	template <class T>
	SLIB_INLINE sl_bool operator>(const Ref<T>& a, const Ref<T>& b) noexcept
	{
		return a._ptr > b._ptr;
	}

	template <class T>
	SLIB_INLINE sl_bool operator<(const Ref<T>& a, const Ref<T>& b) noexcept
	{
		return a._ptr < b._ptr;
	}


	template <class T>
	SLIB_INLINE Ref<T> ToRef(T* other) noexcept
	{
		return Ref<T>(other);
	}

	template <class T>
	SLIB_INLINE Ref<T> ToRef(const Ref<T>& other) noexcept
	{
		return Ref<T>(other);
	}

	template <class T>
	SLIB_INLINE Ref<T> ToRef(const AtomicRef<T>& other) noexcept
	{
		return Ref<T>(other);
	}

	template <class T>
	SLIB_INLINE Ref<T> ToRef(const WeakRef<T>& other) noexcept
	{
		return Ref<T>(other);
	}

	template <class T>
	SLIB_INLINE Ref<T> ToRef(const AtomicWeakRef<T>& other) noexcept
	{
		return Ref<T>(other);
	}

	template <class T>
	SLIB_INLINE WeakRef<T> ToWeakRef(T* other) noexcept
	{
		return WeakRef<T>(other);
	}

	template <class T>
	SLIB_INLINE WeakRef<T> ToWeakRef(const Ref<T>& other) noexcept
	{
		return WeakRef<T>(other);
	}

	template <class T>
	SLIB_INLINE WeakRef<T> ToWeakRef(const AtomicRef<T>& other) noexcept
	{
		return WeakRef<T>(other);
	}

	template <class T>
	SLIB_INLINE WeakRef<T> ToWeakRef(const WeakRef<T>& other) noexcept
	{
		return WeakRef<T>(other);
	}

	template <class T>
	SLIB_INLINE WeakRef<T> ToWeakRef(const AtomicWeakRef<T>& other) noexcept
	{
		return WeakRef<T>(other);
	}

}
