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

#include <slib/core.h>

using namespace slib;

class ExampleNode : public Object
{
private:
	String name;

	WeakRef<ExampleNode> parent;
	List<Ref<ExampleNode>> children;

public:
	ExampleNode()
	{
		Println("Constructor called");
		// Not safe to access `this` reference
	}
	
	~ExampleNode()
	{
		Println("Destructor called: %s", name);
	}

	void init() override
	{
		Object::init();
		
		Println("Initializer called");
		// Safe to access `this` reference
	}
	
	void init(const String& name, Ref<ExampleNode> parent = nullptr)
	{
		this->name = name;
		Println("Init called: %s", name);
		if (parent != nullptr) {
			this->parent = parent;
			parent->children.add(this);
		}
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
	
	Ref<ExampleNode> node0 = New<ExampleNode>();
	Ref<ExampleNode> node = New<ExampleNode>("Root");
	Ref<ExampleNode> node1 = New<ExampleNode>("Node1", node);
	Ref<ExampleNode> node11 = New<ExampleNode>("Node1-1", node1);
	Ref<ExampleNode> node12 = New<ExampleNode>("Node1-2", node1);
	Ref<ExampleNode> node2 = New<ExampleNode>("Node2", node);
	Ref<ExampleNode> node21 = New<ExampleNode>("Node2-1", node2);
	Ref<ExampleNode> node22 = New<ExampleNode>("Node2-2", node2);
	
	node->print();
	
	return 0;
}
