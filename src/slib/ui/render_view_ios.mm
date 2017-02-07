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
	
	CADisplayLink* m_displayLink;
	slib::Ref<slib::RenderEngine> m_engine;
}

-(void)_init;
-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;

@end


SLIB_UI_NAMESPACE_BEGIN

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

SLIB_UI_NAMESPACE_END

@interface _Slib_iOS_GLViewDisplayLink : NSObject
{
	@public __weak _Slib_iOS_GLView* m_view;
}
@end

@implementation _Slib_iOS_GLViewDisplayLink

- (void)onGLRenderFrame
{
	_Slib_iOS_GLView* view = m_view;
	if (view != nil) {
		if (view->m_flagRenderingContinuously || view->m_flagRequestRender) {
			view->m_flagRequestRender = sl_false;
			[view setNeedsDisplay];
		}
	}
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
	
	_Slib_iOS_GLViewDisplayLink* link = [[_Slib_iOS_GLViewDisplayLink alloc] init];
	link->m_view = self;
	m_displayLink = [CADisplayLink displayLinkWithTarget:link selector:@selector(onGLRenderFrame)];
	[m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
}

-(void)dealloc
{
	[m_displayLink invalidate];
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

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
}

IOS_VIEW_EVENTS

@end

#endif
