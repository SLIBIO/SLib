#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"

#include "macro.h"
#include "ref.h"
#include "mutex.h"

namespace slib
{
	
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
	
	private:
		Mutex m_locker;

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

