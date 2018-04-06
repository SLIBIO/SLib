/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ENDIAN
#define CHECKHEADER_SLIB_CORE_ENDIAN

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT Endian
	{
	public:
		static SLIB_INLINE sl_bool checkLittleEndianRuntime()
		{
			sl_uint32 n = 0x12345678;
			return *(sl_uint8*)(&n) == 0x78;
		}

		static SLIB_INLINE sl_bool checkBigEndianRuntime()
		{
			sl_uint32 n = 0x12345678;
			return *(sl_uint8*)(&n) != 0x78;
		}
		
		static SLIB_INLINE sl_bool isLE()
		{
#if defined(SLIB_ARCH_IS_LITTLE_ENDIAN)
			return sl_true;
#elif defined(SLIB_ARCH_IS_BIG_ENDIAN)
			return sl_false;
#else
			return Endian::checkLittleEndianRuntime();
#endif
		}

		static SLIB_INLINE sl_bool isBE()
		{
#if defined(SLIB_ARCH_IS_LITTLE_ENDIAN)
			return sl_false;
#elif defined(SLIB_ARCH_IS_BIG_ENDIAN)
			return sl_true;
#else
			return Endian::checkBigEndianRuntime();
#endif
		}

		static SLIB_INLINE sl_uint16 swap16(sl_uint16 v)
		{
			sl_uint8* b = (sl_uint8*)(&v);
			sl_uint8 t = b[0];
			b[0] = b[1];
			b[1] = t;
			return v;
		}

		static SLIB_INLINE sl_uint32 swap32(sl_uint32 v)
		{
			sl_uint8* b = (sl_uint8*)(&v);
			for (int i = 0; i < 2; i++) {
				sl_uint8 t = b[i];
				b[i] = b[3-i];
				b[3-i] = t;
			}
			return v;
		}

		static SLIB_INLINE sl_uint64 swap64(sl_uint64 v)
		{
			sl_uint8* b = (sl_uint8*)(&v);
			for (int i = 0; i < 4; i++) {
				sl_uint8 t = b[i];
				b[i] = b[7 - i];
				b[7 - i] = t;
			}
			return v;
		}


		static float SLIB_INLINE swapFloat(float v)
		{
			sl_uint8* b = (sl_uint8*)(&v);
			for (int i = 0; i < 2; i++) {
				sl_uint8 t = b[i];
				b[i] = b[3 - i];
				b[3 - i] = t;
			}
			return v;
		}

		static double SLIB_INLINE swapDouble(double v)
		{
			sl_uint8* b = (sl_uint8*)(&v);
			for (int i = 0; i < 4; i++) {
				sl_uint8 t = b[i];
				b[i] = b[3 - i];
				b[3 - i] = t;
			}
			return v;
		}


		// swap only if the system is little endian
		static SLIB_INLINE sl_uint16 swap16LE(sl_uint16 v)
		{
			if (isLE()) {
				return swap16(v);
			} else {
				return v;
			}
		}

		static SLIB_INLINE sl_uint32 swap32LE(sl_uint32 v)
		{
			if (isLE()) {
				return swap32(v);
			} else {
				return v;
			}
		}

		static SLIB_INLINE sl_uint64 swap64LE(sl_uint64 v)
		{
			if (isLE()) {
				return swap64(v);
			} else {
				return v;
			}
		}

		static SLIB_INLINE float swapFloatLE(float v)
		{
			if (isLE()) {
				return swapFloat(v);
			} else {
				return v;
			}
		}

		static SLIB_INLINE double swapDoubleLE(double v)
		{
			if (isLE()) {
				return swapDouble(v);
			} else {
				return v;
			}
		}

		// swap only if the system is big endian
		static SLIB_INLINE sl_uint16 swap16BE(sl_uint16 v)
		{
			if (isLE()) {
				return v;
			} else {
				return swap16(v);
			}
		}

		static SLIB_INLINE sl_uint32 swap32BE(sl_uint32 v)
		{
			if (isLE()) {
				return v;
			} else {
				return swap32(v);
			}
		}

		static SLIB_INLINE sl_uint64 swap64BE(sl_uint64 v)
		{
			if (isLE()) {
				return v;
			} else {
				return swap64(v);
			}
		}

		static SLIB_INLINE float swapFloatBE(float v)
		{
			if (isLE()) {
				return v;
			} else {
				return swapFloat(v);
			}
		}

		static SLIB_INLINE double swapDoubleBE(double v)
		{
			if (isLE()) {
				return v;
			} else {
				return swapDouble(v);
			}
		}
	};

}

#endif

