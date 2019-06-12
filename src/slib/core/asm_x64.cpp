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

#include "slib/core/asm.h"

#ifdef SLIB_ARCH_IS_X64

#if defined(SLIB_COMPILER_IS_VC)
#	include <intrin.h>
#else
#	include <cpuid.h>
#endif

namespace slib
{
	
	namespace priv
	{
		namespace asm_x64
		{
			
			static sl_bool CanUseSse42()
			{
#if defined(SLIB_COMPILER_IS_VC)
				int cpu_info[4];
				__cpuid(cpu_info, 1);
				return (cpu_info[2] & (1 << 20)) != 0;
#else
				unsigned int eax, ebx, ecx, edx;
				return __get_cpuid(1, &eax, &ebx, &ecx, &edx) && ((ecx & (1 << 20)) != 0);
#endif
			}

		}
	}
	
	sl_bool CanUseSse42()
	{
		static sl_bool f = priv::asm_x64::CanUseSse42();
		return f;
	}
	
}

#endif
