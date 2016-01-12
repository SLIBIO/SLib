#include "../../../inc/slib/core/content_type.h"
#include "../../../inc/slib/core/map.h"

SLIB_NAMESPACE_BEGIN

#define GETTER_CONTENT_TYPE(NAME, CONTENT_TYPE) \
String ContentType::get##NAME() \
{ \
	SLIB_STATIC_STRING(s, CONTENT_TYPE); \
	return s; \
}

GETTER_CONTENT_TYPE(TextPlain, "text/plain")
GETTER_CONTENT_TYPE(TextHtml, "text/html")
GETTER_CONTENT_TYPE(TextXml, "text/xml")
GETTER_CONTENT_TYPE(TextCss, "text/css")
GETTER_CONTENT_TYPE(TextJavascript, "text/javascript")
GETTER_CONTENT_TYPE(TextRtf, "text/rtf")
GETTER_CONTENT_TYPE(TextCsv, "text/csv")

GETTER_CONTENT_TYPE(ImageGif, "image/gif")
GETTER_CONTENT_TYPE(ImageJpeg, "image/jpeg")
GETTER_CONTENT_TYPE(ImagePng, "image/png")
GETTER_CONTENT_TYPE(ImageBmp, "image/bmp")
GETTER_CONTENT_TYPE(ImageTiff, "image/tiff")

GETTER_CONTENT_TYPE(AudioOgg, "audio/ogg")
GETTER_CONTENT_TYPE(AudioOpus, "audio/opus")
GETTER_CONTENT_TYPE(AudioVorbis, "audio/vorbis")
GETTER_CONTENT_TYPE(AudioWebm, "audio/webm")
GETTER_CONTENT_TYPE(AudioMpeg, "audio/mpeg")
GETTER_CONTENT_TYPE(AudioMp4, "audio/mp4")

GETTER_CONTENT_TYPE(VideoAvi, "video/avi")
GETTER_CONTENT_TYPE(VideoMpeg, "video/mpeg")
GETTER_CONTENT_TYPE(VideoMp4, "video/mp4")
GETTER_CONTENT_TYPE(VideoOgg, "video/ogg")
GETTER_CONTENT_TYPE(VideoQuicktime, "video/quicktime")
GETTER_CONTENT_TYPE(VideoWebm, "video/webm")
GETTER_CONTENT_TYPE(VideoFlv, "video/x-flv")
GETTER_CONTENT_TYPE(VideoMatroska, "video/x-matroska")

GETTER_CONTENT_TYPE(OctetStream, "application/octet-stream")
GETTER_CONTENT_TYPE(Json, "application/json")
GETTER_CONTENT_TYPE(Pdf, "application/pdf")
GETTER_CONTENT_TYPE(FontWOFF, "application/font-woff")
GETTER_CONTENT_TYPE(FontTTF, "application/x-font-ttf")
GETTER_CONTENT_TYPE(Zip, "application/zip")
GETTER_CONTENT_TYPE(Gzip, "application/gzip")

GETTER_CONTENT_TYPE(MultipartFormData, "multipart/form-data")

struct _ContentType_Mapping
{
	Map<String, String> maps;

	_ContentType_Mapping()
	{
		maps.put("txt", ContentType::getTextPlain());
		maps.put("htm", ContentType::getTextHtml());
		maps.put("html", ContentType::getTextHtml());
		maps.put("xml", ContentType::getTextXml());
		maps.put("css", ContentType::getTextCss());
		maps.put("js", ContentType::getTextJavascript());
		maps.put("rtf", ContentType::getTextRtf());
		maps.put("csv", ContentType::getTextCsv());

		maps.put("gif", ContentType::getImageGif());
		maps.put("jpeg", ContentType::getImageJpeg());
		maps.put("jpg", ContentType::getImageJpeg());
		maps.put("png", ContentType::getImagePng());
		maps.put("bmp", ContentType::getImageBmp());
		maps.put("tiff", ContentType::getImageTiff());
		maps.put("tif", ContentType::getImageTiff());

		maps.put("oga", ContentType::getAudioOgg());
		maps.put("opus", ContentType::getAudioOpus());
		maps.put("weba", ContentType::getAudioWebm());
		maps.put("mpa", ContentType::getAudioMpeg());
		maps.put("mp1", ContentType::getAudioMpeg());
		maps.put("mp2", ContentType::getAudioMpeg());
		maps.put("mp3", ContentType::getAudioMpeg());
		maps.put("m4a", ContentType::getAudioMp4());
		maps.put("aac", ContentType::getAudioMp4());

		maps.put("avi", ContentType::getVideoAvi());
		maps.put("mpg", ContentType::getVideoMpeg());
		maps.put("mpeg", ContentType::getVideoMpeg());
		maps.put("mpv", ContentType::getVideoMpeg());
		maps.put("mp4", ContentType::getVideoMp4());
		maps.put("m4v", ContentType::getVideoMp4());
		maps.put("ogg", ContentType::getVideoOgg());
		maps.put("ogv", ContentType::getVideoOgg());
		maps.put("webm", ContentType::getVideoWebm());
		maps.put("flv", ContentType::getVideoFlv());
		maps.put("mkv", ContentType::getVideoMatroska());

		maps.put("json", ContentType::getJson());
		maps.put("pdf", ContentType::getPdf());
		maps.put("woff", ContentType::getFontWOFF());
		maps.put("ttf", ContentType::getFontTTF());
		maps.put("zip", ContentType::getZip());
		maps.put("gz", ContentType::getGzip());
	}
};

String ContentType::fromFileExtension(const String& fileExt)
{
	SLIB_SAFE_STATIC(_ContentType_Mapping, t);
	return t.maps.getValue(fileExt.toLower(), String::null());
}

SLIB_NAMESPACE_END
