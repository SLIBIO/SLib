#ifndef CHECKHEADER_SLIB_CORE_CONTENT_TYPE
#define CHECKHEADER_SLIB_CORE_CONTENT_TYPE

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT ContentType
{
public:
	// text/plain
	static String getTextPlain();
	// text/html
	static String getTextHtml();
	// text/xml
	static String getTextXml();
	// text/css
	static String getTextCss();
	// text/javascript
	static String getTextJavascript();
	// text/rtf
	static String getTextRtf();
	// text/csv
	static String getTextCsv();

	// image/gif
	static String getImageGif();
	// image/jpeg
	static String getImageJpeg();
	// image/png
	static String getImagePng();
	// image/bmp
	static String getImageBmp();
	// image/tiff
	static String getImageTiff();

	// audio/ogg
	static String getAudioOgg();
	// audio/opus
	static String getAudioOpus();
	// audio/vorbis
	static String getAudioVorbis();
	// audio/vorbis
	static String getAudioWebm();
	// audio/mpeg
	static String getAudioMpeg();
	// audio/mp4
	static String getAudioMp4();

	// video/avi
	static String getVideoAvi();
	// video/mpeg
	static String getVideoMpeg();
	// video/mp4
	static String getVideoMp4();
	// video/ogg
	static String getVideoOgg();
	// video/quicktime
	static String getVideoQuicktime();
	// video/webm
	static String getVideoWebm();
	// video/x-flv
	static String getVideoFlv();
	// video/x-matroska
	static String getVideoMatroska();

	// application/octet-stream
	static String getOctetStream();
	// application/json
	static String getJson();
	// application/pdf
	static String getPdf();
	// application/font-woff
	static String getFontWOFF();
	// application/x-font-ttf
	static String getFontTTF();
	// application/zip
	static String getZip();
	// application/gzip
	static String getGzip();

	// multipart/form-data
	static String getMultipartFormData();

public:
	static String fromFileExtension(const String& fileExt);
};

SLIB_NAMESPACE_END

#endif
