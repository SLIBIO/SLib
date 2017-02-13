#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/ui/mobile_app.h"

#include "view_ios.h"

#include <GLKit/GLKit.h>

@interface _Slib_iOS_GLView : GLKView {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	
	id m_renderer;
	slib::Ref<slib::RenderEngine> m_engine;
	CADisplayLink* m_displayLink;
}

-(void)_init;
-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;

@end


namespace slib
{
	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		_Slib_iOS_GLView* handle = nil;
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		handle = [[_Slib_iOS_GLView alloc] initWithFrame:frame];
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
		if (handle != nil && [handle isKindOfClass:[_Slib_iOS_GLView class]]) {
			_Slib_iOS_GLView* v = (_Slib_iOS_GLView*)handle;
			[v _setRenderContinuously:(mode == RedrawMode::Continuously)];
		}
	}
	
	void RenderView::_requestRender_NW()
	{
		UIView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_Slib_iOS_GLView class]]) {
			_Slib_iOS_GLView* v = (_Slib_iOS_GLView*)view;
			[v _requestRender];
		}
	}	
}

@interface _Slib_iOS_GLViewRenderer : NSObject
{
	@public __weak _Slib_iOS_GLView* m_view;
	@public __weak CADisplayLink* m_displayLink;
	
	BOOL m_flagRunning;
}
@end

@implementation _Slib_iOS_GLViewRenderer

- (void)onGLRenderFrame
{
	if (!m_flagRunning) {
		return;
	}
	_Slib_iOS_GLView* view = m_view;
	if (view != nil) {
		if (view.superview != nil && view.hidden == NO && !(slib::MobileApp::isPaused())) {
			if (view->m_flagRenderingContinuously || view->m_flagRequestRender) {
				view->m_flagRequestRender = sl_false;
				sl_uint32 width = (sl_uint32)(view.frame.size.width * view.contentScaleFactor);
				sl_uint32 height = (sl_uint32)(view.frame.size.height * view.contentScaleFactor);
				if (width > 0 && height > 0) {
					[view display];
				}
			}
		}
	}
}

- (void)onRun
{
	m_flagRunning = YES;
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

@implementation _Slib_iOS_GLView

-(void)_init
{
	m_flagRenderingContinuously = sl_false;
	m_flagRequestRender = sl_true;
	

	EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
	if (context == nil) {
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	}
	if (context != nil) {
		self.context = context;
	}
	
	self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
	self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	
	_Slib_iOS_GLViewRenderer* renderer = [[_Slib_iOS_GLViewRenderer alloc] init];
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
		sl_uint32 width = (sl_uint32)(self.frame.size.width * self.contentScaleFactor);
		sl_uint32 height = (sl_uint32)(self.frame.size.height * self.contentScaleFactor);
		if (m_engine.isNull()) {
			m_engine = slib::GLES::createEngine();
		}
		if (m_engine.isNotNull()) {
			m_engine->setViewport(0, 0, width, height);
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
