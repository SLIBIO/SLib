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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/select_view.h"

#include "view_macos.h"

namespace slib
{
	namespace priv
	{
		namespace select_view
		{
			class SelectViewInstance;
		}
	}
}

@interface SLIBSelectViewHandle : NSPopUpButton
{	
	@public slib::WeakRef<slib::priv::select_view::SelectViewInstance> m_viewInstance;
}
@end

namespace slib
{

	namespace priv
	{
		namespace select_view
		{

			class SelectViewHelper : public SelectView
			{
			public:
				void applyItemsCount(NSPopUpButton* v)
				{
					ObjectLocker lock(this);
					sl_uint32 nOrig = (sl_uint32)([v numberOfItems]);
					sl_uint32 nNew = (sl_uint32)(m_titles.getCount());
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						if (nNew  > 0) {
							for (sl_uint32 i = nOrig; i > nNew; i--) {
								[v removeItemAtIndex:(i - 1)];
							}
						} else {
							[v removeAllItems];
						}
					} else {
						for (sl_uint32 i = nOrig; i < nNew; i++) {
							[v addItemWithTitle:[NSString stringWithFormat:@"%d",i]];
							NSMenuItem* item = [v lastItem];
							if (item != nil) {
								[item setTitle:Apple::getNSStringFromString(m_titles.getValueAt(i), @"")];
							}
						}
					}
				}
				
				void copyItems(NSPopUpButton* v)
				{
					[v removeAllItems];
					applyItemsCount(v);
					if (m_indexSelected >= m_titles.getCount()) {
						m_indexSelected = 0;
					}
					if ([v numberOfItems] > 0) {
						[v selectItemAtIndex:m_indexSelected];
					}
				}
				
			};
			
			class SelectViewInstance : public macOS_ViewInstance, public ISelectViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				NSPopUpButton* getHandle()
				{
					return (NSPopUpButton*)m_handle;
				}
				
				void select(SelectView* view, sl_uint32 index) override
				{
					NSPopUpButton* handle = getHandle();
					if (handle != nil) {
						[handle selectItemAtIndex:index];
					}
				}
				
				void refreshItemsCount(SelectView* view) override
				{
					NSPopUpButton* handle = getHandle();
					if (handle != nil) {
						static_cast<SelectViewHelper*>(view)->applyItemsCount(handle);
					}
				}
				
				void refreshItemsContent(SelectView* view) override
				{
					NSPopUpButton* handle = getHandle();
					if (handle != nil) {
						static_cast<SelectViewHelper*>(view)->copyItems(handle);
					}
				}
				
				void setItemTitle(SelectView* view, sl_uint32 index, const String& title) override
				{
					NSPopUpButton* handle = getHandle();
					if (handle != nil) {
						NSMenuItem* item = [handle itemAtIndex:index];
						if (item != nil) {
							[item setTitle:Apple::getNSStringFromString(title, @"")];
						}
					}
				}
				
				sl_bool measureSize(SelectView* view, UISize& _out) override
				{
					return UIPlatform::measureNativeWidgetFittingSize(this, _out);
				}
				
				void onSelectItem(NSPopUpButton* handle)
				{
					Ref<SelectView> view = CastRef<SelectView>(getView());
					if (view.isNotNull()) {
						view->dispatchSelectItem((sl_uint32)([handle indexOfSelectedItem]));
					}
				}
			};

			SLIB_DEFINE_OBJECT(SelectViewInstance, macOS_ViewInstance)
			
		}
	}

	using namespace priv::select_view;

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		Ref<SelectViewInstance> ret = macOS_ViewInstance::create<SelectViewInstance, SLIBSelectViewHandle>(this, parent);
		if (ret.isNotNull()) {
			NSPopUpButton* handle = ret->getHandle();
			[handle setPullsDown:NO];
			static_cast<SelectViewHelper*>(this)->copyItems(handle);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<ISelectViewInstance> SelectView::getSelectViewInstance()
	{
		return CastRef<SelectViewInstance>(getViewInstance());
	}

}

using namespace slib;
using namespace slib::priv::select_view;

@implementation SLIBSelectViewHandle

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setAction: @selector(onSelect)];
		[self setTarget:self];
	}
	return self;
}

-(void)onSelect
{
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSelectItem(self);
	}
}

@end

#endif
