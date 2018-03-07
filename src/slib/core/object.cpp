/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/object.h"

#include "slib/core/string.h"
#include "slib/core/hash_map.h"
#include "slib/core/variant.h"

namespace slib
{

	SLIB_DEFINE_ROOT_OBJECT(Object)

	Object::Object() noexcept
	{
	}

	Object::~Object() noexcept
	{
	}

	Mutex* Object::getLocker() const noexcept
	{
		return (Mutex*)(&m_locker);
	}

	void Object::lock() const noexcept
	{
		m_locker.lock();
	}

	void Object::unlock() const noexcept
	{
		m_locker.unlock();
	}

	sl_bool Object::tryLock() const noexcept
	{
		return m_locker.tryLock();
	}
	
	Variant Object::getProperty(const String& name) noexcept
	{
		MutexLocker lock(&m_locker);
		if (m_properties.isNotNull()) {
			CHashMap<String, Variant>* map = static_cast<CHashMap<String, Variant>*>(m_properties.get());
			return map->getValue_NoLock(name);
		}
		return Variant::null();
	}
	
	void Object::setProperty(const String& name, const Variant& value) noexcept
	{
		MutexLocker lock(&m_locker);
		CHashMap<String, Variant>* map;
		if (m_properties.isNotNull()) {
			map = static_cast<CHashMap<String, Variant>*>(m_properties.get());
		} else {
			map = new CHashMap<String, Variant>;
			if (map) {
				m_properties = map;
			} else {
				return;
			}
		}
		map->put_NoLock(name, value);
	}
	
	void Object::clearProperty(const String& name) noexcept
	{
		MutexLocker lock(&m_locker);
		if (m_properties.isNotNull()) {
			CHashMap<String, Variant>* map = static_cast<CHashMap<String, Variant>*>(m_properties.get());
			map->remove_NoLock(name);
		}
	}

	ObjectLocker::ObjectLocker() noexcept
	{
	}

	ObjectLocker::ObjectLocker(const Object* object) noexcept
	 : MutexLocker(object ? object->getLocker(): sl_null)
	{
	}

	ObjectLocker::ObjectLocker(const Object* object1, const Object* object2) noexcept
	 : MutexLocker(object1 ? object1->getLocker() : sl_null, object2 ? object2->getLocker() : sl_null)
	{
	}

	ObjectLocker::~ObjectLocker() noexcept
	{
	}

	void ObjectLocker::lock(const Object* object) noexcept
	{
		if (object) {
			MutexLocker::lock(object->getLocker());
		}
	}

	void ObjectLocker::lock(const Object* object1, const Object* object2) noexcept
	{
		if (object1) {
			if (object2) {
				MutexLocker::lock(object1->getLocker(), object2->getLocker());
			} else {
				MutexLocker::lock(object1->getLocker());
			}
		} else {
			if (object2) {
				MutexLocker::lock(object2->getLocker());
			}
		}
	}

}
