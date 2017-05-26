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
#include "slib/core/hash_table.h"

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
	
	void _priv_HashTable::init(HashTableBaseTable* table) noexcept
	{
		Base::zeroMemory(table, sizeof(HashTableBaseTable));
		table->capacityMin = PRIV_SLIB_HASHTABLE_MIN_CAPACITY;
	}
	
	void _priv_HashTable::init(HashTableBaseTable* table, sl_uint32 capacity) noexcept
	{
		if (capacity) {
			capacity = Math::roundUpToPowerOfTwo(capacity);
			if (createNodes(table, capacity)) {
				table->count = 0;
				table->firstNode = sl_null;
				table->lastNode = sl_null;
				Base::zeroMemory(table->nodes, sizeof(Node*)*capacity);
				table->capacityMin = capacity;
				return;
			}
		}
		init(table);
	}
	
	sl_bool _priv_HashTable::validateNodes(HashTableBaseTable* table) noexcept
	{
		if (table->capacity == 0) {
			if (createNodes(table, PRIV_SLIB_HASHTABLE_MIN_CAPACITY)) {
				Base::zeroMemory(table->nodes, sizeof(Node*)*PRIV_SLIB_HASHTABLE_MIN_CAPACITY);
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return sl_true;
		}
	}
	
	sl_bool _priv_HashTable::createNodes(HashTableBaseTable* table, sl_uint32 capacity) noexcept
	{
		if (capacity > PRIV_SLIB_HASHTABLE_MAX_CAPACITY) {
			return sl_false;
		}
		Node** nodes = (Node**)(Base::createMemory(sizeof(Node*)*capacity));
		if (nodes) {
			table->nodes = nodes;
			table->capacity = capacity;
			table->thresholdUp = (sl_uint32)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_UP * capacity);
			table->thresholdDown = (sl_uint32)(PRIV_SLIB_HASHTABLE_LOAD_FACTOR_DOWN * capacity);
			return sl_true;
		}
		return sl_false;
	}

	void _priv_HashTable::free(HashTableBaseTable* table) noexcept
	{
		Node** nodes = table->nodes;
		if (nodes) {
			Base::freeMemory(nodes);
		}
	}
	
	void _priv_HashTable::add(HashTableBaseTable* table, HashTableBaseNode* node, sl_uint32 hash) noexcept
	{

		table->count++;
		
		sl_uint32 capacity = table->capacity;
		Node** nodes = table->nodes;
		
		sl_uint32 index = hash & (capacity - 1);
		
		node->hash = hash;
		
		node->chain = nodes[index];
		
		Node* last = table->lastNode;
		if (last) {
			last->next = node;
		}
		table->lastNode = node;
		node->before = last;
		if (!(table->firstNode)) {
			table->firstNode = node;
		}
		node->next = sl_null;
		
		nodes[index] = node;
		
	}
	
	void _priv_HashTable::remove(HashTableBaseTable* table, HashTableBaseNode* node) noexcept
	{
		table->count--;
		
		Node* before = node->before;
		Node* next = node->next;
		if (before) {
			before->next = next;
		} else {
			table->firstNode = next;
		}
		if (next) {
			next->before = before;
		} else {
			table->lastNode = before;
		}
	}
	
	void _priv_HashTable::expand(HashTableBaseTable* table) noexcept
	{
		if (table->count >= table->thresholdUp) {
			// double capacity
			Node** nodesOld = table->nodes;
			sl_uint32 n = table->capacity;
			if (createNodes(table, n + n)) {
				Node** nodes = table->nodes;
				for (sl_uint32 i = 0; i < n; i++) {
					Node* node = nodesOld[i];
					nodes[i] = sl_null;
					nodes[i | n] = sl_null;
					if (node) {
						sl_uint32 highBitBefore = node->hash & n;
						Node* broken = sl_null;
						nodes[i | highBitBefore] = node;
						while (Node* chain = node->chain) {
							sl_uint32 highBitChain = chain->hash & n;
							if (highBitBefore != highBitChain) {
								if (broken) {
									broken->chain = chain;
								} else {
									nodes[i | highBitChain] = chain;
								}
								broken = node;
								highBitBefore = highBitChain;
							}
							node = chain;
						}
						if (broken) {
							broken->chain = sl_null;
						}
					}
				}
				Base::freeMemory(nodesOld);
			}
		}
	}
	
	void _priv_HashTable::compact(HashTableBaseTable* table) noexcept
	{
		if (table->capacity > table->capacityMin && table->count <= table->thresholdDown) {
			// half capacity
			Node** nodesOld = table->nodes;
			sl_uint32 n = table->capacity >> 1;
			if (createNodes(table, n)) {
				Node** nodes = table->nodes;
				for (sl_uint32 i = 0; i < n; i++) {
					nodes[i] = nodesOld[i];
					Node** link = nodes + i;
					while (Node* node = *link) {
						link = &(node->chain);
					}
					*link = nodesOld[i | n];
				}
				Base::freeMemory(nodesOld);
			}
		}
	}
	
}
