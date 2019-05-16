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

#ifndef CHECKHEADER_SLIB_CRYPTO_HEADER
#define CHECKHEADER_SLIB_CRYPTO_HEADER

// Hash
#include "crypto/hash.h"
#include "crypto/md5.h"
#include "crypto/sha1.h"
#include "crypto/sha2.h"

// Block Cipher
#include "crypto/block_cipher.h"
#include "crypto/aes.h"
#include "crypto/blowfish.h"
#include "crypto/des.h"
#include "crypto/gcm.h"

// Stream Cipher
#include "crypto/rc4.h"
#include "crypto/chacha.h"

// Message authentication code
#include "crypto/hmac.h"
#include "crypto/poly1305.h"

// Public-key cryptosystems
#include "crypto/rsa.h"
#include "crypto/ecc.h"

// Transport Protocol
#include "crypto/tls.h"

// Other
#include "crypto/base64.h"
#include "crypto/jwt.h"

// Third-party
#include "crypto/zlib.h"
#include "crypto/openssl.h"

#endif
