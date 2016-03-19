#ifndef CHECKHEADER_SLIB_MATH_DEFINITION
#define CHECKHEADER_SLIB_MATH_DEFINITION

#include "../core/definition.h"

#define SLIB_MATH_NAMESPACE_BEGIN		SLIB_NAMESPACE_BEGIN
#define SLIB_MATH_NAMESPACE_END			SLIB_NAMESPACE_END

#define SLIB_DECLARE_GEOMETRY_TYPE(TYPE) \
	extern template class TYPE##T<float>; \
	extern template class TYPE##T<double>; \
	typedef TYPE##T<sl_real> TYPE; \
	typedef TYPE##T<float> TYPE##f; \
	typedef TYPE##T<double> TYPE##lf;

#define SLIB_DEFINE_GEOMETRY_TYPE(TYPE) \
	template class TYPE##T<float>; \
	template class TYPE##T<double>;


#define SLIB_DECLARE_GEOMETRY_TYPE_EX(TYPE) \
	extern template class TYPE##T<float>; \
	extern template class TYPE##T<double>; \
	extern template class TYPE##T<sl_int32, float>; \
	extern template class TYPE##T<sl_int64, double>; \
	typedef TYPE##T<sl_real> TYPE; \
	typedef TYPE##T<float> TYPE##f; \
	typedef TYPE##T<double> TYPE##lf; \
	typedef TYPE##T<sl_int32, float> TYPE##i; \
	typedef TYPE##T<sl_int64, double> TYPE##li;

#define SLIB_DEFINE_GEOMETRY_TYPE_EX(TYPE) \
	template class TYPE##T<float>; \
	template class TYPE##T<double>; \
	template class TYPE##T<sl_int32, float>; \
	template class TYPE##T<sl_int64, double>;


#endif
