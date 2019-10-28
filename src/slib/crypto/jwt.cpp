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
#include "slib/crypto/hmac.h"
#include "slib/crypto/sha2.h"

#include "slib/core/string_buffer.h"

namespace slib
{

	namespace priv
	{
		namespace jwt
		{
			
			SLIB_STATIC_STRING(g_field_iss, "iss")
			SLIB_STATIC_STRING(g_field_sub, "sub")
			SLIB_STATIC_STRING(g_field_aud, "aud")
			SLIB_STATIC_STRING(g_field_exp, "exp")
			SLIB_STATIC_STRING(g_field_nbf, "nbf")
			SLIB_STATIC_STRING(g_field_iat, "iat")
			SLIB_STATIC_STRING(g_field_jti, "jti")
			
			SLIB_STATIC_STRING(g_field_typ, "typ")
			SLIB_STATIC_STRING(g_field_cty, "cty")
			SLIB_STATIC_STRING(g_field_alg, "alg")

			SLIB_STATIC_STRING(g_alg_HS256, "HS256")
			SLIB_STATIC_STRING(g_alg_HS384, "HS384")
			SLIB_STATIC_STRING(g_alg_HS512, "HS512")
			SLIB_STATIC_STRING(g_alg_RS256, "RS256")
			SLIB_STATIC_STRING(g_alg_RS384, "RS384")
			SLIB_STATIC_STRING(g_alg_RS512, "RS512")
			SLIB_STATIC_STRING(g_alg_ES256, "ES256")
			SLIB_STATIC_STRING(g_alg_ES384, "ES384")
			SLIB_STATIC_STRING(g_alg_ES512, "ES512")
			SLIB_STATIC_STRING(g_alg_PS256, "PS256")
			SLIB_STATIC_STRING(g_alg_PS384, "PS384")
			SLIB_STATIC_STRING(g_alg_PS512, "PS512")

		}
	}
	
	using namespace priv::jwt;

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Jwt)
	
	Jwt::Jwt() noexcept
	{
		setType("JWT");
		setAlgorithm(JwtAlgorithm::HS256);
	}
	
	String Jwt::encode(const Memory& secret) const noexcept
	{
		StringBuffer sb;
		sb.add(Base64::encodeUrl(header.toJsonString()));
		sb.add(".");
		sb.add(Base64::encodeUrl(payload.toJsonString()));
		String content = sb.merge();
		String signature = generateSignature(secret, content.getData(), content.getLength());
		if (signature.isNotEmpty()) {
			sb.add(".");
			sb.add(signature);
			return sb.merge();
		} else {
			return content;
		}
	}
	
	sl_bool Jwt::decode(const Memory& secret, const String& token) noexcept
	{
		sl_reg pos1 = token.indexOf('.');
		if (pos1 < 0) {
			return sl_false;
		}
		String s1, s2, s3;
		sl_reg pos2 = token.indexOf('.', pos1 + 1);
		if (pos2 < 0) {
			return sl_false;
		}
		s1 = token.substring(0, pos1);
		s2 = token.substring(pos1 + 1, pos2);
		s3 = token.substring(pos2 + 1);
		if (s1.isNotEmpty()) {
			Memory mem = Base64::decode(s1);
			if (mem.isNull()) {
				return sl_false;
			}
			JsonParseParam pp;
			header = Json::parseJson(String::fromUtf8(mem), pp);
			if (pp.flagError) {
				return sl_false;
			}
		}
		if (s2.isNotEmpty()) {
			Memory mem = Base64::decode(s2);
			if (mem.isNull()) {
				return sl_false;
			}
			JsonParseParam pp;
			payload = Json::parseJson(String::fromUtf8(mem), pp);
			if (pp.flagError) {
				return sl_false;
			}
		}
		if (s3.isEmpty()) {
			return sl_false;
		}
		return s3 == generateSignature(secret, token.getData(), pos2);
	}
	
	String Jwt::generateSignature(const Memory& secret, const void* data, sl_size size) const noexcept
	{
		sl_uint8 signature[SHA512::HashSize];
		sl_size sizeSignature;
		switch (getAlgorithm()) {
			case JwtAlgorithm::HS256:
				HMAC<SHA256>::execute(secret.getData(), secret.getSize(), data, size, signature);
				sizeSignature = SHA256::HashSize;
				break;
			case JwtAlgorithm::HS384:
				HMAC<SHA384>::execute(secret.getData(), secret.getSize(), data, size, signature);
				sizeSignature = SHA384::HashSize;
				break;
			case JwtAlgorithm::HS512:
				HMAC<SHA512>::execute(secret.getData(), secret.getSize(), data, size, signature);
				sizeSignature = SHA512::HashSize;
				break;
			default:
				sizeSignature = 0;
				break;
		}
		return Base64::encodeUrl(signature, sizeSignature);
	}
	
	String Jwt::getType() const noexcept
	{
		return header.getItem(g_field_typ).getString();
	}
	
	void Jwt::setType(const String& value) noexcept
	{
		header.putItem(g_field_typ, value);
	}
	
	String Jwt::getContentType() const noexcept
	{
		return header.getItem(g_field_cty).getString();
	}
	
	void Jwt::setContentType(const String& value) noexcept
	{
		header.putItem(g_field_cty, value);
	}
	
	JwtAlgorithm Jwt::getAlgorithm() const noexcept
	{
		String alg = header.getItem(g_field_alg).getString();
		if (alg.equalsIgnoreCase(g_alg_HS256)) return JwtAlgorithm::HS256;
		if (alg.equalsIgnoreCase(g_alg_HS384)) return JwtAlgorithm::HS384;
		if (alg.equalsIgnoreCase(g_alg_HS512)) return JwtAlgorithm::HS512;
		if (alg.equalsIgnoreCase(g_alg_RS256)) return JwtAlgorithm::RS256;
		if (alg.equalsIgnoreCase(g_alg_RS384)) return JwtAlgorithm::RS384;
		if (alg.equalsIgnoreCase(g_alg_RS512)) return JwtAlgorithm::RS512;
		if (alg.equalsIgnoreCase(g_alg_ES256)) return JwtAlgorithm::ES256;
		if (alg.equalsIgnoreCase(g_alg_ES384)) return JwtAlgorithm::ES384;
		if (alg.equalsIgnoreCase(g_alg_ES512)) return JwtAlgorithm::ES512;
		if (alg.equalsIgnoreCase(g_alg_PS256)) return JwtAlgorithm::PS256;
		if (alg.equalsIgnoreCase(g_alg_PS384)) return JwtAlgorithm::PS384;
		if (alg.equalsIgnoreCase(g_alg_PS512)) return JwtAlgorithm::PS512;
		return JwtAlgorithm::None;
	}
	
	void Jwt::setAlgorithm(const JwtAlgorithm& value) noexcept
	{
		switch (value) {
			case JwtAlgorithm::HS256: header.putItem(g_field_alg, g_alg_HS256); break;
			case JwtAlgorithm::HS384: header.putItem(g_field_alg, g_alg_HS384); break;
			case JwtAlgorithm::HS512: header.putItem(g_field_alg, g_alg_HS512); break;
			case JwtAlgorithm::RS256: header.putItem(g_field_alg, g_alg_RS256); break;
			case JwtAlgorithm::RS384: header.putItem(g_field_alg, g_alg_RS384); break;
			case JwtAlgorithm::RS512: header.putItem(g_field_alg, g_alg_RS512); break;
			case JwtAlgorithm::ES256: header.putItem(g_field_alg, g_alg_ES256); break;
			case JwtAlgorithm::ES384: header.putItem(g_field_alg, g_alg_ES384); break;
			case JwtAlgorithm::ES512: header.putItem(g_field_alg, g_alg_ES512); break;
			case JwtAlgorithm::PS256: header.putItem(g_field_alg, g_alg_PS256); break;
			case JwtAlgorithm::PS384: header.putItem(g_field_alg, g_alg_PS384); break;
			case JwtAlgorithm::PS512: header.putItem(g_field_alg, g_alg_PS512); break;
			default:
				header.removeItem(g_field_alg);
				break;
		}
	}
	
	String Jwt::getIssuer() const noexcept
	{
		return payload.getItem(g_field_iss).getString();
	}
	
	void Jwt::setIssuer(const String& value) noexcept
	{
		payload.putItem(g_field_iss, value);
	}
	
	String Jwt::getSubject() const noexcept
	{
		return payload.getItem(g_field_sub).getString();
	}
	
	void Jwt::setSubject(const String& value) noexcept
	{
		payload.putItem(g_field_sub, value);
	}
	
	String Jwt::getAudience() const noexcept
	{
		return payload.getItem(g_field_aud).getString();
	}
	
	void Jwt::setAudience(const String& value) noexcept
	{
		payload.putItem(g_field_aud, value);
	}
	
	Time Jwt::getExpirationTime() const noexcept
	{
		Variant var = payload.getItem(g_field_exp);
		if (var.isNotNull()) {
			return Time::fromUnixTime(var.getInt64());
		} else {
			return Time::zero();
		}
	}
	
	void Jwt::setExpirationTime(const Time& value) noexcept
	{
		if (value.isNotZero()) {
			payload.putItem(g_field_exp, value.toUnixTime());
		}
	}
	
	Time Jwt::getNotBefore() const noexcept
	{
		Variant var = payload.getItem(g_field_nbf);
		if (var.isNotNull()) {
			return Time::fromUnixTime(var.getInt64());
		} else {
			return Time::zero();
		}
	}
	
	void Jwt::setNotBefore(const Time& value) noexcept
	{
		if (value.isNotZero()) {
			payload.putItem(g_field_nbf, value.toUnixTime());
		}
	}
	
	String Jwt::getIssuedAt() const noexcept
	{
		return payload.getItem(g_field_iat).getString();
	}
	
	void Jwt::setIssuedAt(const String& value) noexcept
	{
		payload.putItem(g_field_iat, value);
	}
	
	String Jwt::getId() const noexcept
	{
		return payload.getItem(g_field_jti).getString();
	}
	
	void Jwt::setId(const String& value) noexcept
	{
		payload.putItem(g_field_jti, value);
	}

}
