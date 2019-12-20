/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/social/contact.h"

#include "slib/core/string_buffer.h"

namespace slib
{

	namespace priv
	{
		namespace contact
		{

			String EncodeVCardText(const String& text)
			{
				char* data = text.getData();
				sl_size len = text.getLength();
				StringBuffer buf;
				for (sl_size i = 0; i < len; i++) {
					buf.addStatic("=");
					buf.add(String::fromInt((sl_uint8)(data[i]), 16, 0, sl_true));
				}
				return buf.merge();
			}

		}
	}

	using namespace priv::contact;

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Contact)

	SLIB_DEFINE_JSON_MEMBERS(Contact, namePrefix, nameSuffix, familyName, givenName, middleName, displayName, nickname, phoneNumbers, emails)

	Contact::Contact()
	{
	}

	String Contact::generateVCard()
	{
		StringBuffer buf;
		{
			buf.addStatic("BEGIN:VCARD\r\nVERSION:2.1\r\nN;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			buf.add(EncodeVCardText(familyName));
			buf.addStatic(";");
			buf.add(EncodeVCardText(givenName));
			buf.addStatic(";");
			buf.add(EncodeVCardText(middleName));
			buf.addStatic(";");
			buf.add(EncodeVCardText(namePrefix));
			buf.addStatic("\r\n");
		}
		{
			ListElements<String> list(phoneNumbers.getAllValues());
			for (sl_size i = 0; i < list.count; i++) {
				buf.addStatic("TEL;CELL:");
				buf.add(list[i]);
				buf.addStatic("\r\n");
			}
		}
		{
			ListElements<String> list(emails.getAllValues());
			for (sl_size i = 0; i < list.count; i++) {
				SLIB_STATIC_STRING(s, "EMAIL:");
				buf.add(s);
				buf.add(list[i]);
				buf.addStatic("\r\n");
			}
		}
		{
			SLIB_STATIC_STRING(s, "END:VCARD\r\n");
			buf.add(s);
		}
		return buf.merge();
	}

}

