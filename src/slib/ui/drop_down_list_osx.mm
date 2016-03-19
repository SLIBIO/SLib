#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/drop_down_list.h"

#include "view_osx.h"

@interface _Slib_OSX_DropDownList : NSPopUpButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN

class _DropDownList : public DropDownList
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
					NSString* s = Apple::getNSStringFromString(m_titles.getItemValue(i, String::null()));
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

Ref<ViewInstance> DropDownList::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_DropDownList* handle = [[_Slib_OSX_DropDownList alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setPullsDown:NO];
		((_DropDownList*)this)->__copyItems(handle);
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

sl_uint32 DropDownList::getSelectedIndex()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		return ((_DropDownList*)this)->__getSelectedIndex(v);
	}
	return m_indexSelected;
}

void DropDownList::_select(sl_uint32 index)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		[v selectItemAtIndex:index];
	}
	m_indexSelected = index;
}

void DropDownList::_refreshItemsCount()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_DropDownList*)this)->__applyItemsCount(v);
	}
}

void DropDownList::_refreshItemsContent()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_DropDownList*)this)->__copyItems(v);
	}
}

void DropDownList::_setItemTitle(sl_uint32 index, const String& title)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		((_DropDownList*)this)->__setItemTitle(v, index, title);
	}
}

void DropDownList::setFont(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
		NSPopUpButton* v = (NSPopUpButton*)handle;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_DropDownList
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
		if (slib::DropDownList::checkInstance(view.ptr)) {
			((slib::_DropDownList*)(view.ptr))->__onSelectItem(self);
		}
	}
}
@end

#endif
