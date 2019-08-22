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

#ifndef CHECKHEADER_SLIB_UI_BUTTON_MACOS
#define CHECKHEADER_SLIB_UI_BUTTON_MACOS

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/button.h"
#include "slib/ui/check_box.h"

#include "view_macos.h"

namespace slib
{
	
	namespace priv
	{
		namespace button
		{
			
			class ButtonInstance : public macOS_ViewInstance, public IButtonInstance, public ICheckBoxInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				ButtonInstance();
				
				~ButtonInstance();
				
			public:
				void setText(const String& text) override;
				
				void setDefaultButton(sl_bool flag) override;
				
				sl_bool measureSize(UISize& _out) override;
				
				void getChecked(sl_bool& flag) override;
				
				void setChecked(sl_bool flag) override;
				
			};
			
		}
	}
	
}

@interface SLIBButtonHandle : NSButton
{
	@public slib::WeakRef<slib::priv::button::ButtonInstance> m_viewInstance;
}
@end

#endif

#endif
