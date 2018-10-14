#include <slib/core.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	if (argc != 5) {
		return -1;
	}
	String fileSource = argv[1];
	String fileOutput = argv[2];
	String textFind = argv[3];
	String textReplace = argv[4];
	Memory mem = File::readAllBytes(fileSource);
	if (mem.isNull()) {
		return -1;
	}
	String str = String((sl_char8*)(mem.getData()), mem.getSize());
	str = str.replaceAll(textFind, textReplace);
	if (File::writeAllBytes(fileOutput, Memory::createStatic(str.getData(), str.getLength())) == 0) {
		return -1;
	}
	return 0;
}
