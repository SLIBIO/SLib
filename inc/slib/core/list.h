#ifndef CHECKHEADER_SLIB_CORE_LIST
#define CHECKHEADER_SLIB_CORE_LIST

#include "definition.h"

#include "new.h"
#include "object.h"
#include "iterator.h"
#include "array.h"
#include "algorithm.h"

#define SLIB_LIST_CAPACITY_MIN 5

SLIB_NAMESPACE_BEGIN

template < class T, class COMPARE=Compare<T> >
class SLIB_EXPORT CList : public Object
{
	SLIB_DECLARE_OBJECT
protected:
	T* m_data;
	sl_size m_count;
	sl_size m_capacity;
	
public:
	CList();
	
	CList(sl_size count);
	
	CList(sl_size count, sl_size capacity);
	
	CList(const T* values, sl_size count);
	
	template <class _T>
	CList(const _T* values, sl_size count);
	
	~CList();
	
public:
	static CList<T, COMPARE>* create();
	
	static CList<T, COMPARE>* create(sl_size count);
	
	static CList<T, COMPARE>* create(sl_size count, sl_size capacity);
	
	static CList<T, COMPARE>* createFromElements(const T* values, sl_size count);
	
	template <class _T>
	static CList<T, COMPARE>* createFromElements(const _T* values, sl_size count);
	
	static CList<T, COMPARE>* createFromArray(const Array<T, COMPARE>& array);
	
	template <class _T,  class _COMPARE>
	static CList<T, COMPARE>* createFromArray(const Array<_T, _COMPARE>& array);
	
	static CList<T, COMPARE>* createFromElement(const T& value);
	
	static CList<T, COMPARE>* createCopy(CList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	static CList<T, COMPARE>* createCopy(CList<_T, _COMPARE>* other);
	
public:
	sl_size getCount() const;
	
	sl_size getCapacity() const;
	
	T* getData() const;
	
public:
	T* getItemPtr(sl_size index) const;
	
	sl_bool getItem_NoLock(sl_size index, T* _out = sl_null) const;
	
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue_NoLock(sl_size index, const T& def) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem_NoLock(sl_size index, const T& value) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	sl_bool setCount_NoLock(sl_size count);
	
	sl_bool setCount(sl_size count);
	
	sl_bool insert_NoLock(sl_size index, const T* values, sl_size count);
	
	template <class _T>
	sl_bool insert_NoLock(sl_size index, const _T* values, sl_size count);
	
	sl_bool insert(sl_size index, const T* values, sl_size count);
	
	template <class _T>
	sl_bool insert(sl_size index, const _T* values, sl_size count);
	
	sl_bool insertAll(sl_size index, const CList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	sl_bool insertAll(sl_size index, const CList<_T, _COMPARE>* other);
	
	sl_bool insert_NoLock(sl_size index, const T& value);
	
	sl_bool insert(sl_size index, const T& value);
	
	sl_bool add_NoLock(const T* values, sl_size count);
	
	template <class _T>
	sl_bool add_NoLock(const _T* values, sl_size count);
	
	sl_bool add(const T* values, sl_size count);
	
	template <class _T>
	sl_bool add(const _T* values, sl_size count);
	
	sl_bool addAll(const CList<T, COMPARE>* other);
	
	template <class _T, class _COMPARE>
	sl_bool addAll(const CList<_T, _COMPARE>* other);
	
	sl_bool add_NoLock(const T& value);
	
	sl_bool add(const T& value);
	
	template <class _T, class _COMPARE>
	sl_bool addIfNotExistT_NoLock(const _T& value);
	
	template <class _T, class _COMPARE>
	sl_bool addIfNotExistT(const _T& value);
	
	sl_bool addIfNotExist_NoLock(const T& value);
	
	sl_bool addIfNotExist(const T& value);
	
	sl_bool add_NoLock(const Iterator<T>& iterator);
	
	template <class _T>
	sl_bool add_NoLock(const Iterator<_T>& iterator);
	
	sl_bool add(const Iterator<T>& iterator);
	
	template <class _T>
	sl_bool add(const Iterator<_T>& iterator);
	
	sl_size remove_NoLock(sl_size index, sl_size count = 1);
	
	sl_size remove(sl_size index, sl_size count = 1);
	
	template <class _T, class _COMPARE>
	sl_size removeValueT_NoLock(const _T& value, sl_bool flagAllValues = sl_false);
	
	template <class _T, class _COMPARE>
	sl_size removeValueT(const _T& value, sl_bool flagAllValues = sl_false);

	sl_size removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false);
	
	sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false);
	
	sl_size removeAll_NoLock();
	
	sl_size removeAll();
	
	sl_bool popFront_NoLock(T* _out = sl_null);
	
	sl_bool popFront(T* _out = sl_null);
	
	sl_size popFront_NoLock(sl_size count);
	
	sl_size popFront(sl_size count);
	
	sl_bool popBack_NoLock(T* _out = sl_null);
	
	sl_bool popBack(T* _out = sl_null);
	
	sl_size popBack_NoLock(sl_size count);
	
	sl_size popBack(sl_size count);
	
	template <class _T, class _COMPARE>
	sl_reg indexOfT_NoLock(const _T& value, sl_reg start = 0) const;
	
	template <class _T, class _COMPARE>
	sl_reg indexOfT(const _T& value, sl_reg start = 0) const;
	
	sl_reg indexOf_NoLock(const T& value, sl_reg start = 0) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	template <class _T, class _COMPARE>
	sl_reg lastIndexOfT_NoLock(const _T& value, sl_reg start = -1) const;
	
	template <class _T, class _COMPARE>
	sl_reg lastIndexOfT(const _T& value, sl_reg start = -1) const;
	
	sl_reg lastIndexOf_NoLock(const T& value, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	template <class _T, class _COMPARE>
	sl_bool containsT_NoLock(const _T& value) const;
	
	template <class _T, class _COMPARE>
	sl_bool containsT(const _T& value) const;
	
	sl_bool contains_NoLock(const T& value) const;
	
	sl_bool contains(const T& value) const;
	
	CList<T, COMPARE>* duplicate_NoLock() const;
	
	CList<T, COMPARE>* duplicate() const;
	
	Array<T, COMPARE> toArray_NoLock() const;
	
	Array<T, COMPARE> toArray() const;
	
	template <class _COMPARE>
	void sortBy_NoLock(sl_bool flagAscending = sl_true) const;
	
	template <class _COMPARE>
	void sortBy(sl_bool flagAscending = sl_true) const;
	
	void sort_NoLock(sl_bool flagAscending = sl_true) const;
	
	void sort(sl_bool flagAscending = sl_true) const;
	
	Iterator<T> iterator() const;
	
protected:
	sl_bool _setCountInner(sl_size count);
	
};

template <class T, class COMPARE>
class SafeList;

#define SLIB_TEMPLATE_PARAMS_CList T, COMPARE
#define SLIB_TEMPLATE_DEF_PARAMS_CList class T, class COMPARE

/** auto-referencing object **/
template < class T, class COMPARE=Compare<T> >
class SLIB_EXPORT List
{
public:
	Ref< CList<T, COMPARE> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(List, SafeList, CList)
	
public:
	List(sl_size count);
	
	List(const T* data, sl_size count);
	
	template <class _T>
	List(const _T* data, sl_size count);
	
public:
	template <class _COMPARE>
	const List<T, COMPARE>& from(const List<T, _COMPARE>& other);
	
public:
	static List<T, COMPARE> create();
	
	static List<T, COMPARE> create(sl_size count);
	
	static List<T, COMPARE> create(sl_size count, sl_size capacity);
	
	static List<T, COMPARE> createFromElements(const T* values, sl_size count);
	
	template <class _T>
	static List<T, COMPARE> createFromElements(const _T* values, sl_size count);
	
	static List<T, COMPARE> createFromArray(const Array<T, COMPARE>& array);
	
	template <class _T,  class _COMPARE>
	static List<T, COMPARE> createFromArray(const Array<_T, _COMPARE>& array);
	
	static List<T, COMPARE> createFromElement(const T& e);
	
	static List<T, COMPARE> createCopy(const List<T, COMPARE>& other);
	
	template <class _T, class _COMPARE>
	static List<T, COMPARE> createCopy(const List<_T, _COMPARE>& other);
	
public:
	sl_size getCount() const;
	
	sl_size getCapacity() const;
	
	T* getData() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	T* getItemPtr(sl_size index) const;
	
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	sl_bool setCount_NoLock(sl_size count);
	
	sl_bool setCount(sl_size count);
	
	sl_bool insert_NoLock(sl_size index, const T* values, sl_size count) const;
	
	template <class _T>
	sl_bool insert_NoLock(sl_size index, const _T* values, sl_size count) const;
	
	sl_bool insert(sl_size index, const T* values, sl_size count) const;
	
	template <class _T>
	sl_bool insert(sl_size index, const _T* values, sl_size count) const;
	
	sl_bool insertAll(sl_size index, const List<T, COMPARE>& other) const;
	
	template <class _T, class _COMPARE>
	sl_bool insertAll(sl_size index, const List<_T, _COMPARE>& other) const;
	
	sl_bool insertAll(sl_size index, const SafeList<T, COMPARE>& other) const;
	
	template <class _T, class _COMPARE>
	sl_bool insertAll(sl_size index, const SafeList<_T, _COMPARE>& other) const;
	
	sl_bool insert_NoLock(sl_size index, const T& value) const;
	
	sl_bool insert(sl_size index, const T& value) const;
	
	sl_bool add_NoLock(const T* values, sl_size count);
	
	template <class _T>
	sl_bool add_NoLock(const _T* values, sl_size count);
	
	sl_bool add(const T* values, sl_size count);
	
	template <class _T>
	sl_bool add(const _T* values, sl_size count);
	
	sl_bool addAll(const List<T, COMPARE>& _other);
	
	template <class _T, class _COMPARE>
	sl_bool addAll(const List<_T, _COMPARE>& _other);
	
	sl_bool addAll(const SafeList<T, COMPARE>& _other);
	
	template <class _T, class _COMPARE>
	sl_bool addAll(const SafeList<_T, _COMPARE>& _other);
	
	sl_bool add_NoLock(const T& value);
	
	sl_bool add(const T& value);
	
	template <class _T, class _COMPARE>
	sl_bool addIfNotExistT_NoLock(const _T& value);
	
	template <class _T, class _COMPARE>
	sl_bool addIfNotExistT(const _T& value);
	
	sl_bool addIfNotExist_NoLock(const T& value);
	
	sl_bool addIfNotExist(const T& value);
	
	sl_bool add_NoLock(const Iterator<T>& iterator);
	
	template <class _T>
	sl_bool add_NoLock(const Iterator<_T>& iterator);
	
	sl_bool add(const Iterator<T>& iterator);
	
	template <class _T>
	sl_bool add(const Iterator<_T>& iterator);
	
	sl_size remove_NoLock(sl_size index, sl_size count = 1) const;
	
	sl_size remove(sl_size index, sl_size count = 1) const;
	
	template <class _T, class _COMPARE>
	sl_size removeValueT_NoLock(const _T& value, sl_bool flagAllValues = sl_false) const;
	
	template <class _T, class _COMPARE>
	sl_size removeValueT(const _T& value, sl_bool flagAllValues = sl_false) const;
	
	sl_size removeValue_NoLock(const T& value, sl_bool flagAllValues = sl_false) const;
	
	sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false) const;
	
	sl_size removeAll_NoLock() const;
	
	sl_size removeAll() const;
	
	sl_bool popFront_NoLock(T* _out = sl_null) const;
	
	sl_bool popFront(T* _out = sl_null) const;
	
	sl_size popFront_NoLock(sl_size count) const;
	
	sl_size popFront(sl_size count) const;
	
	sl_bool popBack_NoLock(T* _out = sl_null) const;
	
	sl_bool popBack(T* _out = sl_null) const;
	
	sl_size popBack_NoLock(sl_size count) const;
	
	sl_size popBack(sl_size count) const;
	
	template <class _T, class _COMPARE>
	sl_reg indexOfT_NoLock(const _T& value, sl_reg start = 0) const;
	
	template <class _T, class _COMPARE>
	sl_reg indexOfT(const _T& value, sl_reg start = 0) const;
	
	sl_reg indexOf_NoLock(const T& value, sl_reg start = 0) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	template <class _T, class _COMPARE>
	sl_reg lastIndexOfT_NoLock(const _T& value, sl_reg start = -1) const;
	
	template <class _T, class _COMPARE>
	sl_reg lastIndexOfT(const _T& value, sl_reg start = -1) const;
	
	sl_reg lastIndexOf_NoLock(const T& value, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	template <class _T, class _COMPARE>
	sl_bool containsT_NoLock(const _T& value) const;
	
	template <class _T, class _COMPARE>
	sl_bool containsT(const _T& value) const;
	
	sl_bool contains_NoLock(const T& value) const;
	
	sl_bool contains(const T& value) const;
	
	List<T, COMPARE> duplicate_NoLock() const;
	
	List<T, COMPARE> duplicate() const;
	
	Array<T, COMPARE> toArray_NoLock() const;
	
	Array<T, COMPARE> toArray() const;
	
	template <class _COMPARE>
	void sortBy(sl_bool flagAscending = sl_true) const;
	
	template <class _COMPARE>
	void sortBy_NoLock(sl_bool flagAscending = sl_true) const;
	
	void sort_NoLock(sl_bool flagAscending = sl_true) const;
	
	void sort(sl_bool flagAscending = sl_true) const;
	
	Iterator<T> iterator() const;
	
	const Mutex* getLocker() const;
	
};


/** auto-referencing object **/
template < class T, class COMPARE = Compare<T> >
class SLIB_EXPORT SafeList
{
public:
	SafeRef< CList<T, COMPARE> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(SafeList, List, CList)
	
public:
	SafeList(sl_size count);
	
	SafeList(const T* data, sl_size count);
	
	template <class _T>
	SafeList(const _T* data, sl_size count);
	
public:
	template <class _COMPARE>
	const SafeList<T, COMPARE>& from(const SafeList<T, _COMPARE>& other);
	
public:
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
public:
	sl_bool getItem(sl_size index, T* _out = sl_null) const;
	
	T getItemValue(sl_size index, const T& def) const;
	
	sl_bool setItem(sl_size index, const T& value) const;
	
public:
	sl_bool setCount(sl_size count);
	
	sl_bool insert(sl_size index, const T* values, sl_size count) const;
	
	template <class _T>
	sl_bool insert(sl_size index, const _T* values, sl_size count) const;
	
	sl_bool insertAll(sl_size index, const List<T, COMPARE>& other) const;
	
	template <class _T, class _COMPARE>
	sl_bool insertAll(sl_size index, const List<_T, _COMPARE>& other) const;
	
	sl_bool insertAll(sl_size index, const SafeList<T, COMPARE>& other) const;
	
	template <class _T, class _COMPARE>
	sl_bool insertAll(sl_size index, const SafeList<_T, _COMPARE>& other) const;
	
	sl_bool insert(sl_size index, const T& value) const;
	
	sl_bool add(const T* values, sl_size count);
	
	template <class _T>
	sl_bool add(const _T* values, sl_size count);
	
	sl_bool addAll(const List<T, COMPARE>& _other);
	
	template <class _T, class _COMPARE>
	sl_bool addAll(const List<_T, _COMPARE>& _other);
	
	sl_bool addAll(const SafeList<T, COMPARE>& _other);
	
	template <class _T, class _COMPARE>
	sl_bool addAll(const SafeList<_T, _COMPARE>& _other);
	
	sl_bool add(const T& value);
	
	template <class _T, class _COMPARE>
	sl_bool addIfNotExistT(const _T& value);
	
	sl_bool addIfNotExist(const T& value);
	
	sl_bool add(const Iterator<T>& iterator);
	
	template <class _T>
	sl_bool add(const Iterator<_T>& iterator);
	
	sl_size remove(sl_size index, sl_size count = 1) const;
	
	template <class _T, class _COMPARE>
	sl_size removeValueT(const _T& value, sl_bool flagAllValues = sl_false) const;
	
	sl_size removeValue(const T& value, sl_bool flagAllValues = sl_false) const;
	
	sl_size removeAll() const;
	
	sl_bool popFront(T* _out = sl_null) const;
	
	sl_size popFront(sl_size count) const;
	
	sl_bool popBack(T* _out = sl_null) const;
	
	sl_size popBack(sl_size count) const;

	template <class _T, class _COMPARE>
	sl_reg indexOfT(const _T& value, sl_reg start = 0) const;
	
	sl_reg indexOf(const T& value, sl_reg start = 0) const;
	
	template <class _T, class _COMPARE>
	sl_reg lastIndexOfT(const _T& value, sl_reg start = -1) const;
	
	sl_reg lastIndexOf(const T& value, sl_reg start = -1) const;
	
	template <class _T, class _COMPARE>
	sl_bool containsT(const _T& value) const;
	
	sl_bool contains(const T& value) const;
	
	List<T, COMPARE> duplicate() const;
	
	Array<T, COMPARE> toArray() const;
	
	template <class _COMPARE>
	void sortBy(sl_bool flagAscending = sl_true) const;
	
	void sort(sl_bool flagAscending = sl_true) const;
	
};


template <class T>
class SLIB_EXPORT ListLocker : public ObjectLocker
{
public:
	T* data;
	sl_size count;
	
private:
	List<T> m_list;
	
public:
	ListLocker(const List<T>& list);
	
	template <class _COMPARE>
	ListLocker(const List<T, _COMPARE>& list);
	
	ListLocker(const SafeList<T>& list);
	
	template <class _COMPARE>
	ListLocker(const SafeList<T, _COMPARE>& list);
	
	ListLocker(const CList<T>& list);
	
	template <class _COMPARE>
	ListLocker(const CList<T, _COMPARE>& list);
	
	~ListLocker();
	
public:
	T& operator[](sl_reg index);
	
};


template <class T>
class SLIB_EXPORT ListItems
{
public:
	T* data;
	sl_size count;
	
private:
	List<T> m_list;
	
public:
	ListItems(const List<T>& list);
	
	template <class _COMPARE>
	ListItems(const List<T, _COMPARE>& list);
	
	ListItems(const SafeList<T>& list);
	
	template <class _COMPARE>
	ListItems(const SafeList<T, _COMPARE>& list);
	
	ListItems(const CList<T>& list);
	
	template <class _COMPARE>
	ListItems(const CList<T, _COMPARE>& list);
	
public:
	T& operator[](sl_reg index);
	
};


template <class T, class COMPARE = Compare<T> >
class SLIB_EXPORT ListIterator : public IIterator<T>
{
protected:
	const CList<T, COMPARE>* m_list;
	Ref<Referable> m_refer;
	sl_size m_index;
	
public:
	ListIterator(const CList<T, COMPARE>* list, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(T* _out);
	
	// override
	sl_reg getIndex();
	
};


#define SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(...) \
	extern template class CList<__VA_ARGS__>; \
	extern template class List<__VA_ARGS__>; \
	extern template class SafeList<__VA_ARGS__>; \
	extern template class ListLocker<__VA_ARGS__>; \
	extern template class ListItems<__VA_ARGS__>;

#define SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(...) \
	template class CList<__VA_ARGS__>; \
	template class List<__VA_ARGS__>; \
	template class SafeList<__VA_ARGS__>; \
	template class ListLocker<__VA_ARGS__>; \
	template class ListItems<__VA_ARGS__>;

SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Ref<Referable>)


SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

extern const char _List_ClassID[];

SLIB_DEFINE_TEMPLATE_OBJECT(CList, Object, _List_ClassID)

template <class T, class COMPARE>
CList<T, COMPARE>::CList()
{
	m_data = 0;
	m_count = 0;
	m_capacity = 0;
}

template <class T, class COMPARE>
CList<T, COMPARE>::CList(sl_size count)
{
	if (count > 0) {
		T* data = New<T>::create(count);
		if (data) {
			m_data = data;
			m_count = count;
			m_capacity = count;
			return;
		}
	}
	m_data = 0;
	m_count = 0;
	m_capacity = 0;
}

template <class T, class COMPARE>
CList<T, COMPARE>::CList(sl_size count, sl_size capacity)
{
	if (capacity < count) {
		capacity = count;
	}
	if (capacity > 0) {
		T* data = New<T>::create(capacity);
		if (data) {
			m_data = data;
			m_count = count;
			m_capacity = capacity;
			return;
		}
	}
	m_data = 0;
	m_count = 0;
	m_capacity = 0;
}

template <class T, class COMPARE>
CList<T, COMPARE>::CList(const T* values, sl_size count)
{
	if (count > 0) {
		T* data = New<T>::create(values, count);
		if (data) {
			m_data = data;
			m_count = count;
			m_capacity = count;
			return;
		}
	}
	m_data = 0;
	m_count = 0;
	m_capacity = 0;
}

template <class T, class COMPARE>
template <class _T>
CList<T, COMPARE>::CList(const _T* values, sl_size count)
{
	if (count > 0) {
		T* data = New<T>::create(values, count);
		if (data) {
			m_data = data;
			m_count = count;
			m_capacity = count;
			return;
		}
	}
	m_data = 0;
	m_count = 0;
	m_capacity = 0;
}

template <class T, class COMPARE>
CList<T, COMPARE>::~CList()
{
	New<T>::free(m_data, m_count);
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::create()
{
	return new CList<T, COMPARE>;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::create(sl_size count)
{
	if (count > 0) {
		CList<T, COMPARE>* ret = new CList<T, COMPARE>(count);
		if (ret) {
			if (ret->m_count > 0) {
				return ret;
			}
			delete ret;
		}
	} else {
		return new CList<T, COMPARE>;
	}
	return sl_null;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::create(sl_size count, sl_size capacity)
{
	if (count > 0 || capacity > 0) {
		CList<T, COMPARE>* ret = new CList<T, COMPARE>(count, capacity);
		if (ret) {
			if (ret->m_capacity > 0) {
				return ret;
			}
			delete ret;
		}
	} else {
		return new CList<T, COMPARE>;
	}
	return sl_null;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createFromElements(const T* values, sl_size count)
{
	if (count > 0) {
		CList<T, COMPARE>* ret = new CList<T, COMPARE>(values, count);
		if (ret) {
			if (ret->m_count > 0) {
				return ret;
			}
			delete ret;
		}
	} else {
		return new CList<T, COMPARE>;
	}
	return sl_null;
}

template <class T, class COMPARE>
template <class _T>
CList<T, COMPARE>* CList<T, COMPARE>::createFromElements(const _T* values, sl_size count)
{
	if (count > 0) {
		CList<T, COMPARE>* ret = new CList<T, COMPARE>(values, count);
		if (ret) {
			if (ret->m_count > 0) {
				return ret;
			}
			delete ret;
		}
	} else {
		return new CList<T, COMPARE>;
	}
	return sl_null;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createFromArray(const Array<T, COMPARE>& array)
{
	return createFromElements(array.getData(), array.getCount());
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createFromArray(const Array<_T, _COMPARE>& array)
{
	return createFromElements(array.getData(), array.getCount());
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createFromElement(const T& value)
{
	CList<T, COMPARE>* ret = create(0, 1);
	if (ret) {
		if (ret->add_NoLock(value)) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createCopy(CList<T, COMPARE>* other)
{
	if (other) {
		return createFromElements(other->getData(), other->getCount());
	}
	return sl_null;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::createCopy(CList<_T, _COMPARE>* other)
{
	if (other) {
		return createFromElements(other->getData(), other->getCount());
	}
	return sl_null;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size CList<T, COMPARE>::getCount() const
{
	return m_count;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size CList<T, COMPARE>::getCapacity() const
{
	return m_capacity;
}

template <class T, class COMPARE>
SLIB_INLINE T* CList<T, COMPARE>::getData() const
{
	return m_data;
}

template <class T, class COMPARE>
T* CList<T, COMPARE>::getItemPtr(sl_size index) const
{
	if (index < m_count) {
		return m_data + index;
	}
	return sl_null;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::getItem_NoLock(sl_size index, T* _out) const
{
	if (index < m_count) {
		if (_out) {
			*_out = m_data[index];
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	ObjectLocker lock(this);
	return getItem_NoLock(index, _out);
}

template <class T, class COMPARE>
T CList<T, COMPARE>::getItemValue_NoLock(sl_size index, const T& def) const
{
	if (index < m_count) {
		return m_data[index];
	}
	return def;
}

template <class T, class COMPARE>
T CList<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	ObjectLocker lock(this);
	return getItemValue_NoLock(index, def);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::setItem_NoLock(sl_size index, const T& value) const
{
	if (index < m_count) {
		m_data[index] = value;
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	ObjectLocker lock(this);
	return setItem_NoLock(index, value);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::setCount_NoLock(sl_size count)
{
	return _setCountInner(count);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::setCount(sl_size count)
{
	ObjectLocker lock(this);
	return setCount_NoLock(count);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::insert_NoLock(sl_size index, const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		if (index < oldCount) {
			{
				T* dst = m_data + m_count - 1;
				const T* src = m_data + oldCount - 1;
				sl_size n = oldCount - index;
				while (n) {
					*(dst--) = *(src--);
					n--;
				}
			}
			{
				T* dst = m_data + index;
				const T* src = values;
				sl_size n = count;
				while (n) {
					*(dst++) = *(src++);
					n--;
				}
			}
		} else {
			T* dst = m_data + oldCount;
			const T* src = values;
			sl_size n = count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::insert_NoLock(sl_size index, const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		if (index < oldCount) {
			{
				T* dst = m_data + m_count - 1;
				const T* src = m_data + oldCount - 1;
				sl_size n = oldCount - index;
				while (n) {
					*(dst--) = *(src--);
					n--;
				}
			}
			{
				T* dst = m_data + index;
				const _T* src = values;
				sl_size n = count;
				while (n) {
					*(dst++) = *(src++);
					n--;
				}
			}
		} else {
			T* dst = m_data + oldCount;
			const _T* src = values;
			sl_size n = count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::insert(sl_size index, const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	return insert_NoLock(index, values, count);
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::insert(sl_size index, const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	return insert_NoLock(index, values, count);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::insertAll(sl_size index, const CList<T, COMPARE>* other)
{
	if (!other) {
		return sl_true;
	}
	if (this == other) {
		return sl_false;
	}
	ObjectLocker lock(this, other);
	sl_size count = other->getCount();
	if (count == 0) {
		return sl_true;
	}
	T* otherData = other->getData();
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		if (index < oldCount) {
			T* dst = m_data + m_count - 1;
			T* src = m_data + oldCount - 1;
			sl_size n = oldCount - index;
			while (n) {
				*(dst--) = *(src--);
				n--;
			}
			dst = m_data + index;
			for (sl_size i = 0; i < count; i++) {
				*(dst++) = otherData[i];
			}
		} else {
			T* dst = m_data + oldCount;
			const T* src = otherData;
			sl_size n = count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::insertAll(sl_size index, const CList<_T, _COMPARE>* other)
{
	if (!other) {
		return sl_true;
	}
	if (this == other) {
		return sl_false;
	}
	ObjectLocker lock(this, other);
	sl_size count = other->getCount();
	if (count == 0) {
		return sl_true;
	}
	_T* otherData = other->getData();
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		if (index < oldCount) {
			T* dst = m_data + m_count - 1;
			T* src = m_data + oldCount - 1;
			sl_size n = oldCount - index;
			while (n) {
				*(dst--) = *(src--);
				n--;
			}
			dst = m_data + index;
			for (sl_size i = 0; i < count; i++) {
				*(dst++) = otherData[i];
			}
		} else {
			T* dst = m_data + oldCount;
			const _T* src = otherData;
			sl_size n = count;
			while (n) {
				*(dst++) = *(src++);
				n--;
			}
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::insert_NoLock(sl_size index, const T& value)
{
	return insert_NoLock(index, &value, 1);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::insert(sl_size index, const T& value)
{
	return insert(index, &value, 1);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add_NoLock(const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		T* dst = m_data + oldCount;
		const T* src = values;
		sl_size n = count;
		while (n) {
			*(dst++) = *(src++);
			n--;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::add_NoLock(const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		T* dst = m_data + oldCount;
		const _T* src = values;
		sl_size n = count;
		while (n) {
			*(dst++) = *(src++);
			n--;
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add(const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	return add_NoLock(values, count);
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::add(const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	ObjectLocker lock(this);
	return add_NoLock(values, count);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::addAll(const CList<T, COMPARE>* other)
{
	if (!other) {
		return sl_true;
	}
	if (this == other) {
		return sl_false;
	}
	ObjectLocker lock(this, other);
	sl_size count = other->getCount();
	if (count == 0) {
		return sl_true;
	}
	T* otherData = other->getData();
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		T* dst = m_data + oldCount;
		for (sl_size i = 0; i < count; i++) {
			*(dst++) = otherData[i];
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::addAll(const CList<_T, _COMPARE>* other)
{
	if (!other) {
		return sl_true;
	}
	if (this == other) {
		return sl_false;
	}
	ObjectLocker lock(this, other);
	sl_size count = other->getCount();
	if (count == 0) {
		return sl_true;
	}
	_T* otherData = other->getData();
	sl_size oldCount = m_count;
	if (_setCountInner(oldCount + count)) {
		T* dst = m_data + oldCount;
		for (sl_size i = 0; i < count; i++) {
			*(dst++) = otherData[i];
		}
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add_NoLock(const T& value)
{
	sl_size oldCount = m_count;
	if (oldCount < m_capacity) {
		m_count++;
		new (m_data + oldCount) T(value);
	} else {
		if (!(_setCountInner(oldCount + 1))) {
			return sl_false;
		}
		m_data[oldCount] = value;
	}
	return sl_true;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add(const T& value)
{
	ObjectLocker lock(this);
	return add_NoLock(value);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::addIfNotExistT_NoLock(const _T& value)
{
	if (indexOfT_NoLock<_T, _COMPARE>(value) < 0) {
		return add_NoLock(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::addIfNotExistT(const _T& value)
{
	ObjectLocker lock(this);
	return addIfNotExistT_NoLock<_T, _COMPARE>(value);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::addIfNotExist_NoLock(const T& value)
{
	if (indexOf_NoLock(value) < 0) {
		return add_NoLock(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::addIfNotExist(const T& value)
{
	ObjectLocker lock(this);
	return addIfNotExist_NoLock(value);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add_NoLock(const Iterator<T>& iterator)
{
	T value;
	while (iterator.next(&value)) {
		if (!(add_NoLock(value))) {
			return sl_false;
		}
	}
	return sl_true;
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::add_NoLock(const Iterator<_T>& iterator)
{
	_T value;
	while (iterator.next(&value)) {
		if (!(add_NoLock(value))) {
			return sl_false;
		}
	}
	return sl_true;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::add(const Iterator<T>& iterator)
{
	ObjectLocker lock(this);
	return add_NoLock(iterator);
}

template <class T, class COMPARE>
template <class _T>
sl_bool CList<T, COMPARE>::add(const Iterator<_T>& iterator)
{
	ObjectLocker lock(this);
	return add_NoLock(iterator);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::remove_NoLock(sl_size index, sl_size count)
{
	if (index < m_count && count > 0) {
		if (count > m_count - index) {
			count = m_count - index;
		}
		T* dst = m_data + index;
		T* src = dst + count;
		sl_size n = m_count - index - count;
		while (n) {
			*(dst++) = *(src++);
			n--;
		}
		_setCountInner(m_count - count);
		return count;
	}
	return 0;
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::remove(sl_size index, sl_size count)
{
	ObjectLocker lock(this);
	return remove_NoLock(index, count);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size CList<T, COMPARE>::removeValueT_NoLock(const _T& value, sl_bool flagAllValues)
{
	if (m_data) {
		sl_size n = m_count;
		for (sl_size i = 0; i < n;) {
			if (_COMPARE::equals(m_data[i], value)) {
				for (sl_size j = i + 1; j < n; j++) {
					m_data[j - 1] = m_data[j];
				}
				n--;
				if (!flagAllValues) {
					break;
				}
			} else {
				i++;
			}
		}
		if (n < m_count) {
			_setCountInner(n);
			return m_count - n;
		}
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size CList<T, COMPARE>::removeValueT(const _T& value, sl_bool flagAllValues)
{
	ObjectLocker lock(this);
	return removeValueT_NoLock<_T, _COMPARE>(value, flagAllValues);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::removeValue_NoLock(const T& value, sl_bool flagAllValues)
{
	return removeValueT<T, COMPARE>(value, flagAllValues);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::removeValue(const T& value, sl_bool flagAllValues)
{
	ObjectLocker lock(this);
	return removeValueT_NoLock<T, COMPARE>(value, flagAllValues);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::removeAll_NoLock()
{
	sl_size count = m_count;
	_setCountInner(0);
	return count;
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::removeAll()
{
	ObjectLocker lock(this);
	return removeAll_NoLock();
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::popFront_NoLock(T* _out)
{
	sl_size n = m_count;
	if (n > 0) {
		T* dst = m_data;
		T* src = dst + 1;
		if (_out) {
			*_out = *dst;
		}
		sl_size m = n - 1;
		while (m) {
			*(dst++) = *(src++);
			m--;
		}
		return _setCountInner(n - 1);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::popFront(T* _out)
{
	ObjectLocker lock(this);
	return popFront_NoLock(_out);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::popFront_NoLock(sl_size count)
{
	sl_size n = m_count;
	if (n > 0 && count > 0) {
		if (count > n) {
			count = n;
		}
		T* dst = m_data;
		T* src = dst + count;
		sl_size m = n - count;
		while (m) {
			*(dst++) = *(src++);
			m--;
		}
		if (_setCountInner(n - count)) {
			return count;
		}
	}
	return 0;
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::popFront(sl_size count)
{
	ObjectLocker lock(this);
	return popFront_NoLock(count);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::popBack_NoLock(T* _out)
{
	sl_size n = m_count;
	if (n > 0) {
		if (_out) {
			*_out = m_data[n - 1];
		}
		return _setCountInner(n - 1);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::popBack(T* _out)
{
	ObjectLocker lock(this);
	return popBack_NoLock(_out);
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::popBack_NoLock(sl_size count)
{
	sl_size n = m_count;
	if (n > 0 && count > 0) {
		if (count > n) {
			count = n;
		}
		if (_setCountInner(n - count)) {
			return count;
		}
	}
	return 0;
}

template <class T, class COMPARE>
sl_size CList<T, COMPARE>::popBack(sl_size count)
{
	ObjectLocker lock(this);
	return popBack_NoLock(count);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg CList<T, COMPARE>::indexOfT_NoLock(const _T& value, sl_reg start) const
{
	sl_reg ret = -1;
	if (start < 0) {
		start = 0;
	}
	for (sl_size i = start; i < m_count; i++) {
		if (_COMPARE::equals(m_data[i], value)) {
			ret = i;
			break;
		}
	}
	return ret;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg CList<T, COMPARE>::indexOfT(const _T& value, sl_reg start) const
{
	ObjectLocker lock(this);
	return indexOfT_NoLock<_T, _COMPARE>(value, start);
}

template <class T, class COMPARE>
sl_reg CList<T, COMPARE>::indexOf_NoLock(const T& value, sl_reg start) const
{
	return indexOfT_NoLock<T, COMPARE>(value, start);
}

template <class T, class COMPARE>
sl_reg CList<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	ObjectLocker lock(this);
	return indexOfT_NoLock<T, COMPARE>(value, start);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg CList<T, COMPARE>::lastIndexOfT_NoLock(const _T& value, sl_reg start) const
{
	sl_reg ret = -1;
	if (start < 0 || start >= (sl_reg)m_count) {
		start = m_count - 1;
	}
	for (sl_reg i = start; i >= 0; i--) {
		if (_COMPARE::equals(m_data[i], value)) {
			ret = i;
			break;
		}
	}
	return ret;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg CList<T, COMPARE>::lastIndexOfT(const _T& value, sl_reg start) const
{
	ObjectLocker lock(this);
	return lastIndexOfT_NoLock<_T, _COMPARE>(value, start);
}

template <class T, class COMPARE>
sl_reg CList<T, COMPARE>::lastIndexOf_NoLock(const T& value, sl_reg start) const
{
	return lastIndexOfT_NoLock<T, COMPARE>(value, start);
}

template <class T, class COMPARE>
sl_reg CList<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	ObjectLocker lock(this);
	return lastIndexOfT_NoLock<T, COMPARE>(value, start);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::containsT_NoLock(const _T& value) const
{
	return indexOfT_NoLock<_T, _COMPARE>(value) >= 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool CList<T, COMPARE>::containsT(const _T& value) const
{
	return indexOfT<_T, _COMPARE>(value) >= 0;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::contains_NoLock(const T& value) const
{
	return indexOfT_NoLock<T, COMPARE>(value) >= 0;
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::contains(const T& value) const
{
	return indexOfT<T, COMPARE>(value) >= 0;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::duplicate_NoLock() const
{
	if (m_count > 0) {
		return createFromElements(m_data, m_count);
	}
	return sl_null;
}

template <class T, class COMPARE>
CList<T, COMPARE>* CList<T, COMPARE>::duplicate() const
{
	ObjectLocker lock(this);
	return duplicate_NoLock();
}

template <class T, class COMPARE>
Array<T, COMPARE> CList<T, COMPARE>::toArray_NoLock() const
{
	return Array<T, COMPARE>::create(m_data, m_count);
}

template <class T, class COMPARE>
Array<T, COMPARE> CList<T, COMPARE>::toArray() const
{
	ObjectLocker lock(this);
	return toArray_NoLock();
}

template <class T, class COMPARE>
template <class _COMPARE>
void CList<T, COMPARE>::sortBy_NoLock(sl_bool flagAscending) const
{
	QuickSort<T, _COMPARE>::sort(m_data, m_count, flagAscending);
}

template <class T, class COMPARE>
template <class _COMPARE>
void CList<T, COMPARE>::sortBy(sl_bool flagAscending) const
{
	ObjectLocker lock(this);
	QuickSort<T, _COMPARE>::sort(m_data, m_count, flagAscending);
}

template <class T, class COMPARE>
void CList<T, COMPARE>::sort_NoLock(sl_bool flagAscending) const
{
	QuickSort<T, COMPARE>::sort(m_data, m_count, flagAscending);
}

template <class T, class COMPARE>
void CList<T, COMPARE>::sort(sl_bool flagAscending) const
{
	ObjectLocker lock(this);
	QuickSort<T, COMPARE>::sort(m_data, m_count, flagAscending);
}

template <class T, class COMPARE>
SLIB_INLINE Iterator<T> CList<T, COMPARE>::iterator() const
{
	return new ListIterator<T, COMPARE>(this, sl_null);
}

template <class T, class COMPARE>
sl_bool CList<T, COMPARE>::_setCountInner(sl_size count)
{
	sl_size oldCount = m_count;
	if (oldCount == count) {
		return sl_true;
	}
	if (count < oldCount) {
		New<T>::destructor(m_data + count, oldCount - count);
		m_count = count;
	}
	if (m_capacity < count) {
		sl_size newCapacity = m_capacity * 3 / 2 + 1;
		if (newCapacity < count) {
			newCapacity = count;
		}
		if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
			newCapacity = SLIB_LIST_CAPACITY_MIN;
		}
		T* newData;
		sl_size size = newCapacity * sizeof(T);
		if (m_data) {
			newData = (T*)(Base::reallocMemory(m_data, size));
			if (!newData) {
				newData = (T*)(Base::createMemory(size));
				if (newData) {
					Base::copyMemory(newData, m_data, m_capacity * sizeof(T));
					Base::freeMemory(m_data);
				}
			}
		} else {
			newData = (T*)(Base::createMemory(size));
		}
		if (newData) {
			m_data = newData;
			m_capacity = newCapacity;
		} else {
			return sl_false;
		}
	} else if (m_capacity > SLIB_LIST_CAPACITY_MIN && count < m_capacity / 2) {
		sl_size newCapacity = count * 3 / 2 + 1;
		if (newCapacity < SLIB_LIST_CAPACITY_MIN) {
			newCapacity = SLIB_LIST_CAPACITY_MIN;
		}
		T* newData = (T*)(Base::reallocMemory(m_data, newCapacity * sizeof(T)));
		if (newData) {
			m_data = newData;
			m_capacity = newCapacity;
		}
	}
	if (count > oldCount) {
		New<T>::constructor(m_data + oldCount, count - oldCount);
		m_count = count;
	}
	return sl_true;
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(List, SafeList, CList, ref)

template <class T, class COMPARE>
List<T, COMPARE>::List(sl_size count) : ref(CList<T, COMPARE>::create(count))
{
}

template <class T, class COMPARE>
List<T, COMPARE>::List(const T* data, sl_size count) : ref(CList<T, COMPARE>::createFromElements(data, count))
{
}

template <class T, class COMPARE>
template <class _T>
List<T, COMPARE>::List(const _T* data, sl_size count) : ref(CList<T, COMPARE>::createFromElements(data, count))
{
}

template <class T, class COMPARE>
template <class _COMPARE>
SLIB_INLINE const List<T, COMPARE>& List<T, COMPARE>::from(const List<T, _COMPARE>& other)
{
	return *((List<T, COMPARE>*)((void*)&other));
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::create()
{
	return CList<T, COMPARE>::create();
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::create(sl_size count)
{
	return CList<T, COMPARE>::create(count);
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::create(sl_size count, sl_size capacity)
{
	return CList<T, COMPARE>::create(count, capacity);
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::createFromElements(const T* values, sl_size count)
{
	return CList<T, COMPARE>::createFromElements(values, count);
}

template <class T, class COMPARE>
template <class _T>
List<T, COMPARE> List<T, COMPARE>::createFromElements(const _T* values, sl_size count)
{
	return CList<T, COMPARE>::createFromElements(values, count);
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::createFromArray(const Array<T, COMPARE>& array)
{
	return createFromElements(array.getData(), array.getCount());
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
List<T, COMPARE> List<T, COMPARE>::createFromArray(const Array<_T, _COMPARE>& array)
{
	return createFromElements(array.getData(), array.getCount());
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::createFromElement(const T& e)
{
	return CList<T, COMPARE>::createFromElement(e);
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::createCopy(const List<T, COMPARE>& other)
{
	return CList<T, COMPARE>::createCopy(other.ref.ptr);
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
List<T, COMPARE> List<T, COMPARE>::createCopy(const List<_T, _COMPARE>& other)
{
	return CList<T, COMPARE>::createCopy(other.ref.ptr);
}

template <class T, class COMPARE>
SLIB_INLINE sl_size List<T, COMPARE>::getCount() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getCount();
	}
	return 0;
}

template <class T, class COMPARE>
SLIB_INLINE sl_size List<T, COMPARE>::getCapacity() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getCapacity();
	}
	return 0;
}

template <class T, class COMPARE>
SLIB_INLINE T* List<T, COMPARE>::getData() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getData();
	}
	return 0;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool List<T, COMPARE>::isEmpty() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getCount() == 0;
	}
	return sl_true;
}

template <class T, class COMPARE>
SLIB_INLINE sl_bool List<T, COMPARE>::isNotEmpty() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getCount() != 0;
	}
	return sl_false;
}

template <class T, class COMPARE>
T* List<T, COMPARE>::getItemPtr(sl_size index) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItemPtr(index);
	}
	return sl_null;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItem(index, _out);
	}
	return sl_false;
}

template <class T, class COMPARE>
T List<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getItemValue(index, def);
	}
	return def;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->setItem(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::setCount_NoLock(sl_size count)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->setCount_NoLock(count);
	} else {
		if (count == 0) {
			return sl_true;
		}
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->setCount_NoLock(count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::setCount(sl_size count)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->setCount(count);
	} else {
		if (count == 0) {
			return sl_true;
		}
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->setCount_NoLock(count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insert_NoLock(sl_size index, const T* values, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert_NoLock(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::insert_NoLock(sl_size index, const _T* values, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert_NoLock(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insert(sl_size index, const T* values, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::insert(sl_size index, const _T* values, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insertAll(sl_size index, const List<T, COMPARE>& other) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insertAll(index, other.ref.ptr);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::insertAll(sl_size index, const List<_T, _COMPARE>& other) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insertAll(index, other.ref.ptr);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insertAll(sl_size index, const SafeList<T, COMPARE>& other) const
{
	return insertAll(index, List<T, COMPARE>(other));
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::insertAll(sl_size index, const SafeList<_T, _COMPARE>& other) const
{
	return insertAll(index, List<_T, _COMPARE>(other));
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insert_NoLock(sl_size index, const T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert_NoLock(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::insert(sl_size index, const T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->insert(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add_NoLock(const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add_NoLock(values, count);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::add_NoLock(const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add_NoLock(values, count);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add(const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(values, count);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::add(const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(values, count);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::addAll(const List<T, COMPARE>& _other)
{
	CList<T, COMPARE>* other = _other.ref.ptr;
	if (!other) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->addAll(other);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->addAll(other);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::addAll(const List<_T, _COMPARE>& _other)
{
	CList<_T, _COMPARE>* other = _other.ref.ptr;
	if (!other) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(other);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->addAll(other);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::addAll(const SafeList<T, COMPARE>& other)
{
	return addAll(List<T, COMPARE>(other));
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::addAll(const SafeList<_T, _COMPARE>& other)
{
	return addAll(List<_T, _COMPARE>(other));
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add_NoLock(const T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add_NoLock(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add(const T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::addIfNotExistT_NoLock(const _T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template addIfNotExistT_NoLock<_T, _COMPARE>(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->template addIfNotExistT_NoLock<_T, _COMPARE>(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::addIfNotExistT(const _T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template addIfNotExistT<_T, _COMPARE>(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->template addIfNotExistT_NoLock<_T, _COMPARE>(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::addIfNotExist_NoLock(const T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->addIfNotExist_NoLock(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->addIfNotExist_NoLock(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::addIfNotExist(const T& value)
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->addIfNotExist(value);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->addIfNotExist_NoLock(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add_NoLock(const Iterator<T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add_NoLock(iterator);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::add_NoLock(const Iterator<_T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add_NoLock(iterator);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::add(const Iterator<T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(iterator);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool List<T, COMPARE>::add(const Iterator<_T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->add(iterator);
	} else {
		obj = CList<T, COMPARE>::create();
		if (obj) {
			ref = obj;
			return obj->add_NoLock(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::remove_NoLock(sl_size index, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->remove_NoLock(index, count);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::remove(sl_size index, sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->remove(index, count);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size List<T, COMPARE>::removeValueT_NoLock(const _T& value, sl_bool flagAllValues) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template removeValueT_NoLock<_T, _COMPARE>(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size List<T, COMPARE>::removeValueT(const _T& value, sl_bool flagAllValues) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template removeValueT<_T, _COMPARE>(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::removeValue_NoLock(const T& value, sl_bool flagAllValues) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->removeValue_NoLock(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::removeValue(const T& value, sl_bool flagAllValues) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->removeValue(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::removeAll_NoLock() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->removeAll_NoLock();
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::removeAll() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->removeAll();
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::popFront_NoLock(T* _out) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popFront_NoLock(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::popFront(T* _out) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popFront(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::popFront_NoLock(sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popFront_NoLock(count);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::popFront(sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popFront(count);
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::popBack_NoLock(T* _out) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popBack_NoLock(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::popBack(T* _out) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popBack(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::popBack_NoLock(sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popBack_NoLock(count);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size List<T, COMPARE>::popBack(sl_size count) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->popBack(count);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg List<T, COMPARE>::indexOfT_NoLock(const _T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template indexOfT_NoLock<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg List<T, COMPARE>::indexOfT(const _T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template indexOfT<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg List<T, COMPARE>::indexOf_NoLock(const T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->indexOf_NoLock(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg List<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->indexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg List<T, COMPARE>::lastIndexOfT_NoLock(const _T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template lastIndexOfT_NoLock<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg List<T, COMPARE>::lastIndexOfT(const _T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template lastIndexOfT<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg List<T, COMPARE>::lastIndexOf_NoLock(const T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->lastIndexOf_NoLock(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg List<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->lastIndexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::containsT_NoLock(const _T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template containsT_NoLock<_T, _COMPARE>(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool List<T, COMPARE>::containsT(const _T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->template contains<_T, _COMPARE>(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::contains_NoLock(const T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->contains_NoLock(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool List<T, COMPARE>::contains(const T& value) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->contains(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::duplicate_NoLock() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate_NoLock();
	}
	return List<T, COMPARE>::null();
}

template <class T, class COMPARE>
List<T, COMPARE> List<T, COMPARE>::duplicate() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate();
	}
	return List<T, COMPARE>::null();
}

template <class T, class COMPARE>
Array<T, COMPARE> List<T, COMPARE>::toArray_NoLock() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->toArray_NoLock();
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
Array<T, COMPARE> List<T, COMPARE>::toArray() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->toArray();
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
template <class _COMPARE>
void List<T, COMPARE>::sortBy(sl_bool flagAscending) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		obj->template sortBy<_COMPARE>(flagAscending);
	}
}

template <class T, class COMPARE>
template <class _COMPARE>
void List<T, COMPARE>::sortBy_NoLock(sl_bool flagAscending) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		obj->template sortBy_NoLock<_COMPARE>(flagAscending);
	}
}

template <class T, class COMPARE>
void List<T, COMPARE>::sort_NoLock(sl_bool flagAscending) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		obj->sort_NoLock(flagAscending);
	}
}

template <class T, class COMPARE>
void List<T, COMPARE>::sort(sl_bool flagAscending) const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		obj->sort(flagAscending);
	}
}

template <class T, class COMPARE>
Iterator<T> List<T, COMPARE>::iterator() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return new ListIterator<T, COMPARE>(obj, obj);
	}
	return Iterator<T>::null();
}

template <class T, class COMPARE>
const Mutex* List<T, COMPARE>::getLocker() const
{
	CList<T, COMPARE>* obj = ref.ptr;
	if (obj) {
		return obj->getLocker();
	} else {
		return sl_null;
	}
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(SafeList, List, CList, ref)

template <class T, class COMPARE>
SafeList<T, COMPARE>::SafeList(sl_size count) : ref(CList<T, COMPARE>::create(count))
{
}

template <class T, class COMPARE>
SafeList<T, COMPARE>::SafeList(const T* data, sl_size count) : ref(CList<T, COMPARE>::createFromElements(data, count))
{
}

template <class T, class COMPARE>
template <class _T>
SafeList<T, COMPARE>::SafeList(const _T* data, sl_size count) : ref(CList<T, COMPARE>::createFromElements(data, count))
{
}

template <class T, class COMPARE>
template <class _COMPARE>
SLIB_INLINE const SafeList<T, COMPARE>& SafeList<T, COMPARE>::from(const SafeList<T, _COMPARE>& other)
{
	return *((SafeList<T, COMPARE>*)((void*)&other));
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::getCount() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount();
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::isEmpty() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() == 0;
	}
	return sl_true;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::isNotEmpty() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() != 0;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::getItem(sl_size index, T* _out) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItem(index, _out);
	}
	return sl_false;
}

template <class T, class COMPARE>
T SafeList<T, COMPARE>::getItemValue(sl_size index, const T& def) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getItemValue(index, def);
	}
	return def;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::setItem(sl_size index, const T& value) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->setItem(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::setCount(sl_size count)
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->setCount(count);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->setCount(count);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->setCount(count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::insert(sl_size index, const T* values, sl_size count) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->insert(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool SafeList<T, COMPARE>::insert(sl_size index, const _T* values, sl_size count) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->insert(index, values, count);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::insertAll(sl_size index, const List<T, COMPARE>& other) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->insertAll(index, other.ref.ptr);
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::insertAll(sl_size index, const List<_T, _COMPARE>& other) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->insertAll(index, other.ref.ptr);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::insertAll(sl_size index, const SafeList<T, COMPARE>& other) const
{
	return insertAll(index, List<T, COMPARE>(other));
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::insertAll(sl_size index, const SafeList<_T, _COMPARE>& other) const
{
	return insertAll(index, List<_T, _COMPARE>(other));
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::insert(sl_size index, const T& value) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->insert(index, value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::add(const T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->add(values, count);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->add(values, count);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->add(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool SafeList<T, COMPARE>::add(const _T* values, sl_size count)
{
	if (count == 0) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->add(values, count);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->add(values, count);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->add(values, count);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::addAll(const List<T, COMPARE>& _other)
{
	CList<T, COMPARE>* other = _other.ref.ptr;
	if (!other) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->addAll(other);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->addAll(other);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->addAll(other);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::addAll(const List<_T, _COMPARE>& _other)
{
	CList<_T, _COMPARE>* other = _other.object;
	if (!other) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->addAll(other);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->addAll(other);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->addAll(other);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::addAll(const SafeList<T, COMPARE>& other)
{
	return addAll(List<T, COMPARE>(other));
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::addAll(const SafeList<_T, _COMPARE>& other)
{
	return addAll(List<_T, _COMPARE>(other));
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::add(const T& value)
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->add(value);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->add(value);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->add(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::addIfNotExistT(const _T& value)
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->template addIfNotExistT<_T, _COMPARE>(value);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->template addIfNotExistT<_T, _COMPARE>(value);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->template addIfNotExistT<_T, _COMPARE>(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::addIfNotExist(const T& value)
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->addIfNotExist(value);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->addIfNotExist(value);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->addIfNotExist(value);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::add(const Iterator<T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->add(iterator);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->add(iterator);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->add(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
template <class _T>
sl_bool SafeList<T, COMPARE>::add(const Iterator<_T>& iterator)
{
	if (iterator.isNull()) {
		return sl_true;
	}
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->add(iterator);
	} else {
		SpinLocker lock(SpinLockPoolForList::get(this));
		obj = ref;
		if (obj.isNotNull()) {
			lock.unlock();
			return obj->add(iterator);
		}
		obj = CList<T, COMPARE>::create();
		if (obj.isNotNull()) {
			ref = obj;
			lock.unlock();
			return obj->add(iterator);
		}
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::remove(sl_size index, sl_size count) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->remove(index, count);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_size SafeList<T, COMPARE>::removeValueT(const _T& value, sl_bool flagAllValues) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->template removeValueT<_T, _COMPARE>(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::removeValue(const T& value, sl_bool flagAllValues) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeValue(value, flagAllValues);
	}
	return 0;
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::removeAll() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeAll();
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::popFront(T* _out) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popFront(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::popFront(sl_size count) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popFront(count);
	}
	return 0;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::popBack(T* _out) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popBack(_out);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_size SafeList<T, COMPARE>::popBack(sl_size count) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->popBack(count);
	}
	return 0;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg SafeList<T, COMPARE>::indexOfT(const _T& value, sl_reg start) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->template indexOfT<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg SafeList<T, COMPARE>::indexOf(const T& value, sl_reg start) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->indexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_reg SafeList<T, COMPARE>::lastIndexOfT(const _T& value, sl_reg start) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->template lastIndexOfT<_T, _COMPARE>(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
sl_reg SafeList<T, COMPARE>::lastIndexOf(const T& value, sl_reg start) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->lastIndexOf(value, start);
	}
	return -1;
}

template <class T, class COMPARE>
template <class _T, class _COMPARE>
sl_bool SafeList<T, COMPARE>::containsT(const _T& value) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->template containsT<_T, _COMPARE>(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_bool SafeList<T, COMPARE>::contains(const T& value) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->contains(value);
	}
	return sl_false;
}

template <class T, class COMPARE>
List<T, COMPARE> SafeList<T, COMPARE>::duplicate() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return List<T, COMPARE>::null();
}

template <class T, class COMPARE>
Array<T, COMPARE> SafeList<T, COMPARE>::toArray() const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		return obj->toArray();
	}
	return Array<T, COMPARE>::null();
}

template <class T, class COMPARE>
template <class _COMPARE>
void SafeList<T, COMPARE>::sortBy(sl_bool flagAscending) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		obj->template sortBy<_COMPARE>(flagAscending);
	}
}

template <class T, class COMPARE>
void SafeList<T, COMPARE>::sort(sl_bool flagAscending) const
{
	Ref< CList<T, COMPARE> > obj(ref);
	if (obj.isNotNull()) {
		obj->sort(flagAscending);
	}
}


template <class T>
ListLocker<T>::ListLocker(const List<T>& list) : ObjectLocker(list.ref.ptr)
{
	m_list = list;
	data = list.getData();
	count = list.getCount();
}

template <class T>
template <class _COMPARE>
ListLocker<T>::ListLocker(const List<T, _COMPARE>& list) : ObjectLocker(list.ref.ptr)
{
	m_list = list;
	data = list.getData();
	count = list.getCount();
}

template <class T>
ListLocker<T>::ListLocker(const SafeList<T>& list) : ListLocker(List<T>(list))
{
}

template <class T>
template <class _COMPARE>
ListLocker<T>::ListLocker(const SafeList<T, _COMPARE>& list) : ListLocker(List<T, _COMPARE>(list))
{
}

template <class T>
ListLocker<T>::ListLocker(const CList<T>& list) : ObjectLocker(&list)
{
	data = list.getData();
	count = list.getCount();
}

template <class T>
template <class _COMPARE>
ListLocker<T>::ListLocker(const CList<T, _COMPARE>& list) : ObjectLocker(&list)
{
	data = list.getData();
	count = list.getCount();
}

template <class T>
ListLocker<T>::~ListLocker()
{
	unlock();
}

template <class T>
SLIB_INLINE T& ListLocker<T>::operator[](sl_reg index)
{
	return data[index];
}


template <class T>
ListItems<T>::ListItems(const List<T>& list)
{
	m_list = list;
	data = list.getData();
	count = list.getCount();
}

template <class T>
template <class _COMPARE>
ListItems<T>::ListItems(const List<T, _COMPARE>& list)
{
	m_list = list;
	data = list.getData();
	count = list.getCount();
}

template <class T>
ListItems<T>::ListItems(const SafeList<T>& list) : ListItems(List<T>(list))
{
}

template <class T>
template <class _COMPARE>
ListItems<T>::ListItems(const SafeList<T, _COMPARE>& list) : ListItems(List<T, _COMPARE>(list))
{
}

template <class T>
ListItems<T>::ListItems(const CList<T>& list)
{
	data = list.getData();
	count = list.getCount();
}

template <class T>
template <class _COMPARE>
ListItems<T>::ListItems(const CList<T, _COMPARE>& list)
{
	data = list.getData();
	count = list.getCount();
}

template <class T>
SLIB_INLINE T& ListItems<T>::operator[](sl_reg index)
{
	return data[index];
}


template <class T, class COMPARE>
ListIterator<T, COMPARE>::ListIterator(const CList<T, COMPARE>* list, const Referable* refer)
{
	m_index = 0;
	m_list = list;
	m_refer = refer;
}

template <class T, class COMPARE>
sl_bool ListIterator<T, COMPARE>::hasNext()
{
	return (m_index < m_list->getCount());
}

template <class T, class COMPARE>
sl_bool ListIterator<T, COMPARE>::next(T* _out)
{
	if (m_list->getItem(m_index, _out)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class T, class COMPARE>
sl_reg ListIterator<T, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}

SLIB_NAMESPACE_END

#endif
