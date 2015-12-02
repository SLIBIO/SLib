#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/pointer.h"

SLIB_NAMESPACE_BEGIN

void* _SafePtr_lock(const Ref<_SafePtrContainer>& _object)
{
	Ref<_SafePtrContainer> o = _object;
	_SafePtrContainer* container = o.getObject();
	if (container) {
		SpinLocker lock(&(container->locker));
		Base::interlockedIncrement32(&(container->nRefCount));
		return container->obj;
	} else {
		return sl_null;
	}
}

void _SafePtr_free(const Ref<_SafePtrContainer>& _object)
{
	Ref<_SafePtrContainer> o = _object;
	_SafePtrContainer* container = o.getObject();
	if (container) {
		while (container->obj) {
			SpinLocker lock(&(container->locker));
			if (Base::interlockedCompareExchange32(&(container->nRefCount), 0, 0)) {
				container->obj = sl_null;
				break;
			}
			lock.unlock();
			Base::yield();
		}
	}
}

sl_ptr _RefPtr_null[2] = {0, 0};

void _RefPtr_copy(_RefPtr_Container* target, _RefPtr_Container* source)
{
	if ((void*)(source) == (void*)(&_RefPtr_null)) {
		target->m_pointer = sl_null;
	} else {
		SpinLocker lock(SpinLockPoolForRefPtr::get(source));
		target->m_reference = source->m_reference;
		target->m_pointer = source->m_pointer;
	}
}

void _RefPtr_set(_RefPtr_Container* target, _RefPtr_Container* source)
{
	if (target == source) {
		return;
	}
	if ((void*)(source) == (void*)(&_RefPtr_null)) {
		SpinLock* l = SpinLockPoolForRefPtr::get(target);
		Ref<Referable> refOld = target->m_reference;
		{
			SpinLocker lock(l);
			target->m_pointer = sl_null;
			target->m_reference.setNull();
		}
	} else {
		SpinLock* l1 = SpinLockPoolForRefPtr::get(target);
		SpinLock* l2 = SpinLockPoolForRefPtr::get(source);
		if (l2 < l1) {
			SpinLock* t = l2;
			l2 = l1;
			l1 = t;
		} else if (l2 == l1) {
			l2 = sl_null;
		}
		Ref<Referable> refOld = target->m_reference;
		{
			SpinLocker lock1(l1);
			SpinLocker lock2(l2);
			target->m_pointer = source->m_pointer;
			target->m_reference = source->m_reference;
		}
	}
}

void _RefPtr_set(_RefPtr_Container* target, const void* pointer)
{
	SpinLocker lock(SpinLockPoolForRefPtr::get(target));
	target->m_pointer = (void*)pointer;
	target->m_reference.setNull();
}

void _RefPtr_set(_RefPtr_Container* target, const Ref<Referable, sl_true>& ref)
{
	SpinLocker lock(SpinLockPoolForRefPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = target->m_reference.getObject();
}

void _RefPtr_set(_RefPtr_Container* target, const Ref<Referable, sl_false>& ref)
{
	SpinLocker lock(SpinLockPoolForRefPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = target->m_reference.getObject();
}

void _RefPtr_set(_RefPtr_Container* target, const void* pointer, const Ref<Referable, sl_true>& ref)
{
	SpinLocker lock(SpinLockPoolForRefPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = (void*)pointer;
}

void _RefPtr_set(_RefPtr_Container* target, const void* pointer, const Ref<Referable, sl_false>& ref)
{
	SpinLocker lock(SpinLockPoolForRefPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = (void*)pointer;
}

const sl_ptr _Ptr_null[2] = { 0, 0 };

void _Ptr_copy(_Ptr_Container* target, _Ptr_Container* source)
{
	if ((void*)(source) == (void*)(&_Ptr_null)) {
		target->m_pointer = sl_null;
	} else {
		SpinLocker lock(SpinLockPoolForPtr::get(source));
		target->m_pointer = source->m_pointer;
		target->m_reference = source->m_reference;
	}
}

void _Ptr_set(_Ptr_Container* target, _Ptr_Container* source)
{
	if (target == source) {
		return;
	}
	if ((void*)(source) == (void*)(&_Ptr_null)) {
		SpinLock* l = SpinLockPoolForPtr::get(target);
		Ref<Referable> refOld = target->m_reference;
		{
			SpinLocker lock(l);
			target->m_pointer = sl_null;
			target->m_reference.setNull();
		}
	} else {
		SpinLock* l1 = SpinLockPoolForPtr::get(target);
		SpinLock* l2 = SpinLockPoolForPtr::get(source);
		if (l2 < l1) {
			SpinLock* t = l2;
			l2 = l1;
			l1 = t;
		} else if (l2 == l1) {
			l2 = sl_null;
		}
		Ref<Referable> refOld = target->m_reference;
		{
			SpinLocker lock1(l1);
			SpinLocker lock2(l2);
			target->m_pointer = source->m_pointer;
			target->m_reference = source->m_reference;
		}
	}
}

void _Ptr_set(_Ptr_Container* target, const void* pointer)
{
	SpinLocker lock(SpinLockPoolForPtr::get(target));
	target->m_pointer = (void*)pointer;
	target->m_reference.setNull();
}

void _Ptr_set(_Ptr_Container* target, const Ref<Referable, sl_true>& ref)
{
	SpinLocker lock(SpinLockPoolForPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = target->m_reference.getObject();
}

void _Ptr_set(_Ptr_Container* target, const Ref<Referable, sl_false>& ref)
{
	SpinLocker lock(SpinLockPoolForPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = target->m_reference.getObject();
}

void _Ptr_set(_Ptr_Container* target, const void* pointer, const Ref<Referable, sl_true>& ref)
{
	SpinLocker lock(SpinLockPoolForPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = (void*)pointer;
}

void _Ptr_set(_Ptr_Container* target, const void* pointer, const Ref<Referable, sl_false>& ref)
{
	SpinLocker lock(SpinLockPoolForPtr::get(target));
	target->m_reference = ref;
	target->m_pointer = (void*)pointer;
}

void _Ptr_lock(_Ptr_Container* target, _Ptr_Container* source)
{
	void* pointer;
	Ref<Referable> ref;
	{
		SpinLocker lock(SpinLockPoolForPtr::get(source));
		pointer = source->m_pointer;
		ref = source->m_reference;
	}
	if (ref.isNotNull()) {
		if (WeakRefObject::checkInstance(ref)) {
			WeakRef<Referable> w = (WeakRefObject*)(ref.getObject());
			target->m_reference = w.lock();
			if (target->m_reference.isNull()) {
				target->m_pointer = sl_null;
			} else {
				target->m_pointer = pointer;				
			}
		} else {
			target->m_pointer = pointer;
			target->m_reference = ref;
		}
	} else {
		target->m_pointer = pointer;
	}
}

SLIB_NAMESPACE_END
