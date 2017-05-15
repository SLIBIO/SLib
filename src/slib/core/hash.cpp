/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/hash.h"
#include "slib/core/hashtable.h"

namespace slib
{
	
	// based on adler32
	sl_uint32 HashBytes(const void* _buf, sl_size n) noexcept
	{
		#define PRIV_SLIB_MOD_ADLER 65521
		sl_uint8* buf = (sl_uint8*)_buf;
		sl_uint32 a = 1, b = 0;
		for (sl_size i = 0; i < n; i++) {
			a = (a + buf[i]) % PRIV_SLIB_MOD_ADLER;
			b = (b + a) % PRIV_SLIB_MOD_ADLER;
		}
		return Rehash((b << 16) | a);
	}

	
	#define PRIV_SLIB_HASHTABLE_MIN_CAPACITY 16
	#define PRIV_SLIB_HASHTABLE_MAX_CAPACITY 0x10000000
	#define PRIV_SLIB_HASHTABLE_LOAD_FACTOR_UP 0.75f
	#define PRIV_SLIB_HASHTABLE_LOAD_FACTOR_DOWN 0.25f
	
	void _priv_HashTable::init(HashBaseTable* table) noexcept
	{
		Base::zeroMemory(table, sizeof(HashBaseTable));
	}
	
	void _priv_HashTable::init(HashBaseTable* table, sl_uint32 capacity) noexcept
	{
		if (capacity) {
			if (createEntries(table, capacity)) {
				table->count = 0;
				table->firstEntry = sl_null;
				table->lastEntry = sl_null;
				return;
			}
		}
		Base::zeroMemory(table, sizeof(HashBaseTable));
	}
	
	sl_bool _priv_HashTable::validateEntries(HashBaseTable* table) noexcept
	{
		if (table->capacity == 0) {
			return createEntries(table, PRIV_SLIB_HASHTABLE_MIN_CAPACITY);
		} else {
			return sl_true;
		}
	}
	
	sl_bool _priv_HashTable::createEntries(HashBaseTable* table, sl_uint32 capacity) noexcept
	{
		if (capacity > PRIV_SLIB_HASHTABLE_MAX_CAPACITY) {
			return sl_false;
		}
		Entry** entries = (Entry**)(Base::createMemory(sizeof(Entry*)*capacity));
		if (entries) {
			table->entries = entries;
			table->capacity = capacity;
			table->thresholdUp = (sl_uint32)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_UP * capacity);
			table->thresholdDown = (sl_uint32)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_DOWN * capacity);
			return sl_true;
		}
		return sl_false;
	}

	void _priv_HashTable::free(HashBaseTable* table) noexcept
	{
		Entry** entries = table->entries;
		if (entries) {
			Base::freeMemory(entries);
		}
	}
	
	void _priv_HashTable::add(HashBaseTable* table, HashBaseEntry* entry, sl_uint32 hash) noexcept
	{

		table->count++;
		
		sl_uint32 capacity = table->capacity;
		Entry** entries = table->entries;
		
		sl_uint32 index = hash & (capacity - 1);
		
		entry->hash = hash;
		
		entry->chain = entries[index];
		
		Entry* last = table->lastEntry;
		if (last) {
			last->next = entry;
		}
		table->lastEntry = entry;
		entry->before = last;
		if (!(table->firstEntry)) {
			table->firstEntry = entry;
		}
		entry->next = sl_null;
		
		entries[index] = entry;
		
	}
	
	void _priv_HashTable::remove(HashBaseTable* table, HashBaseEntry* entry) noexcept
	{
		table->count--;
		
		Entry* before = entry->before;
		Entry* next = entry->next;
		if (before) {
			before->next = next;
		} else {
			table->firstEntry = next;
		}
		if (next) {
			next->before = before;
		} else {
			table->lastEntry = before;
		}
	}
	
	void _priv_HashTable::expand(HashBaseTable* table) noexcept
	{
		if (table->count >= table->thresholdUp) {
			// double capacity
			Entry** entriesOld = table->entries;
			sl_uint32 n = table->capacity;
			if (createEntries(table, n + n)) {
				Entry** entries = table->entries;
				for (sl_uint32 i = 0; i < n; i++) {
					Entry* entry = entriesOld[i];
					entries[i] = sl_null;
					entries[i | n] = sl_null;
					if (entry) {
						sl_uint32 highBitBefore = entry->hash & n;
						Entry* broken = sl_null;
						entries[i | highBitBefore] = entry;
						while (Entry* chain = entry->chain) {
							sl_uint32 highBitChain = chain->hash & n;
							if (highBitBefore != highBitChain) {
								if (broken) {
									broken->chain = chain;
								} else {
									entries[i | highBitChain] = chain;
								}
								broken = entry;
								highBitBefore = highBitChain;
							}
							entry = chain;
						}
						if (broken) {
							broken->chain = sl_null;
						}
					}
				}
				Base::freeMemory(entriesOld);
			}
		}
	}
	
	void _priv_HashTable::compact(HashBaseTable* table) noexcept
	{
		if (table->capacity > PRIV_SLIB_HASHTABLE_MIN_CAPACITY && table->count <= table->thresholdDown) {
			// half capacity
			Entry** entriesOld = table->entries;
			sl_uint32 n = table->capacity >> 1;
			if (createEntries(table, n)) {
				Entry** entries = table->entries;
				for (sl_uint32 i = 0; i < n; i++) {
					entries[i] = entriesOld[i];
					Entry** link = entries + i;
					while (Entry* entry = *link) {
						link = &(entry->chain);
					}
					*link = entriesOld[i | n];
				}
				Base::freeMemory(entriesOld);
			}
		}
	}
	
}
