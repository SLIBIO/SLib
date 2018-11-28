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
		Mutex m_locker;
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

