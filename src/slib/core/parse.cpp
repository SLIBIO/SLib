#include "../../../inc/slib/core/parse.h"

#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/core/math.h"

namespace slib
{

	template <class ST, class CT>
	static sl_size _String_applyBackslashEscapes(const ST& s, sl_bool flagDoubleQuote, sl_bool flagAddQuote, sl_bool flagEscapeNonAscii, CT* buf)
	{
		const CT* ch = s.getData();
		sl_size len = s.getLength();
		sl_size d;
		if (flagAddQuote) {
			d = 1;
			if (buf) {
				buf[0] = flagDoubleQuote ? '"' : '\'';
			}
		} else {
			d = 0;
		}
		sl_bool flagPrevEscaped = sl_false;
		for (sl_size i = 0; i < len; i++) {
			CT c = ch[i];
			CT r = 0;
			switch (c) {
				case '\\':
					r = c;
					break;
				case '"':
					if (flagDoubleQuote) {
						r = c;
					}
					break;
				case '\'':
					if (! flagDoubleQuote) {
						r = c;
					}
					break;
				case 0:
					r = '0';
					break;
				case '\n':
					r = 'n';
					break;
				case '\r':
					r = 'r';
					break;
				case '\b':
					r = 'b';
					break;
				case '\f':
					r = 'f';
					break;
				case '\a':
					r = 'a';
					break;
				case '\v':
					r = 'v';
					break;
			}
			if (r) {
				if (buf) {
					buf[d++] = '\\';
					buf[d++] = r;
				} else {
					d += 2;
				}
			} else {
				if (flagEscapeNonAscii && ((sl_uint8)c < 32 || (sl_uint8)c > 126)) {
					if (sizeof(CT) == 1) {
						if (buf) {
							sl_uint8 t = (sl_uint8)c;
							buf[d++] = '\\';
							buf[d++] = 'x';
							buf[d++] = _StringConv_radixPatternLower[(t >> 4) & 15];
							buf[d++] = _StringConv_radixPatternLower[t & 15];
						} else {
							d += 4;
						}
					} else {
						if (buf) {
							sl_uint16 t = (sl_uint16)c;
							buf[d++] = '\\';
							buf[d++] = 'x';
							buf[d++] = _StringConv_radixPatternLower[(t >> 12) & 15];
							buf[d++] = _StringConv_radixPatternLower[(t >> 8) & 15];
							buf[d++] = _StringConv_radixPatternLower[(t >> 4) & 15];
							buf[d++] = _StringConv_radixPatternLower[t & 15];
						} else {
							d += 6;
						}
					}
					flagPrevEscaped = sl_true;
				} else {
					if (flagPrevEscaped) {
						if (SLIB_CHAR_IS_HEX(c)) {
							if (buf) {
								CT t = flagDoubleQuote ? '"' : '\'';
								buf[d++] = t;
								buf[d++] = t;
							} else {
								d += 2;
							}
						}
					}
					if (buf) {
						buf[d++] = c;
					} else {
						d++;
					}
					flagPrevEscaped = sl_false;
				}
			}
		}
		if (flagAddQuote) {
			if (buf) {
				buf[d++] = flagDoubleQuote ? '"' : '\'';
			} else {
				d++;
			}
		}
		return d;
	}

	String ParseUtil::applyBackslashEscapes(const String& str, sl_bool flagDoubleQuote, sl_bool flagAddQuote, sl_bool flagEscapeNonAscii)
	{
		sl_size n = _String_applyBackslashEscapes<String, sl_char8>(str, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii, sl_null);
		if (n == 0) {
			return String::getEmpty();
		}
		String ret = String::allocate(n);
		if (ret.isEmpty()) {
			return sl_null;
		}
		_String_applyBackslashEscapes<String, sl_char8>(str, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii, ret.getData());
		return ret;
	}

	String16 ParseUtil::applyBackslashEscapes(const String16& str, sl_bool flagDoubleQuote, sl_bool flagAddQuote, sl_bool flagEscapeNonAscii)
	{
		sl_size n = _String_applyBackslashEscapes<String16, sl_char16>(str, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii, sl_null);
		String16 ret = String16::allocate(n);
		if (ret.isEmpty()) {
			return sl_null;
		}
		_String_applyBackslashEscapes<String16, sl_char16>(str, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii, ret.getData());
		return ret;
	}

	String ParseUtil::applyBackslashEscapes(const AtomicString& str, sl_bool flagDoubleQuote, sl_bool flagAddQuote, sl_bool flagEscapeNonAscii)
	{
		String s(str);
		return applyBackslashEscapes(s, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii);
	}

	String16 ParseUtil::applyBackslashEscapes(const AtomicString16& str, sl_bool flagDoubleQuote, sl_bool flagAddQuote, sl_bool flagEscapeNonAscii)
	{
		String16 s(str);
		return applyBackslashEscapes(s, flagDoubleQuote, flagAddQuote, flagEscapeNonAscii);
	}

	template <class ST, class CT>
	SLIB_INLINE ST _String_parseBackslashEscapes(const CT* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
	{
		if (lengthParsed) {
			*lengthParsed = 0;
		}
		if (outFlagError) {
			*outFlagError = sl_true;
		}
		if (n <= 0) {
			return sl_null;
		}
		CT chEnd = 0;
		if (sz[0] == '"') {
			chEnd = '"';
		} else if (sz[0] == '\'') {
			chEnd = '\'';
		} else {
			return sl_null;
		}
		SLIB_SCOPED_BUFFER(CT, 2048, buf, n);
		if (buf == sl_null) {
			return sl_null;
		}
		sl_size len = 0;
		sl_bool flagSuccess = sl_false;
		sl_size i = 1;
		for (; i < n; i++) {
			CT ch = sz[i];
			sl_bool flagError = sl_false;
			sl_bool flagBackslash = sl_false;
			switch (ch) {
				case '\\':
					flagBackslash = sl_true;
					i++;
					if (i < n) {
						ch = sz[i];
						switch (ch) {
							case '\\':
							case '"':
							case '\'':
								break;
							case 'n':
								ch = '\n';
								break;
							case 'r':
								ch = '\r';
								break;
							case 't':
								ch = '\t';
								break;
							case 'b':
								ch = '\b';
								break;
							case 'f':
								ch = '\f';
								break;
							case 'a':
								ch = '\a';
								break;
							case '0': case '1': case '2': case '3':
							case '4': case '5': case '6': case '7':
							{
								i++;
								sl_size nh = 2;
								sl_uint32 t = ch - '0';
								while (i < n && nh > 0) {
									ch = sz[i];
									if (ch >= '0' && ch < '8') {
										t = (t << 3) | (ch - '0');
										i++;
									} else {
										break;
									}
								}
								i--;
								ch = (CT)t;
								break;
							}
							case 'x':
							{
								i++;
								sl_uint32 h = SLIB_CHAR_HEX_TO_INT(sz[i]);
								if (h < 16) {
									i++;
									sl_uint32 t = h;
									sl_size nh;
									if (sizeof(CT) == 1) {
										nh = 1;
									} else {
										nh = 3;
									}
									while (i < n && nh > 0) {
										ch = sz[i];
										h = SLIB_CHAR_HEX_TO_INT(ch);
										if (h < 16) {
											t = (t << 4) | h;
											i++;
										} else {
											break;
										}
									}
								} else {
									flagError = sl_true;
								}
								i--;
								break;
							}
							case 'u':
							{
								if (i + 4 < n) {
									i++;
									sl_uint16 t = 0;
									for (int k = 0; k < 4; k++) {
										sl_uint16 h = SLIB_CHAR_HEX_TO_INT(ch);
										if (h < 16) {
											t = (t << 4) | h;
											i++;
										} else {
											flagError = sl_true;
											break;
										}
									}
									if (!flagError) {
										if (sizeof(CT) == 1) {
											sl_char8 u[3];
											sl_size nu = Charsets::utf16ToUtf8((sl_char16*)&t, 1, u, 3);
											if (nu > 0) {
												for (sl_size iu = 0; iu < nu - 1; iu++) {
													buf[len++] = (CT)(u[iu]);
												}
												ch = (CT)(u[nu - 1]);
											} else {
												flagError = sl_true;
											}
										} else {
											ch = (CT)t;
										}
									}
									i--;
								} else {
									flagError = sl_true;
								}
								break;
							}
							case 'U':
							{
								if (i + 8 < n) {
									i++;
									sl_uint32 t = 0;
									for (int k = 0; k < 4; k++) {
										sl_uint32 h = SLIB_CHAR_HEX_TO_INT(ch);
										if (h < 16) {
											t = (t << 4) | h;
											i++;
										} else {
											flagError = sl_true;
											break;
										}
									}
									if (!flagError) {
										if (sizeof(CT) == 1) {
											sl_char8 u[6];
											sl_char32 _t = t;
											sl_size nu = Charsets::utf32ToUtf8(&_t, 1, u, 6);
											if (nu > 0) {
												for (sl_size iu = 0; iu < nu - 1; iu++) {
													buf[len++] = (CT)(u[iu]);
												}
												ch = (CT)(u[nu - 1]);
											} else {
												flagError = sl_true;
											}
										} else {
											sl_char16 u[2];
											sl_char32 _t = t;
											sl_size nu = Charsets::utf32ToUtf16(&_t, 1, u, 2);
											if (nu > 0) {
												for (sl_size iu = 0; iu < nu - 1; iu++) {
													buf[len++] = (CT)(u[iu]);
												}
												ch = (CT)(u[nu - 1]);
											} else {
												flagError = sl_true;
											}
										}
									}
									i--;
								} else {
									flagError = sl_true;
								}
								break;
							}
							default:
								flagError = sl_true;
								break;
						}
					} else {
						flagError = sl_true;
					}
					break;
				case 0:
				case '\r':
				case '\n':
				case '\v':
					flagError = sl_true;
					break;
			}
			if (flagError) {
				break;
			} else {
				if (ch == chEnd && ! flagBackslash) {
					flagSuccess = sl_true;
					i++;
					break;
				} else {
					buf[len++] = ch;
				}
			}
		}
		if (lengthParsed) {
			*lengthParsed = i;
		}
		ST ret;
		if (flagSuccess) {
			ret = ST(buf, len);
			if (outFlagError) {
				*outFlagError = sl_false;
			}
		}
		return ret;
	}

	String ParseUtil::parseBackslashEscapes(const sl_char8* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
	{
		return _String_parseBackslashEscapes<String, sl_char8>(sz, n, lengthParsed, outFlagError);
	}

	String16 ParseUtil::parseBackslashEscapes(const sl_char16* sz, sl_size n, sl_size* lengthParsed, sl_bool* outFlagError)
	{
		return _String_parseBackslashEscapes<String16, sl_char16>(sz, n, lengthParsed, outFlagError);
	}

	String ParseUtil::parseBackslashEscapes(const String& str, sl_size* lengthParsed, sl_bool* flagError)
	{
		return parseBackslashEscapes(str.getData(), str.getLength(), lengthParsed, flagError);
	}

	String16 ParseUtil::parseBackslashEscapes(const String16& str, sl_size* lengthParsed, sl_bool* flagError)
	{
		return parseBackslashEscapes(str.getData(), str.getLength(), lengthParsed, flagError);
	}

	String ParseUtil::parseBackslashEscapes(const AtomicString& str, sl_size* lengthParsed, sl_bool* flagError)
	{
		String s(str);
		return parseBackslashEscapes(s.getData(), s.getLength(), lengthParsed, flagError);
	}

	String16 ParseUtil::parseBackslashEscapes(const AtomicString16& str, sl_size* lengthParsed, sl_bool* flagError)
	{
		String16 s(str);
		return parseBackslashEscapes(s.getData(), s.getLength(), lengthParsed, flagError);
	}

	template <class CT>
	SLIB_INLINE sl_size _String_countLineNumber(const CT* input, sl_size len, sl_size* columnLast)
	{
		sl_size line = 1;
		sl_size col = 1;
		for (sl_size i = 0; i < len; i++) {
			CT ch = input[i];
			if (ch == '\r') {
				line++;
				col = 0;
				if (i + 1 < len && input[i+1] == '\n') {
					i++;
				}
			} else if (ch == '\n') {
				line++;
				col = 0;
			}
			col++;
		}
		if (columnLast) {
			*columnLast = col;
		}
		return line;
	}

	sl_size ParseUtil::countLineNumber(const sl_char8* input, sl_size len, sl_size* columnLast)
	{
		return _String_countLineNumber(input, len, columnLast);
	}

	sl_size ParseUtil::countLineNumber(const sl_char16* input, sl_size len, sl_size* columnLast)
	{
		return _String_countLineNumber(input, len, columnLast);
	}

	sl_size ParseUtil::countLineNumber(const String& str, sl_size pos, sl_size* column)
	{
		return countLineNumber(str.getData(), Math::min(str.getLength(), pos), column);
	}

	sl_size ParseUtil::countLineNumber(const String16& str, sl_size pos, sl_size* column)
	{
		return countLineNumber(str.getData(), Math::min(str.getLength(), pos), column);
	}

	sl_size ParseUtil::countLineNumber(const AtomicString& str, sl_size pos, sl_size* column)
	{
		String s(str);
		return countLineNumber(s, pos, column);
	}

	sl_size ParseUtil::countLineNumber(const AtomicString16& str, sl_size pos, sl_size* column)
	{
		String16 s(str);
		return countLineNumber(s, pos, column);
	}
	
	
	template <class CT>
	SLIB_INLINE sl_reg _ParseUtil_indexOfLine(const CT* input, sl_reg len)
	{
		for (sl_reg i = 0; i < len; i++) {
			CT ch = input[i];
			if (ch == '\r' || ch == '\n') {
				return i;
			}
		}
		return -1;
	}
	
	sl_reg ParseUtil::indexOfLine(const sl_char8* input, sl_size len)
	{
		return _ParseUtil_indexOfLine(input, len);
	}
	
	sl_reg ParseUtil::indexOfLine(const sl_char16* input, sl_size len)
	{
		return _ParseUtil_indexOfLine(input, len);
	}
	
	template <class CT, class ST>
	SLIB_INLINE sl_reg _String_indexOfLine(const ST& str, sl_reg start)
	{
		sl_reg count = str.getLength();
		if (count <= 0) {
			return -1;
		}
		if (start < 0) {
			start = 0;
		} else {
			if (start >= count) {
				return -1;
			}
		}
		CT* sz = str.getData();
		for (sl_reg i = start; i < count; i++) {
			CT ch = sz[i];
			if (ch == '\r' || ch == '\n') {
				return i;
			}
		}
		return -1;
	}
	
	sl_reg ParseUtil::indexOfLine(const String& str, sl_reg start)
	{
		return _String_indexOfLine<sl_char8, String>(str, start);
	}
	
	sl_reg ParseUtil::indexOfLine(const String16& str, sl_reg start)
	{
		return _String_indexOfLine<sl_char16, String16>(str, start);
	}
	
	sl_reg ParseUtil::indexOfLine(const AtomicString& str, sl_reg start)
	{
		return _String_indexOfLine<sl_char8, String>(str, start);
	}
	
	sl_reg ParseUtil::indexOfLine(const AtomicString16& str, sl_reg start)
	{
		return _String_indexOfLine<sl_char16, String16>(str, start);
	}

}
