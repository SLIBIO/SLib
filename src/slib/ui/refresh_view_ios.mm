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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/refresh_view.h"

#include "slib/ui/core.h"

#include "view_ios.h"

@interface SLIBRefreshViewHandle: UIRefreshControl
{
	@public slib::WeakRef<slib::RefreshView> m_view;
}

-(void)handleRefresh;

@end

namespace slib
{
	
	namespace priv
	{
		namespace refresh_view
		{

			class PlatformContainer : public Referable
			{
			public:
				UIRefreshControl* m_refreshControl;
				
			};
			
			class RefreshViewHelper : public RefreshView
			{
			public:
				UIRefreshControl* createControl()
				{
					SLIBRefreshViewHandle* control = [[SLIBRefreshViewHandle alloc] init];
					if (control != nil) {
						control->m_view = this;
						[control addTarget:control action:@selector(handleRefresh) forControlEvents:UIControlEventValueChanged];
						PlatformContainer* container = (PlatformContainer*)(m_platformContainer.get());
						if (!container) {
							Ref<PlatformContainer> _container = new PlatformContainer;
							container = _container.get();
							m_platformContainer = container;
						}
						if (container) {
							container->m_refreshControl = control;
						}
						return control;
					}
					return nil;
				}
				
				UIRefreshControl* getControl()
				{
					PlatformContainer* container = (PlatformContainer*)(m_platformContainer.get());
					if (container) {
						return container->m_refreshControl;
					}
					return nil;
				}
				
				void _onRefresh()
				{
					_onRefresh_NW();
				}
				
			};

		}
	}

	using namespace priv::refresh_view;

	void RefreshView::onAttachChild(View* child)
	{
		UIView* handle = UIPlatform::getViewHandle(child);
		if ([handle isKindOfClass:[UIScrollView class]]) {
			UIScrollView* scrollView = (UIScrollView*)handle;
			UIRefreshControl* control = ((RefreshViewHelper*)this)->createControl();
			if (control != nil) {
				if (@available(iOS 10.0, *)) {
					scrollView.refreshControl = control;
				} else {
					[scrollView addSubview:control];
				}
			}
		}
	}
	
	void RefreshView::_setRefreshing_NW(sl_bool flag)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), RefreshView, _setRefreshing_NW, this, flag));
			return;
		}
		UIRefreshControl* control = ((RefreshViewHelper*)this)->getControl();
		if (control != nil) {
			if (flag) {
				if (!(control.refreshing)) {
					[control beginRefreshing];
				}
			} else {
				if (control.refreshing) {
					[control endRefreshing];
				}
			}
		}
	}
	
}

using namespace slib;
using namespace slib::priv::refresh_view;

@implementation SLIBRefreshViewHandle

-(void)handleRefresh
{
	Ref<RefreshView> view(m_view);
	if (view.isNotNull()) {
		((RefreshViewHelper*)(view.get()))->_onRefresh();
	}
}

@end

#endif
