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

#include "slib/core/content_type.h"

#include "slib/core/hash_map.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
#define DEFINE_CONTENT_TYPE(name, text) \
	namespace priv { \
		namespace content_type { \
			SLIB_STATIC_STRING(g_##name, text); \
		} \
	} \
	const String& ContentTypeHelper::name=priv::content_type::g_##name;

	DEFINE_CONTENT_TYPE(TextPlain, "text/plain")
	DEFINE_CONTENT_TYPE(TextHtml, "text/html")
	DEFINE_CONTENT_TYPE(TextHtml_Utf8, "text/html; charset=UTF-8")
	DEFINE_CONTENT_TYPE(TextXml, "text/xml")
	DEFINE_CONTENT_TYPE(TextCss, "text/css")
	DEFINE_CONTENT_TYPE(TextJavascript, "text/javascript")
	DEFINE_CONTENT_TYPE(TextRtf, "text/rtf")
	DEFINE_CONTENT_TYPE(TextCsv, "text/csv")

	DEFINE_CONTENT_TYPE(ImageGif, "image/gif")
	DEFINE_CONTENT_TYPE(ImageJpeg, "image/jpeg")
	DEFINE_CONTENT_TYPE(ImagePng, "image/png")
	DEFINE_CONTENT_TYPE(ImageBmp, "image/bmp")
	DEFINE_CONTENT_TYPE(ImageTiff, "image/tiff")

	DEFINE_CONTENT_TYPE(AudioOgg, "audio/ogg")
	DEFINE_CONTENT_TYPE(AudioOpus, "audio/opus")
	DEFINE_CONTENT_TYPE(AudioVorbis, "audio/vorbis")
	DEFINE_CONTENT_TYPE(AudioWebm, "audio/webm")
	DEFINE_CONTENT_TYPE(AudioMpeg, "audio/mpeg")
	DEFINE_CONTENT_TYPE(AudioMp4, "audio/mp4")

	DEFINE_CONTENT_TYPE(VideoAvi, "video/avi")
	DEFINE_CONTENT_TYPE(VideoMpeg, "video/mpeg")
	DEFINE_CONTENT_TYPE(VideoMp4, "video/mp4")
	DEFINE_CONTENT_TYPE(VideoOgg, "video/ogg")
	DEFINE_CONTENT_TYPE(VideoQuicktime, "video/quicktime")
	DEFINE_CONTENT_TYPE(VideoWebm, "video/webm")
	DEFINE_CONTENT_TYPE(VideoFlv, "video/x-flv")
	DEFINE_CONTENT_TYPE(VideoMatroska, "video/x-matroska")

	DEFINE_CONTENT_TYPE(OctetStream, "application/octet-stream")
	DEFINE_CONTENT_TYPE(Json, "application/json")
	DEFINE_CONTENT_TYPE(Pdf, "application/pdf")
	DEFINE_CONTENT_TYPE(FontWOFF, "application/font-woff")
	DEFINE_CONTENT_TYPE(FontTTF, "application/x-font-ttf")
	DEFINE_CONTENT_TYPE(Zip, "application/zip")
	DEFINE_CONTENT_TYPE(Gzip, "application/gzip")
	DEFINE_CONTENT_TYPE(Flash, "application/x-shockwave-flash")

	DEFINE_CONTENT_TYPE(WebForm, "application/x-www-form-urlencoded")
	DEFINE_CONTENT_TYPE(MultipartFormData, "multipart/form-data")

	String ContentTypeHelper::toString(ContentType type)
	{
		switch (type) {
			case ContentType::TextPlain:
				return priv::content_type::g_TextPlain;
			case ContentType::TextHtml:
				return priv::content_type::g_TextHtml;
			case ContentType::TextHtml_Utf8:
				return priv::content_type::g_TextHtml_Utf8;
			case ContentType::TextXml:
				return priv::content_type::g_TextXml;
			case ContentType::TextCss:
				return priv::content_type::g_TextCss;
			case ContentType::TextJavascript:
				return priv::content_type::g_TextJavascript;
			case ContentType::TextRtf:
				return priv::content_type::g_TextRtf;
			case ContentType::TextCsv:
				return priv::content_type::g_TextCsv;
				
			case ContentType::ImageGif:
				return priv::content_type::g_ImageGif;
			case ContentType::ImageJpeg:
				return priv::content_type::g_ImageJpeg;
			case ContentType::ImagePng:
				return priv::content_type::g_ImagePng;
			case ContentType::ImageBmp:
				return priv::content_type::g_ImageBmp;
			case ContentType::ImageTiff:
				return priv::content_type::g_ImageTiff;
				
			case ContentType::AudioOgg:
				return priv::content_type::g_AudioOgg;
			case ContentType::AudioOpus:
				return priv::content_type::g_AudioOpus;
			case ContentType::AudioVorbis:
				return priv::content_type::g_AudioVorbis;
			case ContentType::AudioWebm:
				return priv::content_type::g_AudioWebm;
			case ContentType::AudioMpeg:
				return priv::content_type::g_AudioMpeg;
			case ContentType::AudioMp4:
				return priv::content_type::g_AudioMp4;
				
			case ContentType::VideoAvi:
				return priv::content_type::g_VideoAvi;
			case ContentType::VideoMpeg:
				return priv::content_type::g_VideoMpeg;
			case ContentType::VideoMp4:
				return priv::content_type::g_VideoMp4;
			case ContentType::VideoOgg:
				return priv::content_type::g_VideoOgg;
			case ContentType::VideoQuicktime:
				return priv::content_type::g_VideoQuicktime;
			case ContentType::VideoWebm:
				return priv::content_type::g_VideoWebm;
			case ContentType::VideoFlv:
				return priv::content_type::g_VideoFlv;
			case ContentType::VideoMatroska:
				return priv::content_type::g_VideoMatroska;
				
			case ContentType::OctetStream:
				return priv::content_type::g_OctetStream;
			case ContentType::Json:
				return priv::content_type::g_Json;
			case ContentType::Pdf:
				return priv::content_type::g_Pdf;
			case ContentType::FontWOFF:
				return priv::content_type::g_FontWOFF;
			case ContentType::FontTTF:
				return priv::content_type::g_FontTTF;
			case ContentType::Zip:
				return priv::content_type::g_Zip;
			case ContentType::Gzip:
				return priv::content_type::g_Gzip;
			case ContentType::Flash:
				return priv::content_type::g_Flash;

			case ContentType::WebForm:
				return priv::content_type::g_WebForm;
			case ContentType::MultipartFormData:
				return priv::content_type::g_MultipartFormData;
			
			default:
				break;
		}
		return sl_null;
	}

	namespace priv
	{
		namespace content_type
		{
			class Mapping
			{
			public:
				CHashMap<String, ContentType> maps;
				
			public:
				Mapping()
				{
					maps.put("txt", ContentType::TextPlain);
					maps.put("htm", ContentType::TextHtml);
					maps.put("html", ContentType::TextHtml);
					maps.put("xml", ContentType::TextXml);
					maps.put("css", ContentType::TextCss);
					maps.put("js", ContentType::TextJavascript);
					maps.put("rtf", ContentType::TextRtf);
					maps.put("csv", ContentType::TextCsv);
					
					maps.put("gif", ContentType::ImageGif);
					maps.put("jpeg", ContentType::ImageJpeg);
					maps.put("jpg", ContentType::ImageJpeg);
					maps.put("png", ContentType::ImagePng);
					maps.put("bmp", ContentType::ImageBmp);
					maps.put("tiff", ContentType::ImageTiff);
					maps.put("tif", ContentType::ImageTiff);
					
					maps.put("oga", ContentType::AudioOgg);
					maps.put("opus", ContentType::AudioOpus);
					maps.put("weba", ContentType::AudioWebm);
					maps.put("mpa", ContentType::AudioMpeg);
					maps.put("mp1", ContentType::AudioMpeg);
					maps.put("mp2", ContentType::AudioMpeg);
					maps.put("mp3", ContentType::AudioMpeg);
					maps.put("m4a", ContentType::AudioMp4);
					maps.put("aac", ContentType::AudioMp4);
					
					maps.put("avi", ContentType::VideoAvi);
					maps.put("mpg", ContentType::VideoMpeg);
					maps.put("mpeg", ContentType::VideoMpeg);
					maps.put("mpv", ContentType::VideoMpeg);
					maps.put("mp4", ContentType::VideoMp4);
					maps.put("m4v", ContentType::VideoMp4);
					maps.put("ogg", ContentType::VideoOgg);
					maps.put("ogv", ContentType::VideoOgg);
					maps.put("webm", ContentType::VideoWebm);
					maps.put("flv", ContentType::VideoFlv);
					maps.put("mkv", ContentType::VideoMatroska);
					
					maps.put("json", ContentType::Json);
					maps.put("pdf", ContentType::Pdf);
					maps.put("woff", ContentType::FontWOFF);
					maps.put("ttf", ContentType::FontTTF);
					maps.put("zip", ContentType::Zip);
					maps.put("gz", ContentType::Gzip);
					maps.put("swf", ContentType::Flash);
				}
			};

		}
	}

	ContentType ContentTypeHelper::getFromFileExtension(const String& fileExt)
	{
		SLIB_SAFE_STATIC(priv::content_type::Mapping, t)
		if (SLIB_SAFE_STATIC_CHECK_FREED(t)) {
			return ContentType::Unknown;
		}
		return t.maps.getValue(fileExt.toLower(), ContentType::Unknown);
	}

	sl_bool ContentTypeHelper::equalsContentTypeExceptParams(const String& _type1, const String& _type2)
	{
		String type1 = _type1;
		{
			sl_reg index = type1.indexOf(';');
			if (index < 0) {
				type1 = type1.trim();
			} else {
				type1 = type1.substring(0, index).trim();
			}
		}
		String type2 = _type2;
		{
			sl_reg index = type2.indexOf(';');
			if (index < 0) {
				type2 = type2.trim();
			} else {
				type2 = type2.substring(0, index).trim();
			}
		}
		return type1.equalsIgnoreCase(type2);
	}

	sl_bool ContentTypeHelper::equalsContentTypeExceptParams(const String& type1, ContentType type2)
	{
		return equalsContentTypeExceptParams(type1, toString(type2));
	}

}
