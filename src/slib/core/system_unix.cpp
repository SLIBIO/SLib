#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/syscall.h>

#if defined(SLIB_PLATFORM_IS_APPLE)
#include <mach/mach.h>
#endif

#if defined(SLIB_PLATFORM_IS_BLACKBERRY)
#include <Qt/qdeclarativedebug.h>
#include <bb/cascades/Application>
#endif

SLIB_NAMESPACE_BEGIN
#if !defined(SLIB_PLATFORM_IS_APPLE)
String System::getApplicationPath()
{
#if defined(SLIB_PLATFORM_IS_ANDROID)
	sl_uint32 size = 2048;
	char* path = (char*)(Base::createMemory(size+1));
	if (!path) {
		return String::null();
	}
	Base::zeroMemory(path, size+1);
	char a[50];
	sprintf(a, "/proc/%d/cmdline", getpid());
	FILE* fp = fopen(a, "rb");
	int n = fread(path, 1, size, fp);
	fclose(fp);

	String ret;
	if (n > 0) {
		ret = String::fromUtf8(path);
	}
	Base::freeMemory(path);
	return "/data/data/" + ret;
#else
	sl_uint32 size = 2048;
	char* path = (char*)(Base::createMemory(size+1));
	if (!path) {
		return String::null();
	}
	int n = readlink("/proc/self/exe", path, size);
	/*
	-- another solution --

		char a[50];
		sprintf(a, "/proc/%d/exe", getpid());
		int n = readlink(a, path, size);
	*/
	String ret;
	if (n > 0) {
		ret = String::fromUtf8(path, n);
	}
	Base::freeMemory(path);
	return ret;
#endif
}

String System::getTempDirectory()
{
#if defined(SLIB_PLATFORM_IS_MOBILE)
	String dir = System::getApplicationDirectory() + "/temp";
	File::createDirectory(dir);
	return dir;
#else
	return "/tmp";
#endif
}
#endif

sl_uint32 System::getTickCount()
{
	struct timeval tv;
	if (gettimeofday(&tv, 0) == 0) {
		return (sl_uint32)(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
	} else {
		return 0;
	}
}

#if !defined(SLIB_PLATFORM_IS_MOBILE)

SLIB_SAFE_STATIC_GETTER(CList<String>, _System_getGlobalUniqueInstances)

struct _SYS_GLOBAL_UNIQUE_INSTANCE
{
	String name;
	Ref<File> file;
};

void* System::createGlobalUniqueInstance(const String& uniqueName)
{
	if (uniqueName.isEmpty()) {
		return sl_null;
	}
	String name = File::makeSafeFileName(uniqueName);
	CList<String>* lst = _System_getGlobalUniqueInstances();
	if (!lst) {
		return sl_null;
	}
	if (lst->indexOf(name) >= 0) {
		return sl_null;
	}

	SLIB_STATIC_STRING(tmp, "/tmp/");
	String fileName = tmp + name;
	_SYS_GLOBAL_UNIQUE_INSTANCE* instance = new _SYS_GLOBAL_UNIQUE_INSTANCE();
	instance->name = name;
	instance->file = File::openForWrite(fileName);
	if (instance->file.isNull()) {
		delete instance;
		return sl_null;
	}
	if (! (instance->file->lock())) {
		instance->file->close();
		delete instance;
		return sl_null;
	}
	lst->add(name);
	return instance;
}

void System::freeGlobalUniqueInstance(void* instance)
{
	if (instance) {
		_SYS_GLOBAL_UNIQUE_INSTANCE* l = (_SYS_GLOBAL_UNIQUE_INSTANCE*)(instance);
		if (l) {
			l->file->unlock();
			l->file->close();
			File::deleteFile(l->file->getPath());
			CList<String>* lst = _System_getGlobalUniqueInstances();
			if (lst) {
				lst->removeValue(l->name);
			}
			delete l;
		}
	}
}
#endif

sl_uint32 System::getProcessId()
{
	return getpid();
}

sl_uint32 System::getThreadId()
{
#if defined(SLIB_PLATFORM_IS_APPLE)
	int ret = mach_thread_self();
	mach_port_deallocate(mach_task_self(), ret);
	return ret;
#elif defined(SLIB_PLATFORM_IS_ANDROID)
	pid_t tid = gettid();
	return tid;
#else
	pid_t tid = syscall(SYS_gettid);
	return tid;
#endif
}

#if !defined(SLIB_PLATFORM_IS_MOBILE)
sl_bool System::createProcess(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
{
	pid_t pid = fork();
	if (pid == -1) {
		SLIB_LOG_ERROR("System::createProcess", "Cannot fork");
		return sl_false;
	} else {
		if (pid == 0) {
			// Child process

			// Daemonize
			setsid();
			close(0);
			close(1);
			close(2);
			open("/dev/null", O_RDWR);
			dup2(0, 1);
			dup2(0, 2);

			signal(SIGHUP, SIG_IGN);

			char* exe = strdup(pathExecutable.getData());
			char** args = (char**)(Base::createMemory((nCmds + 2) * sizeof(char*)));
			args[0] = exe;
			for (sl_size i = 0; i < nCmds; i++) {
				String8 param = cmds[i];
				args[i+1] = strdup(param.getData());
			}
			args[nCmds+1] = 0;

			execvp(exe, args);
			exit(0);
		} else {
			// parent process
			return sl_true;
		}
	}
}
#endif

void System::sleep(sl_uint32 milliseconds)
{
	struct timespec req;
	req.tv_sec = milliseconds / 1000;
	req.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&req, sl_null);
}

void System::yield()
{
	sched_yield();
}

void System::abort(const String& msg, const String& file, sl_uint32 line)
{
#if defined(SLIB_DEBUG)
#if defined(SLIB_PLATFORM_IS_ANDROID)
	__assert(file.getData(), line, msg.getData());
#elif defined(SLIB_PLATFORM_IS_BLACKBERRY)
	std::__assert(msg.getData(), file.getData(), line, 0);
#else
	__assert(msg.getData(), file.getData(), line);
#endif
#endif
}

void Console::print(const String& s)
{
#if defined(SLIB_PLATFORM_IS_ANDROID)
	SLIB_STATIC_STRING(c, "Console");
	Log::getConsoleLogger()->log(c, s);
#elif defined(SLIB_PLATFORM_IS_BLACKBERRY)
	String8 _s = s;
	qDebug() << _s.getData() << endl;
#else
	String8 _s = s;
	printf("%s", _s.getData());
#endif
}

#if !defined(SLIB_PLATFORM_IS_MOBILE)
String Console::readLine()
{
	String ret;
	char* line = NULL;
	size_t len = 0;
	::getline(&line, &len, stdin);
	if (line) {
		ret = String::fromUtf8(line, (sl_int32)len);
		::free(line);
	}
	return ret;
}
#endif

SLIB_NAMESPACE_END

void sl_abort(const char* msg, const char* file, sl_uint32 line)
{
#if defined(SLIB_DEBUG)
#if defined(SLIB_PLATFORM_IS_ANDROID)
	__assert(file, line, msg);
#elif defined(SLIB_PLATFORM_IS_BLACKBERRY)
	std::__assert(msg, file, line, 0);
#else
	__assert(msg, file, line);
#endif
#endif
}

#endif
