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

#ifndef CHECKHEADER_SLIB_UI_DATE_PICKER
#define CHECKHEADER_SLIB_UI_DATE_PICKER

#include "definition.h"

#include "view.h"

#include "../core/time.h"

namespace slib
{

	class SLIB_EXPORT DatePicker : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		DatePicker();
		
		~DatePicker();

	public:
		Time getDate();
		
		virtual void setDate(const Time& date);
		
	public:		
		SLIB_DECLARE_EVENT_HANDLER(DatePicker, Change, Time& date)

	protected:
		void onUpdateLayout() override;
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	private:
		void _getDate_NW();
		
		void _setDate_NW(const Time& date);
		
		sl_bool _measureSize_NW(UISize& _out);
		
		void _setFont_NW(const Ref<Font>& font) override;
		
	protected:
		Time m_date;
		
	};

}

#endif
