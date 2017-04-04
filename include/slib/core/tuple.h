/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_TUPLE
#define CHECKHEADER_SLIB_CORE_TUPLE

#include "definition.h"

#include "cpp.h"

namespace slib
{
	
	template <class... MEMBERS>
	class Tuple;
	
	template <>
	class Tuple<>
	{
	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		SLIB_INLINE Tuple() = default;

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(Forward<ARGS>(args)...);
		}
	
	};
	
	template <class T1>
	class Tuple<T1>
	{
	public:
		T1 m1;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1>
		SLIB_INLINE Tuple(O1&& _m1):
		 m1(Forward<O1>(_m1))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, Forward<ARGS>(args)...);
		}

	};
	
	template <class T1, class T2>
	class Tuple<T1, T2>
	{
	public:
		T1 m1;
		T2 m2;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, Forward<ARGS>(args)...);
		}
	
	};
	
	template <class T1, class T2, class T3>
	class Tuple<T1, T2, T3>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, Forward<ARGS>(args)...);
		}

	};
	
	template <class T1, class T2, class T3, class T4>
	class Tuple<T1, T2, T3, T4>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, Forward<ARGS>(args)...);
		}
	
	};
	
	template <class T1, class T2, class T3, class T4, class T5>
	class Tuple<T1, T2, T3, T4, T5>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, Forward<ARGS>(args)...);
		}
	
	};
	
	template <class T1, class T2, class T3, class T4, class T5, class T6>
	class Tuple<T1, T2, T3, T4, T5, T6>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5, class O6>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5, O6&& _m6):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5)),
		 m6(Forward<O6>(_m6))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, m6, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, m6, Forward<ARGS>(args)...);
		}
	
	};
	
	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class Tuple<T1, T2, T3, T4, T5, T6, T7>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;
		T7 m7;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5, class O6, class O7>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5, O6&& _m6, O7&& _m7):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5)),
		 m6(Forward<O6>(_m6)),
		 m7(Forward<O7>(_m7))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, m6, m7, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, m6, m7, Forward<ARGS>(args)...);
		}

	};
	
	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class Tuple<T1, T2, T3, T4, T5, T6, T7, T8>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;
		T7 m7;
		T8 m8;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5, class O6, class O7, class O8>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5, O6&& _m6, O7&& _m7, O8&& _m8):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5)),
		 m6(Forward<O6>(_m6)),
		 m7(Forward<O7>(_m7)),
		 m8(Forward<O8>(_m8))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, m6, m7, m8, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, m6, m7, m8, Forward<ARGS>(args)...);
		}

	};
	
	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;
		T7 m7;
		T8 m8;
		T9 m9;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5, class O6, class O7, class O8, class O9>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5, O6&& _m6, O7&& _m7, O8&& _m8, O9&& _m9):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5)),
		 m6(Forward<O6>(_m6)),
		 m7(Forward<O7>(_m7)),
		 m8(Forward<O8>(_m8)),
		 m9(Forward<O9>(_m9))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, m6, m7, m8, m9, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, m6, m7, m8, m9, Forward<ARGS>(args)...);
		}

	};
	
	template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
	class Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
	{
	public:
		T1 m1;
		T2 m2;
		T3 m3;
		T4 m4;
		T5 m5;
		T6 m6;
		T7 m7;
		T8 m8;
		T9 m9;
		T10 m10;

	public:
		SLIB_INLINE Tuple(const Tuple& other) = default;

		SLIB_INLINE Tuple(Tuple&& other) = default;

		template <class O1, class O2, class O3, class O4, class O5, class O6, class O7, class O8, class O9, class O10>
		SLIB_INLINE Tuple(O1&& _m1, O2&& _m2, O3&& _m3, O4&& _m4, O5&& _m5, O6&& _m6, O7&& _m7, O8&& _m8, O9&& _m9, O10&& _m10):
		 m1(Forward<O1>(_m1)),
		 m2(Forward<O2>(_m2)),
		 m3(Forward<O3>(_m3)),
		 m4(Forward<O4>(_m4)),
		 m5(Forward<O5>(_m5)),
		 m6(Forward<O6>(_m6)),
		 m7(Forward<O7>(_m7)),
		 m8(Forward<O8>(_m8)),
		 m9(Forward<O9>(_m9)),
		 m10(Forward<O10>(_m10))
		{}

	public:
		template <class FUNC, class... ARGS>
		SLIB_INLINE void invoke(const FUNC& func, ARGS&&... args)
		{
			func(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, Forward<ARGS>(args)...);
		}

		template <class CLASS, class FUNC, class... ARGS>
		SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func, ARGS&&... args)
		{
			(obj->*func)(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, Forward<ARGS>(args)...);
		}

	};

}

#endif
