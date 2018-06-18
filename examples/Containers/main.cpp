/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <slib/core.h>
#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	// List Example
	{
		int i;
		List<String> list = {"a", "b", "c", "de", "a", "b"};
		
		Println("List Original (Count: %d)", list.getCount());
		i = 0;
		for (auto& item : list) {
			Println("[%d]=%s", i++, item);
		}
		
		Println("Member Access");
		Println("[3]=%s", list[3]);
		
		// Insert some values into the list
		for (i = 0; i < 5; i++) {
			list.add(String::format("t%d", i));
		}
		Println("List after insertion (Count: %d)", list.getCount());
		i = 0;
		for (auto& item : list) {
			Println("[%d]=%s", i++, item);
		}
		
		// Remove some values from the list
		list.removeRange(7, 3);
		list.removeAt(3);
		list.removeValues("b");
		Println("List after removal (Count: %d)", list.getCount());
		i = 0;
		for (auto& item : list) {
			Println("[%d]=%s", i++, item);
		}
		
	}
	
	// Map Example
	{
		
		HashMap<String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}, {"ab", 11}, {"cd", 34}};
		
		Println("HashMap Original (Count: %d)", map.getCount());
		for (auto& item : map) {
			Println("[%s]=%d", item.key, item.value);
		}

		Println("Member Access");
		Println("[ab]=%s", map["ab"]);

		// Insert some values into the map
		map.put("ab", 12);
		map.put("aaa", 111);
		map.put("abc", 123);
		map.put("baa", 211);
		map.put("bac", 213);
		Println("HashMap after insertion (Count: %d)", map.getCount());
		for (auto& item : map) {
			Println("[%s]=%d", item.key, item.value);
		}
		
		// Remove some values from the map
		map.remove("ab");
		map.remove("cd");
		Println("HashMap after removal (Count: %d)", map.getCount());
		for (auto& item : map) {
			Println("[%s]=%d", item.key, item.value);
		}
		
		// Convert to Ordered Map
		Map<String, int> tree;
		tree.putAll(map);
		Println("Ordered Map (Count: %d)", tree.getCount());
		for (auto& item : tree) {
			Println("[%s]=%d", item.key, item.value);
		}

	}
	return 0;
}
