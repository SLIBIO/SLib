#ifndef CHECKHEADER_SLIB_CORE_REF
#define CHECKHEADER_SLIB_CORE_REF

#include "definition.h"

#include "base.h"
#include "atomic.h"
#include "macro.h"

#ifdef SLIB_DEBUG
#define SLIB_DEBUG_REFERENCE
#endif

typedef const void* sl_object_type;

namespace slib
{
	
	class CWeakRef;
	
	class SLIB_EXPORT Referable
	{
	public:
		Referable();

		Referable(const Referable& other);

		virtual ~Referable();

	public:
		sl_reg increaseReference();

		sl_reg decreaseReference();

		sl_reg decreaseReferenceNoFree();

		void makeNeverFree();

	public:
		virtual sl_object_type getObjectType() const;

		virtual sl_bool isInstanceOf(sl_object_type type) const;

	private:
		void _clearWeak();

	public:
		SLIB_INLINE sl_bool _isWeakRef() const
		{
			return m_flagWeakRef;
		}

		CWeakRef* _getWeakObject();

		void _free();

	private:
		sl_reg m_signature;
#if defined(SLIB_DEBUG_REFERENCE)
		void _checkValid();
#endif

	private:
		sl_reg m_nRefCount;
		sl_bool m_flagWeakRef;
		CWeakRef* m_weak;
		SpinLock m_lockWeak;

		friend class CWeakRef;
	};

	
	template <class T>
	class Ref;
	
	template <class T>
	class WeakRef;
	
	template <class T>
	using AtomicRef = Atomic< Ref<T> >;
	
	template <class T>
	using AtomicWeakRef = Atomic< WeakRef<T> >;

	
	template <class T>
	class SLIB_EXPORT Ref
	{
	public:
		constexpr Ref() : _ptr(sl_null) {}

		constexpr Ref(sl_null_t) : _ptr(sl_null) {}

		Ref(T* _other);

		Ref(Ref<T>&& other);

		Ref(const Ref<T>& other);

		template <class _T>
		Ref(Ref<_T>&& other);

		template <class _T>
		Ref(const Ref<_T>& other);

		Ref(AtomicRef<T>&& other);

		Ref(const AtomicRef<T>& other);

		template <class _T>
		Ref(AtomicRef<_T>&& other);

		template <class _T>
		Ref(const AtomicRef<_T>& other);

		Ref(const WeakRef<T>& other);

		template <class _T>
		Ref(const WeakRef<_T>& other);

		Ref(const AtomicWeakRef<T>& other);

		template <class _T>
		Ref(const AtomicWeakRef<_T>& other);

		~Ref();
	
	public:
		static const Ref<T>& null();

		sl_bool isNull() const;
	
		sl_bool isNotNull() const;
	
		void setNull();

		T* get() const&;

		const Ref<Referable>& getReference() const;

		template <class _T>
		static const Ref<T>& from(const Ref<_T>& other);

		template <class _T>
		static Ref<T>& from(Ref<_T>& other);

		template <class _T>
		static Ref<T>&& from(Ref<_T>&& other);

	public:
		Ref<T>& operator=(sl_null_t);

		Ref<T>& operator=(T* other);
	
		Ref<T>& operator=(Ref<T>&& other);

		Ref<T>& operator=(const Ref<T>& other);

		template <class _T>
		Ref<T>& operator=(Ref<_T>&& other);

		template <class _T>
		Ref<T>& operator=(const Ref<_T>& other);

		Ref<T>& operator=(AtomicRef<T>&& other);

		Ref<T>& operator=(const AtomicRef<T>& other);

		template <class _T>
		Ref<T>& operator=(AtomicRef<_T>&& other);

		template <class _T>
		Ref<T>& operator=(const AtomicRef<_T>& other);

		Ref<T>& operator=(const WeakRef<T>& other);

		template <class _T>
		Ref<T>& operator=(const WeakRef<_T>& other);

		Ref<T>& operator=(const AtomicWeakRef<T>& other);

		template <class _T>
		Ref<T>& operator=(const AtomicWeakRef<_T>& other);
	
	public:
		sl_bool operator==(sl_null_t) const;

		sl_bool operator==(T* other) const;

		sl_bool operator==(const Ref<T>& other) const;
	
		template <class _T>
		sl_bool operator==(const Ref<_T>& other) const;

		sl_bool operator==(const AtomicRef<T>& other) const;

		template <class _T>
		sl_bool operator==(const AtomicRef<_T>& other) const;

		sl_bool operator!=(sl_null_t) const;
	
		sl_bool operator!=(T* other) const;

		sl_bool operator!=(const Ref<T>& other) const;
	
		template <class _T>
		sl_bool operator!=(const Ref<_T>& other) const;

		sl_bool operator!=(const AtomicRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const AtomicRef<_T>& other) const;
	
	public:
		T& operator*() const&;
	
		T* operator->() const&;
	
	public:
		void _replaceObject(T* other);

		void _move_init(void* other);
	
		void _move_assign(void* other);

	public:
		T* _ptr;

	};
	
	template <class T>
	class Atomic< Ref<T> >
	{
	public:
		constexpr Atomic() : _ptr(sl_null) {}

		constexpr Atomic(sl_null_t) : _ptr(sl_null) {}

		Atomic(T* other);

		Atomic(AtomicRef<T>&& other);
	
		Atomic(const AtomicRef<T>& other);

		template <class _T>
		Atomic(AtomicRef<_T>&& other);
	
		template <class _T>
		Atomic(const AtomicRef<_T>& other);

		Atomic(Ref<T>&& other);

		Atomic(const Ref<T>& other);

		template <class _T>
		Atomic(Ref<_T>&& other);
	
		template <class _T>
		Atomic(const Ref<_T>& other);

		Atomic(const WeakRef<T>& other);

		template <class _T>
		Atomic(const WeakRef<_T>& other);

		Atomic(const AtomicWeakRef<T>& other);

		template <class _T>
		Atomic(const AtomicWeakRef<_T>& other);
	
		~Atomic();
	
	public:
		static const AtomicRef<T>& null();
	
		sl_bool isNull() const;
	
		sl_bool isNotNull() const;
	
		void setNull();
	
		template <class _T>
		static const AtomicRef<T>& from(const AtomicRef<_T>& other);

		template <class _T>
		static AtomicRef<T>& from(AtomicRef<_T>& other);

		template <class _T>
		static AtomicRef<T>&& from(AtomicRef<_T>&& other);
	
	public:
		AtomicRef<T>& operator=(sl_null_t);

		AtomicRef<T>& operator=(T* other);

		AtomicRef<T>& operator=(AtomicRef<T>&& other);

		AtomicRef<T>& operator=(const AtomicRef<T>& other);

		template <class _T>
		AtomicRef<T>& operator=(AtomicRef<_T>&& other);
	
		template <class _T>
		AtomicRef<T>& operator=(const AtomicRef<_T>& other);

		AtomicRef<T>& operator=(Ref<T>&& other);
	
		AtomicRef<T>& operator=(const Ref<T>& other);

		template <class _T>
		AtomicRef<T>& operator=(Ref<_T>&& other);
	
		template <class _T>
		AtomicRef<T>& operator=(const Ref<_T>& other);

		AtomicRef<T>& operator=(const WeakRef<T>& other);

		template <class _T>
		AtomicRef<T>& operator=(const WeakRef<_T>& other);

		AtomicRef<T>& operator=(const AtomicWeakRef<T>& other);

		template <class _T>
		AtomicRef<T>& operator=(const AtomicWeakRef<_T>& other);
	
	public:
		sl_bool operator==(sl_null_t) const;

		sl_bool operator==(T* other) const;

		sl_bool operator==(const AtomicRef<T>& other) const;

		template <class _T>
		sl_bool operator==(const AtomicRef<_T>& other) const;

		sl_bool operator==(const Ref<T>& other) const;

		template <class _T>
		sl_bool operator==(const Ref<_T>& other) const;

		sl_bool operator!=(sl_null_t) const;

		sl_bool operator!=(T* other) const;

		sl_bool operator!=(const AtomicRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const AtomicRef<_T>& other) const;

		sl_bool operator!=(const Ref<T>& other) const;

		template <class _T>
		sl_bool operator!=(const Ref<_T>& other) const;

	public:
		Ref<T> operator*() const;
	
	public:
		T* _retainObject() const;

		void _replaceObject(T* other);

		void _move_init(void* other);

		void _move_assign(void* other);

	public:
		T* _ptr;
	private:
		SpinLock m_lock;
	
	};

	
	template <class T>
	class SLIB_EXPORT WeakRef
	{
	public:
		SLIB_INLINE WeakRef() = default;

		SLIB_INLINE WeakRef(sl_null_t) {}

		WeakRef(T* other);
	
		WeakRef(WeakRef<T>&& other);

		WeakRef(const WeakRef<T>& other);

		template <class _T>
		WeakRef(WeakRef<_T>&& other);
	
		template <class _T>
		WeakRef(const WeakRef<_T>& other);

		WeakRef(AtomicWeakRef<T>&& other);
	
		WeakRef(const AtomicWeakRef<T>& other);

		template <class _T>
		WeakRef(AtomicWeakRef<_T>&& other);

		template <class _T>
		WeakRef(const AtomicWeakRef<_T>& other);

		WeakRef(const Ref<T>& other);
	
		template <class _T>
		WeakRef(const Ref<_T>& other);
	
		WeakRef(const AtomicRef<T>& other);

		template <class _T>
		WeakRef(const AtomicRef<_T>& other);

		~WeakRef();

	public:
		static const WeakRef<T>& null();

		sl_bool isNull() const;

		sl_bool isNotNull() const;

		void setNull();

		template <class _T>
		static const WeakRef<T>& from(const WeakRef<_T>& other);

		template <class _T>
		static WeakRef<T>& from(WeakRef<_T>& other);

		template <class _T>
		static WeakRef<T>&& from(WeakRef<_T>&& other);

		Ref<T> lock() const;

		static WeakRef<T> fromReferable(Referable* referable);

	public:
		WeakRef<T>& operator=(sl_null_t);

		WeakRef<T>& operator=(T* other);
	
		WeakRef<T>& operator=(WeakRef<T>&& other);

		WeakRef<T>& operator=(const WeakRef<T>& other);

		template <class _T>
		WeakRef<T>& operator=(WeakRef<_T>&& other);
	
		template <class _T>
		WeakRef<T>& operator=(const WeakRef<_T>& other);

		WeakRef<T>& operator=(AtomicWeakRef<T>&& other);

		WeakRef<T>& operator=(const AtomicWeakRef<T>& other);

		template <class _T>
		WeakRef<T>& operator=(AtomicWeakRef<_T>&& other);

		template <class _T>
		WeakRef<T>& operator=(const AtomicWeakRef<_T>& other);
	
		WeakRef<T>& operator=(const Ref<T>& other);
	
		template <class _T>
		WeakRef<T>& operator=(const Ref<_T>& other);
	
		WeakRef<T>& operator=(const AtomicRef<T>& other);

		template <class _T>
		WeakRef<T>& operator=(const AtomicRef<_T>& other);

	public:
		sl_bool operator==(const WeakRef<T>& other) const;

		template <class _T>
		sl_bool operator==(const WeakRef<_T>& other) const;

		sl_bool operator==(const AtomicWeakRef<T>& other) const;
	
		template <class _T>
		sl_bool operator==(const AtomicWeakRef<_T>& other) const;
	
		sl_bool operator!=(const WeakRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const WeakRef<_T>& other) const;

		sl_bool operator!=(const AtomicWeakRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const AtomicWeakRef<_T>& other) const;
	
	private:
		void _set(T* other);

	public:
		Ref<CWeakRef> _weak;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< WeakRef<T> >
	{
	public:
		SLIB_INLINE Atomic() = default;

		SLIB_INLINE Atomic(sl_null_t) {}

		Atomic(T* other);

		Atomic(AtomicWeakRef<T>&& other);

		Atomic(const AtomicWeakRef<T>& other);

		template <class _T>
		Atomic(AtomicWeakRef<_T>&& other);

		template <class _T>
		Atomic(const AtomicWeakRef<_T>& other);

		Atomic(WeakRef<T>&& other);

		Atomic(const WeakRef<T>& other);

		template <class _T>
		Atomic(WeakRef<_T>&& other);

		template <class _T>
		Atomic(const WeakRef<_T>& other);

		Atomic(const Ref<T>& other);

		template <class _T>
		Atomic(const Ref<_T>& other);

		Atomic(const AtomicRef<T>& other);

		template <class _T>
		Atomic(const AtomicRef<_T>& other);

		~Atomic();

	public:
		static const AtomicWeakRef<T>& null();

		sl_bool isNull() const;

		sl_bool isNotNull() const;

		void setNull();

		template <class _T>
		static const AtomicWeakRef<T>& from(const AtomicWeakRef<_T>& other);

		template <class _T>
		static AtomicWeakRef<T>& from(AtomicWeakRef<_T>& other);

		template <class _T>
		static AtomicWeakRef<T>&& from(AtomicWeakRef<_T>&& other);
	
		Ref<T> lock() const;
	
	public:
		AtomicWeakRef<T>& operator=(sl_null_t);

		AtomicWeakRef<T>& operator=(T* other);
	
		AtomicWeakRef<T>& operator=(AtomicWeakRef<T>&& other);

		AtomicWeakRef<T>& operator=(const AtomicWeakRef<T>& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(AtomicWeakRef<_T>&& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(const AtomicWeakRef<_T>& other);

		AtomicWeakRef<T>& operator=(WeakRef<T>&& other);

		AtomicWeakRef<T>& operator=(const WeakRef<T>& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(WeakRef<_T>&& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(const WeakRef<_T>& other);

		AtomicWeakRef<T>& operator=(const Ref<T>& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(const Ref<_T>& other);

		AtomicWeakRef<T>& operator=(const AtomicRef<T>& other);

		template <class _T>
		AtomicWeakRef<T>& operator=(const AtomicRef<_T>& other);

	public:
		sl_bool operator==(const AtomicWeakRef<T>& other) const;

		template <class _T>
		sl_bool operator==(const AtomicWeakRef<_T>& other) const;

		sl_bool operator==(const WeakRef<T>& other) const;

		template <class _T>
		sl_bool operator==(const WeakRef<_T>& other) const;

		sl_bool operator!=(const AtomicWeakRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const AtomicWeakRef<_T>& other) const;

		sl_bool operator!=(const WeakRef<T>& other) const;

		template <class _T>
		sl_bool operator!=(const WeakRef<_T>& other) const;

	public:
		WeakRef<T> operator*() const;
	
	private:
		void _set(T* other);

	public:
		AtomicRef<CWeakRef> _weak;
	
	};
	
	template <class T>
	struct PropertyTypeHelper< WeakRef<T> >
	{
		typedef Ref<T> const& ArgType;
		typedef Ref<T> RetType;
	};

	
	class SLIB_EXPORT CWeakRef : public Referable
	{
		SLIB_DECLARE_OBJECT
	public:
		CWeakRef();

		~CWeakRef();

	public:
		Referable* m_object;
		SpinLock m_lock;

	public:
		static CWeakRef* create(Referable* object);

	public:
		Ref<Referable> lock();

		void release();
	};
	
	class SLIB_EXPORT ReferableKeeper
	{
	public:
		ReferableKeeper(Referable* object);

		~ReferableKeeper();

	private:
		Referable* m_object;

	};
	
	template <class T, class... ARGS>
	Ref<T> New(ARGS&&... args);

	template <class T, class... ARGS>
	Ref<T> Init(ARGS&&... args);

	template <class T, class _T>
	sl_bool IsInstanceOf(_T* object);

	template <class T, class _T>
	sl_bool IsInstanceOf(const Ref<_T>& object);
	
	template <class T, class _T>
	T* CastInstance(_T* object);

	template <class T, class _T>
	const Ref<T>& CastRef(const Ref<_T>& object);

	template <class T, class _T>
	const Ref<T>& CastRef(const Ref<_T>& object, const Ref<T>& def);

}

#include "detail/ref.h"

#endif
