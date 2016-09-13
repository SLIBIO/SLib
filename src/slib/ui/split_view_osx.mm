#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/split_view.h"

#include "view_osx.h"

@interface _Slib_OSX_SplitView : Slib_OSX_ViewHandle {
	
}

@end

SLIB_UI_NAMESPACE_BEGIN

class _SplitView : public SplitView
{
public:
	sl_bool _hitTest(sl_ui_pos x, sl_ui_pos y)
	{
		ObjectLocker lock(this);
		sl_int32 index = _getDividerIndexAtPoint(UIPoint(x, y));
		return index >= 0;
	}
	
};

Ref<ViewInstance> SplitView::createGenericInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_SplitView* handle = [[_Slib_OSX_SplitView alloc] initWithFrame:frame];
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_SplitView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}

- (NSView *)hitTest:(NSPoint)aPoint
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::SplitView::checkInstance(view.ptr)) {
			if (((slib::_SplitView*)(view.ptr))->_hitTest((sl_ui_pos)(aPoint.x), (sl_ui_pos)(aPoint.y))) {
				return self;
			}
		}
	}
	return [super hitTest:aPoint];
}

@end

#endif
