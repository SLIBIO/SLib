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

#ifndef CHECKHEADER_SLIB_CRYPTO_HASH
#define CHECKHEADER_SLIB_CRYPTO_HASH

#include "definition.h"

#include "../core/string.h"
#include "../core/memory.h"
#include "../core/mio.h"
#include "../core/scoped.h"

namespace slib
{
	
	template <class CLASS>
	class SLIB_EXPORT CryptoHash
	{
	public:
		void execute(const void* input, sl_size n, void* output)
		{
			CLASS& h = *((CLASS*)this);
			h.start();
			h.update(input, n);
			h.finish(output);
		}
		
		static void hash(const void* input, sl_size n, void* output)
		{
			CLASS h;
			h.start();
			h.update(input, n);
			h.finish(output);
		}
		
		static void hash(const String& s, void* output)
		{
			hash(s.getData(), s.getLength(), output);
		}
		
		static void hash(const Memory& data, void* output)
		{
			hash(data.getData(), data.getSize(), output);
		}
		
		static Memory hash(const void* input, sl_size n)
		{
			char v[CLASS::HashSize];
			hash(input, n, v);
			return Memory::create(v, CLASS::HashSize);
		}
		
		static Memory hash(const String& s)
		{
			char v[CLASS::HashSize];
			hash(s.getData(), s.getLength(), v);
			return Memory::create(v, CLASS::HashSize);
		}
		
		static Memory hash(const Memory& data)
		{
			char v[CLASS::HashSize];
			hash(data.getData(), data.getSize(), v);
			return Memory::create(v, CLASS::HashSize);
		}
		
		void applyMask_MGF1(const void* seed, sl_uint32 sizeSeed, void* target, sl_uint32 sizeTarget);

	};
	
}

#include "detail/hash.inc"

#endif
