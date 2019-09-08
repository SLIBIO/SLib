#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	// Read emoji list from google noto emoji fonts
	// https://github.com/googlefonts/noto-emoji
	
	String dirSrc = "PATH TO GOOGLE NOTO EMOJI REPO";
	String dirDst = "PATH TO SLIB REPO";

	String pathSrc = dirSrc + "/png/128";
	String pathDst = dirDst + "/external/src/noto_emoji";
	
	StringBuffer sbEmojiString;
	StringBuffer sbEmojiList;
	StringBuffer sbEmojiPng;
	StringBuffer sbEmojiPngSize;
	StringBuffer sbEmojiPngData[20];
	sl_uint32 nDataFile = CountOfArray(sbEmojiPngData);
	
	sbEmojiList.add("const char32_t* emojis[] = {");
	sbEmojiPng.add("const void* png[] = {");
	sbEmojiPngSize.add("unsigned int png_size[] = {");

	// file format: emoji_uXXXXX.png, emoji_uXXXX.png
	sl_bool flagFirst = sl_true;
	sl_uint32 index = 0;
	sl_uint32 indexData = 0;
	ListElements<String> files(File::getFiles(pathSrc));
	for (sl_size i = 0; i < files.count; i++) {
		String fileName = files[i];
		if (fileName.startsWith("emoji_u") && fileName.endsWith(".png")) {
			Memory mem = File::readAllBytes(pathSrc + "/" + fileName);
			if (mem.isNotNull()) {
				Ref<Image> image = Image::loadFromMemory(mem);
				if (image.isNotNull()) {
					image = image->stretchToSmall(4);
					if (image.isNotNull()) {
						mem = image->savePNG();
						if (mem.isNotNull()) {
							Println("%s", fileName);
							String name = fileName.substring(7, fileName.getLength() - 4);
							{
								ListElements<String> nameParts(name.split("_"));
								sbEmojiString.add(String::format("char32_t str_%d[] = {", index));
								for (sl_size i = 0; i < nameParts.count; i++) {
									if (i) {
										sbEmojiString.add(",");
									}
									sbEmojiString.add(String::format("0x%s", nameParts[i]));
								}
								sbEmojiString.add(",0};\r\n");
							}
							sl_uint8* data = (sl_uint8*)(mem.getData());
							sl_size size = mem.getSize();
							if (!flagFirst) {
								sbEmojiList.add(", ");
								sbEmojiPng.add(", ");
								sbEmojiPngSize.add(", ");
							}
							flagFirst = sl_false;
							sbEmojiList.add(String::format("str_%d", index));
							sbEmojiPng.add(String::format("png_data_%d", index));
							sbEmojiPngSize.add(String::format("%d", size));
							{
								sbEmojiPngData[indexData].add(String::format("unsigned char png_data_%d[] = {", index));
								for (sl_size i = 0; i < size; i++) {
									if (i) {
										sbEmojiPngData[indexData].add(",");
									}
									sbEmojiPngData[indexData].add(String::format("%d", data[i]));
								}
								sbEmojiPngData[indexData].add("};\r\n");
							}
							indexData = (indexData + 1) % nDataFile;
							index++;
						}
					}
				}
			}
		}
	}
	
	sbEmojiList.add(", 0};\r\n");
	sbEmojiPng.add("};\r\n");
	sbEmojiPngSize.add("};\r\n");
	
	File::writeAllTextUTF8(pathDst + "/emoji.inc", sbEmojiString.merge() + sbEmojiList.merge());
	File::writeAllTextUTF8(pathDst + "/emoji_png.inc", sbEmojiPng.merge() + sbEmojiPngSize.merge());
	for (sl_uint32 i = 0; i < nDataFile; i++) {
		File::writeAllTextUTF8(String::format("%s/emoji_png_data%d.inc", pathDst, i), sbEmojiPngData[i].merge());
	}
	
	return 0;
}
