#ifndef CHECKHEADER_SLIB_CORE_QUEUE
#define CHECKHEADER_SLIB_CORE_QUEUE

#include "definition.h"

#include "linked_list.h"

SLIB_NAMESPACE_BEGIN

template <class T, class CONTAINER>
class SLIB_EXPORT Queue : public CONTAINER
{
public:
	sl_bool push_NoLock(const T& value, sl_size countLimit = 0);
	
	sl_bool push(const T& value, sl_size countLimit = 0);
	
	void pushAll(const Queue<T, CONTAINER>* other);
	
	sl_bool pop_NoLock(T* _out = sl_null);
	
	sl_bool pop(T* _out = sl_null);
	
	Queue<T, CONTAINER>* duplicate_NoLock() const;
	
	Queue<T, CONTAINER>* duplicate() const;
	
};

template <class T, class CONTAINER>
class SLIB_EXPORT Stack : public CONTAINER
{
public:
	sl_bool push_NoLock(const T& value, sl_size countLimit = 0);
	
	sl_bool push(const T& value, sl_size countLimit = 0);
	
	void pushAll(const Stack<T, CONTAINER>* other);
	
	sl_bool pop_NoLock(T* _out = sl_null);
	
	sl_bool pop(T* _out = sl_null);
	
	Stack<T, CONTAINER>* duplicate_NoLock() const;
	
	Stack<T, CONTAINER>* duplicate() const;
	
};

template <class T>
using LinkedQueue = Queue< T, CLinkedList<T> >;

template <class T>
using LinkedStack = Stack< T, CLinkedList<T> >;


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

SLIB_NAMESPACE_END

#endif
