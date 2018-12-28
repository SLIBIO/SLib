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

#include "slib/crypto/aes.h"

#include "slib/crypto/sha2.h"
#include "slib/core/mio.h"

/*
	AES - Advanced Encryption Standard

	http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
*/

namespace slib
{

	AES::AES()
	{
	}

	AES::~AES()
	{
	}

	sl_uint32 AES::getBlockSize()
	{
		return BlockSize;
	}

#define TO_BYTE(x) ((sl_uint8)(x))

	// S-Box: substitution values for the byte xy
	static const sl_uint8 _priv_AES_SBOX[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};
#define SBOX(x) ((sl_uint32)(_priv_AES_SBOX[x]))

	// Inverse S-Box: inverse substitution values for the byte xy
	static const sl_uint8 _priv_AES_SBOX_INV[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
	};
#define SBOX_INV(x) ((sl_uint32)(_priv_AES_SBOX_INV[x]))

	static const sl_uint8 _priv_AES_RCON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
#define RCON(x) (((sl_uint32)(_priv_AES_RCON[x])) << 24)


/*
	Transfer Tables for Encryption/Decryption Rounds

		By Christophe Devine
*/
#define FT \
	AES_C32(C6, 63, 63, A5), AES_C32(F8, 7C, 7C, 84), AES_C32(EE, 77, 77, 99), AES_C32(F6, 7B, 7B, 8D), \
	AES_C32(FF, F2, F2, 0D), AES_C32(D6, 6B, 6B, BD), AES_C32(DE, 6F, 6F, B1), AES_C32(91, C5, C5, 54), \
	AES_C32(60, 30, 30, 50), AES_C32(02, 01, 01, 03), AES_C32(CE, 67, 67, A9), AES_C32(56, 2B, 2B, 7D), \
	AES_C32(E7, FE, FE, 19), AES_C32(B5, D7, D7, 62), AES_C32(4D, AB, AB, E6), AES_C32(EC, 76, 76, 9A), \
	AES_C32(8F, CA, CA, 45), AES_C32(1F, 82, 82, 9D), AES_C32(89, C9, C9, 40), AES_C32(FA, 7D, 7D, 87), \
	AES_C32(EF, FA, FA, 15), AES_C32(B2, 59, 59, EB), AES_C32(8E, 47, 47, C9), AES_C32(FB, F0, F0, 0B), \
	AES_C32(41, AD, AD, EC), AES_C32(B3, D4, D4, 67), AES_C32(5F, A2, A2, FD), AES_C32(45, AF, AF, EA), \
	AES_C32(23, 9C, 9C, BF), AES_C32(53, A4, A4, F7), AES_C32(E4, 72, 72, 96), AES_C32(9B, C0, C0, 5B), \
	AES_C32(75, B7, B7, C2), AES_C32(E1, FD, FD, 1C), AES_C32(3D, 93, 93, AE), AES_C32(4C, 26, 26, 6A), \
	AES_C32(6C, 36, 36, 5A), AES_C32(7E, 3F, 3F, 41), AES_C32(F5, F7, F7, 02), AES_C32(83, CC, CC, 4F), \
	AES_C32(68, 34, 34, 5C), AES_C32(51, A5, A5, F4), AES_C32(D1, E5, E5, 34), AES_C32(F9, F1, F1, 08), \
	AES_C32(E2, 71, 71, 93), AES_C32(AB, D8, D8, 73), AES_C32(62, 31, 31, 53), AES_C32(2A, 15, 15, 3F), \
	AES_C32(08, 04, 04, 0C), AES_C32(95, C7, C7, 52), AES_C32(46, 23, 23, 65), AES_C32(9D, C3, C3, 5E), \
	AES_C32(30, 18, 18, 28), AES_C32(37, 96, 96, A1), AES_C32(0A, 05, 05, 0F), AES_C32(2F, 9A, 9A, B5), \
	AES_C32(0E, 07, 07, 09), AES_C32(24, 12, 12, 36), AES_C32(1B, 80, 80, 9B), AES_C32(DF, E2, E2, 3D), \
	AES_C32(CD, EB, EB, 26), AES_C32(4E, 27, 27, 69), AES_C32(7F, B2, B2, CD), AES_C32(EA, 75, 75, 9F), \
	AES_C32(12, 09, 09, 1B), AES_C32(1D, 83, 83, 9E), AES_C32(58, 2C, 2C, 74), AES_C32(34, 1A, 1A, 2E), \
	AES_C32(36, 1B, 1B, 2D), AES_C32(DC, 6E, 6E, B2), AES_C32(B4, 5A, 5A, EE), AES_C32(5B, A0, A0, FB), \
	AES_C32(A4, 52, 52, F6), AES_C32(76, 3B, 3B, 4D), AES_C32(B7, D6, D6, 61), AES_C32(7D, B3, B3, CE), \
	AES_C32(52, 29, 29, 7B), AES_C32(DD, E3, E3, 3E), AES_C32(5E, 2F, 2F, 71), AES_C32(13, 84, 84, 97), \
	AES_C32(A6, 53, 53, F5), AES_C32(B9, D1, D1, 68), AES_C32(00, 00, 00, 00), AES_C32(C1, ED, ED, 2C), \
	AES_C32(40, 20, 20, 60), AES_C32(E3, FC, FC, 1F), AES_C32(79, B1, B1, C8), AES_C32(B6, 5B, 5B, ED), \
	AES_C32(D4, 6A, 6A, BE), AES_C32(8D, CB, CB, 46), AES_C32(67, BE, BE, D9), AES_C32(72, 39, 39, 4B), \
	AES_C32(94, 4A, 4A, DE), AES_C32(98, 4C, 4C, D4), AES_C32(B0, 58, 58, E8), AES_C32(85, CF, CF, 4A), \
	AES_C32(BB, D0, D0, 6B), AES_C32(C5, EF, EF, 2A), AES_C32(4F, AA, AA, E5), AES_C32(ED, FB, FB, 16), \
	AES_C32(86, 43, 43, C5), AES_C32(9A, 4D, 4D, D7), AES_C32(66, 33, 33, 55), AES_C32(11, 85, 85, 94), \
	AES_C32(8A, 45, 45, CF), AES_C32(E9, F9, F9, 10), AES_C32(04, 02, 02, 06), AES_C32(FE, 7F, 7F, 81), \
	AES_C32(A0, 50, 50, F0), AES_C32(78, 3C, 3C, 44), AES_C32(25, 9F, 9F, BA), AES_C32(4B, A8, A8, E3), \
	AES_C32(A2, 51, 51, F3), AES_C32(5D, A3, A3, FE), AES_C32(80, 40, 40, C0), AES_C32(05, 8F, 8F, 8A), \
	AES_C32(3F, 92, 92, AD), AES_C32(21, 9D, 9D, BC), AES_C32(70, 38, 38, 48), AES_C32(F1, F5, F5, 04), \
	AES_C32(63, BC, BC, DF), AES_C32(77, B6, B6, C1), AES_C32(AF, DA, DA, 75), AES_C32(42, 21, 21, 63), \
	AES_C32(20, 10, 10, 30), AES_C32(E5, FF, FF, 1A), AES_C32(FD, F3, F3, 0E), AES_C32(BF, D2, D2, 6D), \
	AES_C32(81, CD, CD, 4C), AES_C32(18, 0C, 0C, 14), AES_C32(26, 13, 13, 35), AES_C32(C3, EC, EC, 2F), \
	AES_C32(BE, 5F, 5F, E1), AES_C32(35, 97, 97, A2), AES_C32(88, 44, 44, CC), AES_C32(2E, 17, 17, 39), \
	AES_C32(93, C4, C4, 57), AES_C32(55, A7, A7, F2), AES_C32(FC, 7E, 7E, 82), AES_C32(7A, 3D, 3D, 47), \
	AES_C32(C8, 64, 64, AC), AES_C32(BA, 5D, 5D, E7), AES_C32(32, 19, 19, 2B), AES_C32(E6, 73, 73, 95), \
	AES_C32(C0, 60, 60, A0), AES_C32(19, 81, 81, 98), AES_C32(9E, 4F, 4F, D1), AES_C32(A3, DC, DC, 7F), \
	AES_C32(44, 22, 22, 66), AES_C32(54, 2A, 2A, 7E), AES_C32(3B, 90, 90, AB), AES_C32(0B, 88, 88, 83), \
	AES_C32(8C, 46, 46, CA), AES_C32(C7, EE, EE, 29), AES_C32(6B, B8, B8, D3), AES_C32(28, 14, 14, 3C), \
	AES_C32(A7, DE, DE, 79), AES_C32(BC, 5E, 5E, E2), AES_C32(16, 0B, 0B, 1D), AES_C32(AD, DB, DB, 76), \
	AES_C32(DB, E0, E0, 3B), AES_C32(64, 32, 32, 56), AES_C32(74, 3A, 3A, 4E), AES_C32(14, 0A, 0A, 1E), \
	AES_C32(92, 49, 49, DB), AES_C32(0C, 06, 06, 0A), AES_C32(48, 24, 24, 6C), AES_C32(B8, 5C, 5C, E4), \
	AES_C32(9F, C2, C2, 5D), AES_C32(BD, D3, D3, 6E), AES_C32(43, AC, AC, EF), AES_C32(C4, 62, 62, A6), \
	AES_C32(39, 91, 91, A8), AES_C32(31, 95, 95, A4), AES_C32(D3, E4, E4, 37), AES_C32(F2, 79, 79, 8B), \
	AES_C32(D5, E7, E7, 32), AES_C32(8B, C8, C8, 43), AES_C32(6E, 37, 37, 59), AES_C32(DA, 6D, 6D, B7), \
	AES_C32(01, 8D, 8D, 8C), AES_C32(B1, D5, D5, 64), AES_C32(9C, 4E, 4E, D2), AES_C32(49, A9, A9, E0), \
	AES_C32(D8, 6C, 6C, B4), AES_C32(AC, 56, 56, FA), AES_C32(F3, F4, F4, 07), AES_C32(CF, EA, EA, 25), \
	AES_C32(CA, 65, 65, AF), AES_C32(F4, 7A, 7A, 8E), AES_C32(47, AE, AE, E9), AES_C32(10, 08, 08, 18), \
	AES_C32(6F, BA, BA, D5), AES_C32(F0, 78, 78, 88), AES_C32(4A, 25, 25, 6F), AES_C32(5C, 2E, 2E, 72), \
	AES_C32(38, 1C, 1C, 24), AES_C32(57, A6, A6, F1), AES_C32(73, B4, B4, C7), AES_C32(97, C6, C6, 51), \
	AES_C32(CB, E8, E8, 23), AES_C32(A1, DD, DD, 7C), AES_C32(E8, 74, 74, 9C), AES_C32(3E, 1F, 1F, 21), \
	AES_C32(96, 4B, 4B, DD), AES_C32(61, BD, BD, DC), AES_C32(0D, 8B, 8B, 86), AES_C32(0F, 8A, 8A, 85), \
	AES_C32(E0, 70, 70, 90), AES_C32(7C, 3E, 3E, 42), AES_C32(71, B5, B5, C4), AES_C32(CC, 66, 66, AA), \
	AES_C32(90, 48, 48, D8), AES_C32(06, 03, 03, 05), AES_C32(F7, F6, F6, 01), AES_C32(1C, 0E, 0E, 12), \
	AES_C32(C2, 61, 61, A3), AES_C32(6A, 35, 35, 5F), AES_C32(AE, 57, 57, F9), AES_C32(69, B9, B9, D0), \
	AES_C32(17, 86, 86, 91), AES_C32(99, C1, C1, 58), AES_C32(3A, 1D, 1D, 27), AES_C32(27, 9E, 9E, B9), \
	AES_C32(D9, E1, E1, 38), AES_C32(EB, F8, F8, 13), AES_C32(2B, 98, 98, B3), AES_C32(22, 11, 11, 33), \
	AES_C32(D2, 69, 69, BB), AES_C32(A9, D9, D9, 70), AES_C32(07, 8E, 8E, 89), AES_C32(33, 94, 94, A7), \
	AES_C32(2D, 9B, 9B, B6), AES_C32(3C, 1E, 1E, 22), AES_C32(15, 87, 87, 92), AES_C32(C9, E9, E9, 20), \
	AES_C32(87, CE, CE, 49), AES_C32(AA, 55, 55, FF), AES_C32(50, 28, 28, 78), AES_C32(A5, DF, DF, 7A), \
	AES_C32(03, 8C, 8C, 8F), AES_C32(59, A1, A1, F8), AES_C32(09, 89, 89, 80), AES_C32(1A, 0D, 0D, 17), \
	AES_C32(65, BF, BF, DA), AES_C32(D7, E6, E6, 31), AES_C32(84, 42, 42, C6), AES_C32(D0, 68, 68, B8), \
	AES_C32(82, 41, 41, C3), AES_C32(29, 99, 99, B0), AES_C32(5A, 2D, 2D, 77), AES_C32(1E, 0F, 0F, 11), \
	AES_C32(7B, B0, B0, CB), AES_C32(A8, 54, 54, FC), AES_C32(6D, BB, BB, D6), AES_C32(2C, 16, 16, 3A)

#define AES_C32(a,b,c,d) 0x##a##b##c##d
static const sl_uint32 _priv_AES_FT0[256] = { FT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##d##a##b##c
static const sl_uint32 _priv_AES_FT1[256] = { FT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##c##d##a##b
static const sl_uint32 _priv_AES_FT2[256] = { FT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##b##c##d##a
static const sl_uint32 _priv_AES_FT3[256] = { FT };
#undef AES_C32
#undef FT

#define RT \
	AES_C32(51, F4, A7, 50), AES_C32(7E, 41, 65, 53), AES_C32(1A, 17, A4, C3), AES_C32(3A, 27, 5E, 96), \
	AES_C32(3B, AB, 6B, CB), AES_C32(1F, 9D, 45, F1), AES_C32(AC, FA, 58, AB), AES_C32(4B, E3, 03, 93), \
	AES_C32(20, 30, FA, 55), AES_C32(AD, 76, 6D, F6), AES_C32(88, CC, 76, 91), AES_C32(F5, 02, 4C, 25), \
	AES_C32(4F, E5, D7, FC), AES_C32(C5, 2A, CB, D7), AES_C32(26, 35, 44, 80), AES_C32(B5, 62, A3, 8F), \
	AES_C32(DE, B1, 5A, 49), AES_C32(25, BA, 1B, 67), AES_C32(45, EA, 0E, 98), AES_C32(5D, FE, C0, E1), \
	AES_C32(C3, 2F, 75, 02), AES_C32(81, 4C, F0, 12), AES_C32(8D, 46, 97, A3), AES_C32(6B, D3, F9, C6), \
	AES_C32(03, 8F, 5F, E7), AES_C32(15, 92, 9C, 95), AES_C32(BF, 6D, 7A, EB), AES_C32(95, 52, 59, DA), \
	AES_C32(D4, BE, 83, 2D), AES_C32(58, 74, 21, D3), AES_C32(49, E0, 69, 29), AES_C32(8E, C9, C8, 44), \
	AES_C32(75, C2, 89, 6A), AES_C32(F4, 8E, 79, 78), AES_C32(99, 58, 3E, 6B), AES_C32(27, B9, 71, DD), \
	AES_C32(BE, E1, 4F, B6), AES_C32(F0, 88, AD, 17), AES_C32(C9, 20, AC, 66), AES_C32(7D, CE, 3A, B4), \
	AES_C32(63, DF, 4A, 18), AES_C32(E5, 1A, 31, 82), AES_C32(97, 51, 33, 60), AES_C32(62, 53, 7F, 45), \
	AES_C32(B1, 64, 77, E0), AES_C32(BB, 6B, AE, 84), AES_C32(FE, 81, A0, 1C), AES_C32(F9, 08, 2B, 94), \
	AES_C32(70, 48, 68, 58), AES_C32(8F, 45, FD, 19), AES_C32(94, DE, 6C, 87), AES_C32(52, 7B, F8, B7), \
	AES_C32(AB, 73, D3, 23), AES_C32(72, 4B, 02, E2), AES_C32(E3, 1F, 8F, 57), AES_C32(66, 55, AB, 2A), \
	AES_C32(B2, EB, 28, 07), AES_C32(2F, B5, C2, 03), AES_C32(86, C5, 7B, 9A), AES_C32(D3, 37, 08, A5), \
	AES_C32(30, 28, 87, F2), AES_C32(23, BF, A5, B2), AES_C32(02, 03, 6A, BA), AES_C32(ED, 16, 82, 5C), \
	AES_C32(8A, CF, 1C, 2B), AES_C32(A7, 79, B4, 92), AES_C32(F3, 07, F2, F0), AES_C32(4E, 69, E2, A1), \
	AES_C32(65, DA, F4, CD), AES_C32(06, 05, BE, D5), AES_C32(D1, 34, 62, 1F), AES_C32(C4, A6, FE, 8A), \
	AES_C32(34, 2E, 53, 9D), AES_C32(A2, F3, 55, A0), AES_C32(05, 8A, E1, 32), AES_C32(A4, F6, EB, 75), \
	AES_C32(0B, 83, EC, 39), AES_C32(40, 60, EF, AA), AES_C32(5E, 71, 9F, 06), AES_C32(BD, 6E, 10, 51), \
	AES_C32(3E, 21, 8A, F9), AES_C32(96, DD, 06, 3D), AES_C32(DD, 3E, 05, AE), AES_C32(4D, E6, BD, 46), \
	AES_C32(91, 54, 8D, B5), AES_C32(71, C4, 5D, 05), AES_C32(04, 06, D4, 6F), AES_C32(60, 50, 15, FF), \
	AES_C32(19, 98, FB, 24), AES_C32(D6, BD, E9, 97), AES_C32(89, 40, 43, CC), AES_C32(67, D9, 9E, 77), \
	AES_C32(B0, E8, 42, BD), AES_C32(07, 89, 8B, 88), AES_C32(E7, 19, 5B, 38), AES_C32(79, C8, EE, DB), \
	AES_C32(A1, 7C, 0A, 47), AES_C32(7C, 42, 0F, E9), AES_C32(F8, 84, 1E, C9), AES_C32(00, 00, 00, 00), \
	AES_C32(09, 80, 86, 83), AES_C32(32, 2B, ED, 48), AES_C32(1E, 11, 70, AC), AES_C32(6C, 5A, 72, 4E), \
	AES_C32(FD, 0E, FF, FB), AES_C32(0F, 85, 38, 56), AES_C32(3D, AE, D5, 1E), AES_C32(36, 2D, 39, 27), \
	AES_C32(0A, 0F, D9, 64), AES_C32(68, 5C, A6, 21), AES_C32(9B, 5B, 54, D1), AES_C32(24, 36, 2E, 3A), \
	AES_C32(0C, 0A, 67, B1), AES_C32(93, 57, E7, 0F), AES_C32(B4, EE, 96, D2), AES_C32(1B, 9B, 91, 9E), \
	AES_C32(80, C0, C5, 4F), AES_C32(61, DC, 20, A2), AES_C32(5A, 77, 4B, 69), AES_C32(1C, 12, 1A, 16), \
	AES_C32(E2, 93, BA, 0A), AES_C32(C0, A0, 2A, E5), AES_C32(3C, 22, E0, 43), AES_C32(12, 1B, 17, 1D), \
	AES_C32(0E, 09, 0D, 0B), AES_C32(F2, 8B, C7, AD), AES_C32(2D, B6, A8, B9), AES_C32(14, 1E, A9, C8), \
	AES_C32(57, F1, 19, 85), AES_C32(AF, 75, 07, 4C), AES_C32(EE, 99, DD, BB), AES_C32(A3, 7F, 60, FD), \
	AES_C32(F7, 01, 26, 9F), AES_C32(5C, 72, F5, BC), AES_C32(44, 66, 3B, C5), AES_C32(5B, FB, 7E, 34), \
	AES_C32(8B, 43, 29, 76), AES_C32(CB, 23, C6, DC), AES_C32(B6, ED, FC, 68), AES_C32(B8, E4, F1, 63), \
	AES_C32(D7, 31, DC, CA), AES_C32(42, 63, 85, 10), AES_C32(13, 97, 22, 40), AES_C32(84, C6, 11, 20), \
	AES_C32(85, 4A, 24, 7D), AES_C32(D2, BB, 3D, F8), AES_C32(AE, F9, 32, 11), AES_C32(C7, 29, A1, 6D), \
	AES_C32(1D, 9E, 2F, 4B), AES_C32(DC, B2, 30, F3), AES_C32(0D, 86, 52, EC), AES_C32(77, C1, E3, D0), \
	AES_C32(2B, B3, 16, 6C), AES_C32(A9, 70, B9, 99), AES_C32(11, 94, 48, FA), AES_C32(47, E9, 64, 22), \
	AES_C32(A8, FC, 8C, C4), AES_C32(A0, F0, 3F, 1A), AES_C32(56, 7D, 2C, D8), AES_C32(22, 33, 90, EF), \
	AES_C32(87, 49, 4E, C7), AES_C32(D9, 38, D1, C1), AES_C32(8C, CA, A2, FE), AES_C32(98, D4, 0B, 36), \
	AES_C32(A6, F5, 81, CF), AES_C32(A5, 7A, DE, 28), AES_C32(DA, B7, 8E, 26), AES_C32(3F, AD, BF, A4), \
	AES_C32(2C, 3A, 9D, E4), AES_C32(50, 78, 92, 0D), AES_C32(6A, 5F, CC, 9B), AES_C32(54, 7E, 46, 62), \
	AES_C32(F6, 8D, 13, C2), AES_C32(90, D8, B8, E8), AES_C32(2E, 39, F7, 5E), AES_C32(82, C3, AF, F5), \
	AES_C32(9F, 5D, 80, BE), AES_C32(69, D0, 93, 7C), AES_C32(6F, D5, 2D, A9), AES_C32(CF, 25, 12, B3), \
	AES_C32(C8, AC, 99, 3B), AES_C32(10, 18, 7D, A7), AES_C32(E8, 9C, 63, 6E), AES_C32(DB, 3B, BB, 7B), \
	AES_C32(CD, 26, 78, 09), AES_C32(6E, 59, 18, F4), AES_C32(EC, 9A, B7, 01), AES_C32(83, 4F, 9A, A8), \
	AES_C32(E6, 95, 6E, 65), AES_C32(AA, FF, E6, 7E), AES_C32(21, BC, CF, 08), AES_C32(EF, 15, E8, E6), \
	AES_C32(BA, E7, 9B, D9), AES_C32(4A, 6F, 36, CE), AES_C32(EA, 9F, 09, D4), AES_C32(29, B0, 7C, D6), \
	AES_C32(31, A4, B2, AF), AES_C32(2A, 3F, 23, 31), AES_C32(C6, A5, 94, 30), AES_C32(35, A2, 66, C0), \
	AES_C32(74, 4E, BC, 37), AES_C32(FC, 82, CA, A6), AES_C32(E0, 90, D0, B0), AES_C32(33, A7, D8, 15), \
	AES_C32(F1, 04, 98, 4A), AES_C32(41, EC, DA, F7), AES_C32(7F, CD, 50, 0E), AES_C32(17, 91, F6, 2F), \
	AES_C32(76, 4D, D6, 8D), AES_C32(43, EF, B0, 4D), AES_C32(CC, AA, 4D, 54), AES_C32(E4, 96, 04, DF), \
	AES_C32(9E, D1, B5, E3), AES_C32(4C, 6A, 88, 1B), AES_C32(C1, 2C, 1F, B8), AES_C32(46, 65, 51, 7F), \
	AES_C32(9D, 5E, EA, 04), AES_C32(01, 8C, 35, 5D), AES_C32(FA, 87, 74, 73), AES_C32(FB, 0B, 41, 2E), \
	AES_C32(B3, 67, 1D, 5A), AES_C32(92, DB, D2, 52), AES_C32(E9, 10, 56, 33), AES_C32(6D, D6, 47, 13), \
	AES_C32(9A, D7, 61, 8C), AES_C32(37, A1, 0C, 7A), AES_C32(59, F8, 14, 8E), AES_C32(EB, 13, 3C, 89), \
	AES_C32(CE, A9, 27, EE), AES_C32(B7, 61, C9, 35), AES_C32(E1, 1C, E5, ED), AES_C32(7A, 47, B1, 3C), \
	AES_C32(9C, D2, DF, 59), AES_C32(55, F2, 73, 3F), AES_C32(18, 14, CE, 79), AES_C32(73, C7, 37, BF), \
	AES_C32(53, F7, CD, EA), AES_C32(5F, FD, AA, 5B), AES_C32(DF, 3D, 6F, 14), AES_C32(78, 44, DB, 86), \
	AES_C32(CA, AF, F3, 81), AES_C32(B9, 68, C4, 3E), AES_C32(38, 24, 34, 2C), AES_C32(C2, A3, 40, 5F), \
	AES_C32(16, 1D, C3, 72), AES_C32(BC, E2, 25, 0C), AES_C32(28, 3C, 49, 8B), AES_C32(FF, 0D, 95, 41), \
	AES_C32(39, A8, 01, 71), AES_C32(08, 0C, B3, DE), AES_C32(D8, B4, E4, 9C), AES_C32(64, 56, C1, 90), \
	AES_C32(7B, CB, 84, 61), AES_C32(D5, 32, B6, 70), AES_C32(48, 6C, 5C, 74), AES_C32(D0, B8, 57, 42)

#define AES_C32(a,b,c,d) 0x##a##b##c##d
static const sl_uint32 _priv_AES_RT0[256] = { RT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##d##a##b##c
static const sl_uint32 _priv_AES_RT1[256] = { RT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##c##d##a##b
static const sl_uint32 _priv_AES_RT2[256] = { RT };
#undef AES_C32
#define AES_C32(a,b,c,d) 0x##b##c##d##a
static const sl_uint32 _priv_AES_RT3[256] = { RT };
#undef AES_C32
#undef RT

#define SBOX_INV0(x) (_priv_AES_RT0[SBOX(x)])
#define SBOX_INV1(x) (_priv_AES_RT1[SBOX(x)])
#define SBOX_INV2(x) (_priv_AES_RT2[SBOX(x)])
#define SBOX_INV3(x) (_priv_AES_RT3[SBOX(x)])
#define ROUND_KEY_INV(x) ((SBOX_INV0(TO_BYTE((x) >> 24))) ^ (SBOX_INV1(TO_BYTE((x) >> 16))) ^ (SBOX_INV2(TO_BYTE((x) >> 8))) ^ (SBOX_INV3(TO_BYTE(x))))

	sl_bool AES::setKey(const void* _key, sl_uint32 lenKey)
	{
		const sl_uint8* key = (const sl_uint8*)_key;

		sl_uint32 i, j;
		sl_uint32* W;

		// key expansion
		W = m_roundKeyEnc;

		j = lenKey >> 2;
		for (i = 0; i < j; i++) {
			W[i] = *(key++);
			W[i] <<= 8;
			W[i] |= *(key++);
			W[i] <<= 8;
			W[i] |= *(key++);
			W[i] <<= 8;
			W[i] |= *(key++);
		}
		if (lenKey == 16) {
			m_nCountRounds = 10;
			// 4 * 10 rounds / 4 keys = 10 sections
			for (i = 0; i < 10; i++) {
				// W[i] = Subword(Rotword(W[i-1])) ^ RCON[i/4] ^ W[i-4], i mod 4 = 0
				W[4] = (SBOX(TO_BYTE(W[3] >> 16)) << 24)
					^ (SBOX(TO_BYTE(W[3] >> 8)) << 16)
					^ (SBOX(TO_BYTE(W[3])) << 8)
					^ (SBOX(TO_BYTE(W[3] >> 24)))
					^ RCON(i) ^ W[0];
				// W[i] = W[i-4] ^ W[i-1], i mod 4 != 0
				W[5] = W[1] ^ W[4];
				W[6] = W[2] ^ W[5];
				W[7] = W[3] ^ W[6];
				W += 4;
			}
		} else if (lenKey == 24) {
			m_nCountRounds = 12;
			// 4 * 12 rounds / 6 keys = 8 sections
			for (i = 0; i < 8; i++) {
				// W[i] = Subword(Rotword(W[i-1])) ^ RCON[i/6] ^ W[i-6], i mod 6 = 0
				W[6] = (SBOX(TO_BYTE(W[5] >> 16)) << 24)
					^ (SBOX(TO_BYTE(W[5] >> 8)) << 16)
					^ (SBOX(TO_BYTE(W[5])) << 8)
					^ (SBOX(TO_BYTE(W[5] >> 24)))
					^ RCON(i) ^ W[0];
				// W[i] = W[i-6] ^ W[i-1], i mod 6 != 0
				W[7] = W[1] ^ W[6];
				W[8] = W[2] ^ W[7];
				W[9] = W[3] ^ W[8];
				W[10] = W[4] ^ W[9];
				W[11] = W[5] ^ W[10];
				W += 6;
			}
		} else if (lenKey == 32) {
			m_nCountRounds = 14;
			// 4 * 14 rounds / 8 keys = 7 sections
			for (i = 0; i < 7; i++) {
				// W[i] = Subword(Rotword(W[i-1])) ^ RCON[i/8] ^ W[i-8], i mod 8 = 0
				W[8] = (SBOX(TO_BYTE(W[7] >> 16)) << 24)
					^ (SBOX(TO_BYTE(W[7] >> 8)) << 16)
					^ (SBOX(TO_BYTE(W[7])) << 8)
					^ (SBOX(TO_BYTE(W[7] >> 24)))
					^ RCON(i) ^ W[0];
				// W[i] = W[i-8] ^ W[i-1], i mod 8 != 0, 4
				W[9] = W[1] ^ W[8];
				W[10] = W[2] ^ W[9];
				W[11] = W[3] ^ W[10];
				// W[i] = Subword(W[i-1]) ^ W[i-8], i mod 8 = 4
				W[12] = (SBOX(TO_BYTE(W[11] >> 24)) << 24)
					^ (SBOX(TO_BYTE(W[11] >> 16)) << 16)
					^ (SBOX(TO_BYTE(W[11] >> 8)) << 8)
					^ (SBOX(TO_BYTE(W[11])))
					^ W[4];
				// W[i] = W[i-8] ^ W[i-1], i mod 8 != 0, 4
				W[13] = W[5] ^ W[12];
				W[14] = W[6] ^ W[13];
				W[15] = W[7] ^ W[14];
				W += 8;
			}
		} else {
			return sl_false;
		}

		const sl_uint32* WE = W;
		W = m_roundKeyDec;
		
		sl_uint32 nRounds = m_nCountRounds;
		Base::copyMemory(W, WE, 32);
		W += 4;
		WE -= 4;
		for (i = 1; i < nRounds; i++) {
			for (j = 0; j < 4; j++) {
				W[j] = ROUND_KEY_INV(WE[j]);
			}
			WE -= 4;
			W += 4;
		}
		Base::copyMemory(W, WE, 32);
		return sl_true;
	}

/*
	Encryption Rounds

	n: count of rounds
	W16[0~n]: round key, each element is 16 bytes

Round0 (Pre):
	AddRoundKey(W16[0])
	
Round1 ~ Round(n-1):
	SubBytes
	ShiftRows
	MixColumns
	AddRoundKey(W16[i])

Roundn (Last):
	SubBytes
	ShiftRows
	AddRoundKey(W16[n])

*/

#define AES_ENC_ROUND(I, O) \
	O[0] = _priv_AES_FT0[TO_BYTE(I[0] >> 24)] ^ _priv_AES_FT1[TO_BYTE(I[1] >> 16)] ^ _priv_AES_FT2[TO_BYTE(I[2] >> 8)] ^ _priv_AES_FT3[TO_BYTE(I[3])] ^ W[0]; \
	O[1] = _priv_AES_FT0[TO_BYTE(I[1] >> 24)] ^ _priv_AES_FT1[TO_BYTE(I[2] >> 16)] ^ _priv_AES_FT2[TO_BYTE(I[3] >> 8)] ^ _priv_AES_FT3[TO_BYTE(I[0])] ^ W[1]; \
	O[2] = _priv_AES_FT0[TO_BYTE(I[2] >> 24)] ^ _priv_AES_FT1[TO_BYTE(I[3] >> 16)] ^ _priv_AES_FT2[TO_BYTE(I[0] >> 8)] ^ _priv_AES_FT3[TO_BYTE(I[1])] ^ W[2]; \
	O[3] = _priv_AES_FT0[TO_BYTE(I[3] >> 24)] ^ _priv_AES_FT1[TO_BYTE(I[0] >> 16)] ^ _priv_AES_FT2[TO_BYTE(I[1] >> 8)] ^ _priv_AES_FT3[TO_BYTE(I[2])] ^ W[3]; \
	W += 4;

#define AES_ENC_ROUND_FINAL(I, O) \
	O[0] = (SBOX(TO_BYTE(I[0] >> 24)) << 24) ^ (SBOX(TO_BYTE(I[1] >> 16)) << 16) ^ (SBOX(TO_BYTE(I[2] >> 8)) << 8) ^ (SBOX(TO_BYTE(I[3]))) ^ W[0]; \
	O[1] = (SBOX(TO_BYTE(I[1] >> 24)) << 24) ^ (SBOX(TO_BYTE(I[2] >> 16)) << 16) ^ (SBOX(TO_BYTE(I[3] >> 8)) << 8) ^ (SBOX(TO_BYTE(I[0]))) ^ W[1]; \
	O[2] = (SBOX(TO_BYTE(I[2] >> 24)) << 24) ^ (SBOX(TO_BYTE(I[3] >> 16)) << 16) ^ (SBOX(TO_BYTE(I[0] >> 8)) << 8) ^ (SBOX(TO_BYTE(I[1]))) ^ W[2]; \
	O[3] = (SBOX(TO_BYTE(I[3] >> 24)) << 24) ^ (SBOX(TO_BYTE(I[0] >> 16)) << 16) ^ (SBOX(TO_BYTE(I[1] >> 8)) << 8) ^ (SBOX(TO_BYTE(I[2]))) ^ W[3];

	SLIB_INLINE static void _priv_AES_encipher(const sl_uint32* W, sl_uint32 nRounds, sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3)
	{
		sl_uint32 S1[4]; // status 0
		sl_uint32 S2[4]; // status 1
		S1[0] = d0 ^ W[0];
		S1[1] = d1 ^ W[1];
		S1[2] = d2 ^ W[2];
		S1[3] = d3 ^ W[3];
		
		// round 1 ~ round n-1
		W += 4;
		AES_ENC_ROUND(S1, S2); // Round 1
		AES_ENC_ROUND(S2, S1); // Round 2
		AES_ENC_ROUND(S1, S2); // Round 3
		AES_ENC_ROUND(S2, S1); // Round 4
		AES_ENC_ROUND(S1, S2); // Round 5
		AES_ENC_ROUND(S2, S1); // Round 6
		AES_ENC_ROUND(S1, S2); // Round 7
		AES_ENC_ROUND(S2, S1); // Round 8
		AES_ENC_ROUND(S1, S2); // Round 9
		if (nRounds > 10) {
			AES_ENC_ROUND(S2, S1); // Round 10
			AES_ENC_ROUND(S1, S2); // Round 11
			if (nRounds > 12) {
				AES_ENC_ROUND(S2, S1); // Round 12
				AES_ENC_ROUND(S1, S2); // Round 13
			}
		}
		AES_ENC_ROUND_FINAL(S2, S1);
		
		d0 = S1[0];
		d1 = S1[1];
		d2 = S1[2];
		d3 = S1[3];
	}
	
	void AES::encrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const
	{
		_priv_AES_encipher(m_roundKeyEnc, m_nCountRounds, d0, d1, d2, d3);
	}
	
	void AES::encryptBlock(const void* _src, void *_dst) const
	{
		const sl_uint8* IN = (const sl_uint8*)_src;
		sl_uint8* OUT = (sl_uint8*)_dst;

		sl_uint32 d0 = MIO::readUint32BE(IN);
		sl_uint32 d1 = MIO::readUint32BE(IN + 4);
		sl_uint32 d2 = MIO::readUint32BE(IN + 8);
		sl_uint32 d3 = MIO::readUint32BE(IN + 12);
		
		_priv_AES_encipher(m_roundKeyEnc, m_nCountRounds, d0, d1, d2, d3);

		MIO::writeUint32BE(OUT, d0);
		MIO::writeUint32BE(OUT + 4, d1);
		MIO::writeUint32BE(OUT + 8, d2);
		MIO::writeUint32BE(OUT + 12, d3);
	}


/*
	Decryption Rounds

	n: count of rounds
	W16[0~n]: round key, each element is 16 bytes

Round0 (Pre):
	AddRoundKey(W16[n])
	
Round1 ~ Round(n-1):
	InvShiftRows
	InvSubBytes
	AddRoundKey(W16[n-i])
	InvMixColumns

Roundn (Last):
	InvShiftRows
	InvSubBytes
	AddRoundKey(W16[0])

*/

#define AES_DEC_ROUND(I, O) \
	O[0] = _priv_AES_RT0[TO_BYTE(I[0] >> 24)] ^ _priv_AES_RT1[TO_BYTE(I[3] >> 16)] ^ _priv_AES_RT2[TO_BYTE(I[2] >> 8)] ^ _priv_AES_RT3[TO_BYTE(I[1])] ^ W[0]; \
	O[1] = _priv_AES_RT0[TO_BYTE(I[1] >> 24)] ^ _priv_AES_RT1[TO_BYTE(I[0] >> 16)] ^ _priv_AES_RT2[TO_BYTE(I[3] >> 8)] ^ _priv_AES_RT3[TO_BYTE(I[2])] ^ W[1]; \
	O[2] = _priv_AES_RT0[TO_BYTE(I[2] >> 24)] ^ _priv_AES_RT1[TO_BYTE(I[1] >> 16)] ^ _priv_AES_RT2[TO_BYTE(I[0] >> 8)] ^ _priv_AES_RT3[TO_BYTE(I[3])] ^ W[2]; \
	O[3] = _priv_AES_RT0[TO_BYTE(I[3] >> 24)] ^ _priv_AES_RT1[TO_BYTE(I[2] >> 16)] ^ _priv_AES_RT2[TO_BYTE(I[1] >> 8)] ^ _priv_AES_RT3[TO_BYTE(I[0])] ^ W[3]; \
	W += 4;

#define AES_DEC_ROUND_FINAL(I, O) \
	O[0] = (SBOX_INV(TO_BYTE(I[0] >> 24)) << 24) ^ (SBOX_INV(TO_BYTE(I[3] >> 16)) << 16) ^ (SBOX_INV(TO_BYTE(I[2] >> 8)) << 8) ^ (SBOX_INV(TO_BYTE(I[1]))) ^ W[0]; \
	O[1] = (SBOX_INV(TO_BYTE(I[1] >> 24)) << 24) ^ (SBOX_INV(TO_BYTE(I[0] >> 16)) << 16) ^ (SBOX_INV(TO_BYTE(I[3] >> 8)) << 8) ^ (SBOX_INV(TO_BYTE(I[2]))) ^ W[1]; \
	O[2] = (SBOX_INV(TO_BYTE(I[2] >> 24)) << 24) ^ (SBOX_INV(TO_BYTE(I[1] >> 16)) << 16) ^ (SBOX_INV(TO_BYTE(I[0] >> 8)) << 8) ^ (SBOX_INV(TO_BYTE(I[3]))) ^ W[2]; \
	O[3] = (SBOX_INV(TO_BYTE(I[3] >> 24)) << 24) ^ (SBOX_INV(TO_BYTE(I[2] >> 16)) << 16) ^ (SBOX_INV(TO_BYTE(I[1] >> 8)) << 8) ^ (SBOX_INV(TO_BYTE(I[0]))) ^ W[3];
	
	
	SLIB_INLINE static void _priv_AES_decipher(const sl_uint32* W, sl_uint32 nRounds, sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3)
	{
		sl_uint32 S1[4]; // status 0
		sl_uint32 S2[4]; // status 1
		S1[0] = d0 ^ W[0];
		S1[1] = d1 ^ W[1];
		S1[2] = d2 ^ W[2];
		S1[3] = d3 ^ W[3];
		
		// round 1 ~ round n-1
		W += 4;
		AES_DEC_ROUND(S1, S2); // Round 1
		AES_DEC_ROUND(S2, S1); // Round 2
		AES_DEC_ROUND(S1, S2); // Round 3
		AES_DEC_ROUND(S2, S1); // Round 4
		AES_DEC_ROUND(S1, S2); // Round 5
		AES_DEC_ROUND(S2, S1); // Round 6
		AES_DEC_ROUND(S1, S2); // Round 7
		AES_DEC_ROUND(S2, S1); // Round 8
		AES_DEC_ROUND(S1, S2); // Round 9
		if (nRounds > 10) {
			AES_DEC_ROUND(S2, S1); // Round 10
			AES_DEC_ROUND(S1, S2); // Round 11
			if (nRounds > 12) {
				AES_DEC_ROUND(S2, S1); // Round 12
				AES_DEC_ROUND(S1, S2); // Round 13
			}
		}
		AES_DEC_ROUND_FINAL(S2, S1);

		d0 = S1[0];
		d1 = S1[1];
		d2 = S1[2];
		d3 = S1[3];
	}
	
	void AES::decrypt(sl_uint32& d0, sl_uint32& d1, sl_uint32& d2, sl_uint32& d3) const
	{
		_priv_AES_decipher(m_roundKeyDec, m_nCountRounds, d0, d1, d2, d3);
	}
	
	void AES::decryptBlock(const void* _src, void *_dst) const
	{
		const sl_uint8* IN = (const sl_uint8*)_src;
		sl_uint8* OUT = (sl_uint8*)_dst;
		
		sl_uint32 d0 = MIO::readUint32BE(IN);
		sl_uint32 d1 = MIO::readUint32BE(IN + 4);
		sl_uint32 d2 = MIO::readUint32BE(IN + 8);
		sl_uint32 d3 = MIO::readUint32BE(IN + 12);
		
		_priv_AES_decipher(m_roundKeyDec, m_nCountRounds, d0, d1, d2, d3);
		
		MIO::writeUint32BE(OUT, d0);
		MIO::writeUint32BE(OUT + 4, d1);
		MIO::writeUint32BE(OUT + 8, d2);
		MIO::writeUint32BE(OUT + 12, d3);
	}

	void AES::setKey_SHA256(const String& key)
	{
		char sig[32];
		SHA256::hash(key, sig);
		setKey(sig, 32);
	}


	AES_GCM::AES_GCM()
	{
	}

	AES_GCM::~AES_GCM()
	{
	}

	void AES_GCM::setKey(const void* key, sl_uint32 lenKey)
	{
		m_cipher.setKey(key, lenKey);
		setCipher(&m_cipher);
	}

	void AES_GCM::setKey_SHA256(const String& key)
	{
		m_cipher.setKey_SHA256(key);
		setCipher(&m_cipher);
	}

}
