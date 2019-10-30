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

#ifndef CHECKHEADER_SLIB_CRYPTO_JWT
#define CHECKHEADER_SLIB_CRYPTO_JWT

#include "definition.h"

#include "openssl.h"

#include "../core/json.h"

/*
	JSON Web Token: JSON-based open standard for creating access tokens that assert some number of claims (RFC 7519)
 
 		https://tools.ietf.org/html/rfc7519
 		https://tools.ietf.org/html/rfc7518
*/

namespace slib
{
	
	enum class JwtAlgorithm
	{
		None = 0, // No digital signature or MAC performed
		HS256 = 10, // HMAC using SHA-256
		HS384 = 11, // HMAC using SHA-384
		HS512 = 12, // HMAC using SHA-512
		RS256 = 20, // RSASSA-PKCS1-v1_5 using SHA-256
		RS384 = 21, // RSASSA-PKCS1-v1_5 using SHA-384
		RS512 = 22, // RSASSA-PKCS1-v1_5 using SHA-512
		ES256 = 30, // ECDSA using P-256 and SHA-256
		ES384 = 31, // ECDSA using P-384 and SHA-384
		ES512 = 32, // ECDSA using P-512 and SHA-512
		PS256 = 40, // RSASSA-PSS using SHA-256 and MGF1 with SHA-256
		PS384 = 41, // RSASSA-PSS using SHA-384 and MGF1 with SHA-384
		PS512 = 42  // RSASSA-PSS using SHA-512 and MGF1 with SHA-512
	};
	
	class SLIB_EXPORT Jwt
	{
	public:
		Json header;
		Json payload;
		
	public:
		Jwt() noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Jwt)
		
	public:
		String encode(const Memory& secret) const noexcept;
		
		sl_bool decode(const Memory& secret, const String& token) noexcept;
		
		String generateSignature(const Memory& secret, const void* data, sl_size size) const noexcept;
		
		String encode_OpenSSL(const Ref<OpenSSL_Key>& key) const noexcept;
		
		sl_bool decode_OpenSSL(const Ref<OpenSSL_Key>& key, const String& token) noexcept;
		
		String generateSignature_OpenSSL(const Ref<OpenSSL_Key>& key, const void* data, sl_size size) const noexcept;
		
		sl_bool verifySignature_OpenSSL(const Ref<OpenSSL_Key>& key, const String& signature, const void* data, sl_size size) const noexcept;
		
		// header
	public:
		String getType() const noexcept;
		void setType(const String& value) noexcept;
		
		String getContentType() const noexcept;
		void setContentType(const String& value) noexcept;
		
		JwtAlgorithm getAlgorithm() const noexcept;
		void setAlgorithm(const JwtAlgorithm& value) noexcept;
		
		// payload
	public:
		String getIssuer() const noexcept;
		void setIssuer(const String& value) noexcept;
		
		String getSubject() const noexcept;
		void setSubject(const String& value) noexcept;
		
		String getAudience() const noexcept;
		void setAudience(const String& value) noexcept;
		
		Time getExpirationTime() const noexcept;
		void setExpirationTime(const Time& value) noexcept;
		
		Time getNotBefore() const noexcept;
		void setNotBefore(const Time& value) noexcept;
		
		String getIssuedAt() const noexcept;
		void setIssuedAt(const String& value) noexcept;

		String getId() const noexcept;
		void setId(const String& value) noexcept;

	private:		
		sl_size _decode(const String& token, String& signature) noexcept;
		
	};

}

#endif
