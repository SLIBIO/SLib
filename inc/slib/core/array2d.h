#ifndef CHECKHEADER_SLIB_CORE_ARRAY2D
#define CHECKHEADER_SLIB_CORE_ARRAY2D

#include "definition.h"

#include "ref.h"
#include "array.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT ArrayData2D
	{
	public:
		T* data;
		sl_size width;
		sl_size height;
		sl_reg stride;
		Ref<Referable> refer;

	public:
		T& element(sl_reg x, sl_reg y) const;

	};
	
	template <class T>
	class Array2D;
	
	template <class T>
	using AtomicArray2D = Atomic< Array2D<T> >;
	
	
	class SLIB_EXPORT CArray2DBase : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		CArray2DBase();

		~CArray2DBase();

	};
	
	template <class T>
	class SLIB_EXPORT CArray2D : public CArray2DBase
	{
	protected:
		T* m_data;
		sl_size m_width;
		sl_size m_height;
		sl_reg m_stride;
		sl_bool m_flagStatic;
		Ref<Referable> m_refer;

	protected:
		CArray2D();

	public:
		CArray2D(sl_size width, sl_size height);

		template <class _T>
		CArray2D(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0);

		CArray2D(const T* data, sl_size width, sl_size height, sl_size stride, Referable* refer);

		~CArray2D();

	public:
		static CArray2D<T>* create(sl_size width, sl_size height);

		template <class _T>
		static CArray2D<T>* create(sl_size width, sl_size height, const _T* data, sl_size strideSrc = 0);

		static CArray2D<T>* createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer);

	public:
		T* getData() const;

		sl_size getWidth() const;

		sl_size getHeight() const;

		sl_reg getStride() const;

		sl_bool isStatic() const;

		const Ref<Referable>& getRefer() const;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const;

		T getValueAt(sl_reg x, sl_reg y) const;

		T getValueAt(sl_reg x, sl_reg y, const T& def) const;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const;

	public:
		CArray2D<T>* sub(sl_size x, sl_size y, sl_size width, sl_size height);

		template <class _T>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;

		template <class _T>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;

		template <class _T>
		sl_size copy(sl_size xTarget, sl_size yTarget, const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;

		template <class _T>
		sl_size copy(const CArray2D<_T>* source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;

		CArray2D<T>* duplicate() const;

	};
	
	template <class T>
	class SLIB_EXPORT Array2D
	{
	public:
		Ref< CArray2D<T> > ref;
		SLIB_REF_WRAPPER(Array2D, CArray2D<T>)
	
	public:
		static Array2D<T> create(sl_size width, sl_size height);
	
		template <class _T>
		static Array2D<T> create(sl_size width, sl_size height, const _T* dataIn, sl_reg strideIn = 0);

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride = 0);

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, Referable* refer);

		static Array2D<T> createStatic(const T* data, sl_size width, sl_size height, sl_reg stride, Referable* refer);

	public:
		T* getData() const;

		sl_size getWidth() const;

		sl_size getHeight() const;

		sl_reg getStride() const;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const;

		T getValueAt(sl_reg x, sl_reg y) const;
	
		T getValueAt(sl_reg x, sl_reg y, const T& def) const;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const;

	public:
		Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const;

		template <class _T>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;

		template <class _T>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;

		template <class _T>
		sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;

		template <class _T>
		sl_size copy(const Array2D<_T>& source, sl_size xSource, sl_size ySource, sl_size width, sl_size height) const;

		Array2D<T> duplicate() const;

		sl_bool getData(ArrayData2D<T>& data) const;

	};
	
	template <class T>
	class SLIB_EXPORT Atomic< Array2D<T> >
	{
	public:
		AtomicRef< CArray2D<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CArray2D<T>)
	
	public:
		sl_size getWidth() const;

		sl_size getHeight() const;

		sl_bool isEmpty() const;

		sl_bool isNotEmpty() const;

		sl_bool getAt(sl_reg x, sl_reg y, T* _out = sl_null) const;

		T getValueAt(sl_reg x, sl_reg y) const;
	
		T getValueAt(sl_reg x, sl_reg y, const T& def) const;

		sl_bool setAt(sl_reg x, sl_reg y, const T& value) const;

	public:
		Array2D<T> sub(sl_size x, sl_size y, sl_size width, sl_size height) const;

		template <class _T>
		sl_size read(sl_size x, sl_size y, sl_size width, sl_size height, _T* dataDst, sl_reg strideDst = 0) const;

		template <class _T>
		sl_size write(sl_size x, sl_size y, sl_size width, sl_size height, const _T* dataSrc, sl_reg strideSrc = 0) const;

		template <class _T>
		sl_size copy(sl_size xTarget, sl_size yTarget, const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;
	
		template <class _T>
		sl_size copy(const Array2D<_T>& source, sl_size xSource = 0, sl_size ySource = 0, sl_size width = SLIB_SIZE_MAX, sl_size height = SLIB_SIZE_MAX) const;

		Array2D<T> duplicate() const;

		sl_bool getData(ArrayData2D<T>& data) const;

	};

}

#include "detail/array2d.h"

#endif
