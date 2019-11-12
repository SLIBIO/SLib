#include <slib.h>

using namespace slib;

String Encrypt(const String& plain, const String& key)
{
	if (key.getLength() != 24) {
		return sl_null;
	}
	TripleDES des;
	des.setKey24(key.getData());
	return String::makeHexString(des.encrypt_ECB_PKCS7Padding(plain.getData(), plain.getLength()));
}

String Decrypt(const String& strCipher, const String& key)
{
	if (key.getLength() != 24) {
		return sl_null;
	}
	TripleDES des;
	des.setKey24(key.getData());
	Memory cipher = strCipher.parseHexString();
	return String::fromUtf8(des.decrypt_ECB_PKCS7Padding(cipher.getData(), cipher.getSize()));
}

int main(int argc, const char * argv[])
{
	String key = "123456781234567812345678";
	String encrypt = Encrypt("This string is used to check 3DES.", key);
	String decrypt = Decrypt(encrypt, key);
	Println("Encrypt=%s", encrypt);
	Println("Decrypt=%s", decrypt);
	return 0;
}
