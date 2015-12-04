#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/tab_view.h"

#include "view_osx.h"

@interface _Slib_OSX_TabView : NSTabView<NSTabViewDelegate> {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

@interface _Slib_OSX_TabView_EmptyView : NSView {
}
@end

SLIB_UI_NAMESPACE_BEGIN
class _TabView : public TabView
{
public:
	void __applyTabsCount(NSTabView* tv)
	{
		MutexLocker lock(getLocker());
		sl_uint32 nNew = (sl_uint32)(m_items.getCount());
		sl_uint32 nOrig = (sl_uint32)([tv numberOfTabViewItems]);
		if (nOrig == nNew) {
			return;
		}
		if (nOrig > nNew) {
			for (sl_uint32 i = nOrig; i > nNew; i--) {
				NSTabViewItem* item = [tv tabViewItemAtIndex:(i - 1)];
				if (item != nil) {
					[tv removeTabViewItem:item];
				}
			}
		} else {
			for (sl_uint32 i = nOrig; i < nNew; i++) {
				NSTabViewItem* item = [[NSTabViewItem alloc] initWithIdentifier:[NSString stringWithFormat:@"%d",i]];
				if (item != nil) {
					[tv addTabViewItem:item];
				}
			}
		}
	}
	
	void __copyTabs(NSTabView* tv)
	{
		ListLocker<TabViewItem> items(m_items);
		__applyTabsCount(tv);
		for (sl_uint32 i = 0; i < items.count(); i++) {
			NSTabViewItem* t = [tv tabViewItemAtIndex:i];
			if (t != nil) {
				[t setLabel:Apple::getNSStringFromString(items[i].label)];
				__setTabContentView(tv, i, items[i].contentView);
			}
		}
		if ([tv numberOfTabViewItems] > 0) {
			[tv selectTabViewItemAtIndex:m_indexSelected];
		}
	}
	
	void __setTabContentView(NSTabView* tv, sl_uint32 index, const Ref<View>& _view)
	{
		Ref<View> view = _view;
		NSTabViewItem* item = [tv tabViewItemAtIndex:index];
		if (item == nil) {
			return;
		}
		NSView* handle = nil;
		if (view.isNotNull()) {
			Ref<ViewInstance> instance = view->attachToNewInstance(Ref<ViewInstance>::null());
			if (instance.isNotNull()) {
				handle = UIPlatform::getViewHandle(instance.get());
			}
			NSRect rc = [tv contentRect];
			view->setFrame((sl_real)(rc.origin.x), (sl_real)(rc.origin.y), (sl_real)(rc.size.width), (sl_real)(rc.size.height));
		}
		if (handle == nil) {
			handle = [[_Slib_OSX_TabView_EmptyView alloc] init];
		}
		[item setView:handle];
	}
	
	void __onSelectTab(NSTabView* tv)
	{
		onSelectTab((sl_uint32)([tv indexOfTabViewItem:[tv selectedTabViewItem]]));
	}
	
	void __updateContentViewSize(_Slib_OSX_TabView* tv)
	{
		NSRect rc = [tv contentRect];
		Rectangle frame;
		sl_real w = (sl_real)(rc.size.width);
		sl_real h = (sl_real)(rc.size.height);
		frame.left = (sl_real)(rc.origin.x);
		frame.top = (sl_real)(rc.origin.y);
		frame.right = frame.left + w;
		frame.bottom = frame.top + h;
		ListLocker<TabViewItem> items(m_items);
		for (sl_size i = 0; i < items.count(); i++) {
			Ref<View> view = items[i].contentView;
			if (view.isNotNull()) {
				view->setFrame(frame);
			}
		}
	}
};

Ref<ViewInstance> TabView::createInstance(ViewInstance* _parent)
{
	MutexLocker lock(getLocker());
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_TabView* handle = [[_Slib_OSX_TabView alloc] initWithFrame: frame];
	if (handle != nil) {
		[handle setDelegate:handle];
		((_TabView*)this)->__copyTabs(handle);
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		[handle setFont:hFont];
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

void TabView::_refreshTabsCount()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TabView class]]) {
		_Slib_OSX_TabView* tv = (_Slib_OSX_TabView*)handle;
		((_TabView*)this)->__applyTabsCount(tv);
	}
}

void TabView::_setTabLabel(sl_uint32 index, const String& text)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
		NSTabView* tv = (NSTabView*)handle;
		NSTabViewItem* t = [tv tabViewItemAtIndex:index];
		if (t != nil) {
			[t setLabel:Apple::getNSStringFromString(text)];
		}
	}
}

void TabView::_setTabContentView(sl_uint32 index, const Ref<View>& view)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TabView class]]) {
		_Slib_OSX_TabView* tv = (_Slib_OSX_TabView*)handle;
		((_TabView*)this)->__setTabContentView(tv, index, view);
	}
}

sl_uint32 TabView::getSelectedTabIndex()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
		NSTabView* tv = (NSTabView*)handle;
		m_indexSelected = (sl_uint32)([tv indexOfTabViewItem:[tv selectedTabViewItem]]);
	}
	return m_indexSelected;
}

void TabView::_selectTab(sl_uint32 index)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
		NSTabView* tv = (NSTabView*)handle;
		[tv selectTabViewItemAtIndex:index];
	}
}

Size TabView::getContentViewSize()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
		NSTabView* tv = (NSTabView*)handle;
		NSRect rc = [tv contentRect];
		Size ret;
		ret.width = rc.size.width;
		ret.height = rc.size.height;
		return ret;
	}
	return Size::zero();
}

void TabView::setFont(const Ref<Font>& _font)
{
	Ref<Font> font = _font;
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
		NSTabView* tv = (NSTabView*)handle;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		[tv setFont:hFont];
	}
	m_font = font;
}

void TabView::onResize()
{
	View::onResize();
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_TabView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}

-(void)setFrame:(NSRect)frame
{
	[super setFrame:frame];
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::TabView::checkInstance(view)) {
			((slib::_TabView*)(view.get()))->__updateContentViewSize(self);
		}
	}
}

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::TabView::checkInstance(view)) {
			((slib::_TabView*)(view.get()))->__onSelectTab(self);
		}
	}
}
@end

@implementation _Slib_OSX_TabView_EmptyView

@end

#endif
