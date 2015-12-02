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
