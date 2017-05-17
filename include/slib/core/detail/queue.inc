/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::push_NoLock(const T& value, sl_size countLimit)
	{
		return this->pushBack_NoLock(value, countLimit) != sl_null;
	}

	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::push(const T& value, sl_size countLimit)
	{
		return this->pushBack(value, countLimit) != sl_null;
	}
	
	template <class T, class CONTAINER>
	void Queue<T, CONTAINER>::pushAll(const Queue<T, CONTAINER>* other)
	{
		this->pushBackAll(other);
	}
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::pop_NoLock(T* _out)
	{
		return this->popFront_NoLock(_out);
	}
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::pop(T* _out)
	{
		return this->popFront(_out);
	}
	
	template <class T, class CONTAINER>
	Queue<T, CONTAINER>* Queue<T, CONTAINER>::duplicate_NoLock() const
	{
		CONTAINER* container = CONTAINER::duplicate_NoLock();
		return static_cast<Queue<T, CONTAINER>*>(container);
	}
	
	template <class T, class CONTAINER>
	Queue<T, CONTAINER>* Queue<T, CONTAINER>::duplicate() const
	{
		CONTAINER* container = CONTAINER::duplicate();
		return static_cast<Queue<T, CONTAINER>*>(container);
	}
	
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::push_NoLock(const T& value, sl_size countLimit)
	{
		return this->pushBack_NoLock(value, countLimit) != sl_null;
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::push(const T& value, sl_size countLimit)
	{
		return this->pushBack(value, countLimit) != sl_null;
	}
	
	template <class T, class CONTAINER>
	void Stack<T, CONTAINER>::pushAll(const Stack<T, CONTAINER>* other)
	{
		this->pushBackAll(other);
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::pop_NoLock(T* _out)
	{
		return this->popBack_NoLock(_out);
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::pop(T* _out)
	{
		return this->popBack(_out);
	}
	
	template <class T, class CONTAINER>
	Stack<T, CONTAINER>* Stack<T, CONTAINER>::duplicate_NoLock() const
	{
		CONTAINER* container = CONTAINER::duplicate_NoLock();
		return static_cast<Stack<T, CONTAINER>*>(container);
	}
	
	template <class T, class CONTAINER>
	Stack<T, CONTAINER>* Stack<T, CONTAINER>::duplicate() const
	{
		CONTAINER* container = CONTAINER::duplicate();
		return static_cast<Stack<T, CONTAINER>*>(container);
	}

}
