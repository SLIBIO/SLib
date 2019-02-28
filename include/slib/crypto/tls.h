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

#ifndef CHECKHEADER_SLIB_CRYPTO_TLS
#define CHECKHEADER_SLIB_CRYPTO_TLS

#include "definition.h"

#include "../core/async.h"

namespace slib
{
	
	class SLIB_EXPORT TlsContextParam
	{
	public:
		Memory certificate; // X.509 Certificate (or chain) in PEM format
		Memory privateKey; // Private Key in PEM format
		HashMap<String, Memory> certificates;
		HashMap<String, Memory> privateKeys;

		sl_bool flagVerify;
		
		String serverName; // At Client, sets the server name indication ClientHello extension to contain the value name

	public:
		TlsContextParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TlsContextParam)
		
	public:
		void setCertificate(const Memory& certificate);
		
		void setCertificate(const String& serverName, const Memory& certificate);
		
		void setPrivateKey(const Memory& privateKey);
		
		void setPrivateKey(const String& serverName, const Memory& privateKey);
		
		void setCertificateFile(const String& path_PEM);
		
		void setPrivateKeyFile(const String& path_PEM);
		
		void setCertificateFile(const String& serverName, const String& path_PEM);
		
		void setPrivateKeyFile(const String& serverName, const String& path_PEM);
		
	};
	
	class SLIB_EXPORT TlsStreamResult
	{
	public:
		AsyncStream* stream;
		sl_bool flagError;
		
	public:
		TlsStreamResult(AsyncStream*);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TlsStreamResult)
		
	};
	
	class TlsContext;
	
	class SLIB_EXPORT TlsStreamParam : public TlsContextParam
	{
	public:
		Ref<TlsContext> context;
		
		sl_uint32 readingBufferSize;
		sl_uint32 writingBufferSize;
		
		sl_bool flagAutoStartHandshake;

		Function<void(TlsStreamResult&)> onHandshake;
		
	public:
		TlsStreamParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TlsStreamParam)
		
	};
	
	class SLIB_EXPORT TlsConnectStreamParam : public TlsStreamParam
	{
	public:
		TlsConnectStreamParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TlsConnectStreamParam)
		
	};
	
	class SLIB_EXPORT TlsAcceptStreamParam : public TlsStreamParam
	{
	public:
		TlsAcceptStreamParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TlsAcceptStreamParam)
		
	};
	
	class SLIB_EXPORT TlsContext : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		TlsContext();
		
		~TlsContext();
		
	};
	
	class SLIB_EXPORT TlsAsyncStream : public AsyncStream
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		TlsAsyncStream();
		
		~TlsAsyncStream();
		
	public:
		virtual void handshake() = 0;
		
	};
	
}

#endif
