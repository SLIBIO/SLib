package slib.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
import java.nio.charset.CoderResult;

public class Charsets {

	private final static int LENGTH_TEMP_BUFFER = 2;

	public static int encode(int codepage, ByteBuffer input, ByteBuffer output) {
		int total = 0;
		try {
			Charset charset = Charset.forName("cp" + codepage);
			CharsetEncoder encoder = charset.newEncoder();
			if (encoder != null) {
				input.order(ByteOrder.nativeOrder());
				CharBuffer cinput = input.asCharBuffer();
				if (output != null) {
					encoder.encode(cinput, output, true);
					return output.position();
				} else {
					output = ByteBuffer.allocate(LENGTH_TEMP_BUFFER);
					output.mark();
					for (;;) {
						CoderResult result = encoder.encode(cinput, output, false);
						total += output.position();
						if (!(result.isOverflow())) {
							break;
						}
						output.reset();
					}
					return total;
				}
			}
		} catch (Exception ignore) {
		}
		return total;
	}

	public static int decode(int codepage, ByteBuffer input, ByteBuffer output) {
		int total = 0;
		try {
			Charset charset = Charset.forName("cp" + codepage);
			CharsetDecoder decoder = charset.newDecoder();
			if (decoder != null) {
				if (output != null) {
					output.order(ByteOrder.nativeOrder());
					CharBuffer buf = output.asCharBuffer();
					decoder.decode(input, buf, true);
					return buf.position();
				} else {
					CharBuffer buf = CharBuffer.allocate(LENGTH_TEMP_BUFFER);
					buf.mark();
					for (;;) {
						CoderResult result = decoder.decode(input, buf, false);
						total += buf.position();
						if (!(result.isOverflow())) {
							break;
						}
						buf.reset();
					}
					return total;
				}
			}
		} catch (Exception ignore) {
		}
		return total;
	}

}
