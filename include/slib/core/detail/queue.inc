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

namespace slib
{
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::push_NoLock(const T& value, sl_size countLimit) noexcept
	{
		return this->pushBack_NoLock(value, countLimit) != sl_null;
	}

	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::push(const T& value, sl_size countLimit) noexcept
	{
		return this->pushBack(value, countLimit);
	}
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::pushAll(const Queue<T, CONTAINER>* other) noexcept
	{
		return this->pushBackAll(other);
	}
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::pop_NoLock(T* _out) noexcept
	{
		return this->popFront_NoLock(_out);
	}
	
	template <class T, class CONTAINER>
	sl_bool Queue<T, CONTAINER>::pop(T* _out) noexcept
	{
		return this->popFront(_out);
	}
	
	template <class T, class CONTAINER>
	Queue<T, CONTAINER>* Queue<T, CONTAINER>::duplicate_NoLock() const noexcept
	{
		CONTAINER* container = CONTAINER::duplicate_NoLock();
		return static_cast<Queue<T, CONTAINER>*>(container);
	}
	
	template <class T, class CONTAINER>
	Queue<T, CONTAINER>* Queue<T, CONTAINER>::duplicate() const noexcept
	{
		CONTAINER* container = CONTAINER::duplicate();
		return static_cast<Queue<T, CONTAINER>*>(container);
	}
	
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::push_NoLock(const T& value, sl_size countLimit) noexcept
	{
		return this->pushBack_NoLock(value, countLimit) != sl_null;
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::push(const T& value, sl_size countLimit) noexcept
	{
		return this->pushBack(value, countLimit);
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::pushAll(const Stack<T, CONTAINER>* other) noexcept
	{
		return this->pushBackAll(other);
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::pop_NoLock(T* _out) noexcept
	{
		return this->popBack_NoLock(_out);
	}
	
	template <class T, class CONTAINER>
	sl_bool Stack<T, CONTAINER>::pop(T* _out) noexcept
	{
		return this->popBack(_out);
	}
	
	template <class T, class CONTAINER>
	Stack<T, CONTAINER>* Stack<T, CONTAINER>::duplicate_NoLock() const noexcept
	{
		CONTAINER* container = CONTAINER::duplicate_NoLock();
		return static_cast<Stack<T, CONTAINER>*>(container);
	}
	
	template <class T, class CONTAINER>
	Stack<T, CONTAINER>* Stack<T, CONTAINER>::duplicate() const noexcept
	{
		CONTAINER* container = CONTAINER::duplicate();
		return static_cast<Stack<T, CONTAINER>*>(container);
	}

}
