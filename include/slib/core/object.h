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
		Object() noexcept;
		
		Object(const Object& other) = delete;
		
		Object(Object&& other) = delete;

		~Object() noexcept;

	public:
		Mutex* getLocker() const noexcept;

		void lock() const noexcept;
	
		void unlock() const noexcept;
	
		sl_bool tryLock() const noexcept;
		
		Variant getProperty(const String& name) noexcept;
		
		void setProperty(const String& name, const Variant& value) noexcept;
		
		void clearProperty(const String& name) noexcept;
		
	public:
		Object& operator=(const Object& other) = delete;
		
		Object& operator=(Object&& other) = delete;
	
	private:
		SpinLock m_lockPrivate;
		mutable Mutex* m_locker;
		void* m_properties;

	};
	
	class SLIB_EXPORT ObjectLocker : public MutexLocker
	{
	public:
		ObjectLocker() noexcept;

		ObjectLocker(const Object* object) noexcept;
		
		~ObjectLocker() noexcept;

	public:
		void lock(const Object* object) noexcept;

	};

	class SLIB_EXPORT MultipleObjectsLocker : public MultipleMutexLocker
	{
	public:
		MultipleObjectsLocker() noexcept;
		
		MultipleObjectsLocker(const Object* object) noexcept;
		
		MultipleObjectsLocker(const Object* object1, const Object* object2) noexcept;
		
		~MultipleObjectsLocker() noexcept;
		
	public:
		void lock(const Object* object) noexcept;
		
		void lock(const Object* object1, const Object* object2) noexcept;
		
	};

}

#endif

