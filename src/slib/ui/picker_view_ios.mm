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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/picker_view.h"

#include "view_ios.h"

namespace slib
{
	namespace priv
	{
		namespace picker_view
		{
			class PickerViewInstance;
		}
	}
}

@interface SLIBPickerViewHandle : UIPickerView<UIPickerViewDelegate, UIPickerViewDataSource>
{
	@public sl_uint32 m_selectionBefore;
	@public slib::WeakRef<slib::priv::picker_view::PickerViewInstance> m_viewInstance;
	
	UIFont* m_font;
}

- (void)setFont: (UIFont*)font;

@end

namespace slib
{

	namespace priv
	{
		namespace picker_view
		{

			class PickerViewHelper : public PickerView
			{
			public:
				sl_uint32 getItemsCount()
				{
					return (sl_uint32)(m_titles.getCount());
				}
				
				NSString* getItemTitle(sl_uint32 row)
				{
					String s = m_titles.getValueAt(row);
					return Apple::getNSStringFromString(s);
				}
				
			};
			
			class PickerViewInstance : public iOS_ViewInstance, public IPickerViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBPickerViewHandle* getHandle()
				{
					return (SLIBPickerViewHandle*)m_handle;
				}
				
				Ref<PickerViewHelper> getHelper()
				{
					return CastRef<PickerViewHelper>(getView());
				}
				
				static void _select(SLIBPickerViewHandle* handle, sl_uint32 index)
				{
					[handle selectRow:index inComponent:0 animated:NO];
				}
				
				void select(PickerView* view, sl_uint32 index) override
				{
					SLIBPickerViewHandle* handle = getHandle();
					if (handle != nil) {
						_select(handle, index);
					}
				}
				
				void refreshItemsCount(PickerView* view) override
				{
					refreshItemsContent(view);
				}
				
				void refreshItemsContent(PickerView* view) override
				{
					SLIBPickerViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle reloadAllComponents];
						_select(handle, view->getSelectedIndex());
					}
				}
				
				void setItemTitle(PickerView* view, sl_uint32 index, const String& title) override
				{
					SLIBPickerViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle reloadAllComponents];
					}
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					SLIBPickerViewHandle* handle = getHandle();
					if (handle != nil) {
						setHandleFont(handle, font);
						[handle reloadAllComponents];
					}
				}
				
				void onSelectItem(SLIBPickerViewHandle* handle, sl_uint32 row)
				{
					Ref<PickerViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						helper->dispatchSelectItem(row);
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(PickerViewInstance, iOS_ViewInstance)

		}
	}

	using namespace priv::picker_view;
	
	Ref<ViewInstance> PickerView::createNativeWidget(ViewInstance* parent)
	{
		Ref<PickerViewInstance> ret = iOS_ViewInstance::create<PickerViewInstance, SLIBPickerViewHandle>(this, parent);
		if (ret.isNotNull()) {
			SLIBPickerViewHandle* handle = ret->getHandle();
			iOS_ViewInstance::setHandleFont(handle, getFont());
			PickerViewInstance::_select(handle, m_indexSelected);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IPickerViewInstance> PickerView::getPickerViewInstance()
	{
		return CastRef<PickerViewInstance>(getViewInstance());
	}
	
}

using namespace slib;
using namespace slib::priv::picker_view;

@implementation SLIBPickerViewHandle

IOS_VIEW_DEFINE_ON_FOCUS

-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		self->m_selectionBefore = 0;
		self.dataSource = self;
		self.delegate = self;
	}
	return self;
}

- (void)setFont:(UIFont *)font
{
	m_font = font;
}

- (Ref<PickerViewHelper>)helper
{
	Ref<PickerViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		return instance->getHelper();
	}
	return sl_null;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView;
{
	return 1;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	Ref<PickerViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSelectItem(self, (sl_uint32)row);
	}
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;
{
	Ref<PickerViewHelper> helper = [self helper];
	if (helper.isNotNull()) {
		return (NSInteger)(helper->getItemsCount());
	}
	return 0;
}

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view{
	UILabel* label = (UILabel*)view;
	if (!label){
		label = [[UILabel alloc] init];
		label.font = m_font;
		label.textAlignment = NSTextAlignmentCenter;
	}
	Ref<PickerViewHelper> helper = [self helper];
	if (helper.isNotNull()) {
		label.text = helper->getItemTitle((sl_uint32)row);
	}
	return label;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
	UIFont* font = m_font;
	if (font != nil) {
		CGFloat h = font.lineHeight;
		if (h < 20) {
			h = 20;
		}
		return h;
	} else {
		return 20;
	}
}

@end

#endif
