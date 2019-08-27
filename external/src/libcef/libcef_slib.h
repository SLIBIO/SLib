#ifndef CHECKHEADER_SLIB_EXTERNAL_LIBCEF_SLIB_H
#define CHECKHEADER_SLIB_EXTERNAL_LIBCEF_SLIB_H

#ifndef WRAPPING_CEF_SHARED
#define WRAPPING_CEF_SHARED
#endif

#ifdef WIN32
#	include <windows.h>
#	ifdef max
#		undef max
#	endif
#	ifdef min
#		undef min
#	endif
#endif

#endif