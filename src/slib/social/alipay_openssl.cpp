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

#include "slib/social/alipay.h"

#include "slib/crypto/openssl.h"
#include "slib/crypto/base64.h"

namespace slib
{

	sl_bool AlipayOrder::sign(const StringParam& privateKey_PEM)
	{
		signature.setNull();
		String info = generateString(sl_false);
		
		Ref<OpenSSL_Key> key = OpenSSL_Key::createPrivateKey(privateKey_PEM);
		if (key.isNotNull()) {
			Memory mem = key->sign_RSA_SHA256(info.getData(), info.getLength());
			if (mem.isNotNull()) {
				signature = Base64::encode(mem);
				return sl_true;
			}
		}
		return sl_false;
	}

}
