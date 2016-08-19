#ifndef CHECKHEADER_SLIB_CORE_MAP
#define CHECKHEADER_SLIB_CORE_MAP

#include "definition.h"

#include "object.h"
#include "iterator.h"
#include "list.h"
#include "hashtable.h"
#include "tree.h"

SLIB_NAMESPACE_BEGIN

template <class KT, class VT>
class SLIB_EXPORT Pair
{
public:
	KT key;
	VT value;
};

template <class KT, class VT>
class SLIB_EXPORT IMap : public Object
{
	SLIB_DECLARE_OBJECT
public:
	static IMap<KT, VT>* createDefault();
	
	virtual sl_size getCount() const = 0;
	
	sl_bool isEmpty() const;

	sl_bool isNotEmpty() const;

	virtual VT* getItemPtr(const KT& key) const = 0;
	
	sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const;
	
	sl_bool get(const KT& key, VT* _out = sl_null) const;
	
	VT getValue_NoLock(const KT& key, const VT& def) const;
	
	VT getValue(const KT& key, const VT& def) const;
	
	virtual List<VT> getValues_NoLock(const KT& key) const = 0;
	
	List<VT> getValues(const KT& key) const;
	
	virtual sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null) = 0;
	
	sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	void put(IMap<KT, VT>* other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void put(IMap<_KT, _VT>* other, MapPutMode mode = MapPutMode::Default);
	
	virtual sl_size remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches = sl_false) = 0;
	
	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	virtual sl_size removeAll_NoLock() = 0;
	
	sl_size removeAll();
	
	virtual sl_bool contains_NoLock(const KT& key) const = 0;
	
	sl_bool contains(const KT& key) const;
	
	virtual IMap<KT, VT>* duplicate_NoLock() const = 0;
	
	IMap<KT, VT>* duplicate() const;
	
	virtual Iterator<KT> keyIteratorWithRefer(const Referable* refer) const = 0;
	
	Iterator<KT> keyIterator() const;
	
	virtual List<KT> keys_NoLock() const = 0;
	
	List<KT> keys() const;
	
	virtual Iterator<VT> valueIteratorWithRefer(const Referable* refer) const = 0;
	
	Iterator<VT> valueIterator() const;
	
	virtual List<VT> values_NoLock() const = 0;
	
	List<VT> values() const;
	
	virtual Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const = 0;
	
	Iterator< Pair<KT, VT> > iterator() const;
	
	virtual List< Pair<KT, VT> > pairs_NoLock() const = 0;
	
	List< Pair<KT, VT> > pairs() const;
	
};


template < class KT, class VT, class COMPARE = Compare<KT> >
class SLIB_EXPORT ListMap : public IMap<KT, VT>
{
public:
	class PairKeyCompare
	{
	public:
		static sl_bool equals(const Pair<KT, VT>& a, const KT& b);
	};
	
	class PairCompare
	{
	public:
		static sl_bool equals(const Pair<KT, VT>& a, const Pair<KT, VT>& b);
	};

	CList< Pair<KT, VT> > list;
	
public:
	static ListMap<KT, VT, COMPARE>* create();
	
	// override
	sl_size getCount() const;
	
	// override
	VT* getItemPtr(const KT& key) const;
	
	// override
	List<VT> getValues_NoLock(const KT& key) const;
	
	// override
	sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	sl_bool addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);
	
	// override
	sl_size removeAll_NoLock();
	
	// override
	sl_bool contains_NoLock(const KT& key) const;
	
	sl_bool contains(const KT& key) const;
	
	sl_bool contains_NoLock(const KT& key, const VT& value) const;
	
	sl_bool contains(const KT& key, const VT& value) const;
	
	// override
	IMap<KT, VT>* duplicate_NoLock() const;
	
	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const;
	
	// override
	List<KT> keys_NoLock() const;
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const;
	
	// override
	List<VT> values_NoLock() const;
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const;
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const;
	
};


template < class KT, class VT, class HASH = Hash<KT> >
class SLIB_EXPORT HashMap : public IMap<KT, VT>
{
public:
	HashTable<KT, VT, HASH> table;

public:
	HashMap(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);

public:
	static HashMap<KT, VT, HASH>* create(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);

	// override
	sl_size getCount() const;
	
	// override
	VT* getItemPtr(const KT& key) const;
	
	// override
	List<VT> getValues_NoLock(const KT& key) const;

	// override
	sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

	sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	sl_bool addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);

	// override
	sl_size removeAll_NoLock();

	// override
	sl_bool contains_NoLock(const KT& key) const;
	
	sl_bool contains(const KT& key) const;
	
	sl_bool contains_NoLock(const KT& key, const VT& value) const;
	
	sl_bool contains(const KT& key, const VT& value) const;

	// override
	IMap<KT, VT>* duplicate_NoLock() const;

	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const;

	// override
	List<KT> keys_NoLock() const;
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const;

	// override
	List<VT> values_NoLock() const;
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const;
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const;

};


/*
 TreeMap class Definition                                             
	Now TreeMap is based on BTree, but should be changed to Red-Black
	Tree implementation which is a little better for in-memory structure.
*/
template < class KT, class VT, class COMPARE=Compare<KT> >
class SLIB_EXPORT TreeMap : public IMap<KT, VT>
{
public:
	BTree<KT, VT, COMPARE> tree;

public:
	static TreeMap<KT, VT, COMPARE>* create();
	
	// override
	sl_size getCount() const;
	
	// override
	VT* getItemPtr(const KT& key) const;
	
	// override
	List<VT> getValues_NoLock(const KT& key) const;
	
	// override
	sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);

	sl_bool addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	sl_bool addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist = sl_null);
	
	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches = sl_false);

	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);
	
	sl_size removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches = sl_false);
	
	// override
	sl_size removeAll_NoLock();
	
	// override
	sl_bool contains_NoLock(const KT& key) const;

	sl_bool contains(const KT& key) const;
	
	sl_bool contains_NoLock(const KT& key, const VT& value) const;
	
	sl_bool contains(const KT& key, const VT& value) const;
	
	// override
	IMap<KT, VT>* duplicate_NoLock() const;
	
	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const;
	
	// override
	List<KT> keys_NoLock() const;
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const;
	
	// override
	List<VT> values_NoLock() const;
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const;
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const;

};


template <class KT, class VT>
class SafeMap;

#define SLIB_TEMPLATE_PARAMS_IMap KT, VT
#define SLIB_TEMPLATE_DEF_PARAMS_IMap class KT, class VT

/** auto-referencing object **/
template <class KT, class VT>
class SLIB_EXPORT Map
{
public:
	Ref< IMap<KT, VT> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(Map, SafeMap, IMap)

public:
	static Map<KT, VT> createList();
	
	template <class COMPARE>
	static Map<KT, VT> createListBy();
	
	static Map<KT, VT> createHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	template <class HASH>
	static Map<KT, VT> createHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	static Map<KT, VT> createTree();
	
	template <class COMPARE>
	static Map<KT, VT> createTreeBy();
	
public:
	void initList();
	
	template <class COMPARE>
	void initListBy();
	
	void initHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	template <class HASH>
	void initHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	void initTree();
	
	template <class COMPARE>
	void initTreeBy();
	
public:
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	VT* getItemPtr(const KT& key) const;
	
	VT* getNewItemPtr(const KT& key);
	
	sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const;
	
	sl_bool get(const KT& key, VT* _out = sl_null) const;
	
	VT getValue_NoLock(const KT& key, const VT& def) const;
	
	VT getValue(const KT& key, const VT& def) const;
	
	List<VT> getValues_NoLock(const KT& key) const;
	
	List<VT> getValues(const KT& key) const;
	
	sl_bool put_NoLock(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	void put(const Map<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void put(const Map<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	void put(const SafeMap<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void put(const SafeMap<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	sl_size remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches = sl_false) const;
	
	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false) const;
	
	sl_size removeAll_NoLock() const;
	
	sl_size removeAll() const;
	
	sl_bool contains_NoLock(const KT& key) const;
	
	sl_bool contains(const KT& key) const;
	
	Map<KT, VT> duplicate_NoLock() const;
	
	Map<KT, VT> duplicate() const;
	
	Iterator<KT> keyIterator() const;
	
	List<KT> keys_NoLock() const;
	
	List<KT> keys() const;
	
	Iterator<VT> valueIterator() const;
	
	List<VT> values_NoLock() const;
	
	List<VT> values() const;
	
	Iterator< Pair<KT, VT> > iterator() const;
	
	List< Pair<KT, VT> > pairs_NoLock() const;
	
	List< Pair<KT, VT> > pairs() const;
	
	const Mutex* getLocker() const;
	
};


/** auto-referencing object **/
template <class KT, class VT>
class SLIB_EXPORT SafeMap
{
public:
	SafeRef< IMap<KT, VT> > ref;
	SLIB_DECLARE_TEMPLATE_REF_WRAPPER(SafeMap, Map, IMap)
	
public:
	void initList();
	
	template <class COMPARE>
	void initListBy();
	
	void initHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	template <class HASH>
	void initHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	void initTree();
	
	template <class COMPARE>
	void initTreeBy();
	
public:
	sl_size getCount() const;
	
	sl_bool isEmpty() const;
	
	sl_bool isNotEmpty() const;
	
	sl_bool get(const KT& key, VT* _out = sl_null) const;
	
	VT getValue(const KT& key, const VT& def) const;
	
	List<VT> getValues(const KT& key) const;
	
	sl_bool put(const KT& key, const VT& value, MapPutMode mode = MapPutMode::Default, sl_bool* pFlagExist = sl_null);
	
	void put(const Map<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void put(const Map<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	void put(const SafeMap<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void put(const SafeMap<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	sl_size remove(const KT& key, sl_bool flagRemoveAllMatches = sl_false) const;
	
	sl_size removeAll() const;
	
	sl_bool contains(const KT& key) const;

	Map<KT, VT> duplicate() const;
	
	List<KT> keys() const;
	
	List<VT> values() const;
	
	List< Pair<KT, VT> > pairs() const;
	
};


template < class KT, class VT, class COMPARE = Compare<KT> >
class ListMapKeyIterator : public IIterator<KT>
{
protected:
	const ListMap<KT, VT, COMPARE>* m_map;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	ListMapKeyIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(KT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class COMPARE = Compare<KT> >
class ListMapValueIterator : public IIterator<VT>
{
protected:
	const ListMap<KT, VT, COMPARE>* m_map;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	ListMapValueIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(VT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class COMPARE = Compare<KT> >
class ListMapIterator : public IIterator< Pair<KT, VT> >
{
protected:
	const ListMap<KT, VT, COMPARE>* m_map;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	ListMapIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(Pair<KT, VT>* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class HASH = Hash<KT> >
class HashMapKeyIterator : public IIterator<KT>
{
protected:
	const HashMap<KT, VT, HASH>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapKeyIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(KT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class HASH = Hash<KT> >
class HashMapValueIterator : public IIterator<VT>
{
protected:
	const HashMap<KT, VT, HASH>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapValueIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(VT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class HASH = Hash<KT> >
class HashMapIterator : public IIterator< Pair<KT, VT> >
{
protected:
	const HashMap<KT, VT, HASH>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(Pair<KT, VT>* out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class COMPARE=Compare<KT> >
class TreeMapKeyIterator : public IIterator<KT>
{
protected:
	const TreeMap<KT, VT, COMPARE>* m_map;
	TreePosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	TreeMapKeyIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(KT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class COMPARE=Compare<KT> >
class TreeMapValueIterator: public IIterator<VT>
{
protected:
	const TreeMap<KT, VT, COMPARE>* m_map;
	TreePosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	TreeMapValueIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(VT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class COMPARE=Compare<KT> >
class TreeMapIterator : public IIterator< Pair<KT, VT> >
{
protected:
	const TreeMap<KT, VT, COMPARE>* m_map;
	TreePosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	TreeMapIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(Pair<KT, VT>* _out);
	
	// override
	sl_reg getIndex();
	
};


#define SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(...) \
	extern template class HashTable<__VA_ARGS__>; \
	extern template class BTree<__VA_ARGS__>; \
	extern template class IMap<__VA_ARGS__>; \
	extern template class ListMap<__VA_ARGS__>; \
	extern template class HashMap<__VA_ARGS__>; \
	extern template class TreeMap<__VA_ARGS__>; \
	extern template class Map<__VA_ARGS__>; \
	extern template class SafeMap<__VA_ARGS__>; \

#define SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_MAP(...) \
	template class HashTable<__VA_ARGS__>; \
	template class BTree<__VA_ARGS__>; \
	template class IMap<__VA_ARGS__>; \
	template class ListMap<__VA_ARGS__>; \
	template class HashMap<__VA_ARGS__>; \
	template class TreeMap<__VA_ARGS__>; \
	template class Map<__VA_ARGS__>; \
	template class SafeMap<__VA_ARGS__>; \


SLIB_DECLARE_EXPLICIT_INSTANTIATIONS_FOR_MAP(sl_uint64, Ref<Referable>)

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

extern const char _Map_ClassID[];

SLIB_DEFINE_TEMPLATE_OBJECT(IMap, Object, _Map_ClassID)

template <class KT, class VT>
IMap<KT, VT>* IMap<KT, VT>::createDefault()
{
	return HashMap<KT, VT>::create();
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::isEmpty() const
{
	return getCount() == 0;
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::isNotEmpty() const
{
	return getCount() != 0;
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::get_NoLock(const KT& key, VT* _out) const
{
	VT* p = getItemPtr(key);
	if (p) {
		if (_out) {
			*_out = *p;
		}
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::get(const KT& key, VT* _out) const
{
	ObjectLocker lock(this);
	VT* p = getItemPtr(key);
	if (p) {
		if (_out) {
			*_out = *p;
		}
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT>
VT IMap<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const
{
	VT* p = getItemPtr(key);
	if (p) {
		return *p;
	}
	return def;
}

template <class KT, class VT>
VT IMap<KT, VT>::getValue(const KT& key, const VT& def) const
{
	ObjectLocker lock(this);
	VT* p = getItemPtr(key);
	if (p) {
		return *p;
	}
	return def;
}

template <class KT, class VT>
List<VT> IMap<KT, VT>::getValues(const KT& key) const
{
	ObjectLocker lock(this);
	return getValues_NoLock(key);
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	ObjectLocker lock(this);
	return put_NoLock(key, value, mode, pFlagExist);
}

template <class KT, class VT>
void IMap<KT, VT>::put(IMap<KT, VT>* other, MapPutMode mode)
{
	if (!other) {
		return;
	}
	if (this == other) {
		return;
	}
	ObjectLocker lock(this, other);
	Iterator< Pair<KT, VT> > iterator(other->iterator());
	Pair<KT, VT> v;
	while (iterator.next(&v)) {
		put_NoLock(v.key, v.value, mode);
	}
}

template <class KT, class VT>
template <class _KT, class _VT>
void IMap<KT, VT>::put(IMap<_KT, _VT>* other, MapPutMode mode)
{
	if (!other) {
		return;
	}
	if (this == other) {
		return;
	}
	ObjectLocker lock(this, other);
	Iterator< Pair<_KT,_VT> > iterator(other->iterator());
	Pair<_KT, _VT> v;
	while (iterator.next(&v)) {
		put_NoLock(v.key, v.value, mode);
	}
}

template <class KT, class VT>
sl_size IMap<KT, VT>::remove(const KT& key, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return remove_NoLock(key, flagRemoveAllMatches);
}

template <class KT, class VT>
sl_size IMap<KT, VT>::removeAll()
{
	ObjectLocker lock(this);
	return removeAll_NoLock();
}

template <class KT, class VT>
sl_bool IMap<KT, VT>::contains(const KT& key) const
{
	ObjectLocker lock(this);
	return contains_NoLock(key);
}

template <class KT, class VT>
IMap<KT, VT>* IMap<KT, VT>::duplicate() const
{
	ObjectLocker lock(this);
	return duplicate_NoLock();
}

template <class KT, class VT>
Iterator<KT> IMap<KT, VT>::keyIterator() const
{
	return keyIteratorWithRefer(sl_null);
}

template <class KT, class VT>
List<KT> IMap<KT, VT>::keys() const
{
	ObjectLocker lock(this);
	return keys_NoLock();
}

template <class KT, class VT>
Iterator<VT> IMap<KT, VT>::valueIterator() const
{
	return valueIteratorWithRefer(sl_null);
}

template <class KT, class VT>
List<VT> IMap<KT, VT>::values() const
{
	ObjectLocker lock(this);
	return values_NoLock();
}

template <class KT, class VT>
Iterator< Pair<KT, VT> > IMap<KT, VT>::iterator() const
{
	return iteratorWithRefer(sl_null);
}

template <class KT, class VT>
List< Pair<KT, VT> > IMap<KT, VT>::pairs() const
{
	ObjectLocker lock(this);
	return pairs_NoLock();
}


template <class KT, class VT, class COMPARE>
SLIB_INLINE sl_bool ListMap<KT, VT, COMPARE>::PairKeyCompare::equals(const Pair<KT, VT>& a, const KT& b)
{
	return COMPARE::equals(a.key, b);
}

template <class KT, class VT, class COMPARE>
SLIB_INLINE sl_bool ListMap<KT, VT, COMPARE>::PairCompare::equals(const Pair<KT, VT>& a, const Pair<KT, VT>& b)
{
	return COMPARE::equals(a.key, b.key) && a.value == b.value;
}


template <class KT, class VT, class COMPARE>
ListMap<KT, VT, COMPARE>* ListMap<KT, VT, COMPARE>::create()
{
	return new ListMap<KT, VT, COMPARE>;
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::getCount() const
{
	return list.getCount();
}

template <class KT, class VT, class COMPARE>
VT* ListMap<KT, VT, COMPARE>::getItemPtr(const KT& key) const
{
	sl_reg index = list.template indexOfT_NoLock<KT, PairKeyCompare>(key);
	if (index >= 0) {
		Pair<KT, VT>* p = list.getData() + index;
		return &(p->value);
	}
	return sl_null;
}

template <class KT, class VT, class COMPARE>
List<VT> ListMap<KT, VT, COMPARE>::getValues_NoLock(const KT& key) const
{
	CList<VT>* ret = new CList<VT>;
	if (ret) {
		Pair<KT, VT>* data = list.getData();
		sl_size count = list.getCount();
		for (sl_size i = 0; i < count; i++) {
			if (COMPARE::equals(data[i].key, key)) {
				ret->add_NoLock(data[i].value);
			}
		}
		if (ret->getCount() > 0) {
			return ret;
		}
		delete ret;
	}
	return List<VT>::null();
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	Pair<KT, VT>* items = list.getData();
	if (mode != MapPutMode::AddAlways) {
		sl_reg index = list.template indexOfT_NoLock<KT, PairKeyCompare>(key);
		if (index >= 0) {
			if (pFlagExist) {
				*pFlagExist = sl_true;
			}
			if (mode == MapPutMode::AddNew) {
				return sl_false;
			}
			items[index].value = value;
			return sl_true;
		}
		if (mode == MapPutMode::ReplaceExisting) {
			return sl_false;
		}
	}
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	if (list.add_NoLock(pair)) {
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	if (pFlagExist) {
		*pFlagExist = sl_false;
	}
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	sl_reg index = list.template indexOfT_NoLock<Pair<KT, VT>, PairCompare>(pair);
	if (index >= 0) {
		if (pFlagExist) {
			*pFlagExist = sl_true;
		}
		return sl_false;
	}
	if (list.add_NoLock(pair)) {
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	ObjectLocker lock(this);
	return addIfNewKeyAndValue_NoLock(key, value, pFlagExist);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches)
{
	return list.template removeValueT_NoLock<KT, PairKeyCompare>(key, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::remove(const KT& key, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return remove_NoLock(key, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	return list.template removeValueT_NoLock<Pair<KT, VT>, PairCompare>(pair, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return removeKeyAndValue_NoLock(key, value, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeAll_NoLock()
{
	return list.removeAll_NoLock();
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::contains_NoLock(const KT& key) const
{
	return list.template indexOfT_NoLock<KT, PairKeyCompare>(key) >= 0;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::contains(const KT& key) const
{
	ObjectLocker lock(this);
	return list.template indexOfT_NoLock<KT, PairKeyCompare>(key) >= 0;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::contains_NoLock(const KT& key, const VT& value) const
{
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	return list.template indexOfT_NoLock<Pair<KT, VT>, PairCompare>(pair) >= 0;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::contains(const KT& key, const VT& value) const
{
	ObjectLocker lock(this);
	return contains_NoLock(key, value);
}

template <class KT, class VT, class COMPARE>
IMap<KT, VT>* ListMap<KT, VT, COMPARE>::duplicate_NoLock() const
{
	ListMap<KT, VT, COMPARE>* ret = new ListMap<KT, VT, COMPARE>;
	if (ret) {
		if (ret->list.add_NoLock(list.getData(), list.getCount())) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class COMPARE>
Iterator<KT> ListMap<KT, VT, COMPARE>::keyIteratorWithRefer(const Referable* refer) const
{
	return new ListMapKeyIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List<KT> ListMap<KT, VT, COMPARE>::keys_NoLock() const
{
	CList<KT>* ret = new CList<KT>;
	if (ret) {
		Pair<KT, VT>* dataSrc = list.getData();
		sl_size count = list.getCount();
		if (ret->setCount_NoLock(count)) {
			KT* dataDst = ret->getData();
			for (sl_size i = 0; i < count; i++) {
				dataDst[i] = dataSrc[i].key;
			}
			return ret;
		}
		delete ret;
	}
	return List<KT>::null();
}

template <class KT, class VT, class COMPARE>
Iterator<VT> ListMap<KT, VT, COMPARE>::valueIteratorWithRefer(const Referable* refer) const
{
	return new ListMapValueIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List<VT> ListMap<KT, VT, COMPARE>::values_NoLock() const
{
	CList<VT>* ret = new CList<VT>;
	if (ret) {
		Pair<KT, VT>* dataSrc = list.getData();
		sl_size count = list.getCount();
		if (ret->setCount_NoLock(count)) {
			VT* dataDst = ret->getData();
			for (sl_size i = 0; i < count; i++) {
				dataDst[i] = dataSrc[i].value;
			}
			return ret;
		}
		delete ret;
	}
	return List<VT>::null();
}

template <class KT, class VT, class COMPARE>
Iterator< Pair<KT, VT> > ListMap<KT, VT, COMPARE>::iteratorWithRefer(const Referable* refer) const
{
	return new ListMapIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List< Pair<KT, VT> > ListMap<KT, VT, COMPARE>::pairs_NoLock() const
{
	return (CList< Pair<KT, VT> >*)((void*)(list.duplicate()));
}


template <class KT, class VT, class HASH>
HashMap<KT, VT, HASH>::HashMap(sl_uint32 capacity) : table(capacity)
{
}

template <class KT, class VT, class HASH>
HashMap<KT, VT, HASH>* HashMap<KT, VT, HASH>::create(sl_uint32 capacity)
{
	HashMap<KT, VT, HASH>* ret = new HashMap<KT, VT, HASH>(capacity);
	if (ret) {
		if (ret->table.getCapacity() > 0) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::getCount() const
{
	return (sl_size)(table.getCount());
}

template <class KT, class VT, class HASH>
VT* HashMap<KT, VT, HASH>::getItemPtr(const KT& key) const
{
	return table.getItemPtr(key);
}

template <class KT, class VT, class HASH>
List<VT> HashMap<KT, VT, HASH>::getValues_NoLock(const KT& key) const
{
	return table.getValues(key);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	return table.put(key, value, mode, pFlagExist);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	return table.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	ObjectLocker lock(this);
	return table.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches)
{
	return table.remove(key, flagRemoveAllMatches);
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::remove(const KT& key, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return table.remove(key, flagRemoveAllMatches);
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	return table.removeKeyAndValue(key, value, flagRemoveAllMatches);
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return table.removeKeyAndValue(key, value, flagRemoveAllMatches);
}

template <class KT, class VT, class HASH>
sl_size HashMap<KT, VT, HASH>::removeAll_NoLock()
{
	return table.removeAll();
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::contains_NoLock(const KT& key) const
{
	return table.search(key);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::contains(const KT& key) const
{
	ObjectLocker lock(this);
	return table.search(key);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::contains_NoLock(const KT& key, const VT& value) const
{
	return table.search(key, value);
}

template <class KT, class VT, class HASH>
sl_bool HashMap<KT, VT, HASH>::contains(const KT& key, const VT& value) const
{
	ObjectLocker lock(this);
	return table.search(key, value);
}

template <class KT, class VT, class HASH>
IMap<KT, VT>* HashMap<KT, VT, HASH>::duplicate_NoLock() const
{
	HashMap<KT, VT, HASH>* ret = new HashMap<KT, VT, HASH>;
	if (ret) {
		if (ret->table.copyFrom(&table)) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class HASH>
Iterator<KT> HashMap<KT, VT, HASH>::keyIteratorWithRefer(const Referable* refer) const
{
	return new HashMapKeyIterator<KT, VT, HASH>(this, refer);
}

template <class KT, class VT, class HASH>
List<KT> HashMap<KT, VT, HASH>::keys_NoLock() const
{
	CList<KT>* ret = new CList<KT>;
	if (ret) {
		HashPosition pos;
		KT key;
		while (table.getNextPosition(pos, &key, sl_null)) {
			if (!(ret->add_NoLock(key))) {
				delete ret;
				return List<KT>::null();
			}
		}
		return ret;
	}
	return List<KT>::null();
}

template <class KT, class VT, class HASH>
Iterator<VT> HashMap<KT, VT, HASH>::valueIteratorWithRefer(const Referable* refer) const
{
	return new HashMapValueIterator<KT, VT, HASH>(this, refer);
}

template <class KT, class VT, class HASH>
List<VT> HashMap<KT, VT, HASH>::values_NoLock() const
{
	CList<VT>* ret = new CList<VT>;
	if (ret) {
		HashPosition pos;
		VT value;
		while (table.getNextPosition(pos, sl_null, &value)) {
			if (!(ret->add_NoLock(value))) {
				delete ret;
				return List<VT>::null();
			}
		}
		return ret;
	}
	return List<VT>::null();
}

template <class KT, class VT, class HASH>
Iterator< Pair<KT, VT> > HashMap<KT, VT, HASH>::iteratorWithRefer(const Referable* refer) const
{
	return new HashMapIterator<KT, VT, HASH>(this, refer);
}

template <class KT, class VT, class HASH>
List< Pair<KT, VT> > HashMap<KT, VT, HASH>::pairs_NoLock() const
{
	CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
	if (ret) {
		HashPosition pos;
		Pair<KT, VT> pair;
		while (table.getNextPosition(pos, &(pair.key), &(pair.value))) {
			if (!(ret->add_NoLock(pair))) {
				delete ret;
				return List< Pair<KT, VT> >::null();
			}
		}
		return ret;
	}
	return List< Pair<KT, VT> >::null();
}


template <class KT, class VT, class COMPARE>
TreeMap<KT, VT, COMPARE>* TreeMap<KT, VT, COMPARE>::create()
{
	TreeMap<KT, VT, COMPARE>* ret = new TreeMap<KT, VT, COMPARE>;
	if (ret) {
		if (ret->tree.isValid()) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::getCount() const
{
	return (sl_size)(tree.getCount());
}

template <class KT, class VT, class COMPARE>
VT* TreeMap<KT, VT, COMPARE>::getItemPtr(const KT& key) const
{
	return tree.getItemPtr(key);
}

template <class KT, class VT, class COMPARE>
List<VT> TreeMap<KT, VT, COMPARE>::getValues_NoLock(const KT& key) const
{
	return tree.getValues(key);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	return tree.put(key, value, mode, pFlagExist);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	return tree.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	ObjectLocker lock(this);
	return tree.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches)
{
	return tree.remove(key, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::remove(const KT& key, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return tree.remove(key, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	return tree.removeKeyAndValue(key, value, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeKeyAndValue(const KT& key, const VT& value, sl_bool flagRemoveAllMatches)
{
	ObjectLocker lock(this);
	return tree.removeKeyAndValue(key, value, flagRemoveAllMatches);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeAll_NoLock()
{
	return tree.removeAll();
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::contains_NoLock(const KT& key) const
{
	return tree.search(key, sl_null);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::contains(const KT& key) const
{
	ObjectLocker lock(this);
	return tree.search(key, sl_null);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::contains_NoLock(const KT& key, const VT& value) const
{
	return tree.search(key, value, sl_null);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::contains(const KT& key, const VT& value) const
{
	ObjectLocker lock(this);
	return tree.search(key, value, sl_null);
}

template <class KT, class VT, class COMPARE>
IMap<KT, VT>* TreeMap<KT, VT, COMPARE>::duplicate_NoLock() const
{
	TreeMap<KT, VT, COMPARE>* ret = new TreeMap<KT, VT, COMPARE>;
	if (ret) {
		TreePosition pos;
		KT key;
		VT value;
		while (tree.getNextPosition(pos, &key, &value)) {
			if (!(ret->tree.put(key, value, MapPutMode::AddAlways))) {
				delete ret;
				return sl_null;
			}
		}
		return ret;
	}
	return sl_null;
}

template <class KT, class VT, class COMPARE>
Iterator<KT> TreeMap<KT, VT, COMPARE>::keyIteratorWithRefer(const Referable* refer) const
{
	return new TreeMapKeyIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List<KT> TreeMap<KT, VT, COMPARE>::keys_NoLock() const
{
	CList<KT>* ret = new CList<KT>;
	if (ret) {
		TreePosition pos;
		KT key;
		while (tree.getNextPosition(pos, &key, sl_null)) {
			if (!(ret->add_NoLock(key))) {
				delete ret;
				return List<KT>::null();
			}
		}
		return ret;
	}
	return List<KT>::null();
}

template <class KT, class VT, class COMPARE>
Iterator<VT> TreeMap<KT, VT, COMPARE>::valueIteratorWithRefer(const Referable* refer) const
{
	return new TreeMapValueIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List<VT> TreeMap<KT, VT, COMPARE>::values_NoLock() const
{
	CList<VT>* ret = new CList<VT>;
	if (ret) {
		TreePosition pos;
		VT value;
		while (tree.getNextPosition(pos, sl_null, &value)) {
			if (!(ret->add_NoLock(value))) {
				delete ret;
				return List<VT>::null();
			}
		}
		return ret;
	}
	return List<VT>::null();
}

template <class KT, class VT, class COMPARE>
Iterator< Pair<KT, VT> > TreeMap<KT, VT, COMPARE>::iteratorWithRefer(const Referable* refer) const
{
	return new TreeMapIterator<KT, VT, COMPARE>(this, refer);
}

template <class KT, class VT, class COMPARE>
List< Pair<KT, VT> > TreeMap<KT, VT, COMPARE>::pairs_NoLock() const
{
	CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
	if (ret) {
		TreePosition pos;
		Pair<KT, VT> pair;
		while (tree.getNextPosition(pos, &(pair.key), &(pair.value))) {
			if (!(ret->add_NoLock(pair))) {
				delete ret;
				return List< Pair<KT, VT> >::null();
			}
		}
		return ret;
	}
	return List< Pair<KT, VT> >::null();
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(Map, SafeMap, IMap, ref)

template <class KT, class VT>
Map<KT, VT> Map<KT, VT>::createList()
{
	return ListMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
Map<KT, VT> Map<KT, VT>::createListBy()
{
	return ListMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
Map<KT, VT> Map<KT, VT>::createHash(sl_uint32 initialCapacity)
{
	return HashMap<KT, VT>::create(initialCapacity);
}

template <class KT, class VT>
template <class HASH>
Map<KT, VT> Map<KT, VT>::createHashBy(sl_uint32 initialCapacity)
{
	return HashMap<KT, VT, HASH>::create(initialCapacity);
}

template <class KT, class VT>
Map<KT, VT> Map<KT, VT>::createTree()
{
	return TreeMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
Map<KT, VT> Map<KT, VT>::createTreeBy()
{
	return TreeMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
void Map<KT, VT>::initList()
{
	ref = ListMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
void Map<KT, VT>::initListBy()
{
	ref = ListMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
void Map<KT, VT>::initHash(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT>::create(initialCapacity);
}

template <class KT, class VT>
template <class HASH>
void Map<KT, VT>::initHashBy(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT, HASH>::create(initialCapacity);
}

template <class KT, class VT>
void Map<KT, VT>::initTree()
{
	ref = TreeMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
void Map<KT, VT>::initTreeBy()
{
	ref = TreeMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
sl_size Map<KT, VT>::getCount() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getCount();
	}
	return 0;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::isEmpty() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return (obj->getCount()) == 0;
	}
	return sl_true;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::isNotEmpty() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return (obj->getCount()) > 0;
	}
	return sl_false;
}

template <class KT, class VT>
VT* Map<KT, VT>::getItemPtr(const KT& key) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getItemPtr(key);
	}
	return sl_null;
}

template <class KT, class VT>
VT* Map<KT, VT>::getNewItemPtr(const KT& key)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (!obj) {
		obj = IMap<KT, VT>::createDefault();
		if (obj) {
			ref = obj;
		} else {
			return sl_null;
		}
	}
	VT* ret = obj->getItemPtr(key);
	if (!ret) {
		VT t;
		obj->put(key, t);
		ret = obj->getItemPtr(key);
	}
	return ret;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::get_NoLock(const KT& key, VT* _out) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->get_NoLock(key, _out);
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::get(const KT& key, VT* _out) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->get(key, _out);
	}
	return sl_false;
}

template <class KT, class VT>
VT Map<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getValue_NoLock(key, def);
	}
	return def;
}

template <class KT, class VT>
VT Map<KT, VT>::getValue(const KT& key, const VT& def) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getValue(key, def);
	}
	return def;
}

template <class KT, class VT>
List<VT> Map<KT, VT>::getValues_NoLock(const KT& key) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getValues_NoLock(key);
	}
	return List<VT>::null();
}

template <class KT, class VT>
List<VT> Map<KT, VT>::getValues(const KT& key) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getValues(key);
	}
	return List<VT>::null();
}

template <class KT, class VT>
sl_bool Map<KT, VT>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->put_NoLock(key, value, mode, pFlagExist);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				return obj->put_NoLock(key, value, mode, pFlagExist);
			}
		}
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->put(key, value, mode, pFlagExist);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				return obj->put_NoLock(key, value, mode, pFlagExist);
			}
		}
	}
	return sl_false;
}

template <class KT, class VT>
void Map<KT, VT>::put(const Map<KT, VT>& other, MapPutMode mode)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		obj->put(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				obj->put(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
template <class _KT, class _VT>
void Map<KT, VT>::put(const Map<_KT, _VT>& other, MapPutMode mode)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		obj->put(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				obj->put(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
void Map<KT, VT>::put(const SafeMap<KT, VT>& other, MapPutMode mode)
{
	put(Map<KT, VT>(other), mode);
}

template <class KT, class VT>
template <class _KT, class _VT>
void Map<KT, VT>::put(const SafeMap<_KT, _VT>& other, MapPutMode mode)
{
	put(Map<_KT, _VT>(other), mode);
}

template <class KT, class VT>
sl_size Map<KT, VT>::remove_NoLock(const KT& key, sl_bool flagRemoveAllMatches) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->remove_NoLock(key, flagRemoveAllMatches);
	}
	return 0;
}

template <class KT, class VT>
sl_size Map<KT, VT>::remove(const KT& key, sl_bool flagRemoveAllMatches) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->remove(key, flagRemoveAllMatches);
	}
	return 0;
}

template <class KT, class VT>
sl_size Map<KT, VT>::removeAll_NoLock() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->removeAll_NoLock();
	}
	return 0;
}

template <class KT, class VT>
sl_size Map<KT, VT>::removeAll() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->removeAll();
	}
	return 0;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::contains_NoLock(const KT& key) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->contains_NoLock(key);
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::contains(const KT& key) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->contains(key);
	}
	return sl_false;
}

template <class KT, class VT>
Map<KT, VT> Map<KT, VT>::duplicate_NoLock() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate_NoLock();
	}
	return Map<KT, VT>::null();
}

template <class KT, class VT>
Map<KT, VT> Map<KT, VT>::duplicate() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->duplicate();
	}
	return Map<KT, VT>::null();
}

template <class KT, class VT>
Iterator<KT> Map<KT, VT>::keyIterator() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->keyIteratorWithRefer(obj);
	}
	return Iterator<KT>::null();
}

template <class KT, class VT>
List<KT> Map<KT, VT>::keys_NoLock() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->keys_NoLock();
	}
	return List<KT>::null();
}

template <class KT, class VT>
List<KT> Map<KT, VT>::keys() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->keys();
	}
	return List<KT>::null();
}

template <class KT, class VT>
Iterator<VT> Map<KT, VT>::valueIterator() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->valueIteratorWithRefer(obj);
	}
	return Iterator<VT>::null();
}

template <class KT, class VT>
List<VT> Map<KT, VT>::values_NoLock() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->values_NoLock();
	}
	return List<VT>::null();
}

template <class KT, class VT>
List<VT> Map<KT, VT>::values() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->values();
	}
	return List<VT>::null();
}

template <class KT, class VT>
Iterator< Pair<KT, VT> > Map<KT, VT>::iterator() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->iteratorWithRefer(obj);
	}
	return Iterator< Pair<KT, VT> >::null();
}

template <class KT, class VT>
List< Pair<KT, VT> > Map<KT, VT>::pairs_NoLock() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->pairs_NoLock();
	}
	return List< Pair<KT, VT> >::null();
}

template <class KT, class VT>
List< Pair<KT, VT> > Map<KT, VT>::pairs() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->pairs();
	}
	return List< Pair<KT, VT> >::null();
}

template <class KT, class VT>
const Mutex* Map<KT, VT>::getLocker() const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->getLocker();
	}
	return sl_null;
}


SLIB_DEFINE_TEMPLATE_REF_WRAPPER(SafeMap, Map, IMap, ref)

template <class KT, class VT>
void SafeMap<KT, VT>::initList()
{
	ref = ListMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
void SafeMap<KT, VT>::initListBy()
{
	ref = ListMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
void SafeMap<KT, VT>::initHash(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT>::create(initialCapacity);
}

template <class KT, class VT>
template <class HASH>
void SafeMap<KT, VT>::initHashBy(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT, HASH>::create(initialCapacity);
}

template <class KT, class VT>
void SafeMap<KT, VT>::initTree()
{
	ref = TreeMap<KT, VT>::create();
}

template <class KT, class VT>
template <class COMPARE>
void SafeMap<KT, VT>::initTreeBy()
{
	ref = TreeMap<KT, VT, COMPARE>::create();
}

template <class KT, class VT>
sl_size SafeMap<KT, VT>::getCount() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount();
	}
	return 0;
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::isEmpty() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return (obj->getCount()) == 0;
	}
	return sl_true;
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::isNotEmpty() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return (obj->getCount()) > 0;
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::get(const KT& key, VT* _out) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->get(key, _out);
	}
	return sl_false;
}

template <class KT, class VT>
VT SafeMap<KT, VT>::getValue(const KT& key, const VT& def) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getValue(key, def);
	}
	return def;
}

template <class KT, class VT>
List<VT> SafeMap<KT, VT>::getValues(const KT& key) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getValues(key);
	}
	return List<VT>::null();
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->put(key, value, mode, pFlagExist);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->put(key, value, mode, pFlagExist);
			}
			obj = IMap<KT, VT>::createDefault();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				return obj->put(key, value, mode, pFlagExist);
			}
		}
	}
	return sl_false;
}

template <class KT, class VT>
void SafeMap<KT, VT>::put(const Map<KT, VT>& other, MapPutMode mode)
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		obj->put(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->put(other.ref.ptr, mode);
			}
			obj = IMap<KT, VT>::createDefault();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->put(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
template <class _KT, class _VT>
void SafeMap<KT, VT>::put(const Map<_KT, _VT>& other, MapPutMode mode)
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		obj->put(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->put(other.ref.ptr, mode);
			}
			obj = IMap<KT, VT>::createDefault();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->put(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
void SafeMap<KT, VT>::put(const SafeMap<KT, VT>& other, MapPutMode mode)
{
	put(Map<KT, VT>(other), mode);
}

template <class KT, class VT>
template <class _KT, class _VT>
void SafeMap<KT, VT>::put(const SafeMap<_KT, _VT>& other, MapPutMode mode)
{
	put(Map<_KT, _VT>(other), mode);
}

template <class KT, class VT>
sl_size SafeMap<KT, VT>::remove(const KT& key, sl_bool flagRemoveAllMatches) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->remove(key, flagRemoveAllMatches);
	}
	return 0;
}

template <class KT, class VT>
sl_size SafeMap<KT, VT>::removeAll() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeAll();
	}
	return 0;
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::contains(const KT& key) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->contains(key);
	}
	return sl_false;
}

template <class KT, class VT>
Map<KT, VT> SafeMap<KT, VT>::duplicate() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return Map<KT, VT>::null();
}

template <class KT, class VT>
List<KT> SafeMap<KT, VT>::keys() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->keys();
	}
	return List<KT>::null();
}

template <class KT, class VT>
List<VT> SafeMap<KT, VT>::values() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->values();
	}
	return List<VT>::null();
}

template <class KT, class VT>
List< Pair<KT, VT> > SafeMap<KT, VT>::pairs() const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->pairs();
	}
	return List< Pair<KT, VT> >::null();
}


template <class KT, class VT, class COMPARE>
ListMapKeyIterator<KT, VT, COMPARE>::ListMapKeyIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapKeyIterator<KT, VT, COMPARE>::hasNext()
{
	return (m_index < m_map->getCount());
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapKeyIterator<KT, VT, COMPARE>::next(KT* _out)
{
	Pair<KT, VT> pair;
	if (m_map->list.getItem(m_index, &pair)) {
		if (_out) {
			*_out = pair.key;
		}
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg ListMapKeyIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class COMPARE>
ListMapValueIterator<KT, VT, COMPARE>::ListMapValueIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapValueIterator<KT, VT, COMPARE>::hasNext()
{
	return (m_index < m_map->getCount());
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapValueIterator<KT, VT, COMPARE>::next(VT* _out)
{
	Pair<KT, VT> pair;
	if (m_map->list.getItem(m_index, &pair)) {
		if (_out) {
			*_out = pair.value;
		}
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg ListMapValueIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class COMPARE>
ListMapIterator<KT, VT, COMPARE>::ListMapIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapIterator<KT, VT, COMPARE>::hasNext()
{
	return (m_index < m_map->getCount());
}

template <class KT, class VT, class COMPARE>
sl_bool ListMapIterator<KT, VT, COMPARE>::next(Pair<KT, VT>* _out)
{
	if (m_map->list.getItem(m_index, _out)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg ListMapIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class HASH>
HashMapKeyIterator<KT, VT, HASH>::HashMapKeyIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH>
sl_bool HashMapKeyIterator<KT, VT, HASH>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos);
}

template <class KT, class VT, class HASH>
sl_bool HashMapKeyIterator<KT, VT, HASH>::next(KT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, _out, sl_null)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH>
sl_reg HashMapKeyIterator<KT, VT, HASH>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class HASH>
HashMapValueIterator<KT, VT, HASH>::HashMapValueIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH>
sl_bool HashMapValueIterator<KT, VT, HASH>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos);
}

template <class KT, class VT, class HASH>
sl_bool HashMapValueIterator<KT, VT, HASH>::next(VT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, sl_null, _out)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH>
sl_reg HashMapValueIterator<KT, VT, HASH>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class HASH>
HashMapIterator<KT, VT, HASH>::HashMapIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH>
sl_bool HashMapIterator<KT, VT, HASH>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos, sl_null, sl_null);
}

template <class KT, class VT, class HASH>
sl_bool HashMapIterator<KT, VT, HASH>::next(Pair<KT, VT>* out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, &(out->key), &(out->value))) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH>
sl_reg HashMapIterator<KT, VT, HASH>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class COMPARE>
TreeMapKeyIterator<KT, VT, COMPARE>::TreeMapKeyIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapKeyIterator<KT, VT, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	TreePosition pos = m_pos;
	return m_map->tree.getNextPosition(pos);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapKeyIterator<KT, VT, COMPARE>::next(KT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->tree.getNextPosition(m_pos, _out, sl_null)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg TreeMapKeyIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class COMPARE>
TreeMapValueIterator<KT, VT, COMPARE>::TreeMapValueIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapValueIterator<KT, VT, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	TreePosition pos = m_pos;
	return m_map->tree.getNextPosition(pos);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapValueIterator<KT, VT, COMPARE>::next(VT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->tree.getNextPosition(m_pos, sl_null, _out)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg TreeMapValueIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class COMPARE>
TreeMapIterator<KT, VT, COMPARE>::TreeMapIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapIterator<KT, VT, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	TreePosition pos = m_pos;
	return m_map->tree.getNextPosition(pos);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMapIterator<KT, VT, COMPARE>::next(Pair<KT, VT>* _out)
{
	ObjectLocker lock(m_map);
	if (_out) {
		if (m_map->tree.getNextPosition(m_pos, &(_out->key), &(_out->value))) {
			m_index++;
			return sl_true;
		}
	} else {
		if (m_map->tree.getNextPosition(m_pos, sl_null, sl_null)) {
			m_index++;
			return sl_true;
		}
	}
	return sl_false;
}

template <class KT, class VT, class COMPARE>
sl_reg TreeMapIterator<KT, VT, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}

SLIB_NAMESPACE_END

#endif
