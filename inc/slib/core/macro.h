#ifndef CHECKHEADER_SLIB_CORE_MACRO
#define CHECKHEADER_SLIB_CORE_MACRO

#include "definition.h"

#define SLIB_DECLARE_PRIMITIVE_WRAPPER(TYPE, CLASS) \
public: \
	TYPE value; \
public: \
	SLIB_INLINE CLASS(const CLASS& other) : value(other.value) {} \
	SLIB_INLINE CLASS(TYPE _value) : value(_value) {} \
	SLIB_INLINE operator TYPE() const { return value; } \
	SLIB_INLINE CLASS& operator=(const CLASS& other) { value = other.value; return *this; } \
	SLIB_INLINE CLASS& operator=(TYPE _value) { value = _value; return *this; } \
	SLIB_INLINE sl_bool operator==(const CLASS& other) { return value == other.value; } \
	SLIB_INLINE sl_bool operator==(TYPE _value) { return value == _value; } \
	SLIB_INLINE sl_bool operator!=(const CLASS& other) { return value != other.value; } \
	SLIB_INLINE sl_bool operator!=(TYPE _value) { return value != _value; }

#define SLIB_DECLARE_FLAGS(CLASS) \
	SLIB_DECLARE_PRIMITIVE_WRAPPER(int, CLASS) \
	SLIB_INLINE CLASS() = default; \
	SLIB_INLINE CLASS& operator|=(int _value) { value |= _value; return *this; } \
	SLIB_INLINE CLASS& operator&=(int _value) { value &= _value; return *this; }


#define SLIB_DECLARE_SINGLETON(TYPE) \
public: \
	static slib::Ref<TYPE> getInstance();

#define SLIB_DEFINE_SINGLETON(TYPE) \
	slib::Ref<TYPE> TYPE::getInstance() \
	{ \
		SLIB_SAFE_STATIC(slib::Ref<TYPE>, instance, new TYPE); \
		if (SLIB_SAFE_STATIC_CHECK_FREED(instance)) { \
			return slib::Ref<TYPE>::null(); \
		} \
		return instance; \
	}


#define SLIB_PROPERTY(TYPE, NAME) protected: \
	TYPE _m_property_##NAME; \
public: \
	SLIB_INLINE TYPE const& get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(TYPE const& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_BOOLEAN_PROPERTY(NAME) protected: \
	sl_bool _m_property_##NAME; \
public: \
	SLIB_INLINE sl_bool is##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(sl_bool v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_REF_PROPERTY(TYPE, NAME) protected: \
	slib::SafeRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_WEAK_PROPERTY(TYPE, NAME) protected: \
	slib::SafeWeakRef<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ref<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ref<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_PTR_PROPERTY(TYPE, NAME) protected: \
	slib::SafePtr<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Ptr<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Ptr<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_CALLBACK_PROPERTY(NAME) protected: \
	slib::SafeCallback _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Callback get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Callback& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_FUNCTION_PROPERTY(TYPE, NAME) protected: \
	slib::SafeFunction<TYPE> _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Function<TYPE> get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Function<TYPE>& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_STRING_PROPERTY(NAME) protected: \
	slib::SafeString _m_property_##NAME; \
public: \
	SLIB_INLINE slib::String get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::String& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_STRING16_PROPERTY(NAME) protected: \
	slib::SafeString16 _m_property_##NAME; \
public: \
	SLIB_INLINE slib::String16 get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::String16& v) { \
		_m_property_##NAME = v; \
	}

#define SLIB_VARIANT_PROPERTY(NAME) protected: \
	slib::SafeVariant _m_property_##NAME; \
public: \
	SLIB_INLINE slib::Variant get##NAME() const { \
		return _m_property_##NAME; \
	} \
	SLIB_INLINE void set##NAME(const slib::Variant& v) { \
		_m_property_##NAME = v; \
	}

#endif
