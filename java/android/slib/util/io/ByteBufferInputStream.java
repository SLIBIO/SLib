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

package slib.util.io;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class ByteBufferInputStream extends InputStream {
    
    protected ByteBuffer buf;
    protected int pos;
    protected int mark;
    protected int count;
    
    public ByteBufferInputStream(ByteBuffer buf) {
        this.mark = 0;
        this.buf = buf;
        this.count = buf.capacity();
    }

    public ByteBufferInputStream(ByteBuffer buf, int offset, int length) {
        this.buf = buf;
        pos = offset;
        mark = offset;
        int capacity = buf.capacity();
        count = offset + length > capacity ? capacity : offset + length;
    }
    
    public synchronized int available() {
        return count - pos;
    }
    
    @Override
    public void close() throws IOException {
    }
    
    @Override
    public synchronized void mark(int readlimit) {
        mark = pos;
    }

    @Override
    public boolean markSupported() {
        return true;
    }

    @Override
    public synchronized int read() {
        return (pos++) < count ? buf.get() & 0xFF : -1;
    }
    
    @Override
    public synchronized int read(byte[] buffer, int offset, int length) {
        // Are there any bytes available?
        if (this.pos >= this.count) {
            return -1;
        }
        if (length == 0) {
            return 0;
        }
        int copylen = this.count - pos < length ? this.count - pos : length;
        this.buf.get(buffer, offset, copylen);
        pos += copylen;
        return copylen;
    }
    
    @Override
    public synchronized void reset() {
        pos = mark;
    }
    
    @Override
    public synchronized long skip(long byteCount) {
        if (byteCount <= 0) {
            return 0;
        }
        int temp = pos;
        pos = this.count - pos < byteCount ? this.count : (int) (pos + byteCount);
        buf.position(pos);
        return pos - temp;
    }
}
