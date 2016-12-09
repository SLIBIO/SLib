#include "../../../inc/slib/core/function.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Runnable, Object)


SLIB_DEFINE_REF_WRAPPER(Callback, SafeCallback, Runnable, ref)

Callback::Callback(Runnable* runnable) : ref(runnable)
{
}

Callback& Callback::operator=(Runnable* runnable)
{
	ref = runnable;
	return *this;
}

void Callback::operator()() const
{
	Runnable* runnable = ref.ptr;
	if (runnable) {
		runnable->run();
	}
}


SLIB_DEFINE_REF_WRAPPER(SafeCallback, Callback, Runnable, ref)

SafeCallback::SafeCallback(Runnable* runnable) : ref(runnable)
{
}

SafeCallback& SafeCallback::operator=(Runnable* runnable)
{
	ref = runnable;
	return *this;
}

void SafeCallback::operator()() const
{
	Ref<Runnable> runnable(ref);
	if (runnable.isNotNull()) {
		runnable->run();
	}
}


SLIB_DEFINE_OBJECT(Dispatcher, Object)

SLIB_NAMESPACE_END
