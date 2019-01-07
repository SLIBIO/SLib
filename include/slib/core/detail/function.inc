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
	
	template <class FUNC, class RET_TYPE, class... ARGS>
	class _priv_CallableFromFunction : public Callable<RET_TYPE(ARGS...)>
	{
	protected:
		FUNC func;

	public:
		template <class OTHER_FUNC>
		SLIB_INLINE _priv_CallableFromFunction(OTHER_FUNC&& _func) noexcept
		 : func(Forward<OTHER_FUNC>(_func))
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
		template <class OTHER_FUNC, class OTHER_BIND_TUPLE>
		SLIB_INLINE _priv_BindFromFunction(OTHER_FUNC&& _func, OTHER_BIND_TUPLE&& _binds) noexcept
		 : func(Forward<OTHER_FUNC>(_func)), binds(Forward<OTHER_BIND_TUPLE>(_binds))
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
		template <class OTHER_BIND_TUPLE>
		SLIB_INLINE _priv_BindFromClass(CLASS* _object, FUNC _func, OTHER_BIND_TUPLE&& _binds) noexcept
		 : object(_object), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
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
		template <class T, class OTHER_BIND_TUPLE>
		SLIB_INLINE _priv_BindFromRef(T&& _object, FUNC _func, OTHER_BIND_TUPLE&& _binds) noexcept
		 : object(Forward<T>(_object)), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
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
		template <class T, class OTHER_BIND_TUPLE>
		SLIB_INLINE _priv_BindFromWeakRef(T&& _object, FUNC _func, OTHER_BIND_TUPLE&& _binds) noexcept
		 : object(Forward<T>(_object)), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
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

	
	extern const char _priv_FunctionList_ClassID[];

	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_object_type FunctionList<RET_TYPE(ARGS...)>::ObjectType() noexcept
	{
		return _priv_FunctionList_ClassID;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool FunctionList<RET_TYPE(ARGS...)>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == _priv_FunctionList_ClassID) {
			return sl_true;
		}
		return CallableBase::isDerivedFrom(type);
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_object_type FunctionList<RET_TYPE(ARGS...)>::getObjectType() const noexcept
	{
		return _priv_FunctionList_ClassID;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool FunctionList<RET_TYPE(ARGS...)>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == _priv_FunctionList_ClassID) {
			return sl_true;
		}
		return CallableBase::isDerivedFrom(type);
	}
	
	template <class RET_TYPE, class... ARGS>
	RET_TYPE FunctionList<RET_TYPE(ARGS...)>::invoke(ARGS... params)
	{
		ListElements< Function<RET_TYPE(ARGS...)> > functions(list);
		for (sl_size i = 0; i < functions.count; i++) {
			if (i + 1 == functions.count) {
				return functions[i](params...);
			} else {
				functions[i](params...);
			}
		}
		return NullValue<RET_TYPE>::get();
	}
	
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromList(const List< Function<RET_TYPE(ARGS...)> >& list) noexcept
	{
		return new FunctionList<RET_TYPE(ARGS...)>(list);
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool Function<RET_TYPE(ARGS...)>::isList() const noexcept
	{
		return IsInstanceOf< FunctionList<RET_TYPE(ARGS...)> >(ref.get());
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE const List< Function<RET_TYPE(ARGS...)> >& Function<RET_TYPE(ARGS...)>::getList() const noexcept
	{
		FunctionList<RET_TYPE(ARGS...)>* obj = CastInstance< FunctionList<RET_TYPE(ARGS...)> >(ref.get());
		if (obj) {
			return obj->list;
		} else {
			return List< Function<RET_TYPE(ARGS...)> >::null();
		}
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::add(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNull()) {
			return function;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				((FunctionList<RET_TYPE(ARGS...)>*)object)->list.add_NoLock(function);
			} else {
				ref = new FunctionList<RET_TYPE(ARGS...)>(List< Function<RET_TYPE(ARGS...)> >::createFromElements(*this, function));
			}
		} else {
			ref = function.ref;
		}
		return function;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::addIfNotExist(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNull()) {
			return function;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				((FunctionList<RET_TYPE(ARGS...)>*)object)->list.addIfNotExist_NoLock(function);
			} else {
				if (ref != function.ref) {
					ref = new FunctionList<RET_TYPE(ARGS...)>(List< Function<RET_TYPE(ARGS...)> >::createFromElements(*this, function));
				}
			}
		} else {
			ref = function.ref;
		}
		return function;
	}
	
	template <class RET_TYPE, class... ARGS>
	void Function<RET_TYPE(ARGS...)>::remove(const Function<RET_TYPE(ARGS...)>& function, sl_bool flagRemoveAllMatches) noexcept
	{
		if (function.isNull()) {
			return;
		}
		if (ref == function.ref) {
			ref.setNull();
			return;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				List< Function<RET_TYPE(ARGS...)> >& list = ((FunctionList<RET_TYPE(ARGS...)>*)object)->list;
				sl_size count = list.getCount();
				Function<RET_TYPE(ARGS...)>* data = list.getData();
				if (count == 0) {
					ref.setNull();
				} else if (count == 1) {
					if (*data == function) {
						ref.setNull();
					}
				} else if (count == 2) {
					if (*data == function) {
						if (flagRemoveAllMatches) {
							if (data[1] == function) {
								ref.setNull();
							} else {
								ref = data[1].ref;
							}
						} else {
							ref = data[1].ref;
						}
					} else {
						if (data[1] == function) {
							ref = data->ref;
						}
					}
				} else {
					if (flagRemoveAllMatches) {
						list.removeValues_NoLock(function);
					} else {
						list.remove_NoLock(function);
					}
				}
			}
		}
	}
	
	template <class RET_TYPE, class... ARGS>
	sl_bool Function<RET_TYPE(ARGS...)>::contains(const Function<RET_TYPE(ARGS...)>& function) const noexcept
	{
		if (function.isNull()) {
			return sl_false;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				return ((FunctionList<RET_TYPE(ARGS...)>*)object)->list.contains_NoLock(function);
			}
		}
		return sl_false;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator+=(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNotNull()) {
			*this = *this + function;
		}
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::operator+(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNull()) {
			return *this;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				List< Function<RET_TYPE(ARGS...)> > list(((FunctionList<RET_TYPE(ARGS...)>*)object)->list.duplicate_NoLock());
				list.add_NoLock(function);
				return (Callable<RET_TYPE(ARGS...)>*)(new FunctionList<RET_TYPE(ARGS...)>(list));
			} else {
				return (Callable<RET_TYPE(ARGS...)>*)(new FunctionList<RET_TYPE(ARGS...)>(List< Function<RET_TYPE(ARGS...)> >::createFromElements(*this, function)));
			}
		} else {
			return function;
		}
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator-=(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNotNull()) {
			*this = *this - function;
		}
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::operator-(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		if (function.isNull()) {
			return *this;
		}
		if (ref == function.ref) {
			return sl_null;
		}
		Callable<RET_TYPE(ARGS...)>* object = ref._ptr;
		if (object) {
			if (object->isInstanceOf(FunctionList<RET_TYPE(ARGS...)>::ObjectType())) {
				List< Function<RET_TYPE(ARGS...)> >& list = ((FunctionList<RET_TYPE(ARGS...)>*)object)->list;
				sl_size count = list.getCount();
				if (count >= 0) {
					Function<RET_TYPE(ARGS...)>* data = list.getData();
					if (count == 1) {
						if (*data == function) {
							return sl_null;
						} else {
							return *data;
						}
					} else if (count == 2) {
						if (*data == function) {
							return data[1];
						} else if (data[1] == function) {
							return *data;
						}
					} else {
						sl_reg index = list.indexOf_NoLock(function);
						if (index >= 0) {
							List< Function<RET_TYPE(ARGS...)> > newList(data, index);
							if (index + 1 < count) {
								newList.addElements_NoLock(data + index + 1, count - index - 1);
							}
							return (Callable<RET_TYPE(ARGS...)>*)(new FunctionList<RET_TYPE(ARGS...)>(newList));
						}
					}
				} else {
					return sl_null;
				}
			}
		}
		return *this;
	}

	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool Atomic< Function<RET_TYPE(ARGS...)> >::isList() const noexcept
	{
		Function<RET_TYPE(ARGS...)> func(*this);
		return func.isList();
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE const List< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::getList() const noexcept
	{
		Function<RET_TYPE(ARGS...)> func(*this);
		return func.getList();
	}

	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Atomic< Function<RET_TYPE(ARGS...)> >::add(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		SpinLocker lock(SpinLockPoolForFunction::get(this));
		Function<RET_TYPE(ARGS...)> _this(*this);
		*this = _this + function;
		return function;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Atomic< Function<RET_TYPE(ARGS...)> >::addIfNotExist(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		SpinLocker lock(SpinLockPoolForFunction::get(this));
		Function<RET_TYPE(ARGS...)> _this(*this);
		if (!(_this.contains(function))) {
			*this = _this + function;
		}
		return function;
	}
	
	template <class RET_TYPE, class... ARGS>
	void Atomic< Function<RET_TYPE(ARGS...)> >::remove(const Function<RET_TYPE(ARGS...)>& function, sl_bool flagRemoveAllMatch) noexcept
	{
		SpinLocker lock(SpinLockPoolForFunction::get(this));
		Function<RET_TYPE(ARGS...)> _this(*this);
		if (flagRemoveAllMatch) {
			while (_this.contains(function)) {
				_this -= function;
			}
			*this = _this;
		} else {
			*this = _this - function;
		}
	}
	
	template <class RET_TYPE, class... ARGS>
	sl_bool Atomic< Function<RET_TYPE(ARGS...)> >::contains(const Function<RET_TYPE(ARGS...)>& function) const noexcept
	{
		Function<RET_TYPE(ARGS...)> func(*this);
		return func.contains(function);
	}
	
	template <class RET_TYPE, class... ARGS>
	Atomic< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::operator+=(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		add(function);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Atomic< Function<RET_TYPE(ARGS...)> >::operator+(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		Function<RET_TYPE(ARGS...)> _this(*this);
		return _this + function;
	}
	
	template <class RET_TYPE, class... ARGS>
	Atomic< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::operator-=(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		remove(function);
		return *this;
	}
	
	template <class RET_TYPE, class... ARGS>
	Function<RET_TYPE(ARGS...)> Atomic< Function<RET_TYPE(ARGS...)> >::operator-(const Function<RET_TYPE(ARGS...)>& function) noexcept
	{
		Function<RET_TYPE(ARGS...)> _this(*this);
		return _this - function;
	}
	
}
