#ifndef CHECKHEADER_SLIB_CORE_MAP
#define CHECKHEADER_SLIB_CORE_MAP

#include "definition.h"
#include "object.h"
#include "iterator.h"
#include "list.h"
#include "hashtable.h"
#include "tree.h"

/************************************************************************/
/* IMap Interface Declaration                                           */
/************************************************************************/
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
	SLIB_DECLARE_OBJECT(IMap, Object)
public:
	IMap() {}
	virtual sl_bool get(const KT& key, VT* out = sl_null) = 0;
	virtual VT* getItemPtr(const KT& key) = 0;
	virtual VT getValue(const KT& key, const VT& def) = 0;
	virtual List<VT> getValues(const KT& key) = 0;
	virtual sl_bool put(const KT& key, const VT& value, sl_bool flagReplace) = 0;
	virtual sl_bool remove(const KT& key, sl_bool flagAllKeys) = 0;
	virtual void clear() = 0;
	virtual sl_bool containsKey(const KT& key) = 0;
	virtual Iterator< Pair<KT, VT> > iterator();
	virtual Iterator<KT> keyIterator();
	virtual Iterator<VT> valueIterator();
	virtual Ref< IMap<KT, VT> > duplicate() = 0;
	virtual sl_size getCount() = 0;
};

template <class KT, class VT>
class SLIB_EXPORT MapPairIterator : public IIterator< Pair<KT, VT> >
{
private:
	Iterator<KT> m_key;
	Iterator<VT> m_value;

public:
	MapPairIterator(IMap<KT, VT>* map)
	{
		m_key = map->keyIterator();
		m_value = map->valueIterator();
	}

	~MapPairIterator()
	{
	}

	sl_bool hasNext()
	{
		return m_key.hasNext();
	}

	sl_bool next(Pair<KT, VT>* out)
	{
		sl_bool b = m_key.next(&(out->key));
		b = b && m_value.next(&(out->value));
		return b;
	}

	sl_reg getIndex()
	{
		return m_key.getIndex();
	}
};

template <class KT, class VT>
Iterator< Pair<KT, VT> > IMap<KT, VT>::iterator()
{
	return new MapPairIterator<KT, VT>(this);
}

template <class KT, class VT>
class SLIB_EXPORT MapKeyIterator : public IIterator<KT>
{
private:
	Iterator< Pair<KT, VT> > m_pair;

public:
	MapKeyIterator(IMap<KT, VT>* map)
	{
		m_pair = map->iterator();
	}

	~MapKeyIterator()
	{
	}

	sl_bool hasNext()
	{
		return m_pair.hasNext();
	}

	sl_bool next(KT* out)
	{
		Pair<KT, VT> pair;
		sl_bool b = m_pair.next(&pair);
		if (b) {
			if (out) {
				*out = pair.key;
			}
		}
		return b;
	}

	sl_reg getIndex()
	{
		return m_pair.getIndex();
	}
};

template <class KT, class VT>
Iterator<KT> IMap<KT, VT>::keyIterator()
{
	return new MapKeyIterator<KT, VT>(this);
}

template <class KT, class VT>
class SLIB_EXPORT MapValueIterator : public IIterator<VT>
{
private:
	Iterator< Pair<KT, VT> > m_pair;

public:
	MapValueIterator(IMap<KT, VT>* map)
	{
		m_pair = map->iterator();
	}

	~MapValueIterator()
	{
	}

	sl_bool hasNext()
	{
		return m_pair.hasNext();
	}

	sl_bool next(VT* out)
	{
		Pair<KT, VT> pair;
		sl_bool b = m_pair.next(&pair);
		if (b) {
			if (out) {
				*out = pair.value;
			}
		}
		return b;
	}

	sl_reg getIndex()
	{
		return m_pair.getIndex();
	}
};

template <class KT, class VT>
Iterator<VT> IMap<KT, VT>::valueIterator()
{
	return new MapValueIterator<KT, VT>(this);
}
SLIB_NAMESPACE_END

/************************************************************************/
/* ListMap class Definition                                             */
/************************************************************************/
SLIB_NAMESPACE_BEGIN
template <class KT, class VT, class COMPARE = Compare<KT> >
class SLIB_EXPORT ListMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> BaseMap;
	SLIB_DECLARE_OBJECT(ListMap, BaseMap)
protected:
	SLIB_INLINE ListMap(sl_size size = _SLIB_LIST_DEFAULT_SIZE) : m_list(size) {}
	SLIB_INLINE ListMap(ListMap<KT, VT, COMPARE>* other) : m_list(&(other->m_list)) {}

protected:
	class PairCompare
	{
	public:
		SLIB_INLINE static int compare(const Pair<KT, VT>& a, const Pair<KT, VT>& b)
		{
			return COMPARE::compare(a.key, b.key);
		}

		SLIB_INLINE static sl_bool equals(const Pair<KT, VT>& a, const Pair<KT, VT>& b)
		{
			return COMPARE::equals(a.key, b.key);
		}
	};
	ListObject< Pair<KT, VT>, sl_false, PairCompare > m_list;

public:
	static Ref< IMap<KT, VT> > create(sl_size size = _SLIB_LIST_DEFAULT_SIZE)
	{
		Ref< ListMap<KT, VT, COMPARE> > ret;
		ret = new ListMap<KT, VT, COMPARE>(size);
		if (ret.isNotNull()) {
			if (ret->m_list.getCapacity() == 0) {
				ret.setNull();
			}
		}
		return Ref< IMap<KT, VT> >::from(ret);
	}
	
	sl_bool get(const KT& key, VT* out)
	{
		MutexLocker locker(this->getLocker());
		Pair<KT, VT> pair;
		pair.key = key;
		sl_reg index = m_list.indexOf(pair);
		if (index >= 0) {
			Pair<KT, VT>* p = m_list.getItemPtr(index);
			if (out) {
				*out = p->value;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	VT* getItemPtr(const KT& key)
	{
		Pair<KT, VT> pair;
		pair.key = key;
		sl_reg index = m_list.indexOf(pair);
		if (index >= 0) {
			Pair<KT, VT>* p = m_list.getItemPtr(index);
			return &(p->value);
		} else {
			return sl_null;
		}
	}

	VT getValue(const KT& key, const VT& def)
	{
		MutexLocker locker(this->getLocker());
		Pair<KT, VT> pair;
		pair.key = key;
		sl_reg index = m_list.indexOf(pair);
		if (index >= 0) {
			Pair<KT, VT>* p = m_list.getItemPtr(index);
			return p->value;
		} else {
			return def;
		}
	}

	List<VT> getValues(const KT& key)
	{
		List<VT> ret;
		MutexLocker locker(this->getLocker());
		Pair<KT, VT>* list = m_list.getBuffer();
		for (sl_size i = 0; i < m_list.getCount(); i++) {
			if (COMPARE::equals(list[i].key, key)) {
				ret.add(list[i].value);
			}
		}
		return ret;
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		MutexLocker locker(this->getLocker());
		if (flagReplace) {
			Pair<KT, VT> pair;
			pair.key = key;
			sl_reg index = m_list.indexOf(pair);
			if (index >= 0) {
				Pair<KT, VT>* p = m_list.getItemPtr(index);
				p->value = value;
				return sl_true;
			}
		}
		Pair<KT, VT> pair;
		pair.key = key;
		pair.value = value;
		if (!(m_list.add(pair))) {
			return sl_false;
		}
		return sl_true;
	}

	sl_bool remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		MutexLocker locker(this->getLocker());
		Pair<KT, VT> pair;
		pair.key = key;
		return m_list.removeValue(pair, flagAllKeys);
	}

	void clear()
	{
		MutexLocker locker(this->getLocker());
		m_list.clear();
	}

	sl_bool containsKey(const KT& key)
	{
		MutexLocker locker(this->getLocker());
		Pair<KT, VT> pair;
		pair.key = key;
		return (m_list.indexOf(pair) >= 0);
	}

	sl_size getCount()
	{
		return m_list.getCount();
	}

	class ListIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		Ref< ListMap<KT, VT, COMPARE> > m_map;
		sl_size m_index;

	public:
		ListIterator(const Ref< ListMap<KT, VT, COMPARE> >& map)
		{
			m_index = 0;
			m_map = map;
		}

		~ListIterator()
		{
		}

		sl_bool hasNext()
		{
			return (m_index < m_map->getCount());
		}

		sl_bool next(Pair<KT, VT>* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_list.getItem(m_index, out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}

		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};

	Iterator< Pair<KT, VT> > iterator()
	{
		return new ListIterator(this);
	}

	Ref< IMap<KT, VT> > duplicate()
	{
		MutexLocker locker(this->getLocker());
		Ref< ListMap<KT, VT> > ret;
		ret = new ListMap<KT, VT>(*this);
		if (ret.isNotNull()) {
			if (ret->m_list.getCapacity() == 0) {
				ret.setNull();
			}
		}
		return Ref< IMap<KT, VT> >::from(ret);
	}

};
SLIB_NAMESPACE_END


/************************************************************************
	HashMap class Definition
************************************************************************/
SLIB_NAMESPACE_BEGIN
template < class KT, class VT, class HASH = Hash<KT> >
class SLIB_EXPORT HashMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> BaseMap;
	SLIB_DECLARE_OBJECT(HashMap, BaseMap)
protected:
	HashTable<KT, VT, HASH> m_table;

public:
	SLIB_INLINE HashMap(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY) : m_table(capacity)
	{
	}

public:
	static Ref< IMap<KT, VT> > create(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		Ref< HashMap<KT, VT, HASH> > ret = new HashMap<KT, VT, HASH>(capacity);
		if (ret.isNotNull()) {
			if (ret->m_table.getCapacity()) {
				return Ref< IMap<KT, VT> >::from(ret);
			}
		}
		return Ref< IMap<KT, VT> >::null();
	}

	sl_bool get(const KT& key, VT* out)
	{
		MutexLocker locker(this->getLocker());
		return m_table.get(key, out);
	}

	VT* getItemPtr(const KT& key)
	{
		return m_table.getItemPtr(key);
	}

	VT getValue(const KT& key, const VT& def)
	{
		MutexLocker locker(this->getLocker());
		VT ret;
		if (m_table.get(key, &ret)) {
			return ret;
		} else {
			return def;
		}
	}

	List<VT> getValues(const KT& key)
	{
		MutexLocker locker(this->getLocker());
		return m_table.getValues(key);
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		MutexLocker locker(this->getLocker());
		return m_table.put(key, value, flagReplace);
	}

	sl_bool remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		MutexLocker locker(this->getLocker());
		return m_table.remove(key, flagAllKeys);
	}

	void clear()
	{
		MutexLocker locker(this->getLocker());
		m_table.clear();
	}

	sl_bool containsKey(const KT& key)
	{
		MutexLocker locker(this->getLocker());
		return m_table.search(key);
	}

	class HashKeyIterator : public IIterator<KT>
	{
	public:
		HashPosition m_pos;
		Ref< HashMap<KT, VT, HASH> > m_map;
		sl_size m_index;

		HashKeyIterator(const Ref< HashMap<KT, VT, HASH> >& map)
		{
			m_map = map;
			m_index = 0;
		}

		~HashKeyIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos);
		}
		sl_bool next(KT* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_table.getNextPosition(m_pos, out, sl_null)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator<KT> keyIterator()
	{
		return new HashKeyIterator(this);
	}

	class HashValueIterator : public IIterator<VT>
	{
	public:
		HashPosition m_pos;
		Ref< HashMap<KT, VT, HASH> > m_map;
		sl_size m_index;

		HashValueIterator(const Ref< HashMap<KT, VT, HASH> >& map)
		{
			m_map = map;
			m_index = 0;
		}
		~HashValueIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos);
		}
		sl_bool next(VT* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_table.getNextPosition(m_pos, sl_null, out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator<VT> valueIterator()
	{
		return new HashValueIterator(this);
	}

	class HashIterator : public IIterator< Pair<KT, VT> >
	{
	public:
		HashPosition m_pos;
		Ref< HashMap<KT, VT, HASH> > m_map;
		sl_size m_index;

		HashIterator(const Ref< HashMap<KT, VT, HASH> >& map)
		{
			m_map = map;
			m_index = 0;
		}
		~HashIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos, sl_null, sl_null);
		}
		sl_bool next(Pair<KT, VT>* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_table.getNextPosition(m_pos, &(out->key), &(out->value))) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator< Pair<KT, VT> > iterator()
	{
		return new HashIterator(this);
	}

	Ref< IMap<KT, VT> > duplicate()
	{
		MutexLocker locker(this->getLocker());
		Ref< HashMap<KT, VT, HASH> > ret = new HashMap<KT, VT, HASH>();
		if (ret.isNotNull()) {
			if (ret->m_table.copyFrom(&m_table)) {
				return Ref< IMap<KT, VT> >::from(ret);
			}
		}
		return Ref< IMap<KT, VT> >::null();
	}

	sl_size getCount()
	{
		MutexLocker locker(this->getLocker());
		return (sl_size)(m_table.getCount());
	}
};
SLIB_NAMESPACE_END


/************************************************************************
 TreeMap class Definition                                             
	Now TreeMap is based on BTree, but should be changed to Red-Black
	Tree implementation which is a little better for in-memory structure.
************************************************************************/
SLIB_NAMESPACE_BEGIN
template < class KT, class VT, class COMPARE=Compare<KT> >
class SLIB_EXPORT TreeMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> BaseMap;
	SLIB_DECLARE_OBJECT(TreeMap, BaseMap)
protected:
	BTree<KT, VT, COMPARE> m_tree;

protected:
	SLIB_INLINE TreeMap() {}

public:
	static Ref< IMap<KT, VT> > create()
	{
		Ref< TreeMap<KT, VT, COMPARE> > ret = new TreeMap<KT, VT, COMPARE>();
		if (ret.isNotNull()) {
			if (ret->m_tree.isValid()) {
				return Ref< IMap<KT, VT> >::from(ret);
			}
		}
		return Ref< IMap<KT, VT> >::null();
	}

	sl_bool get(const KT& key, VT* out)
	{
		MutexLocker locker(this->getLocker());
		return m_tree.get(key, out);
	}

	VT* getItemPtr(const KT& key)
	{
		return m_tree.getItemPtr(key);
	}

	VT getValue(const KT& key, const VT& def)
	{
		MutexLocker locker(this->getLocker());
		VT ret;
		if (m_tree.get(key, &ret)) {
			return ret;
		} else {
			return def;
		}
	}

	List<VT> getValues(const KT& key)
	{
		MutexLocker locker(this->getLocker());
		return m_tree.getValues(key);
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		MutexLocker locker(this->getLocker());
		return m_tree.insert(key, value, flagReplace);
	}

	sl_bool remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		MutexLocker locker(this->getLocker());
		return m_tree.remove(key, flagAllKeys);
	}

	void clear()
	{
		MutexLocker locker(this->getLocker());
		m_tree.clear();
	}

	sl_bool containsKey(const KT& key)
	{
		MutexLocker locker(this->getLocker());
		return m_tree.search(key, sl_null);
	}

	class TreeKeyIterator : public IIterator<KT>
	{
	public:
		TreePosition m_pos;
		Ref< TreeMap<KT, VT, COMPARE> > m_map;
		sl_size m_index;

		TreeKeyIterator(const Ref< TreeMap<KT, VT, COMPARE> > map)
		{
			m_map = map;
			m_index = 0;
		}
		~TreeKeyIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		sl_bool next(KT* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_tree.getNextPosition(m_pos, out, sl_null)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator<KT> keyIterator()
	{
		return new TreeKeyIterator(this);
	}

	class TreeValueIterator: public IIterator<VT>
	{
	public:
		TreePosition m_pos;
		Ref< TreeMap<KT, VT, COMPARE> > m_map;
		sl_size m_index;

		TreeValueIterator(const Ref< TreeMap<KT, VT, COMPARE> >& map)
		{
			m_map = map;
			m_index = 0;
		}
		~TreeValueIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		sl_bool next(VT* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_tree.getNextPosition(m_pos, sl_null, out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator<VT> valueIterator()
	{
		return new TreeValueIterator(this);
	}

	class TreeIterator : public IIterator< Pair<KT, VT> >
	{
	public:
		TreePosition m_pos;
		Ref< TreeMap<KT, VT, COMPARE> > m_map;
		sl_size m_index;

		TreeIterator(const Ref< TreeMap<KT, VT, COMPARE> >& map)
		{
			m_map = map;
			m_index = 0;
		}
		~TreeIterator()
		{
		}

		sl_bool hasNext()
		{
			MutexLocker lock(m_map->getLocker());
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		sl_bool next(Pair<KT, VT>* out)
		{
			MutexLocker lock(m_map->getLocker());
			if (m_map->m_tree.getNextPosition(m_pos, &(out->key), &(out->value))) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	Iterator< Pair<KT, VT> > iterator()
	{
		return new TreeIterator(this);
	}

	Ref< IMap<KT, VT> > duplicate()
	{
		MutexLocker locker(this->getLocker());
		Ref< TreeMap<KT, VT, COMPARE> > ret = new TreeMap<KT, VT, COMPARE>;
		if (ret.isNotNull()) {
			TreePosition pos;
			KT key;
			VT value;
			while (m_tree.getNextPosition(pos, &key, &value)) {
				if (!(ret->m_tree.insert(key, value, sl_false))) {
					return Ref< IMap<KT, VT> >::null();
				}
			}
			return Ref< IMap<KT, VT> >::from(ret);
		}
		return Ref< IMap<KT, VT> >::null();
	}

	sl_size getCount()
	{
		MutexLocker locker(this->getLocker());
		return (sl_size)(m_tree.getCount());
	}
};
SLIB_NAMESPACE_END


/************************************************************************/
/* Map Class Definition                                                 */
/************************************************************************/
SLIB_NAMESPACE_BEGIN

/** auto-referencing object **/
template <class KT, class VT>
class SLIB_EXPORT Map
{
	typedef IMap<KT, VT> _MapObj;
	typedef Map<KT, VT> _MapType;
	typedef Ref< IMap<KT, VT> > _MapRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_MapType, _MapObj)
	SLIB_DECLARE_OBJECT_WRAPPER(Map, _MapType, _MapObj, _MapRef)

public:
	SLIB_INLINE static _MapType createList(sl_uint32 size = _SLIB_LIST_DEFAULT_SIZE)
	{
		return ListMap<KT, VT>::create(size);
	}

	template <class COMPARE>
	SLIB_INLINE static _MapType createListBy(sl_uint32 size = _SLIB_LIST_DEFAULT_SIZE)
	{
		return ListMap<KT, VT, COMPARE>::create(size);
	}

	SLIB_INLINE static _MapType createHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		return HashMap<KT, VT>::create(initialCapacity);
	}

	template <class HASH>
	SLIB_INLINE static _MapType createHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		return HashMap<KT, VT, HASH>::create(initialCapacity);
	}

	SLIB_INLINE static _MapType createTree()
	{
		return TreeMap<KT, VT>::create();
	}

	template <class COMPARE>
	SLIB_INLINE static _MapType createTreeBy()
	{
		return TreeMap<KT, VT, COMPARE>::create();
	}

	SLIB_INLINE void initList()
	{
		m_object = ListMap<KT, VT>::create();
	}

	template <class COMPARE>
	SLIB_INLINE void initListBy()
	{
		m_object = ListMap<KT, VT, COMPARE>::create();
	}

	SLIB_INLINE void initHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		m_object = HashMap<KT, VT>::create(initialCapacity);
	}

	template <class HASH>
	SLIB_INLINE void initHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		m_object = HashMap<KT, VT, HASH>::create(initialCapacity);
	}

	SLIB_INLINE void initTree()
	{
		m_object = TreeMap<KT, VT>::create();
	}

	template <class COMPARE>
	SLIB_INLINE void initTreeBy()
	{
		m_object = TreeMap<KT, VT, COMPARE>::create();
	}

	SLIB_INLINE sl_size getCount() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->getCount();
		} else {
			return 0;
		}
	}

	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		return getCount() == 0;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return getCount() != 0;
	}

	SLIB_INLINE sl_bool get(const KT& key, VT* out = sl_null) const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->get(key, out);
		} else {
			return sl_null;
		}
	}

	SLIB_INLINE VT* getItemPtr(const KT& key) const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->getItemPtr(key);
		} else {
			return sl_null;
		}
	}

	VT* getNewItemPtr(const KT& key)
	{
		_MapRef object(m_object);
		if (object.isNull()) {
			*this = createHash();
			object = m_object;
		}
		if (object.isNotNull()) {
			VT* ret = m_object->getItemPtr(key);
			if (!ret) {
				VT t;
				m_object->put(key, t);
				ret = m_object->getItemPtr(key);
			}
			return ret;
		}
		return sl_null;
	}

	SLIB_INLINE VT getValue(const KT& key, const VT& def) const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->getValue(key, def);
		} else {
			return def;
		}
	}

	SLIB_INLINE List<VT> getValues(const KT& key) const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->getValues(key);
		} else {
			return List<VT>::null();
		}
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->put(key, value, flagReplace);
		} else {
			_MapType map(createHash());
			if (map.isNotNull()) {
				if (map.put(key, value, flagReplace)) {
					*this = map;
					return sl_true;
				}
			}
			return sl_false;
		}
	}

	SLIB_INLINE sl_bool add(const KT& key, const VT& value)
	{
		return put(key, value, sl_false);
	}

	SLIB_INLINE void remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			object->remove(key, flagAllKeys);
		}
	}

	SLIB_INLINE void removeAllKeys(const KT& key)
	{
		remove(key, sl_true);
	}

	SLIB_INLINE void clear()
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			object->clear();
		}
	}

	SLIB_INLINE void removeAll()
	{
		clear();
	}
	
	SLIB_INLINE sl_bool containsKey(const KT& key) const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->containsKey(key);
		} else {
			return sl_false;
		}
	}

	SLIB_INLINE Iterator<KT> keyIterator() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->keyIterator();
		} else {
			return Iterator<KT>::null();
		}
	}

	SLIB_INLINE Iterator<VT> valueIterator() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->valueIterator();
		} else {
			return Iterator<VT>::null();
		}
	}

	SLIB_INLINE Iterator< Pair<KT, VT> > iterator() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->iterator();
		} else {
			return Iterator< Pair<KT, VT> >::null();
		}
	}

	SLIB_INLINE _MapType duplicate() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			_MapType ret(object->duplicate());
			return ret;
		} else {
			return null();
		}
	}
	
	SLIB_INLINE const Mutex* getLocker() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			return object->getLocker();
		} else {
			return sl_null;
		}
	}

	List< Pair<KT, VT> > pairs() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			MutexLocker lock(object->getLocker());
			Iterator< Pair<KT, VT> > iterator(object->iterator());
			List< Pair<KT, VT> > ret;
			Pair<KT, VT> v;
			while (iterator.next(&v)) {
				if (!(ret.add(v))) {
					return List< Pair<KT, VT> >::null();
				}
			}
			return ret;
		} else {
			return List< Pair<KT, VT> >::null();
		}
	}

	List<KT> keys() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			MutexLocker lock(object->getLocker());
			Iterator<KT> iterator(object->keyIterator());
			List<KT> ret;
			KT v;
			while (iterator.next(&v)) {
				if (!(ret.add(v))) {
					return List<KT>::null();
				}
			}
			return ret;
		} else {
			return List<KT>::null();
		}
	}

	List<VT> values() const
	{
		_MapRef object(m_object);
		if (object.isNotNull()) {
			MutexLocker lock(object->getLocker());
			Iterator<VT> iterator(object->valueIterator());
			List<VT> ret;
			VT v;
			while (iterator.next(&v)) {
				if (!(ret.add(v))) {
					return List<VT>::null();
				}
			}
			return ret;
		} else {
			return List<VT>::null();
		}
	}

	template <class _KT, class _VT>
	sl_bool put(Map<_KT, _VT>& other, sl_bool flagReplace = sl_true)
	{
		_MapRef dst(m_object);
		if (dst.isNotNull()) {
			Ref< IMap<_KT, _VT> > src(other.m_object);
			if ((void*)(dst.get()) == (void*)(src.get())) {
				return sl_false;
			}
			if (src.isNotNull()) {
				MutexLocker lock(dst->getLocker(), src->getLocker());
				Iterator< Pair<_KT,_VT> > iterator(src->iterator());
				Pair<_KT, _VT> v;
				while (iterator.next(&v)) {
					if (!dst->put(v.key, v.value, flagReplace)) {
						return sl_false;
					}
				}
			}
			return sl_true;
		} else {
			_MapType map(createHash());
			_MapRef dst(map.m_object);
			if (dst.isNotNull()) {
				Ref< IMap<_KT, _VT> > src(other.m_object);
				if (src.isNotNull()) {
					MutexLocker lock(src->getLocker());
					Iterator< Pair<_KT, _VT> > iterator(src->iterator());
					Pair<_KT, _VT> v;
					while (iterator.next(&v)) {
						if (!(dst->put(v.key, v.value, flagReplace))) {
							return sl_false;
						}
					}
				}
				*this = map;
			}
			return sl_true;
		}
	}

	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(Map<_KT, _VT>& other)
	{
		return put(other, sl_false);
	}
};
SLIB_NAMESPACE_END


#endif// CHECKHEADER_SLIB_CORE_MAP
