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
	class _CallableFromFunction : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		FUNC func;

	public:
		template <class _FUNC>
		SLIB_INLINE _CallableFromFunction(_FUNC&& _func) :
		 func(Forward<_FUNC>(_func))
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return func(params...);
		}
	};
	
	template <class BIND_TUPLE, class FUNC, class RET_TYPE, class... ARGS>
	class _BindFromFunction : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class _FUNC, class _BIND_TUPLE>
		SLIB_INLINE _BindFromFunction(_FUNC&& _func, _BIND_TUPLE&& _binds) :
		 func(Forward<_FUNC>(_func)), binds(Forward<_BIND_TUPLE>(_binds))
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invoke(func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _CallableFromClass : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		CLASS* object;
		FUNC func;

	public:
		SLIB_INLINE _CallableFromClass(CLASS* _object, FUNC _func):
		 object(_object), func(_func)
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return (object->*func)(params...);
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _BindFromClass : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		CLASS* object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class _BIND_TUPLE>
		SLIB_INLINE _BindFromClass(CLASS* _object, FUNC _func, _BIND_TUPLE&& _binds):
		 object(_object), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		{}
	
	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invokeMember(object, func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _CallableFromRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		Ref<CLASS> object;
		FUNC func;

	public:
		template <class T>
		SLIB_INLINE _CallableFromRef(T&& _object, FUNC _func):
		 object(Forward<T>(_object)), func(_func)
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return ((object._ptr)->*func)(params...);
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _BindFromRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		Ref<CLASS> object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class T, class _BIND_TUPLE>
		SLIB_INLINE _BindFromRef(T&& _object, FUNC _func, _BIND_TUPLE&& _binds):
		 object(Forward<T>(_object)), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			return binds.invokeMember(object._ptr, func, params...);
		}
	};
	
	template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _CallableFromWeakRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		WeakRef<CLASS> object;
		FUNC func;

	public:
		template <class T>
		SLIB_INLINE _CallableFromWeakRef(T&& _object, FUNC _func):
		 object(Forward<T>(_object)), func(_func)
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			Ref<CLASS> o(object);
			if (o.isNotNull()) {
				return ((o._ptr)->*func)(params...);
			} else {
				return RET_TYPE();
			}
		}
	};
	
	template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
	class _BindFromWeakRef : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		WeakRef<CLASS> object;
		FUNC func;
		BIND_TUPLE binds;

	public:
		template <class T, class _BIND_TUPLE>
		SLIB_INLINE _BindFromWeakRef(T&& _object, FUNC _func, _BIND_TUPLE&& _binds):
		 object(Forward<T>(_object)), func(_func), binds(Forward<_BIND_TUPLE>(_binds))
		{}

	public:
		RET_TYPE invoke(ARGS... params) override
		{
			Ref<CLASS> o(object);
			if (o.isNotNull()) {
				return binds.invokeMember(o._ptr, func, params...);
			} else {
				return RET_TYPE();
			}
		}
	};
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>::Function(const FUNC& func):
	 ref(new _CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func))
	{
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator=(const FUNC& func)
	{
		ref = new _CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE RET_TYPE Function<RET_TYPE(ARGS...)>::operator()(ARGS... args) const
	{
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			return object->invoke(args...);
		} else {
			return RET_TYPE();
		}
	}
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >::Atomic(const FUNC& func):
	 ref(new _CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func))
	{
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::operator=(const FUNC& func)
	{
		ref = new _CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE RET_TYPE Atomic< Function<RET_TYPE(ARGS...)> >::operator()(ARGS... args) const
	{
		Ref< Callable<RET_TYPE(ARGS...)> > object(ref);
		if (object.isNotNull()) {
			return object->invoke(args...);
		} else {
			return RET_TYPE();
		}
	}
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::create(const FUNC& func)
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromFunction<FUNC, RET_TYPE, ARGS...>(func));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromClass(CLASS* object, FUNC func)
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromClass<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromRef(const Ref<CLASS>& object, FUNC func)
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromWeakRef(const WeakRef<CLASS>& object, FUNC func)
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromWeakRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bind(const FUNC& func, const BINDS&... binds)
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _BindFromFunction<Tuple<BINDS...>, FUNC, RET_TYPE, ARGS...>(func, Tuple<BINDS...>(binds...)));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindClass(CLASS* object, FUNC func, const BINDS&... binds)
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _BindFromClass<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(const Ref<CLASS>& object, FUNC func, const BINDS&... binds)
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _BindFromRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(const WeakRef<CLASS>& object, FUNC func, const BINDS&... binds)
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _BindFromWeakRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	
	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromClass(CLASS* object, RET_TYPE (CLASS::*func)(ARGS...))
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromClass<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromRef(const Ref<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...))
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromWeakRef(const WeakRef<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...))
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new _CallableFromWeakRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

}
