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
	SLIB_DECLARE_OBJECT(IMap, Object)
public:
	SLIB_INLINE IMap() {}
	
public:
	virtual sl_size getCount() const = 0;
	
	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}
	
	
	virtual VT* getItemPtr(const KT& key) const = 0;
	
	
	SLIB_INLINE sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const
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
	
	sl_bool get(const KT& key, VT* _out = sl_null) const
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
	
	
	SLIB_INLINE VT getValue_NoLock(const KT& key, const VT& def)
	{
		VT* p = getItemPtr(key);
		if (p) {
			return *p;
		}
		return def;
	}
	
	VT getValue(const KT& key, const VT& def)
	{
		ObjectLocker lock(this);
		VT* p = getItemPtr(key);
		if (p) {
			return *p;
		}
		return def;
	}
	
	
	virtual List<VT> getValues_NoLock(const KT& key) const = 0;
	
	List<VT> getValues(const KT& key) const
	{
		ObjectLocker lock(this);
		return getValues_NoLock(key);
	}
	
	
	virtual sl_bool put_NoLock(const KT& key, const VT& value, sl_bool flagReplace = sl_true) = 0;
	
	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		ObjectLocker lock(this);
		return put_NoLock(key, value, flagReplace);
	}
	
	
	template <class _KT, class _VT>
	sl_bool put(IMap<_KT, _VT>* other, sl_bool flagReplace = sl_true)
	{
		if (!other) {
			return sl_true;
		}
		if (this == other) {
			return sl_false;
		}
		ObjectLocker lock(this, other);
		Iterator< Pair<_KT,_VT> > iterator(other->iterator());
		Pair<_KT, _VT> v;
		while (iterator.next(&v)) {
			if (!(put_NoLock(v.key, v.value, flagReplace))) {
				return sl_false;
			}
		}
		return sl_true;
	}
	
	
	SLIB_INLINE sl_bool add_NoLock(const KT& key, const VT& value)
	{
		return put_NoLock(key, value, sl_false);
	}

	SLIB_INLINE sl_bool add(const KT& key, const VT& value)
	{
		return put(key, value, sl_false);
	}
	
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(const IMap<_KT, _VT>* other)
	{
		return put(other, sl_false);
	}
	
	
	virtual sl_size remove_NoLock(const KT& key, sl_bool flagAllKeys = sl_false) = 0;
	
	sl_size remove(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		ObjectLocker lock(this);
		return remove_NoLock(key, flagAllKeys);
	}
	
	
	SLIB_INLINE sl_size removeAllMatchingKeys_NoLock(const KT& key)
	{
		return remove_NoLock(key, sl_true);
	}
	
	SLIB_INLINE sl_size removeAllMatchingKeys(const KT& key)
	{
		return remove(key, sl_true);
	}
	
	
	virtual sl_size removeAll_NoLock() = 0;
	
	sl_size removeAll()
	{
		ObjectLocker lock(this);
		return removeAll_NoLock();
	}
	
	
	virtual sl_bool containsKey_NoLock(const KT& key) const = 0;
	
	sl_bool containsKey(const KT& key) const
	{
		ObjectLocker lock(this);
		return containsKey_NoLock(key);
	}
	
	
	virtual IMap<KT, VT>* duplicate_NoLock() const = 0;
	
	IMap<KT, VT>* duplicate() const
	{
		ObjectLocker lock(this);
		return duplicate_NoLock();
	}
	
	
	virtual Iterator<KT> keyIteratorWithRefer(const Referable* refer) const = 0;
	
	SLIB_INLINE Iterator<KT> keyIterator() const
	{
		return keyIteratorWithRefer(sl_null);
	}
	
	virtual List<KT> keys_NoLock() const = 0;
	
	List<KT> keys() const
	{
		ObjectLocker lock(this);
		return keys_NoLock();
	}

	
	virtual Iterator<VT> valueIteratorWithRefer(const Referable* refer) const = 0;
	
	SLIB_INLINE Iterator<VT> valueIterator() const
	{
		return valueIteratorWithRefer(sl_null);
	}
	
	virtual List<VT> values_NoLock() const = 0;
	
	List<VT> values() const
	{
		ObjectLocker lock(this);
		return values_NoLock();
	}
	
	
	virtual Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const = 0;
	
	SLIB_INLINE Iterator< Pair<KT, VT> > iterator() const
	{
		return iteratorWithRefer(sl_null);
	}
	
	virtual List< Pair<KT, VT> > pairs_NoLock() const = 0;
	
	List< Pair<KT, VT> > pairs() const
	{
		ObjectLocker lock(this);
		return pairs_NoLock();
	}

	
public:
	static IMap<KT, VT>* createDefault();

};

template < class KT, class VT, class COMPARE = Compare<KT> >
class SLIB_EXPORT ListMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> _Base;
	typedef ListMap<KT, VT, COMPARE> _Type;
	SLIB_DECLARE_OBJECT(ListMap, _Base)

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
	
	CList< Pair<KT, VT>, PairCompare > m_list;
	
public:
	SLIB_INLINE ListMap() {}
	
public:
	SLIB_INLINE static _Type* create()
	{
		return new _Type;
	}
	
public:
	// override
	sl_size getCount() const
	{
		return m_list.count();
	}
	
	// override
	VT* getItemPtr(const KT& key) const
	{
		Pair<KT, VT> pair;
		pair.key = key;
		sl_reg index = m_list.indexOf_NoLock(pair);
		if (index >= 0) {
			Pair<KT, VT>* p = m_list.data() + index;
			return &(p->value);
		}
		return sl_null;
	}
	
	// override
	List<VT> getValues_NoLock(const KT& key) const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			Pair<KT, VT>* data = m_list.data();
			sl_size count = m_list.count();
			for (sl_size i = 0; i < count; i++) {
				if (COMPARE::equals(data[i].key, key)) {
					ret->add_NoLock(data[i].value);
				}
			}
			if (ret->count() > 0) {
				return ret;
			}
			delete ret;
		}
		return List<VT>::null();
	}

	// override
	sl_bool put_NoLock(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		if (flagReplace) {
			Pair<KT, VT> pair;
			pair.key = key;
			sl_reg index = m_list.indexOf_NoLock(pair);
			if (index >= 0) {
				m_list[index].value = value;
				return sl_true;
			}
		}
		Pair<KT, VT> pair;
		pair.key = key;
		pair.value = value;
		if (m_list.add_NoLock(pair)) {
			return sl_true;
		}
		return sl_false;
	}

	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		Pair<KT, VT> pair;
		pair.key = key;
		return m_list.removeValue_NoLock(pair, flagAllKeys);
	}

	// override
	sl_size removeAll_NoLock()
	{
		return m_list.removeAll_NoLock();
	}

	// override
	sl_bool containsKey_NoLock(const KT& key) const
	{
		Pair<KT, VT> pair;
		pair.key = key;
		return (m_list.indexOf_NoLock(pair) >= 0);
	}
	
	// override
	IMap<KT, VT>* duplicate_NoLock() const
	{
		_Type* ret = new _Type;
		if (ret) {
			if (ret->m_list.add_NoLock(m_list.data(), m_list.count())) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	class ListKeyIterator : public IIterator<KT>
	{
	protected:
		const ListMap<KT, VT, COMPARE>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;
		
	public:
		SLIB_INLINE ListKeyIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}
		
	public:
		// override
		sl_bool hasNext()
		{
			return (m_index < m_map->getCount());
		}
		
		// override
		sl_bool next(KT* _out)
		{
			Pair<KT, VT> pair;
			if (m_map->m_list.getItem(m_index, &pair)) {
				if (_out) {
					*_out = pair.key;
				}
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const
	{
		return new ListKeyIterator(this, refer);
	}
	
	// override
	List<KT> keys_NoLock() const
	{
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			Pair<KT, VT>* dataSrc = m_list.data();
			sl_size count = m_list.count();
			if (ret->setCount_NoLock(count)) {
				KT* dataDst = ret->data();
				for (sl_size i = 0; i < count; i++) {
					dataDst[i] = dataSrc[i].key;
				}
				return ret;
			}
			delete ret;
		}
		return List<KT>::null();
	}
	
	class ListValueIterator : public IIterator<VT>
	{
	protected:
		const ListMap<KT, VT, COMPARE>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;
		
	public:
		SLIB_INLINE ListValueIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}
		
	public:
		// override
		sl_bool hasNext()
		{
			return (m_index < m_map->getCount());
		}
		
		// override
		sl_bool next(VT* _out)
		{
			Pair<KT, VT> pair;
			if (m_map->m_list.getItem(m_index, &pair)) {
				if (_out) {
					*_out = pair.value;
				}
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const
	{
		return new ListValueIterator(this, refer);
	}
	
	// override
	List<VT> values_NoLock() const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			Pair<KT, VT>* dataSrc = m_list.data();
			sl_size count = m_list.count();
			if (ret->setCount_NoLock(count)) {
				VT* dataDst = ret->data();
				for (sl_size i = 0; i < count; i++) {
					dataDst[i] = dataSrc[i].value;
				}
				return ret;
			}
			delete ret;
		}
		return List<VT>::null();
	}
	
	class ListMapIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const ListMap<KT, VT, COMPARE>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;
		
	public:
		SLIB_INLINE ListMapIterator(const ListMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}
		
	public:
		// override
		sl_bool hasNext()
		{
			return (m_index < m_map->getCount());
		}
		
		// override
		sl_bool next(Pair<KT, VT>* _out)
		{
			if (m_map->m_list.getItem(m_index, _out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const
	{
		return new ListMapIterator(this, refer);
	}
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const
	{
		return (CList< Pair<KT, VT> >*)((void*)(m_list.duplicate()));
	}

};

template < class KT, class VT, class HASH = Hash<KT> >
class SLIB_EXPORT HashMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> _Base;
	typedef HashMap<KT, VT, HASH> _Type;
	SLIB_DECLARE_OBJECT(HashMap, _Base)
	
protected:
	HashTable<KT, VT, HASH> m_table;

public:
	SLIB_INLINE HashMap(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY) : m_table(capacity)
	{
	}
	
public:
	SLIB_INLINE static _Type* create(sl_uint32 capacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		_Type* ret = new _Type(capacity);
		if (ret) {
			if (ret->m_table.getCapacity() > 0) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}

public:
	// override
	sl_size getCount() const
	{
		return (sl_size)(m_table.getCount());
	}
	
	// override
	VT* getItemPtr(const KT& key) const
	{
		return m_table.getItemPtr(key);
	}
	
	// override
	List<VT> getValues_NoLock(const KT& key) const
	{
		return m_table.getValues(key);
	}

	// override
	sl_bool put_NoLock(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		return m_table.put(key, value, flagReplace);
	}

	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		return m_table.remove(key, flagAllKeys);
	}

	// override
	sl_size removeAll_NoLock()
	{
		return m_table.removeAll();
	}

	// override
	sl_bool containsKey_NoLock(const KT& key) const
	{
		return m_table.search(key);
	}

	// override
	IMap<KT, VT>* duplicate_NoLock() const
	{
		_Type* ret = new _Type;
		if (ret) {
			if (ret->m_table.copyFrom(&m_table)) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}

	class HashKeyIterator : public IIterator<KT>
	{
	protected:
		const HashMap<KT, VT, HASH>* m_map;
		HashPosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		SLIB_INLINE HashKeyIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}

	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos);
		}
		
		// override
		sl_bool next(KT* _out)
		{
			ObjectLocker lock(m_map);
			if (m_map->m_table.getNextPosition(m_pos, _out, sl_null)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const
	{
		return new HashKeyIterator(this, refer);
	}

	// override
	List<KT> keys_NoLock() const
	{
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			HashPosition pos;
			KT key;
			while (m_table.getNextPosition(pos, &key, sl_null)) {
				if (!(ret->add_NoLock(key))) {
					delete ret;
					return List<KT>::null();
				}
			}
			return ret;
		}
		return List<KT>::null();
	}
	
	class HashValueIterator : public IIterator<VT>
	{
	protected:
		const HashMap<KT, VT, HASH>* m_map;
		HashPosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		SLIB_INLINE HashValueIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}
		
	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos);
		}
		
		// override
		sl_bool next(VT* _out)
		{
			ObjectLocker lock(m_map);
			if (m_map->m_table.getNextPosition(m_pos, sl_null, _out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const
	{
		return new HashValueIterator(this, refer);
	}
	
	// override
	List<VT> values_NoLock() const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			HashPosition pos;
			VT value;
			while (m_table.getNextPosition(pos, sl_null, &value)) {
				if (!(ret->add_NoLock(value))) {
					delete ret;
					return List<VT>::null();
				}
			}
			return ret;
		}
		return List<VT>::null();
	}
	
	class HashIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const HashMap<KT, VT, HASH>* m_map;
		HashPosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		SLIB_INLINE HashIterator(const HashMap<KT, VT, HASH>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}
		
	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			HashPosition pos = m_pos;
			return m_map->m_table.getNextPosition(pos, sl_null, sl_null);
		}
		
		// override
		sl_bool next(Pair<KT, VT>* out)
		{
			ObjectLocker lock(m_map);
			if (m_map->m_table.getNextPosition(m_pos, &(out->key), &(out->value))) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const
	{
		return new HashIterator(this, refer);
	}
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const
	{
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			HashPosition pos;
			Pair<KT, VT> pair;
			while (m_table.getNextPosition(pos, &(pair.key), &(pair.value))) {
				if (!(ret->add_NoLock(pair))) {
					delete ret;
					return List< Pair<KT, VT> >::null();
				}
			}
			return ret;
		}
		return List< Pair<KT, VT> >::null();
	}

};

/*
 TreeMap class Definition                                             
	Now TreeMap is based on BTree, but should be changed to Red-Black
	Tree implementation which is a little better for in-memory structure.
*/
template < class KT, class VT, class COMPARE=Compare<KT> >
class SLIB_EXPORT TreeMap : public IMap<KT, VT>
{
	typedef IMap<KT, VT> _Base;
	typedef TreeMap<KT, VT, COMPARE> _Type;
	SLIB_DECLARE_OBJECT(TreeMap, _Base)
	
protected:
	BTree<KT, VT, COMPARE> m_tree;

public:
	SLIB_INLINE TreeMap() {}

public:
	SLIB_INLINE static _Type* create()
	{
		_Type* ret = new _Type;
		if (ret) {
			if (ret->m_tree.isValid()) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}

public:
	// override
	sl_size getCount() const
	{
		return (sl_size)(m_tree.getCount());
	}
	
	// override
	VT* getItemPtr(const KT& key) const
	{
		return m_tree.getItemPtr(key);
	}

	// override
	List<VT> getValues_NoLock(const KT& key) const
	{
		return m_tree.getValues(key);
	}

	// override
	sl_bool put_NoLock(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		return m_tree.insert(key, value, flagReplace);
	}

	// override
	sl_size remove_NoLock(const KT& key, sl_bool flagAllKeys = sl_false)
	{
		return m_tree.remove(key, flagAllKeys);
	}

	// override
	sl_size removeAll_NoLock()
	{
		return m_tree.removeAll();
	}

	// override
	sl_bool containsKey_NoLock(const KT& key) const
	{
		return m_tree.search(key, sl_null);
	}
	
	// override
	IMap<KT, VT>* duplicate_NoLock() const
	{
		_Type* ret = new _Type;
		if (ret) {
			TreePosition pos;
			KT key;
			VT value;
			while (m_tree.getNextPosition(pos, &key, &value)) {
				if (!(ret->m_tree.insert(key, value, sl_false))) {
					delete ret;
					return sl_null;
				}
			}
			return ret;
		}
		return sl_null;
	}

	class TreeKeyIterator : public IIterator<KT>
	{
	protected:
		const TreeMap<KT, VT, COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		SLIB_INLINE TreeKeyIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}

	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		
		// override
		sl_bool next(KT* _out)
		{
			ObjectLocker lock(m_map);
			if (m_map->m_tree.getNextPosition(m_pos, _out, sl_null)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<KT> keyIteratorWithRefer(const Referable* refer) const
	{
		return new TreeKeyIterator(this, refer);
	}
	
	// override
	List<KT> keys_NoLock() const
	{
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			TreePosition pos;
			KT key;
			while (m_tree.getNextPosition(pos, &key, sl_null)) {
				if (!(ret->add_NoLock(key))) {
					delete ret;
					return List<KT>::null();
				}
			}
			return ret;
		}
		return List<KT>::null();
	}

	class TreeValueIterator: public IIterator<VT>
	{
	protected:
		const TreeMap<KT, VT, COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		SLIB_INLINE TreeValueIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}

	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		
		// override
		sl_bool next(VT* _out)
		{
			ObjectLocker lock(m_map);
			if (m_map->m_tree.getNextPosition(m_pos, sl_null, _out)) {
				m_index++;
				return sl_true;
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator<VT> valueIteratorWithRefer(const Referable* refer) const
	{
		return new TreeValueIterator(this, refer);
	}
	
	// override
	List<VT> values_NoLock() const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			TreePosition pos;
			VT value;
			while (m_tree.getNextPosition(pos, sl_null, &value)) {
				if (!(ret->add_NoLock(value))) {
					delete ret;
					return List<VT>::null();
				}
			}
			return ret;
		}
		return List<VT>::null();
	}

	class TreeIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const TreeMap<KT, VT, COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		TreeIterator(const TreeMap<KT, VT, COMPARE>* map, const Referable* refer)
		{
			m_map = map;
			m_index = 0;
			m_refer = refer;
		}

	public:
		// override
		sl_bool hasNext()
		{
			ObjectLocker lock(m_map);
			TreePosition pos = m_pos;
			return m_map->m_tree.getNextPosition(pos);
		}
		
		// override
		sl_bool next(Pair<KT, VT>* _out)
		{
			ObjectLocker lock(m_map);
			if (_out) {
				if (m_map->m_tree.getNextPosition(m_pos, &(_out->key), &(_out->value))) {
					m_index++;
					return sl_true;
				}
			} else {
				if (m_map->m_tree.getNextPosition(m_pos, sl_null, sl_null)) {
					m_index++;
					return sl_true;
				}
			}
			return sl_false;
		}
		
		// override
		sl_reg getIndex()
		{
			return (sl_reg)m_index - 1;
		}
	};
	
	// override
	Iterator< Pair<KT, VT> > iteratorWithRefer(const Referable* refer) const
	{
		return new TreeIterator(this, refer);
	}
	
	// override
	List< Pair<KT, VT> > pairs_NoLock() const
	{
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			TreePosition pos;
			Pair<KT, VT> pair;
			while (m_tree.getNextPosition(pos, &(pair.key), &(pair.value))) {
				if (!(ret->add_NoLock(pair))) {
					delete ret;
					return List< Pair<KT, VT> >::null();
				}
			}
			return ret;
		}
		return List< Pair<KT, VT> >::null();
	}
};

template <class KT, class VT>
SLIB_INLINE IMap<KT, VT>* IMap<KT, VT>::createDefault()
{
	return HashMap<KT, VT>::create();
}

template <class KT, class VT>
class SafeMap;

/** auto-referencing object **/
template <class KT, class VT>
class SLIB_EXPORT Map
{
	typedef IMap<KT, VT> _Obj;
	typedef Map<KT, VT> _Type;
	typedef SafeMap<KT, VT> _SafeType;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(Map, _Obj, _Type, _SafeType)

public:
	SLIB_INLINE static _Type createList()
	{
		return ListMap<KT, VT>::create();
	}

	template <class COMPARE>
	SLIB_INLINE static _Type createListBy()
	{
		return ListMap<KT, VT, COMPARE>::create();
	}

	
	SLIB_INLINE static _Type createHash(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		return HashMap<KT, VT>::create(initialCapacity);
	}

	template <class HASH>
	SLIB_INLINE static _Type createHashBy(sl_uint32 initialCapacity = SLIB_HASHTABLE_DEFAULT_CAPACITY)
	{
		return HashMap<KT, VT, HASH>::create(initialCapacity);
	}

	
	SLIB_INLINE static _Type createTree()
	{
		return TreeMap<KT, VT>::create();
	}

	template <class COMPARE>
	SLIB_INLINE static _Type createTreeBy()
	{
		return TreeMap<KT, VT, COMPARE>::create();
	}

public:
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
	
public:
	SLIB_INLINE sl_size getCount() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}

	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}

	
	SLIB_INLINE sl_bool isEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}
	
	
	SLIB_INLINE VT* getItemPtr(const KT& key) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getItemPtr(key);
		}
		return sl_null;
	}
	
	VT* getNewItemPtr(const KT& key)
	{
		_Obj* obj = m_object.get();
		if (!obj) {
			obj = _Obj::createDefault();
			if (obj) {
				m_object = obj;
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
	
	
	SLIB_INLINE sl_bool get_NoLock(const KT& key, VT* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_null;
	}

	SLIB_INLINE sl_bool get(const KT& key, VT* _out = sl_null) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_null;
	}
	
	
	SLIB_INLINE VT getValue_NoLock(const KT& key, const VT& def) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getValue_NoLock(key, def);
		}
		return def;
	}

	SLIB_INLINE VT getValue(const KT& key, const VT& def) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getValue(key, def);
		}
		return def;
	}
	
	
	SLIB_INLINE List<VT> getValues_NoLock(const KT& key) const
	{
		_Obj obj = m_object.get();
		if (obj) {
			return obj->getValues_NoLock(key);
		}
		return List<VT>::null();
	}

	SLIB_INLINE List<VT> getValues(const KT& key) const
	{
		_Obj obj = m_object.get();
		if (obj) {
			return obj->getValues(key);
		}
		return List<VT>::null();
	}
	
	
	sl_bool put_NoLock(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->put_NoLock(key, value, flagReplace);
		} else {
			obj = _Obj::createDefault();
			if (obj) {
				m_object = obj;
				return obj->put_NoLock(key, value, flagReplace);
			}
		}
		return sl_false;
	}

	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->put(key, value, flagReplace);
		} else {
			obj = _Obj::createDefault();
			if (obj) {
				m_object = obj;
				return obj->put_NoLock(key, value, flagReplace);
			}
		}
		return sl_false;
	}
	
	
	template <class _KT, class _VT>
	sl_bool put(const Map<_KT, _VT>& other, sl_bool flagReplace = sl_true)
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->put(other.getObject(), flagReplace);
		} else {
			obj = _Obj::createDefault();
			if (obj) {
				m_object = obj;
				return obj->put(other.getObject(), flagReplace);
			}
		}
		return sl_false;
	}
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool put(const SafeMap<_KT, _VT>& other, sl_bool flagReplace = sl_true)
	{
		return put(Map<_KT, _VT>(other), flagReplace);
	}
	
	
	SLIB_INLINE sl_bool add_NoLock(const KT& key, const VT& value)
	{
		return put_NoLock(key, value, sl_false);
	}

	SLIB_INLINE sl_bool add(const KT& key, const VT& value)
	{
		return put(key, value, sl_false);
	}
	
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(const Map<_KT, _VT>& other)
	{
		return put(other, sl_false);
	}

	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(const SafeMap<_KT, _VT>& other)
	{
		return put(Map<_KT, _VT>(other), sl_false);
	}
	
	
	SLIB_INLINE sl_size remove_NoLock(const KT& key, sl_bool flagAllKeys = sl_false) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->remove_NoLock(key, flagAllKeys);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size remove(const KT& key, sl_bool flagAllKeys = sl_false) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->remove(key, flagAllKeys);
		}
		return 0;
	}
	

	SLIB_INLINE sl_size removeAllMatchingKeys_NoLock(const KT& key) const
	{
		return remove_NoLock(key, sl_true);
	}

	SLIB_INLINE sl_size removeAllMatchingKeys(const KT& key) const
	{
		return remove(key, sl_true);
	}

	
	SLIB_INLINE sl_size removeAll_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	SLIB_INLINE sl_size removeAll() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}

	
	SLIB_INLINE sl_bool containsKey_NoLock(const KT& key) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->containsKey_NoLock(key);
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool containsKey(const KT& key) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->containsKey(key);
		}
		return sl_false;
	}
	
	
	SLIB_INLINE _Type duplicate_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return _Type::null();
	}
	
	SLIB_INLINE _Type duplicate() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate();
		}
		return _Type::null();
	}
	
	
	SLIB_INLINE Iterator<KT> keyIterator() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->keyIteratorWithRefer(obj);
		}
		return Iterator<KT>::null();
	}
	
	SLIB_INLINE List<KT> keys_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->keys_NoLock();
		}
		return List<KT>::null();
	}
	
	SLIB_INLINE List<KT> keys() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->keys();
		}
		return List<KT>::null();
	}
	
	
	SLIB_INLINE Iterator<VT> valueIterator() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->valueIteratorWithRefer(obj);
		}
		return Iterator<VT>::null();
	}

	SLIB_INLINE List<VT> values_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->values_NoLock();
		}
		return List<VT>::null();
	}
	
	SLIB_INLINE List<VT> values() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->values();
		}
		return List<VT>::null();
	}

	
	SLIB_INLINE Iterator< Pair<KT, VT> > iterator() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->iteratorWithRefer(obj);
		}
		return Iterator< Pair<KT, VT> >::null();
	}
	
	SLIB_INLINE List< Pair<KT, VT> > pairs_NoLock() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->pairs_NoLock();
		}
		return List< Pair<KT, VT> >::null();
	}
	
	SLIB_INLINE List< Pair<KT, VT> > pairs() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->pairs();
		}
		return List< Pair<KT, VT> >::null();
	}
	
public:
	SLIB_INLINE const Mutex* getLocker() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getLocker();
		}
		return sl_null;
	}

};


/** auto-referencing object **/
template <class KT, class VT>
class SLIB_EXPORT SafeMap
{
	typedef IMap<KT, VT> _Obj;
	typedef SafeMap<KT, VT> _Type;
	typedef Map<KT, VT> _LocalType;
	typedef SafeRef<_Obj> _Ref;
	typedef Ref<_Obj> _LocalRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER(SafeMap, _Obj, _Type, _LocalType)
	
public:
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
	
public:
	SLIB_INLINE sl_size getCount() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}
	
	SLIB_INLINE sl_size count() const
	{
		return getCount();
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_bool get(const KT& key, VT* _out = sl_null) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_null;
	}
	
	SLIB_INLINE VT getValue(const KT& key, const VT& def) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getValue(key, def);
		}
		return def;
	}
	
	SLIB_INLINE List<VT> getValues(const KT& key) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->getValues(key);
		}
		return List<VT>::null();
	}
	
	
	sl_bool put(const KT& key, const VT& value, sl_bool flagReplace = sl_true)
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->put(key, value, flagReplace);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->put(key, value, flagReplace);
			}
			obj = _Obj::createDefault();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->put(key, value, flagReplace);
			}
		}
		return sl_false;
	}
	
	
	template <class _KT, class _VT>
	sl_bool put(const Map<_KT, _VT>& other, sl_bool flagReplace = sl_true)
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->put(other.getObject(), flagReplace);
		} else {
			SpinLocker lock(SpinLockPoolForMap::get(this));
			obj = m_object;
			if (obj.isNotNull()) {
				lock.unlock();
				return obj->put(other.getObject(), flagReplace);
			}
			obj = _Obj::createDefault();
			if (obj.isNotNull()) {
				m_object = obj;
				lock.unlock();
				return obj->put(other.getObject(), flagReplace);
			}
		}
		return sl_false;
	}
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool put(const SafeMap<_KT, _VT>& other, sl_bool flagReplace = sl_true)
	{
		return put(Map<_KT, _VT>(other), flagReplace);
	}
	
	
	SLIB_INLINE sl_bool add(const KT& key, const VT& value)
	{
		return put(key, value, sl_false);
	}
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(const Map<_KT, _VT>& other)
	{
		return put(other, sl_false);
	}
	
	template <class _KT, class _VT>
	SLIB_INLINE sl_bool add(const SafeMap<_KT, _VT>& other)
	{
		return put(Map<_KT, _VT>(other), sl_false);
	}
	
	
	SLIB_INLINE sl_size remove(const KT& key, sl_bool flagAllKeys = sl_false) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->remove(key, flagAllKeys);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size removeAllMatchingKeys(const KT& key) const
	{
		return remove(key, sl_true);
	}
	
	SLIB_INLINE sl_size removeAll() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}
	
	SLIB_INLINE sl_bool containsKey(const KT& key) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->containsKey(key);
		}
		return sl_false;
	}
	
	SLIB_INLINE _LocalType duplicate() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return _LocalType::null();
	}
	
	SLIB_INLINE List<KT> keys() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->keys();
		}
		return List<KT>::null();
	}
	
	SLIB_INLINE List<VT> values() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->values();
		}
		return List<VT>::null();
	}
	
	SLIB_INLINE List< Pair<KT, VT> > pairs() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->pairs();
		}
		return List< Pair<KT, VT> >::null();
	}
	
};

SLIB_NAMESPACE_END

#endif
