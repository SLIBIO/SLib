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

#ifndef CHECKHEADER_SLIB_CORE_FUNCTION
#define CHECKHEADER_SLIB_CORE_FUNCTION

#include "definition.h"

#include "ref.h"
#include "ptr.h"
#include "tuple.h"
#include "null_value.h"
#include "list.h"

namespace slib
{
	
	template <class T>
	class Callable;
	
	template <class T>
	class Function;
	
	template <class T>
	using AtomicFunction = Atomic< Function<T> >;
	
	template <class T>
	class FunctionList;

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
		Function(FUNC&& func) noexcept;

	public:
		template <class FUNC>
		Function& operator=(FUNC&& func) noexcept;

		RET_TYPE operator()(ARGS... args) const;
		
		Function& operator+=(const Function&) noexcept;
		
		Function operator+(const Function&) noexcept;
		
		Function& operator-=(const Function&) noexcept;

		Function operator-(const Function&) noexcept;

	public:
		template <class FUNC>
		static Function<RET_TYPE(ARGS...)> create(FUNC&& func) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromMember(CLASS* object, const FUNC& func) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromRef(CLASS* object, const FUNC& func) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromRef(const Ref<CLASS>& object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromWeakRef(CLASS* object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromWeakRef(const WeakRef<CLASS>& object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> fromPtr(const Ptr<CLASS>& object, const FUNC& func) noexcept;
		
		template <class FUNC>
		static Function<RET_TYPE(ARGS...)> bind(FUNC&& func) noexcept;

		template <class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bind(FUNC&& func, const BINDS&... binds) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindMember(CLASS* object, const FUNC& func) noexcept;

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindMember(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindRef(CLASS* object, const FUNC& func) noexcept;

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindRef(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindRef(const Ref<CLASS>& object, const FUNC& func) noexcept;

		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindRef(const Ref<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindWeakRef(CLASS* object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindWeakRef(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindWeakRef(const WeakRef<CLASS>& object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindWeakRef(const WeakRef<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> bindPtr(const Ptr<CLASS>& object, const FUNC& func) noexcept;
		
		template <class CLASS, class FUNC, class... BINDS>
		static Function<RET_TYPE(ARGS...)> bindPtr(const Ptr<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> with(const Ref<CLASS>& object, FUNC&& func) noexcept;

		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> with(const WeakRef<CLASS>& object, FUNC&& func) noexcept;
		
		template <class CLASS, class FUNC>
		static Function<RET_TYPE(ARGS...)> with(const Ptr<CLASS>& object, FUNC&& func) noexcept;
		
		static Function<RET_TYPE(ARGS...)> fromList(const List< Function<RET_TYPE(ARGS...)> >&) noexcept;
		
	public:
		sl_bool isList() const noexcept;
		
		const List< Function<RET_TYPE(ARGS...)> >& getList() const noexcept;

		// return `function`
		Function<RET_TYPE(ARGS...)> add(const Function& function) noexcept;
		
		// return `function`
		Function<RET_TYPE(ARGS...)> addIfNotExist(const Function& function) noexcept;
		
		void remove(const Function& function, sl_bool flagRemoveAllMatch = sl_false) noexcept;
		
		sl_bool contains(const Function& function) const noexcept;
		
	};
	
	template <class RET_TYPE, class... ARGS>
	class SLIB_EXPORT Atomic< Function<RET_TYPE(ARGS...)> >
	{
	public:
		AtomicRef< Callable<RET_TYPE(ARGS...)> > ref;
		SLIB_ATOMIC_REF_WRAPPER(Callable<RET_TYPE(ARGS...)>)

	public:
		template <class FUNC>
		Atomic(FUNC&& func) noexcept;

	public:
		template <class FUNC>
		Atomic& operator=(FUNC&& func) noexcept;

		RET_TYPE operator()(ARGS... args) const;
		
		Atomic& operator+=(const Function<RET_TYPE(ARGS...)>&) noexcept;
		
		Function<RET_TYPE(ARGS...)> operator+(const Function<RET_TYPE(ARGS...)>&) noexcept;
		
		Atomic& operator-=(const Function<RET_TYPE(ARGS...)>&) noexcept;
		
		Function<RET_TYPE(ARGS...)> operator-(const Function<RET_TYPE(ARGS...)>&) noexcept;
		
	public:
		sl_bool isList() const noexcept;
		
		const List< Function<RET_TYPE(ARGS...)> >& getList() const noexcept;
		
		// return `function`
		Function<RET_TYPE(ARGS...)> add(const Function<RET_TYPE(ARGS...)>& function) noexcept;
		
		// return `function`
		Function<RET_TYPE(ARGS...)> addIfNotExist(const Function<RET_TYPE(ARGS...)>& function) noexcept;
		
		void remove(const Function<RET_TYPE(ARGS...)>& function, sl_bool flagRemoveAllMatch = sl_false) noexcept;
		
		sl_bool contains(const Function<RET_TYPE(ARGS...)>& function) const noexcept;
		
	};

	
	template <class RET_TYPE, class... ARGS>
	class SLIB_EXPORT FunctionList<RET_TYPE(ARGS...)> : public Callable<RET_TYPE(ARGS...)>
	{
	public:
		List< Function<RET_TYPE(ARGS...)> > list;
		
	public:
		static sl_object_type ObjectType() noexcept;
		
		static sl_bool isDerivedFrom(sl_object_type type) noexcept;
		
		sl_object_type getObjectType() const noexcept override;
		
		sl_bool isInstanceOf(sl_object_type type) const noexcept override;
		
	public:
		SLIB_INLINE FunctionList() noexcept {}
		
		SLIB_INLINE FunctionList(const List< Function<RET_TYPE(ARGS...)> >& _list) noexcept : list(_list) {}
		
	public:
		RET_TYPE invoke(ARGS... params) override;
		
	};
	
}

#define SLIB_BIND_CLASS(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindMember(OBJECT, &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_BIND_REF(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)
#define SLIB_BIND_WEAKREF(TYPE, CLASS, CALLBACK, OBJECT, ...) slib::Function<TYPE>::bindWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK, ##__VA_ARGS__)

#define SLIB_FUNCTION_MEMBER(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromMember(OBJECT, &CLASS::CALLBACK)
#define SLIB_FUNCTION_REF(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromRef(slib::Ref<CLASS>(OBJECT), &CLASS::CALLBACK)
#define SLIB_FUNCTION_WEAKREF(CLASS, CALLBACK, OBJECT) slib::CreateFunctionFromWeakRef(slib::WeakRef<CLASS>(OBJECT), &CLASS::CALLBACK)

#define SLIB_PROPERTY_FUNCTION(TYPE, NAME) \
protected:	slib::AtomicFunction<TYPE> m_function_##NAME; \
public:		SLIB_INLINE slib::Function<TYPE> get##NAME() const { return m_function_##NAME; } \
 			SLIB_INLINE slib::Function<TYPE> set##NAME(const slib::Function<TYPE>& value) { m_function_##NAME = value; return value; } \
			SLIB_INLINE slib::Function<TYPE> add##NAME(const slib::Function<TYPE>& value) { return m_function_##NAME.add(value); } \
			SLIB_INLINE void remove##NAME(const slib::Function<TYPE>& value) { m_function_##NAME.remove(value); }

#define SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS_WITHOUT_ON(CLASS, NAME, ...) \
public:		slib::Function<void(CLASS* sender, ##__VA_ARGS__)> getOn##NAME() const; \
			slib::Function<void(CLASS* sender, ##__VA_ARGS__)> setOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler); \
			slib::Function<void(CLASS* sender, ##__VA_ARGS__)> addOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler); \
			void removeOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler);

#define SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS_WITHOUT_DISPATCH(CLASS, NAME, ...) \
protected:	virtual void on##NAME(__VA_ARGS__); \
			SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS_WITHOUT_ON(CLASS, NAME, ##__VA_ARGS__);

#define SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS(CLASS, NAME, ...) \
			SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS_WITHOUT_DISPATCH(CLASS, NAME, ##__VA_ARGS__) \
public:		virtual void dispatch##NAME(__VA_ARGS__);

#define SLIB_DECLARE_EVENT_HANDLER(CLASS, NAME, ...) \
protected:	slib::AtomicFunction<void(CLASS* sender, ##__VA_ARGS__)> m_eventHandler_on##NAME; \
			SLIB_DECLARE_EVENT_HANDLER_FUNCTIONS(CLASS, NAME, ##__VA_ARGS__)

#define SLIB_DEFINE_EVENT_HANDLER_WITHOUT_ON(CLASS, NAME, ...) \
	slib::Function<void(CLASS* sender, ##__VA_ARGS__)> CLASS::getOn##NAME() const { return m_eventHandler_on##NAME; } \
	slib::Function<void(CLASS* sender, ##__VA_ARGS__)> CLASS::setOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler) { m_eventHandler_on##NAME = handler; return handler; } \
	slib::Function<void(CLASS* sender, ##__VA_ARGS__)> CLASS::addOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler) { return m_eventHandler_on##NAME.add(handler); } \
	void CLASS::removeOn##NAME(const slib::Function<void(CLASS* sender, ##__VA_ARGS__)>& handler) { m_eventHandler_on##NAME.remove(handler); }

#define SLIB_DEFINE_EVENT_HANDLER(CLASS, NAME, ...) \
	SLIB_DEFINE_EVENT_HANDLER_WITHOUT_ON(CLASS, NAME, ##__VA_ARGS__) \
	void CLASS::on##NAME(__VA_ARGS__) {}

#define SLIB_INVOKE_EVENT_HANDLER(NAME, ...) \
	getOn##NAME()(this, ##__VA_ARGS__); \
	on##NAME(__VA_ARGS__);

#include "detail/function.inc"

#endif
