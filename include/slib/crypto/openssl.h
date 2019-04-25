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

#ifndef CHECKHEADER_SLIB_CRYPTO_OPENSSL
#define CHECKHEADER_SLIB_CRYPTO_OPENSSL

#include "definition.h"

#include "tls.h"
#include "rsa.h"

struct ssl_ctx_st;
struct ssl_st;

namespace slib
{
	
	class SLIB_EXPORT OpenSSL_Context : public TlsContext
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		OpenSSL_Context();
		
		~OpenSSL_Context();
		
	public:
		virtual ssl_ctx_st* getContext() = 0;
		
	};
	
	class SLIB_EXPORT OpenSSL_AsyncStream : public TlsAsyncStream
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		OpenSSL_AsyncStream();
		
		~OpenSSL_AsyncStream();
		
	public:
		virtual ssl_st* getSSL() = 0;

	};
	
	class SLIB_EXPORT OpenSSL
	{
	public:
		static Ref<OpenSSL_Context> createContext(const TlsContextParam& param);
		
		static Ref<OpenSSL_AsyncStream> connectStream(const Ref<AsyncStream>& baseStream, const TlsConnectStreamParam& param);
		
		static Ref<OpenSSL_AsyncStream> acceptStream(const Ref<AsyncStream>& baseStream, const TlsAcceptStreamParam& param);
		
		
		static sl_bool isProbablePrime(const void* num_BigEndian, sl_uint32 nBytes, sl_bool* pFlagError = sl_null);
		
		// returns value in Big-endian form
		static Memory generatePrime(sl_uint32 nBits);
		
		static sl_bool randomBytes(void* bytes, sl_uint32 nBytes, sl_bool flagPrivate = sl_false);
		
		
		static void generateRSA(RSAPrivateKey& _output, sl_uint32 nBits);
		
	};

}

#endif
