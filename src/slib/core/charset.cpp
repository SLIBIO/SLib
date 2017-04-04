/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/charset.h"
#include "slib/core/base.h"

namespace slib
{
	
	sl_size Charsets::utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer)
	{
		if (lenUtf8 < 0) {
			lenUtf8 = Base::getStringLength(utf8, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf8 && (lenUtf16Buffer < 0 || n < lenUtf16Buffer); i++) {
			sl_uint32 ch = (sl_uint32)((sl_uint8)utf8[i]);
			if (ch < 0x80) {
				if (utf16) {
					utf16[n++] = (sl_char16)ch;
				} else {
					n++;
				}
			} else if (ch < 0xC0) {
				// Corrupted data element
			} else if (ch < 0xE0) {
				if (i + 1 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					if ((ch1 & 0xC0) == 0x80) {
						if (utf16) {
							utf16[n++] = (sl_char16)(((ch & 0x1F) << 6) | (ch1 & 0x3F));
						} else {
							n++;
						}
					}
				}
			} else if (ch < 0xF0) {
				if (i + 2 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
					if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80)) {
						if (utf16) {
							utf16[n++] = (sl_char16)(((ch & 0x0F) << 12) | ((ch1 & 0x3F) << 6) | (ch2 & 0x3F));
						} else {
							n++;
						}
					}
				}
			}
		}
		return n;
	}

	sl_size Charsets::utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer)
	{
		if (lenUtf8 < 0) {
			lenUtf8 = Base::getStringLength(utf8, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf8 && (lenUtf32Buffer < 0 || n < lenUtf32Buffer); i++) {
			sl_uint32 ch = (sl_uint32)((sl_uint8)utf8[i]);
			if (ch < 0x80) {
				if (utf32) {
					utf32[n++] = (sl_char32)ch;
				} else {
					n++;
				}
			} else if (ch < 0xC0) {
				// Corrupted data element
			} else if (ch < 0xE0) {
				if (i + 1 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					if ((ch1 & 0xC0) == 0x80) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch & 0x1F) << 6) | (ch1 & 0x3F));
						} else {
							n++;
						}
					}
				}
			} else if (ch < 0xF0) {
				if (i + 2 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
					if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80)) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch & 0x0F) << 12) | ((ch1 & 0x3F) << 6) | (ch2 & 0x3F));
						} else {
							n++;
						}
					}
				}
			} else if (ch < 0xF8) {
				if (i + 3 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
					if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80)) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch & 0x07) << 18) | ((ch1 & 0x3F) << 12) | ((ch2 & 0x3F) << 6) | (ch3 & 0x3F));
						} else {
							n++;
						}
					}
				}
			} else if (ch < 0xFC) {
				if (i + 4 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch4 = (sl_uint32)((sl_uint8)utf8[++i]);
					if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80) && ((ch4 & 0xC0) == 0x80)) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch & 0x03) << 24) | ((ch1 & 0x3F) << 18) | ((ch2 & 0x3F) << 12) | ((ch3 & 0x3F) << 6) | (ch4 & 0x3F));
						} else {
							n++;
						}
					}
				}
			} else if (ch < 0xFE) {
				if (i + 5 < lenUtf8) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch2 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch3 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch4 = (sl_uint32)((sl_uint8)utf8[++i]);
					sl_uint32 ch5 = (sl_uint32)((sl_uint8)utf8[++i]);
					if (((ch1 & 0xC0) == 0x80) && ((ch2 & 0xC0) == 0x80) && ((ch3 & 0xC0) == 0x80) && ((ch4 & 0xC0) == 0x80) && ((ch5 & 0xC0) == 0x80)) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch & 0x01) << 30) | ((ch1 & 0x3F) << 24) | ((ch2 & 0x3F) << 18) | ((ch3 & 0x3F) << 12) | ((ch4 & 0x3F) << 6) | (ch5 & 0x3F));
						} else {
							n++;
						}
					}
				}
			}
		}
		return n;
	}

	sl_size Charsets::utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer)
	{
		if (lenUtf16 < 0) {
			lenUtf16 = Base::getStringLength2(utf16, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf16 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
			sl_uint32 ch = (sl_uint32)(utf16[i]);
			if (ch < 0x80) {
				if (utf8) {
					utf8[n++] = (sl_char8)(ch);
				} else {
					n++;
				}
			} else if (ch < 0x800) {
				if (lenUtf8Buffer < 0 || n + 1 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 6) | 0xC0);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 2;
					}
				}
			} else {
				if (lenUtf8Buffer < 0 || n + 2 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 12) | 0xE0);
						utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 3;
					}
				}
			}
		}
		return n;
	}

	sl_size Charsets::utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer)
	{
		if (lenUtf32 < 0) {
			lenUtf32 = Base::getStringLength4(utf32, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf32 && (lenUtf8Buffer < 0 || n < lenUtf8Buffer); i++) {
			sl_uint32 ch = (sl_uint32)(utf32[i]);
			if (ch < 0x80) {
				if (utf8) {
					utf8[n++] = (sl_char8)(ch);
				} else {
					n++;
				}
			} else if (ch < 0x800) {
				if (lenUtf8Buffer < 0 || n + 1 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 6) | 0xC0);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 2;
					}
				}
			} else if (ch < 0x10000) {
				if (lenUtf8Buffer < 0 || n + 2 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 12) | 0xE0);
						utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 3;
					}
				}
			} else if (ch < 0x200000) {
				if (lenUtf8Buffer < 0 || n + 3 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 18) | 0xF0);
						utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 4;
					}
				}
			} else if (ch < 0x4000000) {
				if (lenUtf8Buffer < 0 || n + 4 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 24) | 0xF8);
						utf8[n++] = (sl_char8)(((ch >> 18) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 5;
					}
				}
			} else if (ch < 0x80000000) {
				if (lenUtf8Buffer < 0 || n + 5 < lenUtf8Buffer) {
					if (utf8) {
						utf8[n++] = (sl_char8)((ch >> 30) | 0xFC);
						utf8[n++] = (sl_char8)(((ch >> 24) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 18) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 12) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)(((ch >> 6) & 0x3F) | 0x80);
						utf8[n++] = (sl_char8)((ch & 0x3F) | 0x80);
					} else {
						n += 6;
					}
				}
			}
		}
		return n;
	}

	sl_size Charsets::utf16ToUtf32(const sl_char16* utf16, sl_reg lenUtf16, sl_char32* utf32, sl_reg lenUtf32Buffer)
	{
		if (lenUtf16 < 0) {
			lenUtf16 = Base::getStringLength2(utf16, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf16 && (lenUtf32Buffer < 0 || n < lenUtf32Buffer); i++) {
			sl_uint32 ch = (sl_uint32)((sl_uint16)utf16[i]);
			if (ch < 0xD800 || ch >= 0xE000) {
				if (utf32) {
					utf32[n++] = (sl_char32)ch;
				} else {
					n++;
				}
			} else {
				if (i + 1 < lenUtf16) {
					sl_uint32 ch1 = (sl_uint32)((sl_uint8)utf16[++i]);
					if (ch < 0xDC00 && ch1 >= 0xDC00 && ch1 < 0xE000) {
						if (utf32) {
							utf32[n++] = (sl_char32)(((ch - 0xD800) << 10) | (ch1 - 0xDC00));
						} else {
							n++;
						}
					}
				}
			}
		}
		return n;
	}

	sl_size Charsets::utf32ToUtf16(const sl_char32* utf32, sl_reg lenUtf32, sl_char16* utf16, sl_reg lenUtf16Buffer)
	{
		if (lenUtf32 < 0) {
			lenUtf32 = Base::getStringLength4(utf32, -1) + 1;
		}
		sl_reg n = 0;
		for (sl_reg i = 0; i < lenUtf32 && (lenUtf16Buffer < 0 || n < lenUtf16Buffer); i++) {
			sl_uint32 ch = (sl_uint32)(utf32[i]);
			if (ch >= 0x10000) {
				if (ch < 0x110000) {
					if (lenUtf16Buffer < 0 || n + 1 < lenUtf16Buffer) {
						if (utf16) {
							ch -= 0x10000;
							utf16[n++] = (sl_char16)(0xD800 + (ch >> 10));
							utf16[n++] = (sl_char16)(0xDC00 + (ch & 0x3FF));
						} else {
							n += 2;
						}
					}
				}
			} else {
				if (ch < 0xD800 || ch >= 0xE000) {
					if (utf16) {
						utf16[n++] = (sl_char16)(ch);
					} else {
						n++;
					}
				}
			}
		}
		return n;
	}

}
