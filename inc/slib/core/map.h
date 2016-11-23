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
	
	void putAll(IMap<KT, VT>* other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void putAll(IMap<_KT, _VT>* other, MapPutMode mode = MapPutMode::Default);
	
	virtual sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) = 0;
	
	sl_bool remove(const KT& key, VT* outValue = sl_null);
	
	virtual sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) = 0;
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);
	
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
	
	// range-based for loop
	IteratorPosition< Pair<KT, VT> > begin() const;
	
	IteratorPosition< Pair<KT, VT> > end() const;
	
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
	sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);
	
	sl_bool remove(const KT& key, VT* outValue = sl_null);
	
	// override
	sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);
	
	sl_bool removeValue_NoLock(const KT& key, const VT& value);
	
	sl_bool removeValue(const KT& key, const VT& value);
	
	sl_size removeValues_NoLock(const KT& key, const VT& value);
	
	sl_size removeValues(const KT& key, const VT& value);
	
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


template < class KT, class VT, class HASH = Hash<KT>, class COMPARE = Compare<KT> >
class SLIB_EXPORT HashMap : public IMap<KT, VT>
{
public:
	HashTable<KT, VT, HASH, COMPARE> table;

public:
	HashMap(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);

public:
	static HashMap<KT, VT, HASH, COMPARE>* create(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);

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
	sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);
	
	sl_bool remove(const KT& key, VT* outValue = sl_null);
	
	// override
	sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);
	
	sl_bool removeValue_NoLock(const KT& key, const VT& value);
	
	sl_bool removeValue(const KT& key, const VT& value);
	
	sl_size removeValues_NoLock(const KT& key, const VT& value);
	
	sl_size removeValues(const KT& key, const VT& value);
	
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
	sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null);
	
	sl_bool remove(const KT& key, VT* outValue = sl_null);
	
	// override
	sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null);
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null);
	
	sl_bool removeValue_NoLock(const KT& key, const VT& value);
	
	sl_bool removeValue(const KT& key, const VT& value);
	
	sl_size removeValues_NoLock(const KT& key, const VT& value);
	
	sl_size removeValues(const KT& key, const VT& value);
	
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
	
	template < class HASH, class COMPARE = Compare<KT> >
	static Map<KT, VT> createHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	static Map<KT, VT> createTree();
	
	template <class COMPARE>
	static Map<KT, VT> createTreeBy();
	
public:
	void initList();
	
	template <class COMPARE>
	void initListBy();
	
	void initHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY);
	
	template < class HASH, class COMPARE = Compare<KT> >
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
	
	void putAll(const Map<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void putAll(const Map<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	void putAll(const SafeMap<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void putAll(const SafeMap<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	sl_bool remove_NoLock(const KT& key, VT* outValue = sl_null) const;
	
	sl_bool remove(const KT& key, VT* outValue = sl_null) const;
	
	sl_size removeItems_NoLock(const KT& key, List<VT>* outValues = sl_null) const;
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const;
	
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
	
	// range-based for loop
	IteratorPosition< Pair<KT, VT> > begin() const;
	
	IteratorPosition< Pair<KT, VT> > end() const;

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
	
	template < class HASH, class COMPARE = Compare<KT> >
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
	
	void putAll(const Map<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void putAll(const Map<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	void putAll(const SafeMap<KT, VT>& other, MapPutMode mode = MapPutMode::Default);
	
	template <class _KT, class _VT>
	void putAll(const SafeMap<_KT, _VT>& other, MapPutMode mode = MapPutMode::Default);
	
	sl_bool remove(const KT& key, VT* outValue = sl_null) const;
	
	sl_size removeItems(const KT& key, List<VT>* outValues = sl_null) const;
	
	sl_size removeAll() const;
	
	sl_bool contains(const KT& key) const;

	Map<KT, VT> duplicate() const;
	
	Iterator<KT> keyIterator() const;
	
	List<KT> keys() const;
	
	Iterator<VT> valueIterator() const;
	
	List<VT> values() const;
	
	Iterator< Pair<KT, VT> > iterator() const;
	
	List< Pair<KT, VT> > pairs() const;
	
	// range-based for loop
	IteratorPosition< Pair<KT, VT> > begin() const;
	
	IteratorPosition< Pair<KT, VT> > end() const;

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


template < class KT, class VT, class HASH = Hash<KT>, class COMPARE = Compare<KT> >
class HashMapKeyIterator : public IIterator<KT>
{
protected:
	const HashMap<KT, VT, HASH, COMPARE>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapKeyIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(KT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class HASH = Hash<KT>, class COMPARE = Compare<KT> >
class HashMapValueIterator : public IIterator<VT>
{
protected:
	const HashMap<KT, VT, HASH, COMPARE>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapValueIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer);
	
public:
	// override
	sl_bool hasNext();
	
	// override
	sl_bool next(VT* _out);
	
	// override
	sl_reg getIndex();
	
};


template < class KT, class VT, class HASH = Hash<KT>, class COMPARE = Compare<KT> >
class HashMapIterator : public IIterator< Pair<KT, VT> >
{
protected:
	const HashMap<KT, VT, HASH, COMPARE>* m_map;
	HashPosition m_pos;
	sl_size m_index;
	Ref<Referable> m_refer;
	
public:
	HashMapIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer);
	
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
void IMap<KT, VT>::putAll(IMap<KT, VT>* other, MapPutMode mode)
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
void IMap<KT, VT>::putAll(IMap<_KT, _VT>* other, MapPutMode mode)
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
sl_bool IMap<KT, VT>::remove(const KT& key, VT* outValue)
{
	ObjectLocker lock(this);
	return remove_NoLock(key, outValue);
}

template <class KT, class VT>
sl_size IMap<KT, VT>::removeItems(const KT& key, List<VT>* outValues)
{
	ObjectLocker lock(this);
	return removeItems_NoLock(key, outValues);
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

template <class KT, class VT>
SLIB_INLINE IteratorPosition< Pair<KT, VT> > IMap<KT, VT>::begin() const
{
	return iterator().begin();
}

template <class KT, class VT>
SLIB_INLINE IteratorPosition< Pair<KT, VT> > IMap<KT, VT>::end() const
{
	return IteratorPosition< Pair<KT, VT> >();
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
sl_bool ListMap<KT, VT, COMPARE>::remove_NoLock(const KT& key, VT* outValue)
{
	if (outValue) {
		Pair<KT, VT> pair;
		if (list.template removeValueT_NoLock<KT, PairKeyCompare>(key, &pair)) {
			*outValue = pair.value;
			return sl_true;
		}
		return sl_false;
	} else {
		return list.template removeValueT_NoLock<KT, PairKeyCompare>(key);
	}
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::remove(const KT& key, VT* outValue)
{
	ObjectLocker lock(this);
	return remove_NoLock(key, outValue);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeItems_NoLock(const KT& key, List<VT>* outValues)
{
	if (outValues) {
		List< Pair<KT, VT> > pairs;
		sl_size n = list.template removeValuesT_NoLock<KT, PairKeyCompare>(key, &pairs);
		if (n) {
			ListItems< Pair<KT, VT> > p(pairs);
			for (sl_size i = 0; i < p.count; i++) {
				outValues->add_NoLock(p[i].value);
			}
		}
		return n;
	} else {
		return list.template removeValuesT_NoLock<KT, PairKeyCompare>(key);
	}
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeItems(const KT& key, List<VT>* outValues)
{
	ObjectLocker lock(this);
	return removeItems_NoLock(key, outValues);
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::removeValue_NoLock(const KT& key, const VT& value)
{
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	return list.template removeValueT_NoLock<Pair<KT, VT>, PairCompare>(pair);
}

template <class KT, class VT, class COMPARE>
sl_bool ListMap<KT, VT, COMPARE>::removeValue(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return removeValue_NoLock(key, value);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeValues_NoLock(const KT& key, const VT& value)
{
	Pair<KT, VT> pair;
	pair.key = key;
	pair.value = value;
	return list.template removeValuesT_NoLock<Pair<KT, VT>, PairCompare>(pair);
}

template <class KT, class VT, class COMPARE>
sl_size ListMap<KT, VT, COMPARE>::removeValues(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return removeValues_NoLock(key, value);
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


template <class KT, class VT, class HASH, class COMPARE>
HashMap<KT, VT, HASH, COMPARE>::HashMap(sl_uint32 capacity) : table(capacity)
{
}

template <class KT, class VT, class HASH, class COMPARE>
HashMap<KT, VT, HASH, COMPARE>* HashMap<KT, VT, HASH, COMPARE>::create(sl_uint32 capacity)
{
	HashMap<KT, VT, HASH, COMPARE>* ret = new HashMap<KT, VT, HASH, COMPARE>(capacity);
	if (ret) {
		if (ret->table.getCapacity() > 0) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::getCount() const
{
	return (sl_size)(table.getCount());
}

template <class KT, class VT, class HASH, class COMPARE>
VT* HashMap<KT, VT, HASH, COMPARE>::getItemPtr(const KT& key) const
{
	return table.getItemPtr(key);
}

template <class KT, class VT, class HASH, class COMPARE>
List<VT> HashMap<KT, VT, HASH, COMPARE>::getValues_NoLock(const KT& key) const
{
	return table.getValues(key);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
{
	return table.put(key, value, mode, pFlagExist);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::addIfNewKeyAndValue_NoLock(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	return table.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::addIfNewKeyAndValue(const KT& key, const VT& value, sl_bool* pFlagExist)
{
	ObjectLocker lock(this);
	return table.addIfNewKeyAndValue(key, value, pFlagExist);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::remove_NoLock(const KT& key, VT* outValue)
{
	return table.remove(key, outValue);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::remove(const KT& key, VT* outValue)
{
	ObjectLocker lock(this);
	return table.remove(key, outValue);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::removeItems_NoLock(const KT& key, List<VT>* outValues)
{
	return table.removeItems(key, outValues);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::removeItems(const KT& key, List<VT>* outValues)
{
	ObjectLocker lock(this);
	return table.removeItems(key, outValues);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::removeValue_NoLock(const KT& key, const VT& value)
{
	return table.removeValue(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::removeValue(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return table.removeValue(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::removeValues_NoLock(const KT& key, const VT& value)
{
	return table.removeValues(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::removeValues(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return table.removeValues(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_size HashMap<KT, VT, HASH, COMPARE>::removeAll_NoLock()
{
	return table.removeAll();
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::contains_NoLock(const KT& key) const
{
	return table.search(key);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::contains(const KT& key) const
{
	ObjectLocker lock(this);
	return table.search(key);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::contains_NoLock(const KT& key, const VT& value) const
{
	return table.search(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMap<KT, VT, HASH, COMPARE>::contains(const KT& key, const VT& value) const
{
	ObjectLocker lock(this);
	return table.search(key, value);
}

template <class KT, class VT, class HASH, class COMPARE>
IMap<KT, VT>* HashMap<KT, VT, HASH, COMPARE>::duplicate_NoLock() const
{
	HashMap<KT, VT, HASH, COMPARE>* ret = new HashMap<KT, VT, HASH, COMPARE>;
	if (ret) {
		if (ret->table.copyFrom(&table)) {
			return ret;
		}
		delete ret;
	}
	return sl_null;
}

template <class KT, class VT, class HASH, class COMPARE>
Iterator<KT> HashMap<KT, VT, HASH, COMPARE>::keyIteratorWithRefer(const Referable* refer) const
{
	return new HashMapKeyIterator<KT, VT, HASH, COMPARE>(this, refer);
}

template <class KT, class VT, class HASH, class COMPARE>
List<KT> HashMap<KT, VT, HASH, COMPARE>::keys_NoLock() const
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

template <class KT, class VT, class HASH, class COMPARE>
Iterator<VT> HashMap<KT, VT, HASH, COMPARE>::valueIteratorWithRefer(const Referable* refer) const
{
	return new HashMapValueIterator<KT, VT, HASH, COMPARE>(this, refer);
}

template <class KT, class VT, class HASH, class COMPARE>
List<VT> HashMap<KT, VT, HASH, COMPARE>::values_NoLock() const
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

template <class KT, class VT, class HASH, class COMPARE>
Iterator< Pair<KT, VT> > HashMap<KT, VT, HASH, COMPARE>::iteratorWithRefer(const Referable* refer) const
{
	return new HashMapIterator<KT, VT, HASH, COMPARE>(this, refer);
}

template <class KT, class VT, class HASH, class COMPARE>
List< Pair<KT, VT> > HashMap<KT, VT, HASH, COMPARE>::pairs_NoLock() const
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
sl_bool TreeMap<KT, VT, COMPARE>::remove_NoLock(const KT& key, VT* outValue)
{
	return tree.remove(key, outValue);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::remove(const KT& key, VT* outValue)
{
	ObjectLocker lock(this);
	return tree.remove(key, outValue);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeItems_NoLock(const KT& key, List<VT>* outValues)
{
	return tree.removeItems(key, outValues);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeItems(const KT& key, List<VT>* outValues)
{
	ObjectLocker lock(this);
	return tree.removeItems(key, outValues);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::removeValue_NoLock(const KT& key, const VT& value)
{
	return tree.removeValue(key, value);
}

template <class KT, class VT, class COMPARE>
sl_bool TreeMap<KT, VT, COMPARE>::removeValue(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return tree.removeValue(key, value);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeValues_NoLock(const KT& key, const VT& value)
{
	return tree.removeValues(key, value);
}

template <class KT, class VT, class COMPARE>
sl_size TreeMap<KT, VT, COMPARE>::removeValues(const KT& key, const VT& value)
{
	ObjectLocker lock(this);
	return tree.removeValues(key, value);
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
template <class HASH, class COMPARE>
Map<KT, VT> Map<KT, VT>::createHashBy(sl_uint32 initialCapacity)
{
	return HashMap<KT, VT, HASH, COMPARE>::create(initialCapacity);
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
template <class HASH, class COMPARE>
void Map<KT, VT>::initHashBy(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT, HASH, COMPARE>::create(initialCapacity);
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
void Map<KT, VT>::putAll(const Map<KT, VT>& other, MapPutMode mode)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		obj->putAll(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				obj->putAll(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
template <class _KT, class _VT>
void Map<KT, VT>::putAll(const Map<_KT, _VT>& other, MapPutMode mode)
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		obj->putAll(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
				obj->putAll(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
void Map<KT, VT>::putAll(const SafeMap<KT, VT>& other, MapPutMode mode)
{
	putAll(Map<KT, VT>(other), mode);
}

template <class KT, class VT>
template <class _KT, class _VT>
void Map<KT, VT>::putAll(const SafeMap<_KT, _VT>& other, MapPutMode mode)
{
	putAll(Map<_KT, _VT>(other), mode);
}

template <class KT, class VT>
sl_bool Map<KT, VT>::remove_NoLock(const KT& key, VT* outValue) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->remove_NoLock(key, outValue);
	}
	return sl_false;
}

template <class KT, class VT>
sl_bool Map<KT, VT>::remove(const KT& key, VT* outValue) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->remove(key, outValue);
	}
	return sl_false;
}

template <class KT, class VT>
sl_size Map<KT, VT>::removeItems_NoLock(const KT& key, List<VT>* outValues) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->removeItems_NoLock(key, outValues);
	}
	return 0;
}

template <class KT, class VT>
sl_size Map<KT, VT>::removeItems(const KT& key, List<VT>* outValues) const
{
	IMap<KT, VT>* obj = ref.ptr;
	if (obj) {
		return obj->removeItems(key, outValues);
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
SLIB_INLINE IteratorPosition< Pair<KT, VT> > Map<KT, VT>::begin() const
{
	return iterator().begin();
}

template <class KT, class VT>
SLIB_INLINE IteratorPosition< Pair<KT, VT> > Map<KT, VT>::end() const
{
	return IteratorPosition< Pair<KT, VT> >();
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
template <class HASH, class COMPARE>
void SafeMap<KT, VT>::initHashBy(sl_uint32 initialCapacity)
{
	ref = HashMap<KT, VT, HASH, COMPARE>::create(initialCapacity);
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
void SafeMap<KT, VT>::putAll(const Map<KT, VT>& other, MapPutMode mode)
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		obj->putAll(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->putAll(other.ref.ptr, mode);
			}
			obj = IMap<KT, VT>::createDefault();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->putAll(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
template <class _KT, class _VT>
void SafeMap<KT, VT>::putAll(const Map<_KT, _VT>& other, MapPutMode mode)
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		obj->putAll(other.ref.ptr, mode);
	} else {
		if (mode != MapPutMode::ReplaceExisting) {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = ref;
			if (obj.isNotNull()) {
				lock.unlock();
				obj->putAll(other.ref.ptr, mode);
			}
			obj = IMap<KT, VT>::createDefault();
			if (obj.isNotNull()) {
				ref = obj;
				lock.unlock();
				obj->putAll(other.ref.ptr, mode);
			}
		}
	}
}

template <class KT, class VT>
void SafeMap<KT, VT>::putAll(const SafeMap<KT, VT>& other, MapPutMode mode)
{
	putAll(Map<KT, VT>(other), mode);
}

template <class KT, class VT>
template <class _KT, class _VT>
void SafeMap<KT, VT>::putAll(const SafeMap<_KT, _VT>& other, MapPutMode mode)
{
	putAll(Map<_KT, _VT>(other), mode);
}

template <class KT, class VT>
sl_bool SafeMap<KT, VT>::remove(const KT& key, VT* outValue) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->remove(key, outValue);
	}
	return sl_false;
}

template <class KT, class VT>
sl_size SafeMap<KT, VT>::removeItems(const KT& key, List<VT>* outValues) const
{
	Ref< IMap<KT, VT> > obj(ref);
	if (obj.isNotNull()) {
		return obj->removeItems(key, outValues);
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
Iterator<KT> SafeMap<KT, VT>::keyIterator() const
{
	Map<KT, VT> map(*this);
	return map.keyIterator();
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
Iterator<VT> SafeMap<KT, VT>::valueIterator() const
{
	Map<KT, VT> map(*this);
	return map.valueIterator();
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
Iterator< Pair<KT, VT> > SafeMap<KT, VT>::iterator() const
{
	Map<KT, VT> map(*this);
	return map.iterator();
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

template <class KT, class VT>
SLIB_INLINE IteratorPosition< Pair<KT, VT> > SafeMap<KT, VT>::begin() const
{
	return iterator().begin();
}

template <class KT, class VT>
SLIB_INLINE IteratorPosition< Pair<KT, VT> > SafeMap<KT, VT>::end() const
{
	return IteratorPosition< Pair<KT, VT> >();
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


template <class KT, class VT, class HASH, class COMPARE>
HashMapKeyIterator<KT, VT, HASH, COMPARE>::HashMapKeyIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapKeyIterator<KT, VT, HASH, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapKeyIterator<KT, VT, HASH, COMPARE>::next(KT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, _out, sl_null)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_reg HashMapKeyIterator<KT, VT, HASH, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class HASH, class COMPARE>
HashMapValueIterator<KT, VT, HASH, COMPARE>::HashMapValueIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapValueIterator<KT, VT, HASH, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapValueIterator<KT, VT, HASH, COMPARE>::next(VT* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, sl_null, _out)) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_reg HashMapValueIterator<KT, VT, HASH, COMPARE>::getIndex()
{
	return (sl_reg)m_index - 1;
}


template <class KT, class VT, class HASH, class COMPARE>
HashMapIterator<KT, VT, HASH, COMPARE>::HashMapIterator(const HashMap<KT, VT, HASH, COMPARE>* map, const Referable* refer)
{
	m_map = map;
	m_index = 0;
	m_refer = refer;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapIterator<KT, VT, HASH, COMPARE>::hasNext()
{
	ObjectLocker lock(m_map);
	HashPosition pos = m_pos;
	return m_map->table.getNextPosition(pos, sl_null, sl_null);
}

template <class KT, class VT, class HASH, class COMPARE>
sl_bool HashMapIterator<KT, VT, HASH, COMPARE>::next(Pair<KT, VT>* _out)
{
	ObjectLocker lock(m_map);
	if (m_map->table.getNextPosition(m_pos, &(_out->key), &(_out->value))) {
		m_index++;
		return sl_true;
	}
	return sl_false;
}

template <class KT, class VT, class HASH, class COMPARE>
sl_reg HashMapIterator<KT, VT, HASH, COMPARE>::getIndex()
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
