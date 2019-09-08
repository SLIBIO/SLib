#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	// Read emoji list from google noto emoji fonts
	// https://github.com/googlefonts/noto-emoji
	
	String dirSrc = "PATH TO GOOGLE NOTO EMOJI REPO";
	String dirDst = "PATH TO SLIB REPO";

	String pathSrc = dirSrc + "/png/128";
	String pathDst = dirDst + "/src/slib/graphics";
	
	StringBuffer sbEmoji;
	StringBuffer sbEmojiPng;
	StringBuffer sbEmojiPngSize;
	StringBuffer sbEmojiPngData[20];
	sl_uint32 nDataFile = CountOfArray(sbEmojiPngData);
	
	sbEmoji.add("unsigned long list[] = {");
	sbEmojiPng.add("const void* png[] = {");
	sbEmojiPngSize.add("unsigned long png_size[] = {");

	// file format: emoji_uXXXXX.png, emoji_uXXXX.png
	sl_bool flagFirst = sl_true;
	sl_uint32 indexData = 0;
	ListElements<String> files(File::getFiles(pathSrc));
	for (sl_size i = 0; i < files.count; i++) {
		String fileName = files[i];
		if (fileName.startsWith("emoji_u") && fileName.endsWith(".png")) {
			String name = fileName.substring(7, fileName.getLength() - 4);
			if (name.getLength() == 4 || name.getLength() == 5) {
				sl_uint32 code;
				if (name.parseUint32(16, &code)) {
					Memory mem = File::readAllBytes(pathSrc + "/" + fileName);
					if (mem.isNotNull()) {
						Println("%s", fileName);
						sl_uint8* data = (sl_uint8*)(mem.getData());
						sl_size size = mem.getSize();
						if (!flagFirst) {
							sbEmoji.add(", ");
							sbEmojiPng.add(", ");
							sbEmojiPngSize.add(", ");
						}
						flagFirst = sl_false;
						sbEmoji.add(String::format("0x%x", code));
						sbEmojiPng.add(String::format("png_data_%x", code));
						sbEmojiPngSize.add(String::format("%d", size));
						sbEmojiPngData[indexData].add(String::format("unsigned char png_data_%x[] = {", code));
						sl_bool flagFirstData = sl_true;
						for (sl_size i = 0; i < size; i++) {
							if (!flagFirstData) {
								sbEmojiPngData[indexData].add(",");
							}
							flagFirstData = sl_false;
							sbEmojiPngData[indexData].add(String::format("%d", data[i]));
						}
						sbEmojiPngData[indexData].add("};\r\n");
						indexData = (indexData + 1) % nDataFile;
					}
				}
			}
		}
	}
	
	sbEmoji.add("};\r\n");
	sbEmojiPng.add("};\r\n");
	sbEmojiPngSize.add("};\r\n");
	
	File::writeAllTextUTF8(pathDst + "/emoji.inc", sbEmoji.merge());
	File::writeAllTextUTF8(pathDst + "/emoji_png.inc", sbEmojiPng.merge() + sbEmojiPngSize.merge());
	for (sl_uint32 i = 0; i < nDataFile; i++) {
		File::writeAllTextUTF8(String::format("%s/emoji_png_data%d.inc", pathDst, i), sbEmojiPngData[i].merge());
	}
	
	return 0;
}
