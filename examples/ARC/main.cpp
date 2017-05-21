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

using namespace slib;

class ExampleNode : public Object
{
private:
	String name;

	WeakRef<ExampleNode> parent;
	List<Ref<ExampleNode>> children;

public:
	ExampleNode(const String& name, Ref<ExampleNode> parent = nullptr): name(name)
	{
		SLIB_KEEP_REF;
		
		Println("Constructor called: %s", name);
		if (parent != nullptr) {
			this->parent = parent;
			parent->children.add(this);
		}
	}

	~ExampleNode()
	{
		Println("Destructor called: %s", name);
	}

	void print()
	{
		Ref<ExampleNode> parent = this->parent;
		if (parent == nullptr) {
			Println("[Print] Root Node: %s", name);
		} else {
			if (children.getCount() > 0) {
				Println("[Print] Node: %s, Parent: %s", name, parent->name);
			} else {
				Println("[Print] Leaf Node: %s, Parent: %s", name, parent->name);
			}
		}
		for (auto& child : children) {
			child->print();
		}
	}

};

int main(int argc, const char * argv[]) {
	
	Ref<ExampleNode> node = new ExampleNode("Node0");
	Ref<ExampleNode> node1 = new ExampleNode("Node1", node);
	Ref<ExampleNode> node11 = new ExampleNode("Node1-1", node1);
	Ref<ExampleNode> node12 = new ExampleNode("Node1-2", node1);
	Ref<ExampleNode> node2 = new ExampleNode("Node2", node);
	Ref<ExampleNode> node21 = new ExampleNode("Node2-1", node2);
	Ref<ExampleNode> node22 = new ExampleNode("Node2-2", node2);
	
	node->print();
	
	return 0;
}
