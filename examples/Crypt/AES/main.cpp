#include <slib.h>

using namespace slib;

String Encrypt(const String& plain, const String& key, const String& iv)
{
	AES aes;
	aes.setKey_SHA256(key);
	char hashIV[16];
	MD5::hash(iv, hashIV);
	return String::makeHexString(aes.encrypt_CBC_PKCS7Padding(hashIV, plain.getData(), plain.getLength()));
}

String Decrypt(const String& strCipher, const String& key, const String& iv)
{
	Memory cipher = strCipher.parseHexString();
	AES aes;
	aes.setKey_SHA256(key);
	char hashIV[16];
	MD5::hash(iv, hashIV);
	return String::fromUtf8(aes.decrypt_CBC_PKCS7Padding(hashIV, cipher.getData(), cipher.getSize()));
}

int main(int argc, const char * argv[])
{
	String iv = "abc";
	String key = "aaaa";
	String encrypt = Encrypt("This string is used to check AES.", key, iv);
	String decrypt = Decrypt(encrypt, key, iv);
	Println("Encrypt=%s", encrypt);
	Println("Decrypt=%s", decrypt);
	return 0;
}
