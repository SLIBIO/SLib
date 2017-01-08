#ifndef CHECKHEADER_SLIB_CORE_SINGLETON
#define CHECKHEADER_SLIB_CORE_SINGLETON

#include "ref.h"
#include "safe_static.h"

#define SLIB_DECLARE_SINGLETON(TYPE) \
public: \
	static slib::Ref<TYPE> getInstance();

#define SLIB_DEFINE_SINGLETON(TYPE, ...) \
	slib::Ref<TYPE> TYPE::getInstance() \
	{ \
		SLIB_TRY_CONVERT_TYPE(TYPE*, Referable*) \
		SLIB_SAFE_STATIC(slib::Ref<TYPE>, instance, new TYPE(__VA_ARGS__)); \
		if (SLIB_SAFE_STATIC_CHECK_FREED(instance)) { \
			return sl_null; \
		} \
		return instance; \
	}

#endif
