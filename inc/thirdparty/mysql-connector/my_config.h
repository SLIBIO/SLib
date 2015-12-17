
#if defined(_WIN64)
#	include "./my_config_win32_x64.h"
#elif defined(_WIN32)
#	include "./my_config_win32_x86.h"
#elif defined(__APPLE__) && defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_OS_MAC
#		include "./my_config_osx.h"
#	endif
#elif defined(__linux__)
#	if defined(__amd64__) || defined(__x86_64__)
#		include "./my_config_linux_x64.h"
#	elif defined(__i386__) || defined(_X86_)
#		include "./my_config_linux_x86.h"
#	endif
#endif
