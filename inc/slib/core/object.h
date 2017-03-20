/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"

#include "macro.h"
#include "ref.h"
#include "mutex.h"

namespace slib
{
	
	class Variant;
	class String;
	
	class SLIB_EXPORT Object : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		Object();

		~Object();

	public:
		Mutex* getLocker() const;

		void lock() const;
	
		void unlock() const;
	
		sl_bool tryLock() const;
		
		Variant getProperty(const String& name);
		
		void setProperty(const String& name, const Variant& value);
		
		void clearProperty(const String& name);
	
	private:
		Mutex m_locker;
		Ref<Referable> m_properties;

	};
	
	class SLIB_EXPORT ObjectLocker : public MutexLocker
	{
	public:
		ObjectLocker();

		ObjectLocker(const Object* object);
	
		ObjectLocker(const Object* object1, const Object* object2);

		~ObjectLocker();

	public:
		void lock(const Object* object);

		void lock(const Object* object1, const Object* object2);

	};

}

#endif

