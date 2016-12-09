#ifndef CHECKHEADER_SLIB_CORE_FUNCTION
#define CHECKHEADER_SLIB_CORE_FUNCTION

#include "definition.h"

#include "object.h"
#include "tuple.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Runnable : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	virtual void run() = 0;
	
};

class SafeCallback;

/** auto-referencing object **/
class SLIB_EXPORT Callback
{
public:
	Ref<Runnable> ref;
	SLIB_DECLARE_REF_WRAPPER(Callback, SafeCallback, Runnable)

public:
	Callback(Runnable* runnable);
	
	template <class FUNC>
	Callback(const FUNC& func);
	
public:
	Callback& operator=(Runnable* runnable);
	
	template <class FUNC>
	Callback& operator=(const FUNC& func);
	
	void operator()() const;
	
public:
	template <class FUNC, class... BINDS>
	static Callback create(const FUNC& func, const BINDS&... binds);
	
	template <class CLASS, class FUNC, class... BINDS>
	static Callback createFromObject(CLASS* object, FUNC func, const BINDS&... binds);
	
	template <class CLASS, class FUNC, class... BINDS>
	static Callback createFromRef(const Ref<CLASS>& object, FUNC func, const BINDS&... binds);
	
	template <class CLASS, class FUNC, class... BINDS>
	static Callback createFromWeakRef(const WeakRef<CLASS>& object, FUNC func, const BINDS&... binds);
	
};

/** auto-referencing object **/
class SLIB_EXPORT SafeCallback
{
public:
	SafeRef<Runnable> ref;
	SLIB_DECLARE_REF_WRAPPER(SafeCallback, Callback, Runnable)
	
public:
	SafeCallback(Runnable* runnable);

	template <class FUNC>
	SafeCallback(const FUNC& func);
	
public:
	SafeCallback& operator=(Runnable* runnable);

	template <class FUNC>
	SafeCallback& operator=(const FUNC& func);
	
	void operator()() const;
	
};


class SLIB_EXPORT Dispatcher : public Object
{
	SLIB_DECLARE_OBJECT

public:
	virtual sl_bool dispatch(const Callback& callback) = 0;

};


template <class RET_TYPE, class... ARGS>
class Functional;

template <class RET_TYPE, class... ARGS>
class Function;

template <class RET_TYPE, class... ARGS>
class SafeFunction;

template <class RET_TYPE, class... ARGS>
class SLIB_EXPORT Functional<RET_TYPE(ARGS...)> : public Referable
{
public:
	virtual RET_TYPE invoke(ARGS... params) = 0;
	
};

#define SLIB_TEMPLATE_PARAMS_Functional RET_TYPE(ARGS...)
#define SLIB_TEMPLATE_DEF_PARAMS_Functional class RET_TYPE, class... ARGS

/** auto-referencing object **/
template <class RET_TYPE, class... ARGS>
class SLIB_EXPORT Function<RET_TYPE(ARGS...)>
{
public:
	Ref< Functional<RET_TYPE(ARGS...)> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(Function, SafeFunction, Functional)
	
public:
	template <class FUNC>
	Function(const FUNC& func);
	
public:
	template <class FUNC>
	Function& operator=(const FUNC& func);
	
	RET_TYPE operator()(ARGS... args) const;

public:
	template <class FUNC>
	static Function<RET_TYPE(ARGS...)> create(const FUNC& func);
	
	template <class CLASS, class FUNC>
	static Function<RET_TYPE(ARGS...)> createFromObject(CLASS* object, FUNC func);
	
	template <class CLASS, class FUNC>
	static Function<RET_TYPE(ARGS...)> createFromRef(const Ref<CLASS>& object, FUNC func);
	
	template <class CLASS, class FUNC>
	static Function<RET_TYPE(ARGS...)> createFromWeakRef(const WeakRef<CLASS>& object, FUNC func);
	
};

/** auto-referencing object **/
template <class RET_TYPE, class... ARGS>
class SLIB_EXPORT SafeFunction<RET_TYPE(ARGS...)>
{
public:
	SafeRef< Functional<RET_TYPE(ARGS...)> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(SafeFunction, Function, Functional)
	
public:
	template <class FUNC>
	SafeFunction(const FUNC& func);
	
public:
	template <class FUNC>
	SafeFunction& operator=(const FUNC& func);
	
	RET_TYPE operator()(ARGS... args) const;
	
};


#define SLIB_CALLBACK(CALLBACK, ...) slib::Callback::create(CALLBACK, ##__VA_ARGS__)
#define SLIB_CALLBACK_CLASS(CLASS, CALLBACK, OBJECT, ...) slib::Callback::createFromObject(OBJECT, &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_CALLBACK_REF(CLASS, CALLBACK, OBJECT, ...) slib::Callback::createFromRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_CALLBACK_WEAKREF(CLASS, CALLBACK, OBJECT, ...) slib::Callback::createFromWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)

#define SLIB_FUNCTION(TYPE, CALLBACK) slib::Function<TYPE>::create(CALLBACK)
#define SLIB_FUNCTION_CLASS(CLASS, CALLBACK, OBJECT) slib::_createFunctionFromObject(OBJECT, &CLASS::CALLBACK)
#define SLIB_FUNCTION_REF(CLASS, CALLBACK, OBJECT) slib::_createFunctionFromRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK)
#define SLIB_FUNCTION_WEAKREF(CLASS, CALLBACK, OBJECT) slib::_createFunctionFromWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK)


template <class FUNC, class... BINDS>
class _FunctionCallbackRunnable : public Runnable
{
public:
	FUNC func;
	Tuple<BINDS...> binds;
public:
	SLIB_INLINE _FunctionCallbackRunnable(const FUNC& _func, const BINDS&... _binds) : func(_func), binds(_binds...) {}
public:
	// override
	void run()
	{
		binds.invoke(func);
	}
};

template <class CLASS, class FUNC, class... BINDS>
class _ObjectCallbackRunnable : public Runnable
{
public:
	CLASS* object;
	FUNC func;
	Tuple<BINDS...> binds;
public:
	SLIB_INLINE _ObjectCallbackRunnable(CLASS* _object, FUNC _func, const BINDS&... _binds) : object(_object), func(_func), binds(_binds...) {}
public:
	// override
	void run()
	{
		binds.invokeMember(object, func);
	}
};

template <class CLASS, class FUNC, class... BINDS>
class _RefCallbackRunnable : public Runnable
{
public:
	Ref<CLASS> object;
	FUNC func;
	Tuple<BINDS...> binds;
public:
	SLIB_INLINE _RefCallbackRunnable(const Ref<CLASS>& _object, FUNC _func, const BINDS&... _binds) : object(_object), func(_func), binds(_binds...) {}
public:
	// override
	void run()
	{
		binds.invokeMember(object.ptr, func);
	}
};

template <class CLASS, class FUNC, class... BINDS>
class _WeakRefCallbackRunnable : public Runnable
{
public:
	WeakRef<CLASS> object;
	FUNC func;
	Tuple<BINDS...> binds;
public:
	SLIB_INLINE _WeakRefCallbackRunnable(const WeakRef<CLASS>& _object, FUNC _func, const BINDS&... _binds) : object(_object), func(_func), binds(_binds...) {}
public:
	// override
	void run()
	{
		Ref<CLASS> o(object);
		if (o.isNotNull()) {
			binds.invokeMember(o.ptr, func);
		}
	}
};


template <class FUNC>
SLIB_INLINE Callback::Callback(const FUNC& func) : ref(new _FunctionCallbackRunnable<FUNC>(func))
{
}

template <class FUNC>
SLIB_INLINE Callback& Callback::operator=(const FUNC& func)
{
	ref = new _FunctionCallbackRunnable<FUNC>(func);
	return *this;
}


template <class FUNC>
SLIB_INLINE SafeCallback::SafeCallback(const FUNC& func) : ref(new _FunctionCallbackRunnable<FUNC>(func))
{
}

template <class FUNC>
SLIB_INLINE SafeCallback& SafeCallback::operator=(const FUNC& func)
{
	ref = new _FunctionCallbackRunnable<FUNC>(func);
	return *this;
}


template <class FUNC, class... BINDS>
SLIB_INLINE Callback Callback::create(const FUNC& func, const BINDS&... binds)
{
	return (Runnable*)(new _FunctionCallbackRunnable<FUNC, BINDS...>(func, binds...));
}

template <class CLASS, class FUNC, class... BINDS>
SLIB_INLINE Callback Callback::createFromObject(CLASS* object, FUNC func, const BINDS&... binds)
{
	if (object) {
		return (Runnable*)(new _ObjectCallbackRunnable<CLASS, FUNC, BINDS...>(object, func, binds...));
	}
	return Callback::null();
}

template <class CLASS, class FUNC, class... BINDS>
SLIB_INLINE Callback Callback::createFromRef(const Ref<CLASS>& object, FUNC func, const BINDS&... binds)
{
	if (object.isNotNull()) {
		return (Runnable*)(new _RefCallbackRunnable<CLASS, FUNC, BINDS...>(object, func, binds...));
	}
	return Callback::null();
}

template <class CLASS, class FUNC, class... BINDS>
SLIB_INLINE Callback Callback::createFromWeakRef(const WeakRef<CLASS>& object, FUNC func, const BINDS&... binds)
{
	if (object.isNotNull()) {
		return (Runnable*)(new _WeakRefCallbackRunnable<CLASS, FUNC, BINDS...>(object, func, binds...));
	}
	return Callback::null();
}


template <class FUNC, class RET_TYPE, class... ARGS>
class _FunctionFunctional : public Functional<RET_TYPE(ARGS...)>
{
public:
	FUNC func;
public:
	SLIB_INLINE _FunctionFunctional(const FUNC& _func) : func(_func) {}
public:
	// override
	RET_TYPE invoke(ARGS... params)
	{
		return func(params...);
	}
};

template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
class _ObjectFunctional : public Functional<RET_TYPE(ARGS...)>
{
public:
	CLASS* object;
	FUNC func;
public:
	SLIB_INLINE _ObjectFunctional(CLASS* _object, FUNC _func) : object(_object), func(_func) {}
public:
	// override
	RET_TYPE invoke(ARGS... params)
	{
		return (object->*func)(params...);
	}
};

template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
class _RefFunctional : public Functional<RET_TYPE(ARGS...)>
{
public:
	Ref<CLASS> object;
	FUNC func;
public:
	SLIB_INLINE _RefFunctional(const Ref<CLASS>& _object, FUNC _func) : object(_object), func(_func) {}
public:
	// override
	RET_TYPE invoke(ARGS... params)
	{
		return ((object.ptr)->*func)(params...);
	}
};

template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
class _WeakRefFunctional : public Functional<RET_TYPE(ARGS...)>
{
public:
	WeakRef<CLASS> object;
	FUNC func;
public:
	SLIB_INLINE _WeakRefFunctional(const WeakRef<CLASS>& _object, FUNC _func) : object(_object), func(_func) {}
public:
	// override
	RET_TYPE invoke(ARGS... params)
	{
		Ref<CLASS> o(object);
		if (o.isNotNull()) {
			return ((o.ptr)->*func)(params...);
		}
		return RET_TYPE();
	}
};


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(Function, SafeFunction, Functional, ref)

template <class RET_TYPE, class... ARGS>
template <class FUNC>
SLIB_INLINE Function<RET_TYPE(ARGS...)>::Function(const FUNC& func) : ref(new _FunctionFunctional<FUNC, RET_TYPE, ARGS...>(func))
{
}

template <class RET_TYPE, class... ARGS>
template <class FUNC>
SLIB_INLINE Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator=(const FUNC& func)
{
	ref = new _FunctionFunctional<FUNC, RET_TYPE, ARGS...>(func);
	return *this;
}

template <class RET_TYPE, class... ARGS>
SLIB_INLINE RET_TYPE Function<RET_TYPE(ARGS...)>::operator()(ARGS... args) const
{
	Functional<RET_TYPE(ARGS...)>* object = ref.ptr;
	if (object) {
		return object->invoke(args...);
	} else {
		return RET_TYPE();
	}
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(SafeFunction, Function, Functional, ref)

template <class RET_TYPE, class... ARGS>
template <class FUNC>
SLIB_INLINE SafeFunction<RET_TYPE(ARGS...)>::SafeFunction(const FUNC& func) : ref(new _FunctionFunctional<FUNC, RET_TYPE, ARGS...>(func))
{
}

template <class RET_TYPE, class... ARGS>
template <class FUNC>
SLIB_INLINE SafeFunction<RET_TYPE(ARGS...)>& SafeFunction<RET_TYPE(ARGS...)>::operator=(const FUNC& func)
{
	ref = new _FunctionFunctional<FUNC, RET_TYPE, ARGS...>(func);
	return *this;
}

template <class RET_TYPE, class... ARGS>
SLIB_INLINE RET_TYPE SafeFunction<RET_TYPE(ARGS...)>::operator()(ARGS... args) const
{
	Ref< Functional<RET_TYPE(ARGS...)> > object(ref);
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
	return (const Functional<RET_TYPE(ARGS...)>*)(new _FunctionFunctional<FUNC, RET_TYPE, ARGS...>(func));
}

template <class RET_TYPE, class... ARGS>
template <class CLASS, class FUNC>
SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::createFromObject(CLASS* object, FUNC func)
{
	if (object) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _ObjectFunctional<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}

template <class RET_TYPE, class... ARGS>
template <class CLASS, class FUNC>
SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::createFromRef(const Ref<CLASS>& object, FUNC func)
{
	if (object.isNotNull()) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _RefFunctional<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}

template <class RET_TYPE, class... ARGS>
template <class CLASS, class FUNC>
SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::createFromWeakRef(const WeakRef<CLASS>& object, FUNC func)
{
	if (object.isNotNull()) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _WeakRefFunctional<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}


template <class CLASS, class RET_TYPE, class... ARGS>
SLIB_INLINE Function<RET_TYPE(ARGS...)> _createFunctionFromObject(CLASS* object, RET_TYPE (CLASS::*func)(ARGS...))
{
	if (object) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _ObjectFunctional<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}

template <class CLASS, class RET_TYPE, class... ARGS>
SLIB_INLINE Function<RET_TYPE(ARGS...)> _createFunctionFromRef(const Ref<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...))
{
	if (object.isNotNull()) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _RefFunctional<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}

template <class CLASS, class RET_TYPE, class... ARGS>
SLIB_INLINE Function<RET_TYPE(ARGS...)> _createFunctionFromWeakRef(const WeakRef<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...))
{
	if (object.isNotNull()) {
		return (const Functional<RET_TYPE(ARGS...)>*)(new _WeakRefFunctional<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
	}
	return Function<RET_TYPE(ARGS...)>::null();
}

SLIB_NAMESPACE_END

#endif
