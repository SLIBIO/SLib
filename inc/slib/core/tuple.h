#ifndef CHECKHEADER_SLIB_CORE_TUPLE
#define CHECKHEADER_SLIB_CORE_TUPLE

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class... MEMBERS>
class Tuple;

template <>
class Tuple<>
{
public:
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(); }
};

template <class T>
class Tuple<T>
{
public:
	T m;
	SLIB_INLINE Tuple(const T& _m) : m(_m) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m); }
};

template <class T1, class T2>
class Tuple<T1, T2>
{
public:
	T1 m1;
	T2 m2;
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2) : m1(_m1), m2(_m2) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2); }
};

template <class T1, class T2, class T3>
class Tuple<T1, T2, T3>
{
public:
	T1 m1;
	T2 m2;
	T3 m3;
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3) : m1(_m1), m2(_m2), m3(_m3) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3); }
};

template <class T1, class T2, class T3, class T4>
class Tuple<T1, T2, T3, T4>
{
public:
	T1 m1;
	T2 m2;
	T3 m3;
	T4 m4;
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4) : m1(_m1), m2(_m2), m3(_m3), m4(_m4) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4); }
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
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4, const T5& _m5) : m1(_m1), m2(_m2), m3(_m3), m4(_m4), m5(_m5) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4, m5); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4, m5); }
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
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4, const T5& _m5, const T6& _m6) : m1(_m1), m2(_m2), m3(_m3), m4(_m4), m5(_m5), m6(_m6) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4, m5, m6); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4, m5, m6); }
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
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4, const T5& _m5, const T6& _m6, const T7& _m7) : m1(_m1), m2(_m2), m3(_m3), m4(_m4), m5(_m5), m6(_m6), m7(_m7) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4, m5, m6, m7); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4, m5, m6, m7); }
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
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4, const T5& _m5, const T6& _m6, const T7& _m7, const T8& _m8) : m1(_m1), m2(_m2), m3(_m3), m4(_m4), m5(_m5), m6(_m6), m7(_m7), m8(_m8) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4, m5, m6, m7, m8); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4, m5, m6, m7, m8); }
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
	SLIB_INLINE Tuple(const T1& _m1, const T2& _m2, const T3& _m3, const T4& _m4, const T5& _m5, const T6& _m6, const T7& _m7, const T8& _m8, const T9& _m9) : m1(_m1), m2(_m2), m3(_m3), m4(_m4), m5(_m5), m6(_m6), m7(_m7), m8(_m8), m9(_m9) {}
	template <class FUNC> SLIB_INLINE void invoke(const FUNC& func) { func(m1, m2, m3, m4, m5, m6, m7, m8, m9); }
	template <class CLASS, class FUNC> SLIB_INLINE void invokeMember(CLASS* obj, const FUNC& func) { (obj->*func)(m1, m2, m3, m4, m5, m6, m7, m8, m9); }
};

SLIB_NAMESPACE_END

#endif
