
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
//	Windows phone

#elif defined(_WIN64)
#	include "platforms/x64_win/vpx_scale_rtcd.h"
#elif defined(_WIN32)
//	windows x86 /x64
#	include "platforms/x86_win/vpx_scale_rtcd.h"

#elif defined(__APPLE__) && defined(__MACH__)
#		include <TargetConditionals.h>
#		if TARGET_OS_IPHONE
#			if TARGET_IPHONE_SIMULATOR
#           	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#					include "./platforms/x64_ios_simulator/vpx_scale_rtcd.h"
#				else
#					include "./platforms/x86_ios_simulator/vpx_scale_rtcd.h"
#				endif
#			else
#           	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#					include "./platforms/arm64_ios/vpx_scale_rtcd.h"					
#				else
#					include "./platforms/armv7_ios/vpx_scale_rtcd.h"
#				endif
#			endif
#		elif TARGET_OS_MAC
#           if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#               include "./platforms/x64_mac/vpx_scale_rtcd.h"
#           else
#               include "./platforms/x86_mac/vpx_scale_rtcd.h"
#           endif
#		endif
#elif defined(__ANDROID__)
//	android
#	if defined(__amd64__) || defined(__x86_64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)

#	else
#		include "./platforms/armv6_android/vpx_scale_rtcd.h"
#	endif
#endif

