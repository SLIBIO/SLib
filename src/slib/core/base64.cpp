#include "../../../inc/slib/core/base64.h"
#include "../../../inc/slib/core/log.h"

SLIB_NAMESPACE_BEGIN
#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

String Base64::encode(const void* buf, sl_uint32 size)
{
	if (size == 0) {
		return String::null();
	}
	const sl_uint8* input = (const sl_uint8*)buf;
	sl_uint32 last = size % 3;
	sl_uint32 countBlock = (size / 3) + (last ? 1 : 0);
	String8 ret = String8::allocate(countBlock * 4);
	if (ret.isEmpty()) {
		return ret;
	}
	sl_char8* output = ret.getBuf();
	sl_uint32 n = 0;
	for (sl_uint32 iBlock = 0; iBlock < countBlock; iBlock++) {
		sl_uint8 n0 = input[0];
		sl_uint8 n1 = (n + 1 < size) ? input[1] : 0;
		sl_uint8 n2 = (n + 2 < size) ? input[2] : 0;
		output[0] = BASE64_CHARS[(n0 & 0xFC) >> 2];
		output[1] = BASE64_CHARS[((n0 & 0x03) << 4) + ((n1 & 0xF0) >> 4)];
		output[2] = BASE64_CHARS[((n1 & 0x0F) << 2) + ((n2 & 0xC0) >> 6)];
		output[3] = BASE64_CHARS[n2 & 0x3F];
		input += 3;
		n += 3;
		output += 4;
	}
	if (last == 1) {
		*(output - 2) = '=';
	}
	if (last >= 1) {
		*(output - 1) = '=';
	}
	return ret;
}

SLIB_INLINE sl_uint32 _Base64_index(sl_char8 c)
{
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	}
	if (c >= 'a' && c <= 'z') {
		return 26 + (c - 'a');
	}
	if (c >= '0' && c <= '9') {
		return 52 + (c - '0');
	}
	if (c == '+') {
		return 62;
	}
	if (c == '/') {
		return 63;
	}
	return 64;
}

sl_uint32 Base64::decode(const String& str, void* buf, sl_uint32 size)
{
	sl_uint8* output = (sl_uint8*)buf;
	sl_uint32 len = str.getLength();
	const sl_char8* input = str.getBuf();
	// trim right (CR, LF)
	while (len > 0) {
		sl_char16 ch = input[len - 1];
		if (ch == '\r' || ch == '\n' || ch == ' ') {
			len--;
		} else {
			break;
		}
	}
	if (len < 4) {
		return 0;
	}
	sl_uint32 data[4];
	sl_uint32 indexInput = 0;
	sl_uint32 indexBlock = 0;
	sl_uint32 posInBlock = 0;
	sl_uint32 countPadding = 0;
	if (input[len - 1] == '=') {
		countPadding = 1;
		if (input[len - 2] == '=') {
			countPadding = 2;
		}
	}
	while (indexInput < len) {
		sl_char8 ch = input[indexInput];
		if (ch == '\r' || ch == '\n' || ch == ' ') {
			indexInput++;
			continue;
		}
		sl_uint32 sig = _Base64_index(ch);
		if (ch == '=' && indexInput >= len - countPadding) {
			sig = 0;
		}
		if (sig >= 64) {
			break;
		}
		data[posInBlock++] = sig;
		if (posInBlock >= 4) {
			posInBlock = 0;
			sl_uint32 p = indexBlock * 3;
			output[p] = (sl_uint8)((data[0] << 2) + ((data[1] & 0x30) >> 4));
			output[p + 1] = (sl_uint8)(((data[1] & 0xf) << 4) + ((data[2] & 0x3c) >> 2));
			output[p + 2] = (sl_uint8)(((data[2] & 0x3) << 6) + data[3]);
			indexBlock++;
		}
		indexInput++;
	}
	if (indexInput == len && indexBlock > 0 && posInBlock == 0) {
		return indexBlock * 3 - countPadding;
	}
	return 0;
}

Memory Base64::decode(const String& base64)
{
	sl_uint32 len = base64.length();
	if (len < 4) {
		return Memory::null();
	}
	sl_uint32 size = len / 4 * 3;
	sl_uint8* buf = (sl_uint8*)(Base::createMemory(size));
	if (!buf) {
		return Memory::null();
	}
	sl_uint32 sizeOutput = decode(base64, buf, size);
	Memory ret;
	if (sizeOutput > 0) {
		ret = Memory::create(buf, sizeOutput);
	}
	Base::freeMemory(buf);
	return ret;
}
SLIB_NAMESPACE_END

