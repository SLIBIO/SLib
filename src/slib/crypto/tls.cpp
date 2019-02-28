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

#include "slib/crypto/tls.h"

#include "slib/core/file.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TlsContextParam)

	TlsContextParam::TlsContextParam()
	 : flagVerify(sl_false)
	{
	}
	
	void TlsContextParam::setCertificate(const Memory& _certificate)
	{
		certificate = _certificate;
	}
	
	void TlsContextParam::setCertificate(const String& serverName, const Memory& certificate)
	{
		certificates.put(serverName, certificate);
	}
	
	void TlsContextParam::setPrivateKey(const Memory& _privateKey)
	{
		privateKey = _privateKey;
	}
		
	void TlsContextParam::setPrivateKey(const String& serverName, const Memory& privateKey)
	{
		privateKeys.put(serverName, privateKey);
	}
	
	void TlsContextParam::setCertificateFile(const String& path_PEM)
	{
		certificate = File::readAllBytes(path_PEM);
	}
	
	void TlsContextParam::setCertificateFile(const String& serverName, const String& path_PEM)
	{
		setCertificate(serverName, File::readAllBytes(path_PEM));
	}
	
	void TlsContextParam::setPrivateKeyFile(const String& path_PEM)
	{
		privateKey = File::readAllBytes(path_PEM);
	}
	
	void TlsContextParam::setPrivateKeyFile(const String& serverName, const String& path_PEM)
	{
		setPrivateKey(serverName, File::readAllBytes(path_PEM));
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TlsStreamResult)
	
	TlsStreamResult::TlsStreamResult(AsyncStream* _stream)
	 : stream(_stream), flagError(sl_true)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TlsStreamParam)
	
	TlsStreamParam::TlsStreamParam():
		readingBufferSize(0x40000),
		writingBufferSize(0x40000),
		flagAutoStartHandshake(sl_true)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TlsConnectStreamParam)

	TlsConnectStreamParam::TlsConnectStreamParam()
	{
		readingBufferSize = 0x100000;
		writingBufferSize = 0x20000;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TlsAcceptStreamParam)
	
	TlsAcceptStreamParam::TlsAcceptStreamParam()
	{
		readingBufferSize = 0x10000;
		writingBufferSize = 0x80000;
	}
	
	SLIB_DEFINE_OBJECT(TlsContext, Object)
	
	TlsContext::TlsContext()
	{
	}
	
	TlsContext::~TlsContext()
	{
	}
	
	SLIB_DEFINE_OBJECT(TlsAsyncStream, AsyncStream)
	
	TlsAsyncStream::TlsAsyncStream()
	{
	}

	TlsAsyncStream::~TlsAsyncStream()
	{
	}

}
