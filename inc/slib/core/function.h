#ifndef CHECKHEADER_SLIB_CORE_FUNCTION
#define CHECKHEADER_SLIB_CORE_FUNCTION

#include "definition.h"

#include "object.h"
#include "tuple.h"

namespace slib
{
	
	template <class T>
	class Callable;
	
	template <class T>
	class Function;
	
	template <class T>
	using AtomicFunction = Atomic< Function<T> >;
	
	class CallableBase : public Referable
	{
	public:
		SLIB_DECLARE_OBJECT
	};
	
	template <class RET_TYPE, class... ARGS>
	class SLIB_EXPORT Callable<RET_TYPE(ARGS...)> : public CallableBase
	{
	public:
		virtual RET_TYPE invoke(ARGS... params) = 0;

	};
	
	template <class RET_TYPE, class... ARGS>
	class SLIB_EXPORT Function<RET_TYPE(ARGS...)>
	{
	public:
		Ref< Callable<RET_TYPE(ARGS...)> > ref;
		SLIB_REF_WRAPPER(Function, Callable<RET_TYPE(ARGS...)>)

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
		static Function<RET_TYPE(ARGS...)> fromClass(CLASS* object, FUNC func);

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromRef(const Ref<CLASS>& object, FUNC func);

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromWeakRef(const WeakRef<CLASS>& object, FUNC func);
	
		template <class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bind(const FUNC& func, const BINDS&... binds);

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindClass(CLASS* object, FUNC func, const BINDS&... binds);

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindRef(const Ref<CLASS>& object, FUNC func, const BINDS&... binds);

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindWeakRef(const WeakRef<CLASS>& object, FUNC func, const BINDS&... binds);

	};
	
	template <class RET_TYPE, class... ARGS>
	class SLIB_EXPORT Atomic< Function<RET_TYPE(ARGS...)> >
	{
	public:
		AtomicRef< Callable<RET_TYPE(ARGS...)> > ref;
		SLIB_ATOMIC_REF_WRAPPER(Callable<RET_TYPE(ARGS...)>)

	public:
		template <class FUNC>
		Atomic(const FUNC& func);

	public:
		template <class FUNC>
		Atomic& operator=(const FUNC& func);

		RET_TYPE operator()(ARGS... args) const;

	};

}

#define SLIB_BIND_CLASS(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindClass(OBJECT, &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_BIND_REF(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_BIND_WEAKREF(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)

#define SLIB_FUNCTION_CLASS(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromClass(OBJECT, &CLASS::CALLBACK)
#define SLIB_FUNCTION_REF(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK)
#define SLIB_FUNCTION_WEAKREF(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK)

#include "detail/function.h"

#endif
