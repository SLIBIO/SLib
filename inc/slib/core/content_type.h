#ifndef CHECKHEADER_SLIB_CORE_CONTENT_TYPE
#define CHECKHEADER_SLIB_CORE_CONTENT_TYPE

#include "definition.h"

#include "string.h"

namespace slib
{
	
	enum class ContentType
	{
		Unknown = 0,

		// text/plain
		TextPlain,
		// text/html
		TextHtml,
		// text/html; charset=UTF-8
		TextHtml_Utf8,
		// text/xml
		TextXml,
		// text/css
		TextCss,
		// text/javascript
		TextJavascript,
		// text/rtf
		TextRtf,
		// text/csv
		TextCsv,

		// image/gif
		ImageGif,
		// image/jpeg
		ImageJpeg,
		// image/png
		ImagePng,
		// image/bmp
		ImageBmp,
		// image/tiff
		ImageTiff,

		// audio/ogg
		AudioOgg,
		// audio/opus
		AudioOpus,
		// audio/vorbis
		AudioVorbis,
		// audio/webm
		AudioWebm,
		// audio/mpeg
		AudioMpeg,
		// audio/mp4
		AudioMp4,

		// video/avi
		VideoAvi,
		// video/mpeg
		VideoMpeg,
		// video/mp4
		VideoMp4,
		// video/ogg
		VideoOgg,
		// video/quicktime
		VideoQuicktime,
		// video/webm
		VideoWebm,
		// video/x-flv
		VideoFlv,
		// video/x-matroska
		VideoMatroska,

		// application/octet-stream
		OctetStream,
		// application/json
		Json,
		// application/pdf
		Pdf,
		// application/font-woff
		FontWOFF,
		// application/x-font-ttf
		FontTTF,
		// application/zip
		Zip,
		// application/gzip
		Gzip,
		// application/x-shockwave-flash
		Flash,
	
		// application/x-www-form-urlencoded
		WebForm,
		// multipart/form-data
		MultipartFormData,


	};
	
	class SLIB_EXPORT ContentTypes
	{
	public:
		static String toString(ContentType type);

		static ContentType getFromFileExtension(const String& fileExt);

	public:
		// text/plain
		static const String& TextPlain;
		// text/html
		static const String& TextHtml;
		// text/html; charset=UTF-8
		static const String& TextHtml_Utf8;
		// text/xml
		static const String& TextXml;
		// text/css
		static const String& TextCss;
		// text/javascript
		static const String& TextJavascript;
		// text/rtf
		static const String& TextRtf;
		// text/csv
		static const String& TextCsv;

		// image/gif
		static const String& ImageGif;
		// image/jpeg
		static const String& ImageJpeg;
		// image/png
		static const String& ImagePng;
		// image/bmp
		static const String& ImageBmp;
		// image/tiff
		static const String& ImageTiff;

		// audio/ogg
		static const String& AudioOgg;
		// audio/opus
		static const String& AudioOpus;
		// audio/vorbis
		static const String& AudioVorbis;
		// audio/webm
		static const String& AudioWebm;
		// audio/mpeg
		static const String& AudioMpeg;
		// audio/mp4
		static const String& AudioMp4;

		// video/avi
		static const String& VideoAvi;
		// video/mpeg
		static const String& VideoMpeg;
		// video/mp4
		static const String& VideoMp4;
		// video/ogg
		static const String& VideoOgg;
		// video/quicktime
		static const String& VideoQuicktime;
		// video/webm
		static const String& VideoWebm;
		// video/x-flv
		static const String& VideoFlv;
		// video/x-matroska
		static const String& VideoMatroska;

		// application/octet-stream
		static const String& OctetStream;
		// application/json
		static const String& Json;
		// application/pdf
		static const String& Pdf;
		// application/font-woff
		static const String& FontWOFF;
		// application/x-font-ttf
		static const String& FontTTF;
		// application/zip
		static const String& Zip;
		// application/gzip
		static const String& Gzip;
		// application/x-shockwave-flash
		static const String& Flash;
	
		// application/x-www-form-urlencoded
		static const String& WebForm;
		// multipart/form-data
		static const String& MultipartFormData;
	
	};

}

#endif
