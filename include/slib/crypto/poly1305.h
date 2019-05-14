/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_CRYPTO_POLY1305
#define CHECKHEADER_SLIB_CRYPTO_POLY1305

#include "definition.h"

/*
	Poly1305

 https://en.wikipedia.org/wiki/Poly1305
 
 cryptographic message authentication code (MAC) created by Daniel J. Bernstein
 
*/

namespace slib
{

	class SLIB_EXPORT Poly1305
	{
	public:
		Poly1305();
		
		~Poly1305();
		
	public:
		// key: 32 bytes (256 bits)
		void start(const void* key);
		
		void update(const void* input, sl_size n);
		
		// output: 16 bytes (128 bits)
		void finish(void* output);
		
		// key: 32 bytes (256 bits)
		// output: 16 bytes (128 bits)
		static void execute(const void* key, const void* message, sl_size lenMessage, void* output);
		
	private:
		// input: 16 * nBlocks bytes
		void updateBlocks(const void* input, sl_size nBlocks);
		
	private:
		sl_uint32 m_r[5];
		sl_uint32 m_h[5];
		sl_uint32 m_pad[4];
		sl_uint32 m_leftOver;
		sl_uint8 m_buffer[16];
		sl_bool m_flagFinal;
		
	};

}

#endif
