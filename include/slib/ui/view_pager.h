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

#ifndef CHECKHEADER_SLIB_VIEW_PAGER
#define CHECKHEADER_SLIB_VIEW_PAGER

#include "definition.h"

#include "view.h"
#include "adapter.h"

namespace slib
{

	class SLIB_EXPORT ViewPager : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewPager();
		
		~ViewPager();

	public:
		void setAdapter(const Ref<ViewAdapter>& adapter, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void addPage(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);

		sl_uint64 getCurrentIndex();
		
		void selectPage(sl_uint64 index, UIUpdateMode mode = UIUpdateMode::Redraw);

	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
		void onChangePadding() override;
		
	protected:
		AtomicRef<ViewAdapter> m_adapter;
		CHashMap< sl_uint64, Ref<View> > m_cache;
		sl_uint64 m_indexCurrent;

	};

}

#endif
