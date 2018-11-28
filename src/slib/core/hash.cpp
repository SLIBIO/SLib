/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/hash.h"
#include "slib/core/hash_table.h"

#include "slib/core/math.h"

namespace slib
{

	/****************************************************
	 
		Fowler–Noll–Vo hash function / FNV-1a hash
	 
	 https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	 http://www.isthe.com/chongo/tech/comp/fnv/index.html
	 
	****************************************************/
	sl_uint32 HashBytes32(const void* _buf, sl_size n) noexcept
	{
		sl_uint8* buf = (sl_uint8*)_buf;
		sl_uint32 hash = 0x811c9dc5;
		sl_uint32 prime = 16777619;
		for (sl_size i = 0; i < n; i++) {
			hash ^= buf[i];
			hash *= prime;
		}
		return hash;
	}
	
	sl_uint64 HashBytes64(const void* _buf, sl_size n) noexcept
	{
		sl_uint8* buf = (sl_uint8*)_buf;
		sl_uint64 hash = SLIB_UINT64(0xcbf29ce484222325);
		sl_uint64 prime = SLIB_UINT64(1099511628211);
		for (sl_size i = 0; i < n; i++) {
			hash ^= buf[i];
			hash *= prime;
		}
		return hash;
	}
	
	sl_size HashBytes(const void* buf, sl_size n) noexcept
	{
#ifdef SLIB_ARCH_IS_64BIT
		return HashBytes64(buf, n);
#else
		return HashBytes32(buf, n);
#endif
	}

	
	#define PRIV_SLIB_HASHTABLE_MIN_CAPACITY 16
	#define PRIV_SLIB_HASHTABLE_LOAD_FACTOR_UP 0.75f
	#define PRIV_SLIB_HASHTABLE_LOAD_FACTOR_DOWN 0.25f
	
	void _priv_HashTable::fixCapacityRange(HashTableStructBase* table) noexcept
	{
		sl_size _capacityMinimum = table->capacityMinimum;
		table->capacityMinimum = Math::roundUpToPowerOfTwo(_capacityMinimum);
		if (table->capacityMinimum < _capacityMinimum) {
			table->capacityMinimum = table->capacityMinimum << 1;
		}
		if (table->capacityMinimum < PRIV_SLIB_HASHTABLE_MIN_CAPACITY || (sl_reg)(table->capacityMinimum) < 0) {
			table->capacityMinimum = PRIV_SLIB_HASHTABLE_MIN_CAPACITY;
		}
		if (table->capacityMaximum == 0) {
			table->capacityMaximum = SLIB_SIZE_MAX;
		} else {
			if (table->capacityMaximum < table->capacityMinimum) {
				table->capacityMaximum = table->capacityMinimum;
			}
		}
	}
	
	void _priv_HashTable::updateThresholds(HashTableStructBase* table) noexcept
	{
		if (table->capacity == 0) {
			return;
		}
		table->thresholdDown = (sl_size)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_DOWN * table->capacity);
		if (table->thresholdDown < table->capacityMinimum) {
			table->thresholdDown = table->capacityMinimum;
		}
		table->thresholdUp = (sl_size)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_UP * table->capacity);
		if (table->thresholdUp > table->capacityMaximum) {
			table->thresholdUp = table->capacityMaximum;
		}
	}

	void _priv_HashTable::init(HashTableStructBase* table, sl_size capacityMinimum, sl_size capacityMaximum) noexcept
	{
		table->nodes = sl_null;
		table->count = 0;
		table->capacity = 0;
		table->capacityMinimum = capacityMinimum;
		table->capacityMaximum = capacityMaximum;
		table->thresholdUp = 0;
		table->thresholdDown = 0;
		fixCapacityRange(table);
	}
	
	void _priv_HashTable::move(HashTableStructBase* dst, HashTableStructBase* src) noexcept
	{
		Base::copyMemory(dst, src, sizeof(HashTableStructBase));
		clear(src);
	}
	
	void _priv_HashTable::clear(HashTableStructBase* table) noexcept
	{
		table->nodes = sl_null;
		table->count = 0;
		table->capacity = 0;
		table->thresholdUp = 0;
		table->thresholdDown = 0;
	}
	
	void _priv_HashTable::setMinimumCapacity(HashTableStructBase* table, sl_size capacity) noexcept
	{
		table->capacityMinimum = capacity;
		fixCapacityRange(table);
		updateThresholds(table);
	}
	
	void _priv_HashTable::setMaximumCapacity(HashTableStructBase* table, sl_size capacity) noexcept
	{
		table->capacityMaximum = capacity;
		fixCapacityRange(table);
		updateThresholds(table);
	}

	sl_bool _priv_HashTable::validateNodes(HashTableStructBase* table) noexcept
	{
		if (table->capacity == 0) {
			sl_size capacity = table->capacityMinimum;
			NODE** nodes = (NODE**)(Base::createMemory(sizeof(NODE*)*capacity));
			if (nodes) {
				Base::zeroMemory(nodes, sizeof(NODE*)*capacity);
				table->nodes = nodes;
				table->capacity = capacity;
				updateThresholds(table);
				return sl_true;
			}
			return sl_false;
		} else {
			return sl_true;
		}
	}
	
	sl_bool _priv_HashTable::reallocNodes(HashTableStructBase* table, sl_size capacity) noexcept
	{
		if (capacity > table->capacityMaximum) {
			return sl_false;
		}
		NODE** nodes = (NODE**)(Base::reallocMemory(table->nodes, sizeof(NODE*)*capacity));
		if (nodes) {
			table->nodes = nodes;
			table->capacity = capacity;
			updateThresholds(table);
			return sl_true;
		}
		return sl_false;
	}

	void _priv_HashTable::expand(HashTableStructBase* table) noexcept
	{
		if (table->capacity < table->capacityMaximum && table->count >= table->thresholdUp) {
			// double capacity
			sl_size n = table->capacity;
			if (reallocNodes(table, n + n)) {
				NODE** nodes = table->nodes;
				for (sl_size i = 0; i < n; i++) {
					NODE* node = nodes[i];
					nodes[i | n] = sl_null;
					if (node) {
						nodes[i] = sl_null;
						sl_size highBitBefore = node->hash & n;
						NODE* broken = sl_null;
						nodes[i | highBitBefore] = node;
						while (NODE* chain = node->next) {
							sl_size highBitChain = chain->hash & n;
							if (highBitBefore != highBitChain) {
								if (broken) {
									broken->next = chain;
								} else {
									nodes[i | highBitChain] = chain;
								}
								broken = node;
								highBitBefore = highBitChain;
							}
							node = chain;
						}
						if (broken) {
							broken->next = sl_null;
						}
					}
				}
			}
		}
	}
	
	void _priv_HashTable::shrink(HashTableStructBase* table) noexcept
	{
		while (table->capacity > table->capacityMinimum && table->count <= table->thresholdDown) {
			// half capacity
			NODE** nodes = table->nodes;
			sl_size n = table->capacity >> 1;
			for (sl_size i = 0; i < n; i++) {
				NODE** link = nodes + i;
				while (NODE* node = *link) {
					link = &(node->next);
				}
				*link = nodes[i | n];
			}
			reallocNodes(table, n);
		}
	}

}
