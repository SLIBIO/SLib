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

namespace slib
{
	
	/*
	 Public-Key Cryptography Standards (PKCS) #1: RSA Cryptography Specifications Version 2.1
	 
	 https://tools.ietf.org/html/rfc3447#section-7.1
	 
	 Section 7.1 RSAES-OAEP
	*/
	template <class HASH>
	sl_bool RSA::encrypt_oaep_v21(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, HASH& hash, const void* _input, sl_uint32 sizeInput, void* _output, const void* label, sl_uint32 sizeLabel)
	{
		sl_uint32 sizeRSA;
		if (keyPublic) {
			sizeRSA = keyPublic->getLength();
		} else {
			sizeRSA = keyPrivate->getLength();
		}
		if (sizeInput == 0 || sizeRSA < sizeInput + 2 * HASH::HashSize + 2) {
			return sl_false;
		}
		
		const sl_uint8* input = (const sl_uint8*)_input;
		sl_uint8* output = (sl_uint8*)_output;
		sl_uint8* seed = output + 1;
		sl_uint8* DB = seed + HASH::HashSize;
		sl_uint32 sizeDB = sizeRSA - HASH::HashSize - 1;
		sl_uint8* lHash = DB;
		sl_uint8* PS = lHash + HASH::HashSize; // Zero Area
		sl_uint8* M = output + (sizeRSA - sizeInput);
		
		// avoid error when input=output
		for (sl_uint32 i = sizeInput; i > 0; i--) {
			M[i - 1] = input[i - 1];
		}
		*output = 0;
		Math::randomMemory(seed, HASH::HashSize);
		hash.execute(label, sizeLabel, lHash);
		
		Base::zeroMemory(PS, M - PS - 1);
		*(M - 1) = 1;
		hash.applyMask_MGF1(seed, HASH::HashSize, DB, sizeDB);
		hash.applyMask_MGF1(DB, sizeDB, seed, HASH::HashSize);
		
		return execute(keyPublic, keyPrivate, output, output);
	}
	
	template <class HASH>
	sl_uint32 RSA::decrypt_oaep_v21(const RSAPublicKey* keyPublic, const RSAPrivateKey* keyPrivate, HASH& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
	{
		sl_uint32 sizeRSA;
		if (keyPublic) {
			sizeRSA = keyPublic->getLength();
		} else {
			sizeRSA = keyPrivate->getLength();
		}
		if (sizeRSA < 2 * HASH::HashSize + 2) {
			return 0;
		}
		SLIB_SCOPED_BUFFER(sl_uint8, 4096, buf, sizeRSA);
		if (!buf) {
			return 0;
		}
		if (!(execute(keyPublic, keyPrivate, input, buf))) {
			return 0;
		}
		
		sl_uint8* seed = buf + 1;
		sl_uint8* DB = seed + HASH::HashSize;
		sl_uint32 sizeDB = sizeRSA - HASH::HashSize - 1;
		sl_uint8* lHash = DB;
		
		hash.applyMask_MGF1(DB, sizeDB, seed, HASH::HashSize);
		hash.applyMask_MGF1(seed, HASH::HashSize, DB, sizeDB);
		
		hash.execute(label, sizeLabel, seed);
		sl_uint8 _check = buf[0];
		sl_uint32 i;
		for (i = 0; i < HASH::HashSize; i++) {
			_check |= (seed[i] - lHash[i]);
		}
		for (i = HASH::HashSize; i < sizeDB; i++) {
			if (_check == 0 && DB[i] == 1) {
				sl_uint32 size = sizeDB - i - 1;
				if (size > sizeOutputBuffer) {
					size = sizeOutputBuffer;
				}
				Base::copyMemory(output, DB + i + 1, size);
				return size;
			}
			if (DB[i] != 0) {
				_check = 1;
			}
		}
		return 0;
	}
	
	template <class HASH>
	sl_bool RSA::encryptPublic_oaep_v21(const RSAPublicKey& key, HASH& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label, sl_uint32 sizeLabel)
	{
		return encrypt_oaep_v21(&key, sl_null, hash, input, sizeInput, output, label, sizeLabel);
	}
	
	template <class HASH>
	sl_bool RSA::encryptPrivate_oaep_v21(const RSAPrivateKey& key, HASH& hash, const void* input, sl_uint32 sizeInput, void* output, const void* label, sl_uint32 sizeLabel)
	{
		return encrypt_oaep_v21(sl_null, &key, hash, input, sizeInput, output, label, sizeLabel);
	}
	
	template <class HASH>
	sl_uint32 RSA::decryptPublic_oaep_v21(const RSAPublicKey& key, HASH& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
	{
		return decrypt_oaep_v21(&key, sl_null, hash, input, output, sizeOutputBuffer, label, sizeLabel);
	}
	
	template <class HASH>
	sl_uint32 RSA::decryptPrivate_oaep_v21(const RSAPrivateKey& key, HASH& hash, const void* input, void* output, sl_uint32 sizeOutputBuffer, const void* label, sl_uint32 sizeLabel)
	{
		return decrypt_oaep_v21(sl_null, &key, hash, input, output, sizeOutputBuffer, label, sizeLabel);
	}

}
