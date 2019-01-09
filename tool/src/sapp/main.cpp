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

#include <slib/core.h>
#include <slib/ui.h>

#include "sapp.h"

using namespace slib;

int main(int argc, const char * argv[])
{
	CList<String> args;
	for (int i = 0; i < argc; i++) {
		String arg = argv[i];
		arg = arg.trim();
		if (arg == "-NSDocumentRevisionsDebugMode") {
			i++;
		} else {
			args.add_NoLock(arg);
		}
	}
	String command;
	if (args.getCount() < 2) {
		while (1) {
			Println("Input the command or file path");
			command = Console::readLine().trim();
			if (command.isNotNull()) {
				break;
			}
		}
	} else {
		command = args[1];
	}
	if (command == "gen") {
		String path;
		if (args.getCount() < 3) {
			path = System::getCurrentDirectory();
		} else {
			path = args[2];
		}
		if (File::isDirectory(path)) {
			path += "/sapp.xml";
			if (!(File::isFile(path))) {
				Println("sapp.xml is not found in %s", path);
				return -1;
			}
		} else {
			if (!(File::isFile(path))) {
				Println("sapp file is not found in %s", path);
				return -1;
			}
		}
		Ref<SAppDocument> doc = new SAppDocument;
		if (!(doc->open(path))) {
			return -1;
		}
		if (!(doc->openResources())) {
			return -1;
		}
		if (!(doc->generateCpp())) {
			return -1;
		}
	} else {
		String path = command;
		if (File::isFile(path + ".xml")) {
			path += ".xml";
		} else if (File::isFile(path + ".uiml")) {
			path += ".uiml";
		} else if (!(File::isFile(path))) {
			Println("File is not found in %s", path);
			return -1;
		}
		path = File::getRealPath(path);
		if (!(File::isFile(path))) {
			Println("File is not found in %s", path);
			return -1;
		}
		String pathDir = File::getParentDirectoryPath(path);
		if (File::getFileName(pathDir) == "ui") {
			String pathApp = File::getParentDirectoryPath(pathDir);
			if (!(File::isFile(pathApp + "/sapp.xml"))) {
				Println("sapp.xml is not found in %s", pathApp);
				return -1;
			}
			Ref<SAppDocument> doc = new SAppDocument;
			if (!(doc->open(pathApp + "/sapp.xml"))) {
				return -1;
			}
			if (!(doc->openUiResource(path))) {
				return -1;
			}
			String layoutName = File::getFileNameOnly(path);
			SAppSimulateLayoutParam param;
			String pathConfig = pathApp + "/.sapp.conf";
			Json config = Json::parseJsonFromTextFile(pathConfig);
			param.windowSize.x = config["simulator_window_width"].getInt32(param.windowSize.x);
			param.windowSize.y = config["simulator_window_height"].getInt32(param.windowSize.y);
			param.onCloseWindow = [pathConfig](Window* window, UIEvent* ev) {
				Json config = Json::parseJsonFromTextFile(pathConfig);
				if (config.isNull()) {
					config = Json::createMap();
				}
				UISize size = window->getClientSize();
				config.putItem("simulator_window_width", size.x);
				config.putItem("simulator_window_height", size.y);
				File::writeAllTextUTF8(pathConfig, config.toJsonString());
				UI::quitApp();
			};
			doc->simulateLayoutInWindow(layoutName, param);
			UI::runApp();
		} else {
			Println("Not supported file: %s", path);
			return -1;
		}
	}
	return 0;
}
