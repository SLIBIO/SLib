#ifndef CHECKHEADER_SLIB_CORE_DETAIL_MAP
#define CHECKHEADER_SLIB_CORE_DETAIL_MAP

#include "../map.h"

namespace slib
{
	
	template <class KT, class VT, class KEY_EQUALS>
	class ListMapKeyIterator : public IIterator<KT>
	{
	protected:
		const ListMap<KT, VT, KEY_EQUALS>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		ListMapKeyIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(KT* _out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class KEY_EQUALS>
	class ListMapValueIterator : public IIterator<VT>
	{
	protected:
		const ListMap<KT, VT, KEY_EQUALS>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		ListMapValueIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(VT* _out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class KEY_EQUALS>
	class ListMapIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const ListMap<KT, VT, KEY_EQUALS>* m_map;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		ListMapIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(Pair<KT, VT>* _out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	class HashMapKeyIterator : public IIterator<KT>
	{
	protected:
		const HashMap<KT, VT, HASH, KEY_EQUALS>* m_map;
		HashEntry<KT, VT>* m_entry;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		HashMapKeyIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(KT* _out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	class HashMapValueIterator : public IIterator<VT>
	{
	protected:
		const HashMap<KT, VT, HASH, KEY_EQUALS>* m_map;
		HashEntry<KT, VT>* m_entry;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		HashMapValueIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(VT* _out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	class HashMapIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const HashMap<KT, VT, HASH, KEY_EQUALS>* m_map;
		HashEntry<KT, VT>* m_entry;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		HashMapIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(Pair<KT, VT>* out);

		// override
		sl_reg getIndex();

	};
	
	
	template <class KT, class VT, class KEY_COMPARE>
	class TreeMapKeyIterator : public IIterator<KT>
	{
	protected:
		const TreeMap<KT, VT, KEY_COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		TreeMapKeyIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer);

	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(KT* _out);

		// override
		sl_reg getIndex();

	};
	
	template <class KT, class VT, class KEY_COMPARE>
	class TreeMapValueIterator: public IIterator<VT>
	{
	protected:
		const TreeMap<KT, VT, KEY_COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		TreeMapValueIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer);
		
	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(VT* _out);

		// override
		sl_reg getIndex();
		
	};
	
	template <class KT, class VT, class KEY_COMPARE>
	class TreeMapIterator : public IIterator< Pair<KT, VT> >
	{
	protected:
		const TreeMap<KT, VT, KEY_COMPARE>* m_map;
		TreePosition m_pos;
		sl_size m_index;
		Ref<Referable> m_refer;

	public:
		TreeMapIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer);
		
	public:
		// override
		sl_bool hasNext();

		// override
		sl_bool next(Pair<KT, VT>* _out);

		// override
		sl_reg getIndex();
		
	};
	
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
		VT* p = getItemPointer(key);
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
		VT* p = getItemPointer(key);
		if (p) {
			if (_out) {
				*_out = *p;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue_NoLock(const KT& key) const
	{
		VT* p = getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue(const KT& key) const
	{
		ObjectLocker lock(this);
		VT* p = getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const
	{
		VT* p = getItemPointer(key);
		if (p) {
			return *p;
		}
		return def;
	}
	
	template <class KT, class VT>
	VT IMap<KT, VT>::getValue(const KT& key, const VT& def) const
	{
		ObjectLocker lock(this);
		VT* p = getItemPointer(key);
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
	template <class _KT, class _VT>
	void IMap<KT, VT>::putAll_NoLock(IMap<_KT, _VT>* other, MapPutMode mode)
	{
		if (!other) {
			return;
		}
		if (this == other) {
			return;
		}
		Iterator< Pair<_KT,_VT> > iterator(other->toIterator());
		Pair<_KT, _VT> v;
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
		Iterator< Pair<_KT,_VT> > iterator(other->toIterator());
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
	Iterator<KT> IMap<KT, VT>::getKeyIterator() const
	{
		return getKeyIteratorWithRefer(sl_null);
	}
	
	template <class KT, class VT>
	List<KT> IMap<KT, VT>::getAllKeys() const
	{
		ObjectLocker lock(this);
		return getAllKeys_NoLock();
	}
	
	template <class KT, class VT>
	Iterator<VT> IMap<KT, VT>::getValueIterator() const
	{
		return getValueIteratorWithRefer(sl_null);
	}
	
	template <class KT, class VT>
	List<VT> IMap<KT, VT>::getAllValues() const
	{
		ObjectLocker lock(this);
		return getAllValues_NoLock();
	}
	
	template <class KT, class VT>
	Iterator< Pair<KT, VT> > IMap<KT, VT>::toIterator() const
	{
		return toIteratorWithRefer(sl_null);
	}
	
	template <class KT, class VT>
	List< Pair<KT, VT> > IMap<KT, VT>::toList() const
	{
		ObjectLocker lock(this);
		return toList_NoLock();
	}
	
	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > IMap<KT, VT>::begin() const
	{
		return toIterator().begin();
	}
	
	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > IMap<KT, VT>::end() const
	{
		return IteratorPosition< Pair<KT, VT> >();
	}
	
	
	template <class KT, class VT, class KEY_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>::PairKeyCompare::PairKeyCompare(const KEY_EQUALS& _key_equals) : key_equals(_key_equals)
	{
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	SLIB_INLINE sl_bool ListMap<KT, VT, KEY_EQUALS>::PairKeyCompare::operator()(const Pair<KT, VT>& a, const KT& b) const
	{
		return key_equals(a.key, b);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>::PairCompare<_VT, VALUE_EQUALS>::PairCompare(const KEY_EQUALS& _key_equals, const VALUE_EQUALS& _value_equals) : key_equals(_key_equals), value_equals(_value_equals)
	{
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	SLIB_INLINE sl_bool ListMap<KT, VT, KEY_EQUALS>::PairCompare<_VT, VALUE_EQUALS>::operator()(const Pair<KT, VT>& a, const Pair<KT, _VT>& b) const
	{
		return key_equals(a.key, b.key) && value_equals(a.value, b.value);
	}
	
	
	template <class KT, class VT, class KEY_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>::ListMap(const KEY_EQUALS& key_equals) : m_equals(key_equals)
	{
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	ListMap<KT, VT, KEY_EQUALS>* ListMap<KT, VT, KEY_EQUALS>::create(const KEY_EQUALS& key_equals)
	{
		return new ListMap<KT, VT, KEY_EQUALS>(key_equals);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	VT ListMap<KT, VT, KEY_EQUALS>::operator[](const KT& key) const
	{
		ObjectLocker lock(this);
		sl_reg index = list.indexOf_NoLock(key, 0, PairKeyCompare(m_equals));
		if (index >= 0) {
			Pair<KT, VT>* p = list.getData() + index;
			return p->value;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_size ListMap<KT, VT, KEY_EQUALS>::getCount() const
	{
		return list.getCount();
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	VT* ListMap<KT, VT, KEY_EQUALS>::getItemPointer(const KT& key) const
	{
		sl_reg index = list.indexOf_NoLock(key, 0, PairKeyCompare(m_equals));
		if (index >= 0) {
			Pair<KT, VT>* p = list.getData() + index;
			return &(p->value);
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	List<VT> ListMap<KT, VT, KEY_EQUALS>::getValues_NoLock(const KT& key) const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			Pair<KT, VT>* data = list.getData();
			sl_size count = list.getCount();
			for (sl_size i = 0; i < count; i++) {
				if (m_equals(data[i].key, key)) {
					ret->add_NoLock(data[i].value);
				}
			}
			if (ret->getCount() > 0) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
	{
		if (pFlagExist) {
			*pFlagExist = sl_false;
		}
		Pair<KT, VT>* items = list.getData();
		if (mode != MapPutMode::AddAlways) {
			sl_reg index = list.indexOf_NoLock(key, 0, PairKeyCompare(m_equals));
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
		if (list.add_NoLock(Pair<KT, VT>(key, value))) {
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::addIfNewKeyAndValue_NoLock(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
	{
		if (pFlagExist) {
			*pFlagExist = sl_false;
		}
		sl_reg index = list.indexOf_NoLock(Pair<KT, _VT>(key, value), 0, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals));
		if (index >= 0) {
			if (pFlagExist) {
				*pFlagExist = sl_true;
			}
			return sl_false;
		}
		if (list.add_NoLock(Pair<KT, VT>(key, value))) {
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return addIfNewKeyAndValue_NoLock(key, value, pFlagExist, value_equals);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::remove_NoLock(const KT& key, VT* outValue)
	{
		if (outValue) {
			Pair<KT, VT> pair;
			if (list.removeValue_NoLock(key, &pair, PairKeyCompare(m_equals))) {
				*outValue = pair.value;
				return sl_true;
			}
			return sl_false;
		} else {
			return list.removeValue_NoLock(key, sl_null, PairKeyCompare(m_equals));
		}
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_size ListMap<KT, VT, KEY_EQUALS>::removeItems_NoLock(const KT& key, List<VT>* outValues)
	{
		if (outValues) {
			List< Pair<KT, VT> > pairs;
			sl_size n = list.removeElementsByValue_NoLock(key, &pairs, PairKeyCompare(m_equals));
			if (n) {
				ListElements< Pair<KT, VT> > p(pairs);
				for (sl_size i = 0; i < p.count; i++) {
					outValues->add_NoLock(p[i].value);
				}
			}
			return n;
		} else {
			return list.removeElementsByValue_NoLock(key, sl_null, PairKeyCompare(m_equals));
		}
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::removeKeyAndValue_NoLock(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		if (outValue) {
			Pair<KT, VT> pair;
			if (list.removeValue_NoLock(Pair<KT, _VT>(key, value), &pair, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals))) {
				*outValue = pair.value;
				return sl_true;
			}
			return sl_false;
		} else {
			return list.removeValue_NoLock(Pair<KT, _VT>(key, value), sl_null, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals));
		}
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::removeKeyAndValue(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return removeKeyAndValue_NoLock(key, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_size ListMap<KT, VT, KEY_EQUALS>::removeItemsByKeyAndValue_NoLock(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		if (outValues) {
			List< Pair<KT, VT> > pairs;
			sl_size n = list.removeElementsByValue_NoLock(Pair<KT, _VT>(key, value), &pairs, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals));
			if (n) {
				ListElements< Pair<KT, VT> > p(pairs);
				for (sl_size i = 0; i < p.count; i++) {
					outValues->add_NoLock(p[i].value);
				}
			}
			return n;
		} else {
			return list.removeElementsByValue_NoLock(Pair<KT, _VT>(key, value), sl_null, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals));
		}
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_size ListMap<KT, VT, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return removeItemsByKeyAndValue_NoLock(key, value, outValues, value_equals);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_size ListMap<KT, VT, KEY_EQUALS>::removeAll_NoLock()
	{
		return list.removeAll_NoLock();
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::contains_NoLock(const KT& key) const
	{
		return list.indexOf_NoLock(key, 0, PairKeyCompare(m_equals)) >= 0;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::containsKeyAndValue_NoLock(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		return list.indexOf_NoLock(Pair<KT, _VT>(key, value), 0, PairCompare<_VT, VALUE_EQUALS>(m_equals, value_equals)) >= 0;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool ListMap<KT, VT, KEY_EQUALS>::containsKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		ObjectLocker lock(this);
		return contains_NoLock(key, value, value_equals);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	IMap<KT, VT>* ListMap<KT, VT, KEY_EQUALS>::duplicate_NoLock() const
	{
		ListMap<KT, VT, KEY_EQUALS>* ret = new ListMap<KT, VT, KEY_EQUALS>(m_equals);
		if (ret) {
			if (ret->list.addElements_NoLock(list.getData(), list.getCount())) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	Iterator<KT> ListMap<KT, VT, KEY_EQUALS>::getKeyIteratorWithRefer(Referable* refer) const
	{
		return new ListMapKeyIterator<KT, VT, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	List<KT> ListMap<KT, VT, KEY_EQUALS>::getAllKeys_NoLock() const
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
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	Iterator<VT> ListMap<KT, VT, KEY_EQUALS>::getValueIteratorWithRefer(Referable* refer) const
	{
		return new ListMapValueIterator<KT, VT, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	List<VT> ListMap<KT, VT, KEY_EQUALS>::getAllValues_NoLock() const
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
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	Iterator< Pair<KT, VT> > ListMap<KT, VT, KEY_EQUALS>::toIteratorWithRefer(Referable* refer) const
	{
		return new ListMapIterator<KT, VT, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class KEY_EQUALS>
	List< Pair<KT, VT> > ListMap<KT, VT, KEY_EQUALS>::toList_NoLock() const
	{
		return list.duplicate();
	}
	
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>::HashMap(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals) : table(capacity)
	{
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMap<KT, VT, HASH, KEY_EQUALS>* HashMap<KT, VT, HASH, KEY_EQUALS>::create(sl_uint32 capacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		HashMap<KT, VT, HASH, KEY_EQUALS>* ret = new HashMap<KT, VT, HASH, KEY_EQUALS>(capacity, hash, key_equals);
		if (ret) {
			if (ret->table.getCapacity() > 0) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT HashMap<KT, VT, HASH, KEY_EQUALS>::operator[](const KT& key) const
	{
		ObjectLocker lock(this);
		VT* p = table.getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::getCount() const
	{
		return (sl_size)(table.getCount());
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	VT* HashMap<KT, VT, HASH, KEY_EQUALS>::getItemPointer(const KT& key) const
	{
		return table.getItemPointer(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getValues_NoLock(const KT& key) const
	{
		return table.getValues(key);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
	{
		return table.put(key, value, mode, pFlagExist);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue_NoLock(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
	{
		return table.addIfNewKeyAndValue(key, value, pFlagExist, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return table.addIfNewKeyAndValue(key, value, pFlagExist, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::remove_NoLock(const KT& key, VT* outValue)
	{
		return table.remove(key, outValue);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItems_NoLock(const KT& key, List<VT>* outValues)
	{
		return table.removeItems(key, outValues);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue_NoLock(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		return table.removeKeyAndValue(key, value, outValue, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::removeKeyAndValue(const KT& key, const _VT& value, VT* outValue, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return table.removeKeyAndValue(key, value, outValue, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue_NoLock(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		return table.removeItemsByKeyAndValue(key, value, outValues, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_EQUALS& value_equals)
	{
		ObjectLocker lock(this);
		return table.removeItemsByKeyAndValue(key, value, outValues, value_equals);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_size HashMap<KT, VT, HASH, KEY_EQUALS>::removeAll_NoLock()
	{
		return table.removeAll();
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::contains_NoLock(const KT& key) const
	{
		return table.search(key) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::containsKeyAndValue_NoLock(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		return table.searchKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	template <class _VT, class VALUE_EQUALS>
	sl_bool HashMap<KT, VT, HASH, KEY_EQUALS>::containsKeyAndValue(const KT& key, const _VT& value, const VALUE_EQUALS& value_equals) const
	{
		ObjectLocker lock(this);
		return table.searchKeyAndValue(key, value, value_equals) != sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	IMap<KT, VT>* HashMap<KT, VT, HASH, KEY_EQUALS>::duplicate_NoLock() const
	{
		HashMap<KT, VT, HASH, KEY_EQUALS>* ret = new HashMap<KT, VT, HASH, KEY_EQUALS>;
		if (ret) {
			if (ret->table.copyFrom(&table)) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	Iterator<KT> HashMap<KT, VT, HASH, KEY_EQUALS>::getKeyIteratorWithRefer(Referable* refer) const
	{
		return new HashMapKeyIterator<KT, VT, HASH, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<KT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllKeys_NoLock() const
	{
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				if (!(ret->add_NoLock(entry->key))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	Iterator<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getValueIteratorWithRefer(Referable* refer) const
	{
		return new HashMapValueIterator<KT, VT, HASH, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List<VT> HashMap<KT, VT, HASH, KEY_EQUALS>::getAllValues_NoLock() const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				if (!(ret->add_NoLock(entry->value))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	Iterator< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_EQUALS>::toIteratorWithRefer(Referable* refer) const
	{
		return new HashMapIterator<KT, VT, HASH, KEY_EQUALS>(this, refer);
	}
	
	template <class KT, class VT, class HASH, class KEY_EQUALS>
	List< Pair<KT, VT> > HashMap<KT, VT, HASH, KEY_EQUALS>::toList_NoLock() const
	{
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			HashEntry<KT, VT>* entry = table.getFirstEntry();
			while (entry) {
				Pair<KT, VT> pair(entry->key, entry->value);
				if (!(ret->add_NoLock(pair))) {
					delete ret;
					return sl_null;
				}
				entry = entry->next;
			}
			return ret;
		}
		return sl_null;
	}
	
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>::TreeMap(const KEY_COMPARE& key_compare) : tree(key_compare)
	{
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	TreeMap<KT, VT, KEY_COMPARE>* TreeMap<KT, VT, KEY_COMPARE>::create(const KEY_COMPARE& key_compare)
	{
		TreeMap<KT, VT, KEY_COMPARE>* ret = new TreeMap<KT, VT, KEY_COMPARE>(key_compare);
		if (ret) {
			if (ret->tree.isValid()) {
				return ret;
			}
			delete ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT TreeMap<KT, VT, KEY_COMPARE>::operator[](const KT& key) const
	{
		ObjectLocker lock(this);
		VT* p = tree.getItemPointer(key);
		if (p) {
			return *p;
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::getCount() const
	{
		return (sl_size)(tree.getCount());
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	VT* TreeMap<KT, VT, KEY_COMPARE>::getItemPointer(const KT& key) const
	{
		return tree.getItemPointer(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getValues_NoLock(const KT& key) const
	{
		return tree.getValues(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
	{
		return tree.put(key, value, mode, pFlagExist);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue_NoLock(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_COMPARE& value_compare)
	{
		return tree.addIfNewKeyAndValue(key, value, pFlagExist, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::addIfNewKeyAndValue(const KT& key, const _VT& value, sl_bool* pFlagExist, const VALUE_COMPARE& value_compare)
	{
		ObjectLocker lock(this);
		return tree.addIfNewKeyAndValue(key, value, pFlagExist, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::remove_NoLock(const KT& key, VT* outValue)
	{
		return tree.remove(key, outValue);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItems_NoLock(const KT& key, List<VT>* outValues)
	{
		return tree.removeItems(key, outValues);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::removeKeyAndValue_NoLock(const KT& key, const _VT& value, VT* outValue, const VALUE_COMPARE& value_compare)
	{
		return tree.removeKeyAndValue(key, value, outValue, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::removeKeyAndValue(const KT& key, const _VT& value, VT* outValue, const VALUE_COMPARE& value_compare)
	{
		ObjectLocker lock(this);
		return tree.removeKeyAndValue(key, value, outValue, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue_NoLock(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_COMPARE& value_compare)
	{
		return tree.removeItemsByKeyAndValue(key, value, outValues, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeItemsByKeyAndValue(const KT& key, const _VT& value, List<VT>* outValues, const VALUE_COMPARE& value_compare)
	{
		ObjectLocker lock(this);
		return tree.removeItemsByKeyAndValue(key, value, outValues, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_size TreeMap<KT, VT, KEY_COMPARE>::removeAll_NoLock()
	{
		return tree.removeAll();
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::contains_NoLock(const KT& key) const
	{
		return tree.search(key);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::containsKeyAndValue_NoLock(const KT& key, const _VT& value, const VALUE_COMPARE& value_compare) const
	{
		return tree.searchKeyAndValue(key, value, sl_null, sl_null, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	template <class _VT, class VALUE_COMPARE>
	sl_bool TreeMap<KT, VT, KEY_COMPARE>::containsKeyAndValue(const KT& key, const _VT& value, const VALUE_COMPARE& value_compare) const
	{
		ObjectLocker lock(this);
		return tree.searchKeyAndValue(key, value, sl_null, sl_null, value_compare);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	IMap<KT, VT>* TreeMap<KT, VT, KEY_COMPARE>::duplicate_NoLock() const
	{
		TreeMap<KT, VT, KEY_COMPARE>* ret = new TreeMap<KT, VT, KEY_COMPARE>;
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
	
	template <class KT, class VT, class KEY_COMPARE>
	Iterator<KT> TreeMap<KT, VT, KEY_COMPARE>::getKeyIteratorWithRefer(Referable* refer) const
	{
		return new TreeMapKeyIterator<KT, VT, KEY_COMPARE>(this, refer);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<KT> TreeMap<KT, VT, KEY_COMPARE>::getAllKeys_NoLock() const
	{
		CList<KT>* ret = new CList<KT>;
		if (ret) {
			TreePosition pos;
			KT key;
			while (tree.getNextPosition(pos, &key, sl_null)) {
				if (!(ret->add_NoLock(key))) {
					delete ret;
					return sl_null;
				}
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	Iterator<VT> TreeMap<KT, VT, KEY_COMPARE>::getValueIteratorWithRefer(Referable* refer) const
	{
		return new TreeMapValueIterator<KT, VT, KEY_COMPARE>(this, refer);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List<VT> TreeMap<KT, VT, KEY_COMPARE>::getAllValues_NoLock() const
	{
		CList<VT>* ret = new CList<VT>;
		if (ret) {
			TreePosition pos;
			VT value;
			while (tree.getNextPosition(pos, sl_null, &value)) {
				if (!(ret->add_NoLock(value))) {
					delete ret;
					return sl_null;
				}
			}
			return ret;
		}
		return sl_null;
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	Iterator< Pair<KT, VT> > TreeMap<KT, VT, KEY_COMPARE>::toIteratorWithRefer(Referable* refer) const
	{
		return new TreeMapIterator<KT, VT, KEY_COMPARE>(this, refer);
	}
	
	template <class KT, class VT, class KEY_COMPARE>
	List< Pair<KT, VT> > TreeMap<KT, VT, KEY_COMPARE>::toList_NoLock() const
	{
		CList< Pair<KT, VT> >* ret = new CList< Pair<KT, VT> >;
		if (ret) {
			TreePosition pos;
			Pair<KT, VT> pair;
			while (tree.getNextPosition(pos, &(pair.key), &(pair.value))) {
				if (!(ret->add_NoLock(pair))) {
					delete ret;
					return sl_null;
				}
			}
			return ret;
		}
		return sl_null;
	}
	
	
	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::create()
	{
		return IMap<KT, VT>::createDefault();
	}
	
	template <class KT, class VT>
	template <class KEY_EQUALS>
	Map<KT, VT> Map<KT, VT>::createList(const KEY_EQUALS& key_equals)
	{
		return ListMap<KT, VT, KEY_EQUALS>::create(key_equals);
	}
	
	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	Map<KT, VT> Map<KT, VT>::createHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		return HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}
	
	template <class KT, class VT>
	template <class KEY_COMPARE>
	Map<KT, VT> Map<KT, VT>::createTree(const KEY_COMPARE& key_compare)
	{
		return TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}
	
	template <class KT, class VT>
	template <class _KT, class _VT>
	const Map<KT, VT>& Map<KT, VT>::from(const Map<_KT, _VT>& other)
	{
		return *(reinterpret_cast<Map<KT, VT> const*>(&other));
	}

	template <class KT, class VT>
	void Map<KT, VT>::init()
	{
		ref = IMap<KT, VT>::createDefault();
	}

	template <class KT, class VT>
	template <class KEY_EQUALS>
	void Map<KT, VT>::initList(const KEY_EQUALS& key_equals)
	{
		ref = ListMap<KT, VT, KEY_EQUALS>::create(key_equals);
	}
	
	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	void Map<KT, VT>::initHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		ref = HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}
	
	template <class KT, class VT>
	template <class KEY_COMPARE>
	void Map<KT, VT>::initTree(const KEY_COMPARE& key_compare)
	{
		ref = TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}
	
	template <class KT, class VT>
	VT Map<KT, VT>::operator[](const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}
	
	template <class KT, class VT>
	sl_size Map<KT, VT>::getCount() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}
	
	template <class KT, class VT>
	sl_bool Map<KT, VT>::isEmpty() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::isNotEmpty() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT* Map<KT, VT>::getItemPointer(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getItemPointer(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	VT* Map<KT, VT>::getNewItemPointer(const KT& key)
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (!obj) {
			obj = IMap<KT, VT>::createDefault();
			if (obj) {
				ref = obj;
			} else {
				return sl_null;
			}
		}
		VT* ret = obj->getItemPointer(key);
		if (!ret) {
			VT t;
			obj->put(key, t);
			ret = obj->getItemPointer(key);
		}
		return ret;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::get_NoLock(const KT& key, VT* _out) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->get_NoLock(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::get(const KT& key, VT* _out) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue_NoLock(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue_NoLock(const KT& key, const VT& def) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue_NoLock(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	VT Map<KT, VT>::getValue(const KT& key, const VT& def) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValue(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getValues_NoLock(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValues_NoLock(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getValues(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValues(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::put_NoLock(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
	{
		IMap<KT, VT>* obj = ref._ptr;
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
		IMap<KT, VT>* obj = ref._ptr;
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
	template <class _KT, class _VT>
	void Map<KT, VT>::putAll_NoLock(const Map<_KT, _VT>& other, MapPutMode mode)
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->putAll_NoLock(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					obj->putAll_NoLock(other.ref._ptr, mode);
				}
			}
		}
	}
	
	template <class KT, class VT>
	template <class _KT, class _VT>
	void Map<KT, VT>::putAll_NoLock(const AtomicMap<_KT, _VT>& other, MapPutMode mode)
	{
		putAll_NoLock(Map<_KT, _VT>(other), mode);
	}

	template <class KT, class VT>
	template <class _KT, class _VT>
	void Map<KT, VT>::putAll(const Map<_KT, _VT>& other, MapPutMode mode)
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			obj->putAll(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				obj = IMap<KT, VT>::createDefault();
				if (obj) {
					ref = obj;
					obj->putAll(other.ref._ptr, mode);
				}
			}
		}
	}

	template <class KT, class VT>
	template <class _KT, class _VT>
	void Map<KT, VT>::putAll(const AtomicMap<_KT, _VT>& other, MapPutMode mode)
	{
		putAll(Map<_KT, _VT>(other), mode);
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::remove_NoLock(const KT& key, VT* outValue) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->remove_NoLock(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::remove(const KT& key, VT* outValue) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeItems_NoLock(const KT& key, List<VT>* outValues) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeItems_NoLock(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeItems(const KT& key, List<VT>* outValues) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeItems(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeAll_NoLock() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll_NoLock();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Map<KT, VT>::removeAll() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::contains_NoLock(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->contains_NoLock(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Map<KT, VT>::contains(const KT& key) const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->contains(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::duplicate_NoLock() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Map<KT, VT> Map<KT, VT>::duplicate() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator<KT> Map<KT, VT>::getKeyIterator() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getKeyIteratorWithRefer(obj);
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<KT> Map<KT, VT>::getAllKeys_NoLock() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<KT> Map<KT, VT>::getAllKeys() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator<VT> Map<KT, VT>::getValueIterator() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getValueIteratorWithRefer(obj);
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getAllValues_NoLock() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List<VT> Map<KT, VT>::getAllValues() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator< Pair<KT, VT> > Map<KT, VT>::toIterator() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->toIteratorWithRefer(obj);
		}
		return sl_null;
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Map<KT, VT>::toList_NoLock() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->toList_NoLock();
		}
		return sl_null;
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Map<KT, VT>::toList() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > Map<KT, VT>::begin() const
	{
		return toIterator().begin();
	}

	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > Map<KT, VT>::end() const
	{
		return IteratorPosition< Pair<KT, VT> >();
	}

	template <class KT, class VT>
	const Mutex* Map<KT, VT>::getLocker() const
	{
		IMap<KT, VT>* obj = ref._ptr;
		if (obj) {
			return obj->getLocker();
		}
		return sl_null;
	}

	
	template <class KT, class VT>
	template <class _KT, class _VT>
	const Atomic< Map<KT, VT> >& Atomic< Map<KT, VT> >::from(const Atomic< Map<_KT, _VT> >& other)
	{
		return *(reinterpret_cast<Atomic< Map<KT, VT> > const*>(&other));
	}

	template <class KT, class VT>
	void Atomic< Map<KT, VT> >::init()
	{
		ref = IMap<KT, VT>::createDefault();
	}

	template <class KT, class VT>
	template <class KEY_EQUALS>
	void Atomic< Map<KT, VT> >::initList(const KEY_EQUALS& key_equals)
	{
		ref = ListMap<KT, VT, KEY_EQUALS>::create(key_equals);
	}

	template <class KT, class VT>
	template <class HASH, class KEY_EQUALS>
	void Atomic< Map<KT, VT> >::initHash(sl_uint32 initialCapacity, const HASH& hash, const KEY_EQUALS& key_equals)
	{
		ref = HashMap<KT, VT, HASH, KEY_EQUALS>::create(initialCapacity, hash, key_equals);
	}

	template <class KT, class VT>
	template <class KEY_COMPARE>
	void Atomic< Map<KT, VT> >::initTree(const KEY_COMPARE& key_compare)
	{
		ref = TreeMap<KT, VT, KEY_COMPARE>::create(key_compare);
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::operator[](const KT& key) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::getCount() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::isEmpty() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) == 0;
		}
		return sl_true;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::isNotEmpty() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return (obj->getCount()) > 0;
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::get(const KT& key, VT* _out) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->get(key, _out);
		}
		return sl_false;
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::getValue(const KT& key) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key);
		} else {
			return VT();
		}
	}

	template <class KT, class VT>
	VT Atomic< Map<KT, VT> >::getValue(const KT& key, const VT& def) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValue(key, def);
		}
		return def;
	}

	template <class KT, class VT>
	List<VT> Atomic< Map<KT, VT> >::getValues(const KT& key) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getValues(key);
		}
		return sl_null;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::put(const KT& key, const VT& value, MapPutMode mode, sl_bool* pFlagExist)
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
	template <class _KT, class _VT>
	void Atomic< Map<KT, VT> >::putAll(const Map<_KT, _VT>& other, MapPutMode mode)
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			obj->putAll(other.ref._ptr, mode);
		} else {
			if (mode != MapPutMode::ReplaceExisting) {
				SpinLocker lock(SpinLockPoolForMap::get(this));
				obj = ref;
				if (obj.isNotNull()) {
					lock.unlock();
					obj->putAll(other.ref._ptr, mode);
				}
				obj = IMap<KT, VT>::createDefault();
				if (obj.isNotNull()) {
					ref = obj;
					lock.unlock();
					obj->putAll(other.ref._ptr, mode);
				}
			}
		}
	}

	template <class KT, class VT>
	template <class _KT, class _VT>
	void Atomic< Map<KT, VT> >::putAll(const AtomicMap<_KT, _VT>& other, MapPutMode mode)
	{
		putAll(Map<_KT, _VT>(other), mode);
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::remove(const KT& key, VT* outValue) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->remove(key, outValue);
		}
		return sl_false;
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::removeItems(const KT& key, List<VT>* outValues) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeItems(key, outValues);
		}
		return 0;
	}

	template <class KT, class VT>
	sl_size Atomic< Map<KT, VT> >::removeAll() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->removeAll();
		}
		return 0;
	}

	template <class KT, class VT>
	sl_bool Atomic< Map<KT, VT> >::contains(const KT& key) const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->contains(key);
		}
		return sl_false;
	}

	template <class KT, class VT>
	Map<KT, VT> Atomic< Map<KT, VT> >::duplicate() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator<KT> Atomic< Map<KT, VT> >::getKeyIterator() const
	{
		Map<KT, VT> map(*this);
		return map.getKeyIterator();
	}

	template <class KT, class VT>
	List<KT> Atomic< Map<KT, VT> >::getAllKeys() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllKeys();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator<VT> Atomic< Map<KT, VT> >::getValueIterator() const
	{
		Map<KT, VT> map(*this);
		return map.getValueIterator();
	}

	template <class KT, class VT>
	List<VT> Atomic< Map<KT, VT> >::getAllValues() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->getAllValues();
		}
		return sl_null;
	}

	template <class KT, class VT>
	Iterator< Pair<KT, VT> > Atomic< Map<KT, VT> >::toIterator() const
	{
		Map<KT, VT> map(*this);
		return map.toIterator();
	}

	template <class KT, class VT>
	List< Pair<KT, VT> > Atomic< Map<KT, VT> >::toList() const
	{
		Ref< IMap<KT, VT> > obj(ref);
		if (obj.isNotNull()) {
			return obj->toList();
		}
		return sl_null;
	}

	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > Atomic< Map<KT, VT> >::begin() const
	{
		return toIterator().begin();
	}

	template <class KT, class VT>
	SLIB_INLINE IteratorPosition< Pair<KT, VT> > Atomic< Map<KT, VT> >::end() const
	{
		return IteratorPosition< Pair<KT, VT> >();
	}


	template <class KT, class VT, class KEY_EQUALS>
	ListMapKeyIterator<KT, VT, KEY_EQUALS>::ListMapKeyIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapKeyIterator<KT, VT, KEY_EQUALS>::hasNext()
	{
		return (m_index < m_map->getCount());
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapKeyIterator<KT, VT, KEY_EQUALS>::next(KT* _out)
	{
		if (_out) {
			Pair<KT, VT> pair;
			if (m_map->list.getAt_NoLock(m_index, &pair)) {
				*_out = pair.key;
				m_index++;
				return sl_true;
			}
		} else {
			if (m_map->list.getAt_NoLock(m_index, sl_null)) {
				m_index++;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_reg ListMapKeyIterator<KT, VT, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class KEY_EQUALS>
	ListMapValueIterator<KT, VT, KEY_EQUALS>::ListMapValueIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapValueIterator<KT, VT, KEY_EQUALS>::hasNext()
	{
		return (m_index < m_map->getCount());
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapValueIterator<KT, VT, KEY_EQUALS>::next(VT* _out)
	{
		if (_out) {
			Pair<KT, VT> pair;
			if (m_map->list.getAt_NoLock(m_index, &pair)) {
				*_out = pair.value;
				m_index++;
				return sl_true;
			}
		} else {
			if (m_map->list.getAt_NoLock(m_index, sl_null)) {
				m_index++;
				return sl_true;
			}
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_reg ListMapValueIterator<KT, VT, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class KEY_EQUALS>
	ListMapIterator<KT, VT, KEY_EQUALS>::ListMapIterator(const ListMap<KT, VT, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapIterator<KT, VT, KEY_EQUALS>::hasNext()
	{
		return (m_index < m_map->getCount());
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_bool ListMapIterator<KT, VT, KEY_EQUALS>::next(Pair<KT, VT>* _out)
	{
		if (m_map->list.getAt_NoLock(m_index, _out)) {
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_EQUALS>
	sl_reg ListMapIterator<KT, VT, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMapKeyIterator<KT, VT, HASH, KEY_EQUALS>::HashMapKeyIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_entry(map->table.getFirstEntry()), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapKeyIterator<KT, VT, HASH, KEY_EQUALS>::hasNext()
	{
		return m_entry != sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapKeyIterator<KT, VT, HASH, KEY_EQUALS>::next(KT* _out)
	{
		if (m_entry) {
			if (_out) {
				*_out = m_entry->key;
			}
			m_entry = m_entry->next;
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_reg HashMapKeyIterator<KT, VT, HASH, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMapValueIterator<KT, VT, HASH, KEY_EQUALS>::HashMapValueIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_entry(map->table.getFirstEntry()), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapValueIterator<KT, VT, HASH, KEY_EQUALS>::hasNext()
	{
		return m_entry != sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapValueIterator<KT, VT, HASH, KEY_EQUALS>::next(VT* _out)
	{
		if (m_entry) {
			if (_out) {
				*_out = m_entry->value;
			}
			m_entry = m_entry->next;
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_reg HashMapValueIterator<KT, VT, HASH, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class HASH, class KEY_EQUALS>
	HashMapIterator<KT, VT, HASH, KEY_EQUALS>::HashMapIterator(const HashMap<KT, VT, HASH, KEY_EQUALS>* map, Referable* refer)
	: m_map(map), m_entry(map->table.getFirstEntry()), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapIterator<KT, VT, HASH, KEY_EQUALS>::hasNext()
	{
		return m_entry != sl_null;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_bool HashMapIterator<KT, VT, HASH, KEY_EQUALS>::next(Pair<KT, VT>* _out)
	{
		if (m_entry) {
			if (_out) {
				_out->key = m_entry->key;
				_out->value = m_entry->value;
			}
			m_entry = m_entry->next;
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class HASH, class KEY_EQUALS>
	sl_reg HashMapIterator<KT, VT, HASH, KEY_EQUALS>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class KEY_COMPARE>
	TreeMapKeyIterator<KT, VT, KEY_COMPARE>::TreeMapKeyIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapKeyIterator<KT, VT, KEY_COMPARE>::hasNext()
	{
		TreePosition pos = m_pos;
		return m_map->tree.getNextPosition(pos);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapKeyIterator<KT, VT, KEY_COMPARE>::next(KT* _out)
	{
		if (m_map->tree.getNextPosition(m_pos, _out, sl_null)) {
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_reg TreeMapKeyIterator<KT, VT, KEY_COMPARE>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class KEY_COMPARE>
	TreeMapValueIterator<KT, VT, KEY_COMPARE>::TreeMapValueIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapValueIterator<KT, VT, KEY_COMPARE>::hasNext()
	{
		TreePosition pos = m_pos;
		return m_map->tree.getNextPosition(pos);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapValueIterator<KT, VT, KEY_COMPARE>::next(VT* _out)
	{
		if (m_map->tree.getNextPosition(m_pos, sl_null, _out)) {
			m_index++;
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_reg TreeMapValueIterator<KT, VT, KEY_COMPARE>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}


	template <class KT, class VT, class KEY_COMPARE>
	TreeMapIterator<KT, VT, KEY_COMPARE>::TreeMapIterator(const TreeMap<KT, VT, KEY_COMPARE>* map, Referable* refer)
	: m_map(map), m_index(0), m_refer(refer)
	{
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapIterator<KT, VT, KEY_COMPARE>::hasNext()
	{
		TreePosition pos = m_pos;
		return m_map->tree.getNextPosition(pos);
	}

	template <class KT, class VT, class KEY_COMPARE>
	sl_bool TreeMapIterator<KT, VT, KEY_COMPARE>::next(Pair<KT, VT>* _out)
	{
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

	template <class KT, class VT, class KEY_COMPARE>
	sl_reg TreeMapIterator<KT, VT, KEY_COMPARE>::getIndex()
	{
		return (sl_reg)m_index - 1;
	}

}

#endif
