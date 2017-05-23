/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	template <class FUNC, class RET_TYPE, class... ARGS>
	class _priv_CallableFromFunction : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		FUNC func;

	public:
		template <class _FUNC>
		SLIB_INLINE _priv_CallableFromFunction(_FUNC&& _func) noexcept
		 : func(Forward<_FUNC>(_func))
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return func(params...);
		}
	};
	
	template <class BIND_TUPLE, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_BindFromFunction : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class _FUNC, class _BIND_TUPLE>
		SLIB_INLINE _priv_BindFromFunction(_FUNC&& _func, _BIND_TUPLE&& _binds) noexcept
		 : func(Forward<_FUNC>(_func)), binds(Forward<_BIND_TUPLE>(_binds))
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invoke(func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_CallableFromClass : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		CLASS* object;
		FUNC func;

	public:
		SLIB_INLINE _priv_CallableFromClass(CLASS* _object, FUNC _func) noexcept
		 : object(_object), func(_func)
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return (object->*func)(params...);
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_BindFromClass : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		CLASS* object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class _BIND_TUPLE>
		SLIB_INLINE _priv_BindFromClass(CLASS* _object, FUNC _func, _BIND_TUPLE&& _binds) noexcept
		 : object(_object), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		 {}
	
	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invokeMember(object, func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_CallableFromRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		Ref<CLASS> object;
		FUNC func;

	public:
		template <class T>
		SLIB_INLINE _priv_CallableFromRef(T&& _object, FUNC _func) noexcept
		 : object(Forward<T>(_object)), func(_func)
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return ((object._ptr)->*func)(params...);
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_BindFromRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		Ref<CLASS> object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class T, class _BIND_TUPLE>
		SLIB_INLINE _priv_BindFromRef(T&& _object, FUNC _func, _BIND_TUPLE&& _binds) noexcept
		 : object(Forward<T>(_object)), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invokeMember(object._ptr, func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_CallableFromWeakRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		WeakRef<CLASS> object;
		FUNC func;

	public:
		template <class T>
		SLIB_INLINE _priv_CallableFromWeakRef(T&& _object, FUNC _func) noexcept
		 : object(Forward<T>(_object)), func(_func)
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			Ref<CLASS> o(object);
			if (o.isNotNull()) {
				return ((o._ptr)->*func)(params...);
			} else {
				return NullValue<RET_TYPE>::get();
			}
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _priv_BindFromWeakRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		WeakRef<CLASS> object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class T, class _BIND_TUPLE>
		SLIB_INLINE _priv_BindFromWeakRef(T&& _object, FUNC _func, _BIND_TUPLE&& _binds) noexcept
		 : object(Forward<T>(_object)), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		 {}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			Ref<CLASS> o(object);
			if (o.isNotNull()) {
				return binds.invokeMember(o._ptr, func, params...);
			} else {
				return NullValue<RET_TYPE>::get();
			}
		}
	};
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>::Function(const FUNC& func) noexcept
	 : ref(new _priv_CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func))
	 {}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator=(const FUNC& func) noexcept
	{
		ref = new _priv_CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE RET_TYPE Function<RET_TYPE(ARGS...)>::operator()(ARGS... args) const
	{
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			return object->invoke(args...);
		} else {
			return NullValue<RET_TYPE>::get();
		}
	}
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >::Atomic(const FUNC& func) noexcept
	 : ref(new _priv_CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func))
	 {}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::operator=(const FUNC& func) noexcept
	{
		ref = new _priv_CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE RET_TYPE Atomic< Function<RET_TYPE(ARGS...)> >::operator()(ARGS... args) const
	{
		Ref< Callable<RET_TYPE(ARGS...)> > object(ref);
		if (object.isNotNull()) {
			return object->invoke(args...);
		} else {
			return NullValue<RET_TYPE>::get();
		}
	}
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::create(const FUNC& func) noexcept
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromClass(CLASS* object, FUNC func) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromClass<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromRef(const Ref<CLASS>& object, FUNC func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromWeakRef(const WeakRef<CLASS>& object, FUNC func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromWeakRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bind(const FUNC& func, const BINDS&... binds) noexcept
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_BindFromFunction<Tuple<BINDS...>, FUNC, RET_TYPE, ARGS...>(func, Tuple<BINDS...>(binds...)));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindClass(CLASS* object, FUNC func, const BINDS&... binds) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_BindFromClass<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(const Ref<CLASS>& object, FUNC func, const BINDS&... binds) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_BindFromRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(const WeakRef<CLASS>& object, FUNC func, const BINDS&... binds) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_BindFromWeakRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	
	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromClass(CLASS* object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromClass<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromRef(const Ref<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromWeakRef(const WeakRef<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _priv_CallableFromWeakRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

}
