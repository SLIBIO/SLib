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

namespace slib
{

	sl_bool SAppDocument::_registerRawResources(const String& fileDirPath)
	{
		_log(g_str_log_open_raws_begin.arg(fileDirPath));
		List<String> _list = File::getFiles(fileDirPath);
		_list.sort();
		ListElements<String> list(_list);
		for (sl_size i = 0; i < list.count; i++) {
			const String& fileName = list[i];
			if (fileName.isNotEmpty() && !(fileName.startsWith('.'))) {
				String name;
				if (!(_registerRawResource(fileName, fileDirPath + "/" + fileName, name))) {
					return sl_false;
				}
			}
		}
		return sl_true;
	}

	sl_bool SAppDocument::_registerRawResource(const String& _resourceName, const String& filePath, String& outName)
	{
		String resourceName = Resources::makeResourceName(_resourceName);
		if (m_raws.find(resourceName)) {
			_logError(g_str_error_resource_raw_name_duplicated.arg(resourceName, filePath));
			return sl_false;
		}
		Ref<SAppRawResource> res = new SAppRawResource;
		if (res.isNull()) {
			_logError(g_str_error_out_of_memory);
			return sl_false;
		}
		res->name = resourceName;
		res->filePath = filePath;
		if (!(m_raws.put(resourceName, res))) {
			_logError(g_str_error_out_of_memory);
			return sl_false;
		}
		outName = resourceName;
		return sl_true;
	}

	sl_bool SAppDocument::_generateRawCpp(const String& targetPath)
	{
		_log(g_str_log_generate_cpp_raws_begin);
		
		if (!(File::isDirectory(targetPath + "/raw"))) {
			File::createDirectory(targetPath + "/raw");
			if (!(File::isDirectory(targetPath + "/raw"))) {
				_log(g_str_error_directory_create_failed.arg(targetPath + "/raw"));
				return sl_false;
			}
		}

		StringBuffer sbHeader, sbCpp, sbMap;
		
		sbHeader.add(String::format(
									"#pragma once%n%n"
									"#include <slib/core/resource.h>%n%n"
									"namespace %s%n"
									"{%n\tnamespace raw%n\t{%n%n"
									, m_conf.generate_cpp_namespace));
		sbCpp.add(String::format(
								 "#include \"raws.h\"%n%n"
								 "namespace %s%n"
								 "{%n\tnamespace raw%n\t{%n%n"
								 , m_conf.generate_cpp_namespace));
		
		sbMap.add("\t\tSLIB_DEFINE_RAW_RESOURCE_MAP_BEGIN\r\n");
		
		StringBuffer sbData;
		
		for (auto& pair : m_raws) {
			
			if (pair.value.isNotNull()) {
				
				Ref<SAppRawResource> res = pair.value;
				
				sbHeader.add(String::format("\t\tSLIB_DECLARE_RAW_RESOURCE(%s)%n", res->name));
				
				sl_size size = (sl_size)(File::getSize(res->filePath));
				if (size > RAW_MAX_SIZE) {
					_logError(g_str_error_resource_raw_size_big.arg(res->filePath));
					return sl_false;
				}
				
				sbCpp.add(String::format("\t\tSLIB_DEFINE_RAW_RESOURCE(%s, %d)%n", res->name, size));
				
				sbMap.add(String::format("\t\t\tSLIB_DEFINE_RAW_RESOURCE_MAP_ITEM(%s)%n", res->name));
				
				sbData.add(String::format("\t\t#include \"raw/%s.inc\"%n", res->name));
				
				_generateRawDataFile(targetPath + "/raw/" + res->name + ".inc", res->filePath, res->name);
				
			}
		}
		
		sbMap.add("\t\tSLIB_DEFINE_RAW_RESOURCE_MAP_END\r\n");
		
		sbHeader.add("\r\n\t\tSLIB_DECLARE_RAW_RESOURCE_MAP\r\n\r\n\t}\r\n}\r\n");
		
		sbCpp.add("\r\n");
		sbCpp.link(sbMap);
		
		sbCpp.add("\r\n");
		sbCpp.link(sbData);
		
		sbCpp.add("\r\n\t}\r\n}\r\n");
		
		String pathHeader = targetPath + "/raws.h";
		String contentHeader = sbHeader.merge();
		if (File::readAllTextUTF8(pathHeader) != contentHeader) {
			if (!(File::writeAllTextUTF8(pathHeader, contentHeader))) {
				_logError(g_str_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}			
		}
		
		String pathCpp = targetPath + "/raws.cpp";
		String contentCpp = sbCpp.merge();
		if (File::readAllTextUTF8(pathCpp) != contentCpp) {
			if (!(File::writeAllTextUTF8(pathCpp, contentCpp))) {
				_logError(g_str_error_file_write_failed.arg(pathCpp));
				return sl_false;
			}
		}
		return sl_true;
	}
	
	sl_bool SAppDocument::_generateRawDataFile(const String& targetPath, const String& sourcePath, const String& resourceName)
	{
		if (!(File::exists(sourcePath))) {
			return sl_false;
		}
		Ref<File> fileSrc = File::openForRead(sourcePath);
		if (fileSrc.isNull()) {
			return sl_false;
		}
		Time timeModified = fileSrc->getModifiedTime();
		String signature = String::format("// Source: %s Size: %d bytes, Modified Time: %04y-%02m-%02d %02H:%02M:%02S", File::getFileName(sourcePath), fileSrc->getSize(), timeModified);
		if (File::exists(targetPath)) {
			Ref<File> fileDst = File::openForRead(targetPath);
			if (fileDst.isNotNull()) {
				String line = fileDst->readLine();
				if (line == signature) {
					return sl_true;
				}
				fileDst->close();
				File::deleteFile(targetPath);
			}
		}
		Memory mem = fileSrc->readAllBytes();
		Ref<File> fileDst = File::openForWrite(targetPath);
		if (fileDst.isNotNull()) {
			fileDst->writeTextUTF8(signature);
			fileDst->writeTextUTF8(String::format("\r\nnamespace %s {%nconst sl_uint8 bytes[] = {%n", resourceName, mem.getData()));
			fileDst->writeTextUTF8(SAppUtil::generateBytesArrayDefinition(mem.getData(), mem.getSize(), 16, 0));
			static const sl_char8 strDataEnd[] = "};\r\n}\r\n";
			fileDst->write(strDataEnd, sizeof(strDataEnd) - 1);
			return sl_true;
		}
		return sl_false;
	}

}
