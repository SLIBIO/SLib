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

#include "util.h"

#include <slib/core.h>
#include <slib/crypto.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	auto& c = EllipticCurve::secp256k1();
	sl_size n = c.p.getMostSignificantBits();
	ECPoint pt = c.G;
	for (sl_size i = 1; i < n; i++) {
		pt = c.doublePoint(pt);
		Println("// (2^%d)*G", i);
		Println("{");
		{
			Printf("\tconst static sl_uint8 _x[] = {");
			Memory x = pt.x.getBytesBE();
			sl_uint8* buf = (sl_uint8*)(x.getData());
			sl_size m = x.getSize();
			for (sl_size k = 0; k < m; k++) {
				if (k) {
					Printf(",0x%02X", buf[k]);
				} else {
					Printf("0x%02X", buf[k]);
				}
			}
			Println("};");
			Println("\tpt.x = BigInt::fromBytesBE(_x, sizeof(_x));");
		}
		{
			Printf("\tconst static sl_uint8 _y[] = {");
			Memory y = pt.y.getBytesBE();
			sl_uint8* buf = (sl_uint8*)(y.getData());
			sl_size m = y.getSize();
			for (sl_size k = 0; k < m; k++) {
				if (k) {
					Printf(",0x%02X", buf[k]);
				} else {
					Printf("0x%02X", buf[k]);
				}
			}
			Println("};");
			Println("\tpt.y = BigInt::fromBytesBE(_y, sizeof(_y));");
		}
		Println("\tpow2g.add_NoLock(pt);");
		Println("}");
	}
	return 0;
}
