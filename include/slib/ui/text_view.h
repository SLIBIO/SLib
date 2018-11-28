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

#ifndef CHECKHEADER_SLIB_UI_TEXT_VIEW
#define CHECKHEADER_SLIB_UI_TEXT_VIEW

#include "definition.h"

#include "view.h"
#include "text.h"

namespace slib
{
	
	class SLIB_EXPORT TextView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextView();
		
		~TextView();

	public:
		sl_bool isEditable();
		
		void setEditable(sl_bool flagEditable);
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onKeyEvent(UIEvent* event) override;
		
	protected:
		sl_bool m_flagEditable;
		
	};

}

#endif
