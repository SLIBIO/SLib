/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/object.h"

#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/variant.h"

namespace slib
{

	SLIB_DEFINE_ROOT_OBJECT(Object)

	Object::Object()
	{
	}

	Object::~Object()
	{
	}

	Mutex* Object::getLocker() const
	{
		return (Mutex*)(&m_locker);
	}

	void Object::lock() const
	{
		m_locker.lock();
	}

	void Object::unlock() const
	{
		m_locker.unlock();
	}

	sl_bool Object::tryLock() const
	{
		return m_locker.tryLock();
	}
	
	Variant Object::getProperty(const String& name)
	{
		MutexLocker lock(&m_locker);
		if (m_properties.isNotNull()) {
			HashMap<String, Variant>* map = static_cast<HashMap<String, Variant>*>(m_properties.get());
			return map->getValue_NoLock(name);
		}
		return Variant::null();
	}
	
	void Object::setProperty(const String& name, const Variant& value)
	{
		MutexLocker lock(&m_locker);
		HashMap<String, Variant>* map;
		if (m_properties.isNotNull()) {
			map = static_cast<HashMap<String, Variant>*>(m_properties.get());
		} else {
			map = new HashMap<String, Variant>;
			if (map) {
				m_properties = map;
			} else {
				return;
			}
		}
		map->put_NoLock(name, value);
	}
	
	void Object::clearProperty(const String& name)
	{
		MutexLocker lock(&m_locker);
		if (m_properties.isNotNull()) {
			HashMap<String, Variant>* map = static_cast<HashMap<String, Variant>*>(m_properties.get());
			map->remove_NoLock(name);
		}
	}

	ObjectLocker::ObjectLocker()
	{
	}

	ObjectLocker::ObjectLocker(const Object* object) : MutexLocker(object ? object->getLocker(): sl_null)
	{
	}

	ObjectLocker::ObjectLocker(const Object* object1, const Object* object2) : MutexLocker(object1 ? object1->getLocker() : sl_null, object2 ? object2->getLocker() : sl_null)
	{
	}

	ObjectLocker::~ObjectLocker()
	{
	}

	void ObjectLocker::lock(const Object* object)
	{
		if (object) {
			MutexLocker::lock(object->getLocker());
		}
	}

	void ObjectLocker::lock(const Object* object1, const Object* object2)
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
