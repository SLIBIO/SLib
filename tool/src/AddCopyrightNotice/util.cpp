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

#include "util.h"

#include "slib/core/file.h"

using namespace slib;

void applyCopyrightNoticeToSourceFile(const String& pathFile, const String& copyrightNotice)
{
	Memory mem = File::readAllBytes(pathFile);
	if (mem.isNotNull()) {
		char* sz = (char*)(mem.getData());
		sl_size n = mem.getSize();
		sl_size posEndCopyright = 0;
		if (n > 8 && sz[0] == '/' && sz[1] == '*') {
			const char t[] = "Copyright";
			sl_size m = sizeof(t) - 1;
			sl_bool flagFoundCopyright = sl_false;
			sl_bool flagFoundEnd = sl_false;
			sl_size i = 2;
			for (; i < n; i++) {
				if (!flagFoundCopyright) {
					if (i + m <= n) {
						if (Base::compareMemory((sl_int8*)(sz + i), (sl_int8*)t, m) == 0) {
							flagFoundCopyright = sl_true;
						}
					}
				}
				if (i + 6 <= n && sz[i] == '*' && sz[i+1] == '/' && sz[i+2] == '\r' && sz[i+3] == '\n' && sz[i+4] == '\r' && sz[i+5] == '\n') {
					flagFoundEnd = sl_true;
					if (flagFoundCopyright) {
						posEndCopyright = i + 6;
					}
					break;
				} else if (i + 4 <= n && sz[i] == '*' && sz[i+1] == '/' && sz[i+2] == '\n' && sz[i+3] == '\n') {
					flagFoundEnd = sl_true;
					if (flagFoundCopyright) {
						posEndCopyright = i + 4;
					}
					break;
				}
			}
		}
		MemoryBuffer out;
		String _copyrightNotice;
		char* content = (char*)(mem.getData()) + posEndCopyright;
		sl_size lenContent = n - posEndCopyright;
		if (Base::findMemory(content, '\r', lenContent) != sl_null || Base::findMemory(content, '\n', lenContent) == sl_null) {
			if (copyrightNotice.indexOf('\r') < 0) {
				_copyrightNotice = copyrightNotice.replaceAll("\n", "\r\n");
				out.addStatic(_copyrightNotice.getData(), _copyrightNotice.getLength());
			} else {
				out.addStatic(copyrightNotice.getData(), copyrightNotice.getLength());
			}
			out.addStatic("\r\n\r\n");
		} else {
			_copyrightNotice = copyrightNotice.replaceAll("\r\n", "\n");
			out.addStatic(_copyrightNotice.getData(), _copyrightNotice.getLength());
			out.addStatic("\n\n");
		}
		out.addStatic(content, lenContent);
		mem = out.merge();
		File::writeAllBytes(pathFile, mem);
	}
}

void applyCopyrightNoticeToAllSourceFilesInPath(const String& pathDir, const String& copyrightNotice)
{
	for (auto& file : File::getAllDescendantFiles(pathDir)) {
		String name = File::getFileNameOnly(file);
		if (name.isNotEmpty() && !(name.startsWith('.'))) {
			applyCopyrightNoticeToSourceFile(pathDir + "/" + file, copyrightNotice);
		}
	}
}
