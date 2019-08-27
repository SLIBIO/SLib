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
	
	namespace priv
	{
		namespace function
		{
			
			template <class RET_TYPE>
			class FunctionInvoker
			{
			public:
				template <class FUNC, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, ARGS&&... args)
				{
					return func(Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
				{
					return (obj->*func)(Forward<ARGS>(args)...);
				}
			};
			
			template<>
			class FunctionInvoker<void>
			{
			public:
				template <class FUNC, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, ARGS&&... args)
				{
					func(Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
				{
					(obj->*func)(Forward<ARGS>(args)...);
				}
			};

			template <class RET_TYPE>
			class BindInvoker
			{
			public:
				template <class FUNC, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<>& t, ARGS&&... args)
				{
					return func(Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<>& t, ARGS&&... args)
				{
					return (obj->*func)(Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1>& t, ARGS&&... args)
				{
					return func(t.m1, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class... ARGS>
				SLIB_INLINE static RET_TYPE invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& t, ARGS&&... args)
				{
					return func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, t.m10, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class... ARGS>
				SLIB_INLINE static RET_TYPE invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& t, ARGS&&... args)
				{
					return (obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, t.m10, Forward<ARGS>(args)...);
				}
				
			};
			
			template<>
			class BindInvoker<void>
			{
			public:
				template <class FUNC, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<>& t, ARGS&&... args)
				{
					func(Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<>& t, ARGS&&... args)
				{
					(obj->*func)(Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1>& t, ARGS&&... args)
				{
					func(t.m1, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, Forward<ARGS>(args)...);
				}
				
				template <class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class... ARGS>
				SLIB_INLINE static void invoke(const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& t, ARGS&&... args)
				{
					func(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, t.m10, Forward<ARGS>(args)...);
				}
				
				template <class CLASS, class FUNC, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class... ARGS>
				SLIB_INLINE static void invokeMember(CLASS* obj, const FUNC& func, const Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& t, ARGS&&... args)
				{
					(obj->*func)(t.m1, t.m2, t.m3, t.m4, t.m5, t.m6, t.m7, t.m8, t.m9, t.m10, Forward<ARGS>(args)...);
				}
				
			};
			
			template <class FUNC, class RET_TYPE, class... ARGS>
			class CallableFromFunction : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				FUNC func;
				
			public:
				template <class OTHER_FUNC>
				SLIB_INLINE CallableFromFunction(OTHER_FUNC&& _func) noexcept
				: func(Forward<OTHER_FUNC>(_func))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return FunctionInvoker<RET_TYPE>::invoke(func, Forward<ARGS>(args)...);
				}
			};
			
			// prevent infinite encapsulation
			template <class RET_TYPE, class... ARGS>
			class CallableFromFunction<Function<RET_TYPE(ARGS...)>, RET_TYPE, ARGS...> : public Callable<RET_TYPE(ARGS...)> {};
			template <class RET_TYPE, class... ARGS>
			class CallableFromFunction<Atomic< Function<RET_TYPE(ARGS...)> >, RET_TYPE, ARGS...> : public Callable<RET_TYPE(ARGS...)> {};

			template <class BIND_TUPLE, class FUNC, class RET_TYPE, class... ARGS>
			class BindFromFunction : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				FUNC func;
				BIND_TUPLE binds;
				
			public:
				template <class OTHER_FUNC, class OTHER_BIND_TUPLE>
				SLIB_INLINE BindFromFunction(OTHER_FUNC&& _func, OTHER_BIND_TUPLE&& _binds) noexcept
				: func(Forward<OTHER_FUNC>(_func)), binds(Forward<OTHER_BIND_TUPLE>(_binds))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return BindInvoker<RET_TYPE>::invoke(func, binds, Forward<ARGS>(args)...);
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableFromMember : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				CLASS* object;
				FUNC func;
				
			public:
				SLIB_INLINE CallableFromMember(CLASS* _object, const FUNC& _func) noexcept
				: object(_object), func(_func)
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return FunctionInvoker<RET_TYPE>::invokeMember(object, func, Forward<ARGS>(args)...);
				}
			};
			
			template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class BindFromMember : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				CLASS* object;
				FUNC func;
				BIND_TUPLE binds;
				
			public:
				template <class OTHER_BIND_TUPLE>
				SLIB_INLINE BindFromMember(CLASS* _object, const FUNC& _func, OTHER_BIND_TUPLE&& _binds) noexcept
				: object(_object), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return BindInvoker<RET_TYPE>::invokeMember(object, func, binds, Forward<ARGS>(args)...);
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableFromRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ref<CLASS> object;
				FUNC func;
				
			public:
				template <class T>
				SLIB_INLINE CallableFromRef(T&& _object, const FUNC& _func) noexcept
				: object(Forward<T>(_object)), func(_func)
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return FunctionInvoker<RET_TYPE>::invokeMember(object._ptr, func, Forward<ARGS>(args)...);
				}
			};
			
			template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class BindFromRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ref<CLASS> object;
				FUNC func;
				BIND_TUPLE binds;
				
			public:
				template <class T, class OTHER_BIND_TUPLE>
				SLIB_INLINE BindFromRef(T&& _object, const FUNC& _func, OTHER_BIND_TUPLE&& _binds) noexcept
				: object(Forward<T>(_object)), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return BindInvoker<RET_TYPE>::invokeMember(object._ptr, func, binds, Forward<ARGS>(args)...);
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableFromWeakRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				WeakRef<CLASS> object;
				FUNC func;
				
			public:
				template <class T>
				SLIB_INLINE CallableFromWeakRef(T&& _object, const FUNC& _func) noexcept
				: object(Forward<T>(_object)), func(_func)
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ref<CLASS> o(object);
					if (o.isNotNull()) {
						return FunctionInvoker<RET_TYPE>::invokeMember(o._ptr, func, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
			template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class BindFromWeakRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				WeakRef<CLASS> object;
				FUNC func;
				BIND_TUPLE binds;
				
			public:
				template <class T, class OTHER_BIND_TUPLE>
				SLIB_INLINE BindFromWeakRef(T&& _object, const FUNC& _func, OTHER_BIND_TUPLE&& _binds) noexcept
				: object(Forward<T>(_object)), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ref<CLASS> o(object);
					if (o.isNotNull()) {
						return BindInvoker<RET_TYPE>::invokeMember(o._ptr, func, binds, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableFromPtr : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ptr<CLASS> object;
				FUNC func;
				
			public:
				template <class T>
				SLIB_INLINE CallableFromPtr(T&& _object, const FUNC& _func) noexcept
				: object(Forward<T>(_object)), func(_func)
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ptr<CLASS> o(object.lock());
					if (o.isNotNull()) {
						return FunctionInvoker<RET_TYPE>::invokeMember(o._ptr, func, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
			template <class BIND_TUPLE, class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class BindFromPtr : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ptr<CLASS> object;
				FUNC func;
				BIND_TUPLE binds;
				
			public:
				template <class T, class OTHER_BIND_TUPLE>
				SLIB_INLINE BindFromPtr(T&& _object, const FUNC& _func, OTHER_BIND_TUPLE&& _binds) noexcept
				: object(Forward<T>(_object)), func(_func), binds(Forward<OTHER_BIND_TUPLE>(_binds))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ptr<CLASS> o(object.lock());
					if (o.isNotNull()) {
						return BindInvoker<RET_TYPE>::invokeMember(o._ptr, func, binds, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableWithRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ref<CLASS> object;
				FUNC func;
				
			public:
				template <class OTHER_CLASS, class OTHER_FUNC>
				SLIB_INLINE CallableWithRef(OTHER_CLASS&& _object, OTHER_FUNC&& _func) noexcept
				: object(Forward<OTHER_CLASS>(_object)), func(Forward<OTHER_FUNC>(_func))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					return FunctionInvoker<RET_TYPE>::invoke(func, Forward<ARGS>(args)...);
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableWithWeakRef : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				WeakRef<CLASS> object;
				FUNC func;
				
			public:
				template <class OTHER_CLASS, class OTHER_FUNC>
				SLIB_INLINE CallableWithWeakRef(OTHER_CLASS&& _object, OTHER_FUNC&& _func) noexcept
				: object(Forward<OTHER_CLASS>(_object)), func(Forward<OTHER_FUNC>(_func))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ref<CLASS> o(object);
					if (o.isNotNull()) {
						return FunctionInvoker<RET_TYPE>::invoke(func, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
			template <class CLASS, class FUNC, class RET_TYPE, class... ARGS>
			class CallableWithPtr : public Callable<RET_TYPE(ARGS...)>
			{
			protected:
				Ptr<CLASS> object;
				FUNC func;
				
			public:
				template <class OTHER_CLASS, class OTHER_FUNC>
				SLIB_INLINE CallableWithPtr(OTHER_CLASS&& _object, OTHER_FUNC&& _func) noexcept
				: object(Forward<OTHER_CLASS>(_object)), func(Forward<OTHER_FUNC>(_func))
				{}
				
			public:
				RET_TYPE invoke(ARGS... args) override
				{
					Ptr<CLASS> o(object.lock());
					if (o.isNotNull()) {
						return FunctionInvoker<RET_TYPE>::invoke(func, Forward<ARGS>(args)...);
					} else {
						return NullValue<RET_TYPE>::get();
					}
				}
			};
			
		}
	}
	
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>::Function(FUNC&& func) noexcept
	: ref(new priv::function::CallableFromFunction<typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func)))
	{}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)>& Function<RET_TYPE(ARGS...)>::operator=(FUNC&& func) noexcept
	{
		ref = new priv::function::CallableFromFunction<typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func));
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
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >::Atomic(FUNC&& func) noexcept
	 : ref(new priv::function::CallableFromFunction<typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func)))
	 {}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Atomic< Function<RET_TYPE(ARGS...)> >& Atomic< Function<RET_TYPE(ARGS...)> >::operator=(FUNC&& func) noexcept
	{
		ref = new priv::function::CallableFromFunction<typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func));
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
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::create(FUNC&& func) noexcept
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromFunction<typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func)));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromMember(CLASS* object, const FUNC& func) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromMember<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromRef(CLASS* object, const FUNC& func) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromRef(const Ref<CLASS>& object, const FUNC& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromWeakRef(CLASS* object, const FUNC& func) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromWeakRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromWeakRef(const WeakRef<CLASS>& object, const FUNC& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromWeakRef<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::fromPtr(const Ptr<CLASS>& object, const FUNC& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromPtr<CLASS, FUNC, RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bind(FUNC&& func) noexcept
	{
		return create(Forward<FUNC>(func));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bind(FUNC&& func, const BINDS&... binds) noexcept
	{
		return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromFunction<Tuple<BINDS...>, typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(Forward<FUNC>(func), Tuple<BINDS...>(binds...)));
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindMember(CLASS* object, const FUNC& func) noexcept
	{
		return fromMember(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindMember(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromMember<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(CLASS* object, const FUNC& func) noexcept
	{
		return fromRef(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(const Ref<CLASS>& object, const FUNC& func) noexcept
	{
		return fromRef(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindRef(const Ref<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(CLASS* object, const FUNC& func) noexcept
	{
		return fromWeakRef(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(CLASS* object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromWeakRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(const WeakRef<CLASS>& object, const FUNC& func) noexcept
	{
		return fromWeakRef(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindWeakRef(const WeakRef<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromWeakRef<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindPtr(const Ptr<CLASS>& object, const FUNC& func) noexcept
	{
		return fromPtr(object, func);
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC, class... BINDS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::bindPtr(const Ptr<CLASS>& object, const FUNC& func, const BINDS&... binds) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::BindFromPtr<Tuple<BINDS...>, CLASS, FUNC, RET_TYPE, ARGS...>(object, func, Tuple<BINDS...>(binds...)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::with(const Ref<CLASS>& object, FUNC&& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableWithRef<CLASS, typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(object, Forward<FUNC>(func)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::with(const WeakRef<CLASS>& object, FUNC&& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableWithWeakRef<CLASS, typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(object, Forward<FUNC>(func)));
		}
		return sl_null;
	}
	
	template <class RET_TYPE, class... ARGS>
	template <class CLASS, class FUNC>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> Function<RET_TYPE(ARGS...)>::with(const Ptr<CLASS>& object, FUNC&& func) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableWithPtr<CLASS, typename RemoveConstReference<FUNC>::Type, RET_TYPE, ARGS...>(object, Forward<FUNC>(func)));
		}
		return sl_null;
	}
	
	
	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromMember(CLASS* object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromMember<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromRef(const Ref<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	template <class CLASS, class RET_TYPE, class... ARGS>
	SLIB_INLINE Function<RET_TYPE(ARGS...)> CreateFunctionFromWeakRef(const WeakRef<CLASS>& object, RET_TYPE (CLASS::*func)(ARGS...)) noexcept
	{
		if (object.isNotNull()) {
			return static_cast<Callable<RET_TYPE(ARGS...)>*>(new priv::function::CallableFromWeakRef<CLASS, RET_TYPE (CLASS::*)(ARGS...), RET_TYPE, ARGS...>(object, func));
		}
		return sl_null;
	}

	
	namespace priv
	{
		namespace function_list
		{
			extern const char g_classID[];
		}
	}

	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_object_type FunctionList<RET_TYPE(ARGS...)>::ObjectType() noexcept
	{
		return priv::function_list::g_classID;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool FunctionList<RET_TYPE(ARGS...)>::isDerivedFrom(sl_object_type type) noexcept
	{
		if (type == priv::function_list::g_classID) {
			return sl_true;
		}
		return CallableBase::isDerivedFrom(type);
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_object_type FunctionList<RET_TYPE(ARGS...)>::getObjectType() const noexcept
	{
		return priv::function_list::g_classID;
	}
	
	template <class RET_TYPE, class... ARGS>
	SLIB_INLINE sl_bool FunctionList<RET_TYPE(ARGS...)>::isInstanceOf(sl_object_type type) const noexcept
	{
		if (type == priv::function_list::g_classID) {
			return sl_true;
		}
		return CallableBase::isDerivedFrom(type);
	}
	
	template <class RET_TYPE, class... ARGS>
	RET_TYPE FunctionList<RET_TYPE(ARGS...)>::invoke(ARGS... args)
	{
		ListElements< Function<RET_TYPE(ARGS...)> > functions(list);
		for (sl_size i = 0; i < functions.count; i++) {
			if (i + 1 == functions.count) {
				return functions[i](args...);
			} else {
				functions[i](args...);
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
							if ((sl_size)index + 1 < count) {
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
