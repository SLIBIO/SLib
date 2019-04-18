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

#include "slib/ui/core.h"
#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"
#include "slib/ui/mobile_app.h"

#include "view_ios.h"

#include <GLKit/GLKit.h>

@interface _priv_Slib_iOS_GLView : GLKView {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	
	id m_renderer;
	slib::Ref<slib::RenderEngine> m_engine;
	CADisplayLink* m_displayLink;
	
	CGFloat m_viewportWidth;
	CGFloat m_viewportHeight;
}

-(void)_init;
-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;

@end


namespace slib
{
	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		_priv_Slib_iOS_GLView* handle = nil;
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		handle = [[_priv_Slib_iOS_GLView alloc] initWithFrame:frame];
		IOS_VIEW_CREATE_INSTANCE_END
		if (handle != nil && ret.isNotNull()) {
			[handle _init];
			[handle _setRenderContinuously:(m_redrawMode == RedrawMode::Continuously)];
		}
		return ret;
	}
	
	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_GLView class]]) {
			_priv_Slib_iOS_GLView* v = (_priv_Slib_iOS_GLView*)handle;
			[v _setRenderContinuously:(mode == RedrawMode::Continuously)];
		}
	}
	
	void RenderView::_requestRender_NW()
	{
		UIView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_priv_Slib_iOS_GLView class]]) {
			_priv_Slib_iOS_GLView* v = (_priv_Slib_iOS_GLView*)view;
			[v _requestRender];
		}
	}	
}

@interface SLib_iOS_GLViewRenderer : NSObject
{
	@public __weak _priv_Slib_iOS_GLView* m_view;
	@public __weak CADisplayLink* m_displayLink;
	
	BOOL m_flagRunning;
	BOOL m_flagViewVisible;
	int m_frameNumber;
}
@end

@implementation SLib_iOS_GLViewRenderer

- (void)onGLRenderFrame
{
	if (!m_flagRunning) {
		return;
	}
	_priv_Slib_iOS_GLView* view = m_view;
	if (view != nil) {
		if (m_frameNumber % 10 == 0) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[self queryViewState];
			});
		}
		if (m_flagViewVisible && !(slib::MobileApp::isPaused())) {
			if (view->m_flagRenderingContinuously || view->m_flagRequestRender) {
				view->m_flagRequestRender = sl_false;
				[view display];
			}
		}
	}
	m_frameNumber++;
}

- (void)queryViewState
{
	_priv_Slib_iOS_GLView* view = m_view;
	if (view == nil) {
		return;
	}
	BOOL flagVisible = NO;
	sl_uint32 width = (sl_uint32)(view.bounds.size.width * view.contentScaleFactor);
	sl_uint32 height = (sl_uint32)(view.bounds.size.height * view.contentScaleFactor);
	view->m_viewportWidth = width;
	view->m_viewportHeight = height;
	if (width > 0 && height > 0) {
		UIView* viewCheck = view;
		for (;;) {
			if (viewCheck.hidden) {
				break;
			} else {
				UIView* parent = viewCheck.superview;
				if (parent == nil) {
					flagVisible = YES;
					break;
				}
				viewCheck = parent;
			}
		}
	}
	m_flagViewVisible = flagVisible;
}

- (void)onRun
{
	m_flagRunning = YES;
	m_flagViewVisible = NO;
	m_frameNumber = 0;
#if defined(SLIB_PLATFORM_IS_IOS_SIMULATOR)
	slib::TimeCounter timer;
	while (m_flagRunning) {
		[self onGLRenderFrame];
		sl_uint64 t = timer.getElapsedMilliseconds();
		if (t < 15) {
			[NSThread sleepForTimeInterval:(15 - (sl_uint32)(t)) / 1000.0];
		}
		timer.reset();
	}
#else
	NSRunLoop* loop = [NSRunLoop currentRunLoop];
	[m_displayLink addToRunLoop:loop forMode:NSDefaultRunLoopMode];
	[loop run];
#endif
}

- (void)stop
{
	m_flagRunning = NO;
}

@end

@implementation _priv_Slib_iOS_GLView

-(void)_init
{
	m_flagRenderingContinuously = sl_false;
	m_flagRequestRender = sl_true;
	m_viewportWidth = 0;
	m_viewportHeight = 0;
	
	EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
	if (context == nil) {
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	}
	if (context != nil) {
		self.context = context;
	}
	
	self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
	self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	
	SLib_iOS_GLViewRenderer* renderer = [[SLib_iOS_GLViewRenderer alloc] init];
	renderer->m_view = self;
	m_displayLink = [CADisplayLink displayLinkWithTarget:renderer selector:@selector(onGLRenderFrame)];
	renderer->m_displayLink = m_displayLink;
	[NSThread detachNewThreadSelector:@selector(onRun) toTarget:renderer withObject:nil];
	m_renderer = renderer;
	
}

-(void)dealloc
{
	[m_displayLink invalidate];
	[m_renderer stop];
}

-(void)_requestRender
{
	m_flagRequestRender = sl_true;
}

-(void)_setRenderContinuously:(BOOL)flag
{
	m_flagRenderingContinuously = flag;
}

- (void)drawRect:(CGRect)dirtyRect
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNull()) {
		return;
	}
	
	slib::Ref<slib::View> _view = instance->getView();
	if (slib::RenderView* view = slib::CastInstance<slib::RenderView>(_view.get())) {
		if (m_engine.isNull()) {
			m_engine = slib::GLES::createEngine();
		}
		if (m_engine.isNotNull()) {
			m_engine->setViewport(0, 0, m_viewportWidth, m_viewportHeight);
			view->dispatchFrame(m_engine.get());
		}
	}

}

- (void)setNeedsDisplay
{
}

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
}

IOS_VIEW_EVENTS

@end

#endif
