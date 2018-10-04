/*
 *  Copyright (c) 2008-2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_CONSOLE
#define CHECKHEADER_SLIB_CORE_CONSOLE

#include "definition.h"

#include "string.h"
#include "object.h"
#include "list.h"
#include "variant.h"

namespace slib
{
	
	class SLIB_EXPORT Console
	{
	public:
		static void print(const String& s);

		template <class... ARGS>
		static void print(const String& format, ARGS&&... args);
	
		static void println(const String& s);

		template <class... ARGS>
		static void println(const String& format, ARGS&&... args);
	
		static String readLine();

		static sl_char16 readChar(sl_bool flagPrintEcho = sl_false);

	};
	
	template <class... ARGS>
	void Printf(const String& format, ARGS&&... args);
	
	template <class... ARGS>
	void Println(const String& format, ARGS&&... args);
	
}

#include "detail/console.inc"

#endif
