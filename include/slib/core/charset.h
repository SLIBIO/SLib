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

#ifndef CHECKHEADER_SLIB_CORE_CHARSET
#define CHECKHEADER_SLIB_CORE_CHARSET

#include "definition.h"

namespace slib
{

	// Codepage constants based on Windows
	enum class Charset
	{
		Unknown = 0,
		
		ANSI = 0x10000, // Windows Only
		OEM = 1, // Windows Only
		MAC = 2, // Windows Only
		THREAD_ANSI = 3, // Windows Only
		
		SYMBOL = 42, // SYMBOL translations

		CP_037 = 37, // IBM EBCDIC US-Canada
		CP_437 = 437, // OEM United States
		CP_500 = 500, // IBM EBCDIC International
		CP_708 = 708, // Arabic (ASMO 708)
		CP_709 = 709, // Arabic (ASMO-449+, BCON V4)
		CP_710 = 710, // Arabic - Transparent Arabic
		CP_720 = 720, // Arabic (Transparent ASMO); Arabic (DOS)
		CP_737 = 737, // OEM Greek (formerly 437G); Greek (DOS)
		CP_775 = 775, // OEM Baltic; Baltic (DOS)
		CP_850 = 850, // OEM Multilingual Latin 1; Western European (DOS)
		CP_852 = 852, // OEM Latin 2; Central European (DOS)
		CP_855 = 855, // OEM Cyrillic (primarily Russian)
		CP_857 = 857, // OEM Turkish; Turkish (DOS)
		CP_858 = 858, // OEM Multilingual Latin 1 + Euro symbol
		CP_860 = 860, // OEM Portuguese; Portuguese (DOS)
		CP_861 = 861, // OEM Icelandic; Icelandic (DOS)
		CP_862 = 862, // OEM Hebrew; Hebrew (DOS)
		CP_863 = 863, // OEM French Canadian; French Canadian (DOS)
		CP_864 = 864, // OEM Arabic; Arabic (864)
		CP_865 = 865, // OEM Nordic; Nordic (DOS)
		CP_866 = 866, // OEM Russian; Cyrillic (DOS)
		CP_869 = 869, // OEM Modern Greek; Greek, Modern (DOS)
		CP_870 = 870, // IBM EBCDIC Multilingual/ROECE (Latin 2); IBM EBCDIC Multilingual Latin 2
		CP_874 = 874, // ANSI/OEM Thai (ISO 8859-11); Thai (Windows)
		CP_875 = 875, // IBM EBCDIC Greek Modern
		CP_932 = 932, // ANSI/OEM Japanese; Japanese (Shift-JIS)
		SHIFT_JIS = CP_932,
		JAPANESE_932 = CP_932,
		CP_936 = 936, // ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
		GB2312 = CP_936,
		GBK = CP_936,
		CHINESE_936 = CP_936,
		CP_949 = 949, // ANSI/OEM Korean
		KS_C_5601_1987 = CP_949,
		KOREAN_949 = CP_949,
		CP_950 = 950, // Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)
		BIG5 = CP_950,
		CHINESE_950 = CP_950,
		CP_1026	= 1026, // IBM EBCDIC Turkish (Latin 5)
		CP_1047 = 1047, // IBM EBCDIC Latin 1/Open System
		CP_1140 = 1140, // IBM EBCDIC US-Canada (037 + Euro symbol); IBM EBCDIC (US-Canada-Euro)
		CP_1141 = 1141, // IBM EBCDIC Germany (20273 + Euro symbol); IBM EBCDIC (Germany-Euro)
		CP_1142 = 1142, // IBM EBCDIC Denmark-Norway (20277 + Euro symbol); IBM EBCDIC (Denmark-Norway-Euro)
		CP_1143 = 1143, // IBM EBCDIC Finland-Sweden (20278 + Euro symbol); IBM EBCDIC (Finland-Sweden-Euro)
		CP_1144 = 1144, // IBM EBCDIC Italy (20280 + Euro symbol); IBM EBCDIC (Italy-Euro)
		CP_1145 = 1145, // IBM EBCDIC Latin America-Spain (20284 + Euro symbol); IBM EBCDIC (Spain-Euro)
		CP_1146 = 1146, // IBM EBCDIC United Kingdom (20285 + Euro symbol); IBM EBCDIC (UK-Euro)
		CP_1147 = 1147, // IBM EBCDIC France (20297 + Euro symbol); IBM EBCDIC (France-Euro)
		CP_1148 = 1148, // IBM EBCDIC International (500 + Euro symbol); IBM EBCDIC (International-Euro)
		CP_1149 = 1149, // IBM EBCDIC Icelandic (20871 + Euro symbol); IBM EBCDIC (Icelandic-Euro)
		CP_1200 = 1200, // Unicode UTF-16, little endian byte order (BMP of ISO 10646)
		UTF16LE = CP_1200,
		CP_1201 = 1201, // Unicode UTF-16, big endian byte order
		UTF16BE = CP_1201,
		CP_1250 = 1250, // ANSI Central European; Central European (Windows)
		CP_1251 = 1251, // ANSI Cyrillic; Cyrillic (Windows)
		CP_1252 = 1252, // ANSI Latin 1; Western European (Windows)
		CP_1253 = 1253, // ANSI Greek; Greek (Windows)
		CP_1254 = 1254, // ANSI Turkish; Turkish (Windows)
		CP_1255 = 1255, // ANSI Hebrew; Hebrew (Windows)
		CP_1256	= 1256, // ANSI Arabic; Arabic (Windows)
		CP_1257	= 1257, // ANSI Baltic; Baltic (Windows)
		CP_1258 = 1258, // ANSI/OEM Vietnamese; Vietnamese (Windows)
		CP_1361 = 1361, // Korean (Johab)
		JOHAB = CP_1361,
		CP_10000 = 10000, // MAC Roman; Western European (Mac)
		CP_10001 = 10001, // Japanese (Mac)
		CP_10002 = 10002, // MAC Traditional Chinese (Big5); Chinese Traditional (Mac)
		CP_10003 = 10003, // Korean (Mac)
		CP_10004 = 10004, // Arabic (Mac)
		CP_10005 = 10005, // Hebrew (Mac)
		CP_10006 = 10006, // Greek (Mac)
		CP_10007 = 10007, // Cyrillic (Mac)
		CP_10008 = 10008, // MAC Simplified Chinese (GB 2312); Chinese Simplified (Mac)
		CP_10010 = 10010, // Romanian (Mac)
		CP_10017 = 10017, // Ukrainian (Mac)
		CP_10021 = 10021, // Thai (Mac)
		CP_10029 = 10029, // MAC Latin 2; Central European (Mac)
		CP_10079 = 10079, // Icelandic (Mac)
		CP_10081 = 10081, // Turkish (Mac)
		CP_10082 = 10082, // Croatian (Mac)
		CP_12000 = 12000, // Unicode UTF-32, little endian byte order
		UTF32LE = CP_12000,
		CP_12001 = 12001, // Unicode UTF-32, big endian byte order
		UTF32BE = CP_12001,
		CP_20000 = 20000, // Chinese Traditional (CNS)
		CP_20001 = 20001, // TCA Taiwan
		CP_20002 = 20002, // Chinese Traditional (Eten)
		CP_20003 = 20003, // IBM5550 Taiwan
		CP_20004 = 20004, // TeleText Taiwan
		CP_20005 = 20005, // Wang Taiwan
		CP_20105 = 20105, // IA5 (IRV International Alphabet No. 5, 7-bit); Western European (IA5)
		CP_20106 = 20106, // IA5 German (7-bit)
		CP_20107 = 20107, // x-IA5-Swedish	IA5 Swedish (7-bit)
		CP_20108 = 20108, // x-IA5-Norwegian	IA5 Norwegian (7-bit)
		CP_20127 = 20127, // us-ascii	US-ASCII (7-bit)
		CP_20261 = 20261, // x-cp20261	T.61
		CP_20269 = 20269, // x-cp20269	ISO 6937 Non-Spacing Accent
		CP_20273 = 20273, // IBM273	IBM EBCDIC Germany
		CP_20277 = 20277, // IBM277	IBM EBCDIC Denmark-Norway
		CP_20278 = 20278, // IBM278	IBM EBCDIC Finland-Sweden
		CP_20280 = 20280, // IBM280	IBM EBCDIC Italy
		CP_20284 = 20284, // IBM284	IBM EBCDIC Latin America-Spain
		CP_20285 = 20285, // IBM285	IBM EBCDIC United Kingdom
		CP_20290 = 20290, // IBM290	IBM EBCDIC Japanese Katakana Extended
		CP_20297 = 20297, // IBM297	IBM EBCDIC France
		CP_20420 = 20420, // IBM420	IBM EBCDIC Arabic
		CP_20423 = 20423, // IBM423	IBM EBCDIC Greek
		CP_20424 = 20424, // IBM424	IBM EBCDIC Hebrew
		CP_20833 = 20833, // x-EBCDIC-KoreanExtended	IBM EBCDIC Korean Extended
		CP_20838 = 20838, // IBM-Thai	IBM EBCDIC Thai
		CP_20866 = 20866, // koi8-r	Russian (KOI8-R); Cyrillic (KOI8-R)
		CP_20871 = 20871, // IBM871	IBM EBCDIC Icelandic
		CP_20880 = 20880, // IBM880	IBM EBCDIC Cyrillic Russian
		CP_20905 = 20905, // IBM905	IBM EBCDIC Turkish
		CP_20924 = 20924, // IBM00924	IBM EBCDIC Latin 1/Open System (1047 + Euro symbol)
		CP_20932 = 20932, // EUC-JP	Japanese (JIS 0208-1990 and 0212-1990)
		CP_20936 = 20936, // Simplified Chinese (GB2312); Chinese Simplified (GB2312-80)
		CP_20949 = 20949, // Korean Wansung
		KOREAN_20949 = CP_20949,
		WANSUNG = CP_20949,
		CP_21025 = 21025, // IBM EBCDIC Cyrillic Serbian-Bulgarian
		CP_21866 = 21866, // Ukrainian (KOI8-U); Cyrillic (KOI8-U)
		CP_28591 = 28591, // ISO 8859-1 Latin 1; Western European (ISO)
		CP_28592 = 28592, // ISO 8859-2 Central European; Central European (ISO)
		CP_28593 = 28593, // ISO 8859-3 Latin 3
		CP_28594 = 28594, // ISO 8859-4 Baltic
		CP_28595 = 28595, // ISO 8859-5 Cyrillic
		CP_28596 = 28596, // ISO 8859-6 Arabic
		CP_28597 = 28597, // ISO 8859-7 Greek
		CP_28598 = 28598, // ISO 8859-8 Hebrew; Hebrew (ISO-Visual)
		CP_28599 = 28599, // ISO 8859-9 Turkish
		CP_28603 = 28603, // ISO 8859-13 Estonian
		CP_28605 = 28605, // ISO 8859-15 Latin 9
		CP_29001 = 29001, // Europa 3
		CP_38598 = 38598, // ISO 8859-8 Hebrew; Hebrew (ISO-Logical)
		CP_50220 = 50220, // ISO 2022 Japanese with no halfwidth Katakana; Japanese (JIS)
		CP_50221 = 50221, // ISO 2022 Japanese with halfwidth Katakana; Japanese (JIS-Allow 1 byte Kana)
		CP_50222 = 50222, // ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)
		CP_50225 = 50225, // ISO 2022 Korean
		CP_50227 = 50227, // ISO 2022 Simplified Chinese; Chinese Simplified (ISO 2022)
		CP_50229 = 50229, // ISO 2022 Traditional Chinese
		CP_50930 = 50930, // EBCDIC Japanese (Katakana) Extended
		CP_50931 = 50931, // EBCDIC US-Canada and Japanese
		CP_50933 = 50933, // EBCDIC Korean Extended and Korean
		CP_50935 = 50935, // EBCDIC Simplified Chinese Extended and Simplified Chinese
		CP_50936 = 50936, // EBCDIC Simplified Chinese
		CP_50937 = 50937, // EBCDIC US-Canada and Traditional Chinese
		CP_50939 = 50939, // EBCDIC Japanese (Latin) Extended and Japanese
		CP_51932 = 51932, // EUC Japanese
		CP_51936 = 51936, // EUC Simplified Chinese; Chinese Simplified (EUC)
		CP_51949 = 51949, // EUC Korean
		CP_51950 = 51950, // EUC Traditional Chinese
		CP_52936 = 52936, // HZ-GB2312 Simplified Chinese; Chinese Simplified (HZ)
		CP_54936 = 54936, // GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)
		GB18030 = CP_54936,
		CP_57002 = 57002, // ISCII Devanagari
		X_ISCII_DE = CP_57002,
		CP_57003 = 57003, // ISCII Bangla
		X_ISCII_BE = CP_57003,
		CP_57004 = 57004, // ISCII Tamil
		X_ISCII_TA = CP_57004,
		CP_57005 = 57005, // ISCII Telugu
		X_ISCII_TE = CP_57005,
		CP_57006 = 57006, // ISCII Assamese
		X_ISCII_AS = CP_57006,
		CP_57007 = 57007, // ISCII Odia
		X_ISCII_OR = CP_57007,
		CP_57008 = 57008, // ISCII Kannada
		X_ISCII_KA = CP_57008,
		CP_57009 = 57009, // ISCII Malayalam
		X_ISCII_MA = CP_57009,
		CP_57010 = 57010, // ISCII Gujarati
		X_ISCII_GU = CP_57010,
		CP_57011 = 57011, // ISCII Punjabi
		X_ISCII_PA = CP_57011,
		CP_65000 = 65000, // Unicode (UTF-7)
		UTF7 = CP_65000,
		CP_65001 = 65001, // Unicode (UTF-8)
		UTF8 = CP_65001
	};
	
	class Charsets
	{
	public:
		static sl_size utf8ToUtf16(const sl_char8* utf8, sl_reg lenUtf8, sl_char16* utf16, sl_reg lenUtf16Buffer) noexcept;

		static sl_size encode8_UTF16BE(const sl_char8* utf8, sl_reg lenUtf8, void* utf16, sl_reg sizeUtf16Buffer) noexcept;
		
		static sl_size encode8_UTF16LE(const sl_char8* utf8, sl_reg lenUtf8, void* utf16, sl_reg sizeUtf16Buffer) noexcept;
		
		static sl_size utf8ToUtf32(const sl_char8* utf8, sl_reg lenUtf8, sl_char32* utf32, sl_reg lenUtf32Buffer) noexcept;

		static sl_size encode8_UTF32BE(const sl_char8* utf8, sl_reg lenUtf8, void* utf32, sl_reg sizeUtf32Buffer) noexcept;

		static sl_size encode8_UTF32LE(const sl_char8* utf8, sl_reg lenUtf8, void* utf32, sl_reg sizeUtf32Buffer) noexcept;

		static sl_size utf16ToUtf8(const sl_char16* utf16, sl_reg lenUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;

		static sl_size decode8_UTF16BE(const void* utf16, sl_size sizeUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;

		static sl_size decode8_UTF16LE(const void* utf16, sl_size sizeUtf16, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;

		static sl_size utf32ToUtf8(const sl_char32* utf32, sl_reg lenUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;
	
		static sl_size decode8_UTF32BE(const void* utf32, sl_size sizeUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;
		
		static sl_size decode8_UTF32LE(const void* utf32, sl_size sizeUtf32, sl_char8* utf8, sl_reg lenUtf8Buffer) noexcept;
		
		static sl_size utf16ToUtf32(const sl_char16* utf16, sl_reg lenUtf16, sl_char32* utf32, sl_reg lenUtf32Buffer) noexcept;

		static sl_size encode16_UTF32BE(const sl_char16* utf16, sl_reg lenUtf16, void* utf32, sl_reg sizeUtf32Buffer) noexcept;

		static sl_size encode16_UTF32LE(const sl_char16* utf16, sl_reg lenUtf16, void* utf32, sl_reg sizeUtf32Buffer) noexcept;

		static sl_size utf32ToUtf16(const sl_char32* utf32, sl_reg lenUtf32, sl_char16* utf16, sl_reg lenUtf16Buffer) noexcept;

		static sl_size decode16_UTF32BE(const void* utf32, sl_size sizeUtf32, sl_char16* utf16, sl_reg lenUtf16Buffer) noexcept;

		static sl_size decode16_UTF32LE(const void* utf32, sl_size sizeUtf32, sl_char16* utf16, sl_reg lenUtf16Buffer) noexcept;
		
		
		static sl_size encode8(const sl_char8* utf8, sl_size lenUtf8, Charset charset, void* output, sl_reg sizeOutputBuffer);
		
		static sl_size decode8(Charset charset, const void* input, sl_size sizeInput, sl_char8* utf8, sl_reg lenUtf8Buffer);
		
		static sl_size encode16(const sl_char16* utf8, sl_size lenUtf16, Charset charset, void* output, sl_reg sizeOutputBuffer);
		
		static sl_size decode16(Charset charset, const void* input, sl_size sizeInput, sl_char16* utf16, sl_reg lenUtf16Buffer);
		
	};

}

#endif
