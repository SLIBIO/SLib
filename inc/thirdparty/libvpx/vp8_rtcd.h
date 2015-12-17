
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
//	Windows phone

#elif defined(_WIN64)
#	include "platforms/x64_win/vp8_rtcd.h"
#elif defined(_WIN32)
//	windows x86 /x64
#	include "platforms/x86_win/vp8_rtcd.h"

#elif defined(__APPLE__) && defined(__MACH__)
#		include <TargetConditionals.h>
#		if TARGET_OS_IPHONE
#			if TARGET_IPHONE_SIMULATOR
#           	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#					include "./platforms/x64_ios_simulator/vp8_rtcd.h"
#				else
#					include "./platforms/x86_ios_simulator/vp8_rtcd.h"
#				endif
#			else
#           	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#					include "./platforms/arm64_ios/vp8_rtcd.h"					
#				else
#					include "./platforms/armv7_ios/vp8_rtcd.h"
#				endif
#			endif
#		elif TARGET_OS_MAC
#           if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#               include "./platforms/x64_mac/vp8_rtcd.h"
#           else
#               include "./platforms/x86_mac/vp8_rtcd.h"
#           endif
#		endif
#elif defined(__ANDROID__)
//	android
#	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)

#	else
#		include "./platforms/armv6_android/vp8_rtcd.h"
#	endif
#endif

