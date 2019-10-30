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

#include "slib/crypto/jwt.h"

#include "slib/crypto/base64.h"
#include "slib/crypto/openssl.h"

#include "slib/core/string_buffer.h"

namespace slib
{

	String Jwt::encode_OpenSSL(const Ref<OpenSSL_Key>& privateKey) const noexcept
	{
		StringBuffer sb;
		sb.add(Base64::encodeUrl(header.toJsonString()));
		sb.add(".");
		sb.add(Base64::encodeUrl(payload.toJsonString()));
		String content = sb.merge();
		String signature = generateSignature_OpenSSL(privateKey, content.getData(), content.getLength());
		if (signature.isNotEmpty()) {
			sb.add(".");
			sb.add(signature);
			return sb.merge();
		} else {
			return content;
		}
	}

	sl_bool Jwt::decode_OpenSSL(const Ref<OpenSSL_Key>& publicKey, const String& token) noexcept
	{
		String signature;
		sl_size n = _decode(token, signature);
		if (n) {
			return verifySignature_OpenSSL(publicKey, signature, token.getData(), n);
		}
		return sl_false;
	}

	String Jwt::generateSignature_OpenSSL(const Ref<OpenSSL_Key>& key, const void* data, sl_size size) const noexcept
	{
		if (key.isNull()) {
			return sl_null;
		}
		Memory sig;
		switch (getAlgorithm()) {
			case JwtAlgorithm::RS256:
				sig = key->sign_RSA_SHA256(data, size);
				break;
			case JwtAlgorithm::RS384:
				sig = key->sign_RSA_SHA384(data, size);
				break;
			case JwtAlgorithm::RS512:
				sig = key->sign_RSA_SHA512(data, size);
				break;
			case JwtAlgorithm::ES256:
				sig = key->sign_ECDSA_SHA256(data, size);
				break;
			case JwtAlgorithm::ES384:
				sig = key->sign_ECDSA_SHA384(data, size);
				break;
			case JwtAlgorithm::ES512:
				sig = key->sign_ECDSA_SHA512(data, size);
				break;
			case JwtAlgorithm::PS256:
				sig = key->sign_RSA_PSS_SHA256(data, size);
				break;
			case JwtAlgorithm::PS384:
				sig = key->sign_RSA_PSS_SHA384(data, size);
				break;
			case JwtAlgorithm::PS512:
				sig = key->sign_RSA_PSS_SHA512(data, size);
				break;
			default:
				return sl_null;
		}
		if (sig.isNotNull()) {
			return Base64::encodeUrl(sig);
		}
		return sl_null;
	}

	sl_bool Jwt::verifySignature_OpenSSL(const Ref<OpenSSL_Key>& key, const String& signature, const void* data, sl_size size) const noexcept
	{
		if (key.isNull()) {
			return sl_false;
		}
		Memory sig = Base64::decode(signature);
		void* s = sig.getData();
		if (!s) {
			return sl_false;
		}
		sl_uint32 sizeSig = (sl_uint32)(sig.getSize());
		switch (getAlgorithm()) {
			case JwtAlgorithm::RS256:
				return key->verify_RSA_SHA256(data, size, s, sizeSig);
			case JwtAlgorithm::RS384:
				return key->verify_RSA_SHA384(data, size, s, sizeSig);
			case JwtAlgorithm::RS512:
				return key->verify_RSA_SHA512(data, size, s, sizeSig);
			case JwtAlgorithm::ES256:
				return key->verify_ECDSA_SHA256(data, size, s, sizeSig);
			case JwtAlgorithm::ES384:
				return key->verify_ECDSA_SHA384(data, size, s, sizeSig);
			case JwtAlgorithm::ES512:
				return key->verify_ECDSA_SHA512(data, size, s, sizeSig);
			case JwtAlgorithm::PS256:
				return key->verify_RSA_PSS_SHA256(data, size, s, sizeSig);
			case JwtAlgorithm::PS384:
				return key->verify_RSA_PSS_SHA384(data, size, s, sizeSig);
			case JwtAlgorithm::PS512:
				return key->verify_RSA_PSS_SHA512(data, size, s, sizeSig);
			default:
				return sl_false;
		}
		return sl_false;
	}

}
