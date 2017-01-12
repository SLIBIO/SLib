#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/list.h"
#include "../../../inc/slib/core/safe_static.h"

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

#define _PATH_MAX 1024

SLIB_NAMESPACE_BEGIN

#if !defined(SLIB_PLATFORM_IS_APPLE)
String System::getApplicationPath()
{
	char path[_PATH_MAX] = {0};
#if defined(SLIB_PLATFORM_IS_ANDROID)
	char a[50];
	::sprintf(a, "/proc/%d/cmdline", getpid());
	FILE* fp = fopen(a, "rb");
	int n = fread(path, 1, _PATH_MAX - 1, fp);
	fclose(fp);

	String ret;
	if (n > 0) {
		ret = String::fromUtf8(path);
	}
	return "/data/data/" + ret;
#else
	int n = readlink("/proc/self/exe", path, _PATH_MAX-1);
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

String System::getCurrentDirectory()
{
	char path[_PATH_MAX] = {0};
	char* r = ::getcwd(path, _PATH_MAX-1);
	if (r) {
		return path;
	}
	return sl_null;
}

sl_bool System::setCurrentDirectory(const String& dir)
{
	int iRet = ::chdir(dir.getData());
	if (iRet == 0) {
		return sl_true;
	}
	return sl_false;
}

sl_uint32 System::getTickCount()
{
	struct timeval tv;
	if (gettimeofday(&tv, 0) == 0) {
		return (sl_uint32)(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
	} else {
		return 0;
	}
}

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
		LogError("System::createProcess", "Cannot fork");
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

			char* exe = pathExecutable.getData();
			char* args[1024];
			args[0] = exe;
			if (nCmds > 1020) {
				nCmds = 1020;
			}
			for (sl_size i = 0; i < nCmds; i++) {
				args[i+1] = cmds[i].getData();
			}
			args[nCmds+1] = 0;

			::execvp(exe, args);
			::exit(1);
			return sl_false;
		} else {
			// parent process
			return sl_true;
		}
	}
}

void System::exec(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
{
	char* exe = pathExecutable.getData();
	char* args[1024];
	args[0] = exe;
	if (nCmds > 1020) {
		nCmds = 1020;
	}
	for (sl_size i = 0; i < nCmds; i++) {
		args[i+1] = cmds[i].getData();
	}
	args[nCmds+1] = 0;
	
	::execvp(exe, args);
	::exit(1);
}

void System::exit(int code)
{
	::exit(code);
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

#if !defined(SLIB_PLATFORM_IS_MOBILE)
volatile double __signal_fpe_dummy = 0.0f;
void System::setCrashHandler(SIGNAL_HANDLER handler)
{
	struct sigaction sa;
	sa.sa_flags = SA_NODEFER;
	sa.sa_handler = handler;
	sigemptyset(&(sa.sa_mask));
	sigaction(SIGFPE, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS, &sa, NULL);
	sigaction(SIGILL, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGIOT, &sa, NULL);
#if defined(SLIB_PLATFORM_IS_MACOS)
	sigaction(SIGEMT, &sa, NULL);
#endif
	sigaction(SIGSYS, &sa, NULL);
}
#endif

void Console::print(const String& s)
{
#if defined(SLIB_PLATFORM_IS_ANDROID)
	SLIB_STATIC_STRING(c, "Console");
	Logger::getConsoleLogger()->log(c, s);
#elif defined(SLIB_PLATFORM_IS_BLACKBERRY)
	String _s = s;
	qDebug() << _s.getData() << endl;
#else
	String _s = s;
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

#if !defined(SLIB_PLATFORM_IS_MOBILE)

typedef CList<String> _GlobalUniqueInstanceList;

SLIB_SAFE_STATIC_GETTER(_GlobalUniqueInstanceList, _getGlobalUniqueInstanceList)

class _GlobalUniqueInstance : public GlobalUniqueInstance
{
public:
	String m_name;
	Ref<File> m_file;
	
public:
	_GlobalUniqueInstance()
	{
	}
	
	~_GlobalUniqueInstance()
	{
		m_file->unlock();
		m_file->close();
		File::deleteFile(m_file->getPath());
		_GlobalUniqueInstanceList* list = _getGlobalUniqueInstanceList();
		if (list) {
			list->removeValue(m_name);
		}
	}
	
};

Ref<GlobalUniqueInstance> GlobalUniqueInstance::create(const String& _name)
{
	if (_name.isEmpty()) {
		return sl_null;
	}
	
	String name = File::makeSafeFileName(_name);
	_GlobalUniqueInstanceList* list = _getGlobalUniqueInstanceList();
	if (!list) {
		return sl_null;
	}
	if (list->indexOf(name) >= 0) {
		return sl_null;
	}
	
	Ref<_GlobalUniqueInstance> instance = new _GlobalUniqueInstance;
	if (instance.isNotNull()) {
		String fileName = "/tmp/.slib_global_lock_" + name;
		Ref<File> file = File::openForWrite(fileName);
		if (file.isNotNull()) {
			if (file->lock()) {
				instance->m_name = name;
				instance->m_file = file;
				list->add(name);
				return instance;
			}
			file->close();
			File::deleteFile(fileName);
		}
	}
	return sl_null;
}

sl_bool GlobalUniqueInstance::exists(const String& _name)
{
	if (_name.isEmpty()) {
		return sl_false;
	}
	String name = File::makeSafeFileName(_name);
	_GlobalUniqueInstanceList* list = _getGlobalUniqueInstanceList();
	if (!list) {
		return sl_false;
	}
	if (list->indexOf(name) >= 0) {
		return sl_true;
	}
	String fileName = "/tmp/.slib_global_lock_" + name;
	if (File::exists(fileName)) {
		Ref<File> file = File::openForWrite(fileName);
		if (file.isNull()) {
			return sl_true;
		} else {
			if (file->lock()) {
				file->unlock();
			} else {
				return sl_true;
			}
			file->close();
			File::deleteFile(fileName);
		}
	}
	return sl_false;
}

#endif

void _abort(const char* msg, const char* file, sl_uint32 line)
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

SLIB_NAMESPACE_END

#endif
