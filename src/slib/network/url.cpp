#include "../../../inc/slib/network/url.h"

#include "../../../inc/slib/core/scoped.h"

namespace slib
{

/*
		Uniform Resource Identifier (URI): Generic Syntax
			https://tools.ietf.org/html/rfc3986

   unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
   reserved      = gen-delims / sub-delims
   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                 / "*" / "+" / "," / ";" / "="
*/

	const sl_bool _URL_unreserved_pattern[128] = {
		/*		NUL		SOH		STX		ETX		EOT		ENQ		ACK		BEL		*/
		/*00*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		BS		HT		LF		VT		FF		CR		SO		SI		*/
		/*08*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		*/
		/*10*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		CAN		EM		SUB		ESC		FS		GS		RS		US		*/
		/*18*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		SP		!		"		#		$		%		&		'		*/
		/*20*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		(		)		*		+		,		-		.		/		*/
		/*28*/	0,		0,		0,		0,		0,		1,		1,		0,
		/*		0		1		2		3		4		5		6		7		*/
		/*30*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		8		9		:		;		<		=		>		?		*/
		/*38*/	1,		1,		0,		0,		0,		0,		0,		0,
		/*		@		A		B		C		D		E		F		G		*/
		/*40*/	0,		1,		1,		1,		1,		1,		1,		1,
		/*		H		I		J		K		L		M		N		O		*/
		/*48*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		P		Q		R		S		T		U		V		W		*/
		/*50*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		X		Y		Z		[		\		]		^		_		*/
		/*58*/	1,		1,		1,		0,		0,		0,		0,		1,
		/*		`		a		b		c		d		e		f		g		*/
		/*60*/	0,		1,		1,		1,		1,		1,		1,		1,
		/*		h		i		j		k		l		m		n		o		*/
		/*68*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		p		q		r		s		t		u		v		w		*/
		/*70*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		x		y		z		{		|		}		~		DEL		*/
		/*78*/	1,		1,		1,		0,		0,		0,		1,		0
	};

	const sl_bool _URL_unreserved_pattern_uri_components[128] = {
		/*		NUL		SOH		STX		ETX		EOT		ENQ		ACK		BEL		*/
		/*00*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		BS		HT		LF		VT		FF		CR		SO		SI		*/
		/*08*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		*/
		/*10*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		CAN		EM		SUB		ESC		FS		GS		RS		US		*/
		/*18*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		SP		!		"		#		$		%		&		'		*/
		/*20*/	0,		1,		0,		0,		0,		0,		0,		1,
		/*		(		)		*		+		,		-		.		/		*/
		/*28*/	1,		1,		1,		0,		0,		1,		1,		0,
		/*		0		1		2		3		4		5		6		7		*/
		/*30*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		8		9		:		;		<		=		>		?		*/
		/*38*/	1,		1,		0,		0,		0,		0,		0,		0,
		/*		@		A		B		C		D		E		F		G		*/
		/*40*/	0,		1,		1,		1,		1,		1,		1,		1,
		/*		H		I		J		K		L		M		N		O		*/
		/*48*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		P		Q		R		S		T		U		V		W		*/
		/*50*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		X		Y		Z		[		\		]		^		_		*/
		/*58*/	1,		1,		1,		0,		0,		0,		0,		1,
		/*		`		a		b		c		d		e		f		g		*/
		/*60*/	0,		1,		1,		1,		1,		1,		1,		1,
		/*		h		i		j		k		l		m		n		o		*/
		/*68*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		p		q		r		s		t		u		v		w		*/
		/*70*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		x		y		z		{		|		}		~		DEL		*/
		/*78*/	1,		1,		1,		0,		0,		0,		1,		0
	};

	const sl_bool _URL_unreserved_pattern_uri[128] = {
		/*		NUL		SOH		STX		ETX		EOT		ENQ		ACK		BEL		*/
		/*00*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		BS		HT		LF		VT		FF		CR		SO		SI		*/
		/*08*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		*/
		/*10*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		CAN		EM		SUB		ESC		FS		GS		RS		US		*/
		/*18*/	0,		0,		0,		0,		0,		0,		0,		0,
		/*		SP		!		"		#		$		%		&		'		*/
		/*20*/	0,		1,		0,		1,		1,		0,		1,		1,
		/*		(		)		*		+		,		-		.		/		*/
		/*28*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		0		1		2		3		4		5		6		7		*/
		/*30*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		8		9		:		;		<		=		>		?		*/
		/*38*/	1,		1,		1,		1,		0,		1,		0,		1,
		/*		@		A		B		C		D		E		F		G		*/
		/*40*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		H		I		J		K		L		M		N		O		*/
		/*48*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		P		Q		R		S		T		U		V		W		*/
		/*50*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		X		Y		Z		[		\		]		^		_		*/
		/*58*/	1,		1,		1,		0,		0,		0,		0,		1,
		/*		`		a		b		c		d		e		f		g		*/
		/*60*/	0,		1,		1,		1,		1,		1,		1,		1,
		/*		h		i		j		k		l		m		n		o		*/
		/*68*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		p		q		r		s		t		u		v		w		*/
		/*70*/	1,		1,		1,		1,		1,		1,		1,		1,
		/*		x		y		z		{		|		}		~		DEL		*/
		/*78*/	1,		1,		1,		0,		0,		0,		1,		0
	};
	
	Url::Url()
	{
	}
	
	Url::~Url()
	{
	}
	
	void Url::parse(const String& url)
	{
		const sl_char8* str = url.getData();
		sl_reg n = url.getLength();
		sl_reg indexHost = 0;
		sl_reg indexPath = -1;
		sl_reg indexQuery = -1;
		sl_reg index = 0;
		while (index < n) {
			sl_char8 ch = str[index];
			do {
				if (indexHost == 0) {
					if (ch == ':') {
						if (index + 3 <= n && str[index + 1] == '/' && str[index + 2] == '/') {
							indexHost = index + 3;
							index = indexHost;
						} else {
							index++;
						}
						break;
					}
				}
				if (indexPath < 0) {
					if (ch == '/') {
						indexPath = index;
						index++;
						break;
					}
				}
				if (indexQuery < 0) {
					if (ch == '?') {
						indexQuery = index + 1;
						index = indexQuery;
						break;
					}
				}
				index++;
			} while (0);
		}
		if (indexQuery > 0) {
			if (indexPath < 0 || indexPath >= indexQuery) {
				indexPath = indexQuery - 1;
			}
		}
		if (indexPath >= 0) {
			if (indexHost > indexPath) {
				indexHost = 0;
			}
		}
		if (indexHost >= 3) {
			scheme = String(str, indexHost - 3);
		} else {
			scheme.setNull();
		}
		if (indexPath >= 0) {
			host = String(str + indexHost, indexPath - indexHost);
			if (indexQuery > 0) {
				path = String(str + indexPath, indexQuery - 1 - indexPath);
				query = String(str + indexQuery, n - indexQuery);
			} else {
				path = String(str + indexPath, n - indexPath);
				query.setNull();
			}
		} else {
			host = String(str + indexHost, n - indexHost);
			path.setNull();
			query.setNull();
		}
	}
	
	String _URL_encodePercentByUTF8(const String& value, const sl_bool patternUnreserved[128])
	{
		sl_size n = value.getLength();
		if (n > 0) {
			const sl_char8* src = value.getData();
			SLIB_SCOPED_BUFFER(sl_char8, 1024, dst, n * 3);
			if (dst == sl_null) {
				return sl_null;
			}
			sl_size k = 0;
			for (sl_size i = 0; i < n; i++) {
				sl_uint32 v = (sl_uint32)(sl_uint8)(src[i]);
				if (v < 128 && patternUnreserved[v]) {
					dst[k++] = (sl_char8)(v);
				} else {
					dst[k++] = '%';
					dst[k++] = _StringConv_radixPatternUpper[(v >> 4) & 15];
					dst[k++] = _StringConv_radixPatternUpper[v & 15];
				}
			}
			return String(dst, k);
		} else {
			return sl_null;
		}
	}
	
	String Url::encodePercentByUTF8(const String& value)
	{
		return _URL_encodePercentByUTF8(value, _URL_unreserved_pattern);
	}
	
	String Url::decodePercentByUTF8(const String& value)
	{
		sl_size n = value.getLength();
		if (n > 0) {
			const sl_char8* src = value.getData();
			sl_char8* dst = (sl_char8*)(Base::createMemory(n));
			sl_size k = 0;
			for (sl_size i = 0; i < n; i++) {
				sl_uint32 v = (sl_uint32)(src[i]);
				if (v == '%') {
					if (i < n - 2) {
						sl_uint32 a1 = (sl_uint32)(src[i + 1]);
						sl_uint32 h1 = SLIB_CHAR_HEX_TO_INT(a1);
						if (h1 < 16) {
							sl_uint32 a2 = (sl_uint32)(src[i + 2]);
							sl_uint32 h2 = SLIB_CHAR_HEX_TO_INT(a2);
							if (h2 < 16) {
								dst[k++] = (sl_char8)((h1 << 4) | h2);
								i += 2;
							}
						}
					} else {
						dst[k++] = '%';
					}
				} else if (v < 256) {
					dst[k++] = (sl_char8)(v);
				}
			}
			String ret = String::fromUtf8(dst, k);
			Base::freeMemory(dst);
			return ret;
		} else {
			return sl_null;
		}
	}
	
	String Url::encodeUriComponentByUTF8(const String& value)
	{
		return _URL_encodePercentByUTF8(value, _URL_unreserved_pattern_uri_components);
	}
	
	String Url::decodeUriComponentByUTF8(const String& value)
	{
		return decodePercentByUTF8(value);
	}
	
	String Url::encodeUriByUTF8(const String& value)
	{
		return _URL_encodePercentByUTF8(value, _URL_unreserved_pattern_uri);
	}
	
	String Url::decodeUriByUTF8(const String& value)
	{
		return decodePercentByUTF8(value);
	}
	
}
