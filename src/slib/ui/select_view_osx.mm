#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/select_view.h"

#include "view_osx.h"

@interface _Slib_OSX_SelectView : NSPopUpButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN

class _SelectView : public SelectView
{
public:
	void __applyItemsCount(NSPopUpButton* v)
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
					NSString* s = Apple::getNSStringFromString(m_titles.getValueAt(i));
					if (s == nil) {
						s = @"";
					}
					[item setTitle:s];
				}
			}
		}
	}
	
	void __copyItems(NSPopUpButton* v)
	{
		[v removeAllItems];
		__applyItemsCount(v);
		if (m_indexSelected >= m_titles.getCount()) {
			m_indexSelected = 0;
		}
		if ([v numberOfItems] > 0) {
			[v selectItemAtIndex:m_indexSelected];
		}
	}
	
	void __setItemTitle(NSPopUpButton* v, sl_uint32 index, const String& title)
	{
		NSString* s = Apple::getNSStringFromString(title);
		if (s == nil) {
			s = @"";
		}
		NSMenuItem* item = [v itemAtIndex:index];
		if (item != nil) {
			[item setTitle:s];
		}
	}
	
	sl_uint32 __getSelectedIndex(NSPopUpButton* v)
	{
		return (sl_uint32)([v indexOfSelectedItem]);
	}
	
	void __onSelectItem(NSPopUpButton* v)
	{
		onSelectItem(__getSelectedIndex(v));
	}
};

Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_SelectView* handle = [[_Slib_OSX_SelectView alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setPullsDown:NO];
		((_SelectView*)this)->__copyItems(handle);
		
		Ref<Font> font = getFont();
		NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

void SelectView::_getSelectedIndex_NW()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		m_indexSelected = ((_SelectView*)this)->__getSelectedIndex(v);
	}
}

void SelectView::_select_NW(sl_uint32 index)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		[v selectItemAtIndex:index];
	}
}

void SelectView::_refreshItemsCount_NW()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_SelectView*)this)->__applyItemsCount(v);
	}
}

void SelectView::_refreshItemsContent_NW()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_SelectView*)this)->__copyItems(v);
	}
}

void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_SelectView*)this)->__setItemTitle(v, index, title);
	}
}

void SelectView::_setFont_NW(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_SelectView
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
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			_view->__onSelectItem(self);
		}
	}
}
@end

#endif
