#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	String path;
	for (;;) {
		Println("Input the directory:");
		path = Console::readLine().trim();
		if (path.isNotEmpty()) {
			break;
		}
	}
	ListElements<String> files(File::getAllDescendantFiles(path));
	if (files.count == 0) {
		Println("No files found!");
		return 0;
	}

	String extensions;
	for (;;) {
		Println("Input the file extensions:");
		extensions = Console::readLine().trim();
		if (extensions.isNotEmpty()) {
			break;
		}
	}
	List<String> exts(extensions.split(" "));

	String prefix;
	for (;;) {
		Println("Input the prefix of symbols:");
		prefix = Console::readLine().trim();
		if (prefix.isNotEmpty()) {
			break;
		}
	}
	String matchBeforeSymbol;
	sl_bool flagMatchBeforeNot = sl_false;
	{
		sl_reg index = prefix.lastIndexOf(' ');
		if (index >= 0) {
			matchBeforeSymbol = prefix.substring(0, index).trim();
			prefix = prefix.substring(index + 1).trim();
			if (matchBeforeSymbol.startsWith('!')) {
				matchBeforeSymbol = matchBeforeSymbol.substring(1).trim();
				flagMatchBeforeNot = sl_true;
			}
		}
	}

	String format;
	for (;;) {
		Println("Input the format of symbol output:");
		format = Console::readLine().trim();
		if (format.isNotEmpty()) {
			break;
		}
	}

	Map<String, sl_bool> symbols;
	Map<String, sl_bool> symbolsToRemove;
	for (sl_size i = 0; i < files.count; i++) {
		String ext = File::getFileExtension(files[i]);
		if (exts.contains_NoLock(ext)) {
			Println("Parsing file: %s", files[i]);
			String content = File::readAllText(path + "/" + files[i]);
			if (content.isNotEmpty()) {
				sl_char8* buf = content.getData();
				sl_reg len = content.getLength();
				sl_reg pos = 0;
				sl_reg startSymbol = -1;
				String symbolBefore;
				while (pos <= len) {
					sl_char8 ch = buf[pos];
					if (SLIB_CHAR_IS_C_NAME(ch)) {
						if (startSymbol < 0) {
							startSymbol = pos;
						}
					} else {
						if (startSymbol >= 0) {
							if (!SLIB_CHAR_IS_DIGIT(buf[startSymbol])) {
								String symbol(buf + startSymbol, pos - startSymbol);
								do {
									if (pos - startSymbol < (sl_reg)(prefix.getLength())) {
										break;
									}
									if (!(Base::equalsMemory(prefix.getData(), buf + startSymbol, prefix.getLength()))) {
										break;
									}
									if (matchBeforeSymbol.isNotEmpty()) {
										if (flagMatchBeforeNot) {
											if (symbolBefore == matchBeforeSymbol) {
												symbolsToRemove.put_NoLock(symbol, sl_true);
												break;
											}
										} else {
											if (symbolBefore != matchBeforeSymbol) {
												break;
											}
										}
									}
									symbols.put_NoLock(symbol, sl_true);
								} while (0);								
								symbolBefore = symbol;
							}
							startSymbol = -1;
						}
					}
					pos++;
				}
			}
		}
	}

	for (auto& item : symbolsToRemove) {
		symbols.remove_NoLock(item.key);
	}

	if (symbols.isEmpty()) {
		Println("No symbol is found!");
		return 0;
	}

	Println("Found symbols:\n");
	for (auto& item : symbols) {
		Println(format, item.key);
	}

	Println("\nPress any key to exit!");
	Console::readChar();
	return 0;
}
