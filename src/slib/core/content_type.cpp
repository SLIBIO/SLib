#include "../../../inc/slib/core/content_type.h"

#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/safe_static.h"

#define DEFINE_CONTENT_TYPE(name, text) \
	SLIB_STATIC_STRING(_g_szContentType_##name, text); \
	const String& ContentTypes::name=_g_szContentType_##name;

namespace slib
{

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

	String ContentTypes::toString(ContentType type)
	{
		switch (type) {
			case ContentType::TextPlain:
				return _g_szContentType_TextPlain;
			case ContentType::TextHtml:
				return _g_szContentType_TextHtml;
			case ContentType::TextHtml_Utf8:
				return _g_szContentType_TextHtml_Utf8;
			case ContentType::TextXml:
				return _g_szContentType_TextXml;
			case ContentType::TextCss:
				return _g_szContentType_TextCss;
			case ContentType::TextJavascript:
				return _g_szContentType_TextJavascript;
			case ContentType::TextRtf:
				return _g_szContentType_TextRtf;
			case ContentType::TextCsv:
				return _g_szContentType_TextCsv;
				
			case ContentType::ImageGif:
				return _g_szContentType_ImageGif;
			case ContentType::ImageJpeg:
				return _g_szContentType_ImageJpeg;
			case ContentType::ImagePng:
				return _g_szContentType_ImagePng;
			case ContentType::ImageBmp:
				return _g_szContentType_ImageBmp;
			case ContentType::ImageTiff:
				return _g_szContentType_ImageTiff;
				
			case ContentType::AudioOgg:
				return _g_szContentType_AudioOgg;
			case ContentType::AudioOpus:
				return _g_szContentType_AudioOpus;
			case ContentType::AudioVorbis:
				return _g_szContentType_AudioVorbis;
			case ContentType::AudioWebm:
				return _g_szContentType_AudioWebm;
			case ContentType::AudioMpeg:
				return _g_szContentType_AudioMpeg;
			case ContentType::AudioMp4:
				return _g_szContentType_AudioMp4;
				
			case ContentType::VideoAvi:
				return _g_szContentType_VideoAvi;
			case ContentType::VideoMpeg:
				return _g_szContentType_VideoMpeg;
			case ContentType::VideoMp4:
				return _g_szContentType_VideoMp4;
			case ContentType::VideoOgg:
				return _g_szContentType_VideoOgg;
			case ContentType::VideoQuicktime:
				return _g_szContentType_VideoQuicktime;
			case ContentType::VideoWebm:
				return _g_szContentType_VideoWebm;
			case ContentType::VideoFlv:
				return _g_szContentType_VideoFlv;
			case ContentType::VideoMatroska:
				return _g_szContentType_VideoMatroska;
				
			case ContentType::OctetStream:
				return _g_szContentType_OctetStream;
			case ContentType::Json:
				return _g_szContentType_Json;
			case ContentType::Pdf:
				return _g_szContentType_Pdf;
			case ContentType::FontWOFF:
				return _g_szContentType_FontWOFF;
			case ContentType::FontTTF:
				return _g_szContentType_FontTTF;
			case ContentType::Zip:
				return _g_szContentType_Zip;
			case ContentType::Gzip:
				return _g_szContentType_Gzip;
			case ContentType::Flash:
				return _g_szContentType_Flash;

			case ContentType::WebForm:
				return _g_szContentType_WebForm;
			case ContentType::MultipartFormData:
				return _g_szContentType_MultipartFormData;
			
			default:
				break;
		}
		return sl_null;
	}

	class _ContentType_Mapping
	{
	public:
		HashMap<String, ContentType> maps;

		_ContentType_Mapping()
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

	ContentType ContentTypes::getFromFileExtension(const String& fileExt)
	{
		SLIB_SAFE_STATIC(_ContentType_Mapping, t)
		if (SLIB_SAFE_STATIC_CHECK_FREED(t)) {
			return ContentType::Unknown;
		}
		return t.maps.getValue(fileExt.toLower(), ContentType::Unknown);
	}

}
