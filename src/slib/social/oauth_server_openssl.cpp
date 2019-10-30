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

#include "slib/social/oauth_server.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(OAuthServerWithJwtAndOpenSSL, OAuthServerWithJwt)

	OAuthServerWithJwtAndOpenSSL::OAuthServerWithJwtAndOpenSSL()
	{
		setAlgorithm(JwtAlgorithm::HS256);
	}

	OAuthServerWithJwtAndOpenSSL::~OAuthServerWithJwtAndOpenSSL()
	{
	}

	Ref<OpenSSL_Key>& OAuthServerWithJwtAndOpenSSL::getPrivateKey()
	{
		return m_privateKey;
	}

	void OAuthServerWithJwtAndOpenSSL::setPrivateKey(const Ref<OpenSSL_Key>& key)
	{
		m_privateKey = key;
	}

	void OAuthServerWithJwtAndOpenSSL::setPrivateKey(const String& pem)
	{
		m_privateKey = OpenSSL_Key::createPrivateKey(pem);
	}

	Ref<OpenSSL_Key>& OAuthServerWithJwtAndOpenSSL::getPublicKey()
	{
		return m_publicKey;
	}

	void OAuthServerWithJwtAndOpenSSL::setPublicKey(const Ref<OpenSSL_Key>& key)
	{
		m_publicKey = key;
	}

	void OAuthServerWithJwtAndOpenSSL::setPublicKey(const String& pem)
	{
		m_publicKey = OpenSSL_Key::createPublicKey(pem);
	}

	String OAuthServerWithJwtAndOpenSSL::encrypt(const Jwt& jwt)
	{
		JwtAlgorithm alg = getAlgorithm();
		if (alg == JwtAlgorithm::HS256 || alg == JwtAlgorithm::HS384 || alg == JwtAlgorithm::HS512) {
			return OAuthServerWithJwt::encrypt(jwt);
		}
		return jwt.encode_OpenSSL(m_privateKey);
	}

	sl_bool OAuthServerWithJwtAndOpenSSL::decrypt(const String& str, Jwt& jwt)
	{
		JwtAlgorithm alg = getAlgorithm();
		if (alg == JwtAlgorithm::HS256 || alg == JwtAlgorithm::HS384 || alg == JwtAlgorithm::HS512) {
			return OAuthServerWithJwt::decrypt(str, jwt);
		}
		return jwt.decode_OpenSSL(m_publicKey, str);
	}

}
