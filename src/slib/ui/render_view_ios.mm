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
	
	slib::AtomicRef<slib::Thread> m_thread;
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
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setRedrawMode_NW(mode);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_GLView class]]) {
		_Slib_iOS_GLView* v = (_Slib_iOS_GLView*)handle;
		[v _setRenderContinuously:(mode == RedrawMode::Continuously)];
	}
}

void RenderView::_requestRender_NW()
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_requestRender_NW();
		});
		return;
	}
	
	UIView* view = UIPlatform::getViewHandle(this);
	if (view != nil && [view isKindOfClass:[_Slib_iOS_GLView class]]) {
		_Slib_iOS_GLView* v = (_Slib_iOS_GLView*)view;
		v->m_flagRequestRender = sl_true;
	}
}

struct _iOS_GLDesc
{
	EAGLContext* m_context = nil;
	Ref<RenderEngine> m_engine;
	
	sl_bool create()
	{
		EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
		if (context == nil) {
			context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		}
		if (context != nil) {
			Ref<RenderEngine> engine = GLES::createEngine();
			if (engine.isNotNull()) {
				m_context = context;
				m_engine = engine;
				return sl_true;
			}
		}
		return sl_false;
	}
};

void _iOS_GLCallback(__weak _Slib_iOS_GLView* _handle)
{
	TimeCounter timer;
	
	_iOS_GLDesc desc;
	
	while (1) {
		
		sl_bool flagWorking = sl_false;
		
		if (Thread::isNotStoppingCurrent()) {
			
			_Slib_iOS_GLView* handle = _handle;
			if (handle == nil) {
				return;
			}
			
			Ref<iOS_ViewInstance> instance = handle->m_viewInstance;
			if (instance.isNull()) {
				return;
			}
			
			if (handle.superview != nil && handle.hidden == NO && !(MobileApp::isPaused())) {
				
				if (desc.m_context == nil) {
					if (!(desc.create())) {
						return;
					}
					handle.context = desc.m_context;
				}
				
				sl_uint32 width = (sl_uint32)(handle.frame.size.width * handle.contentScaleFactor);
				sl_uint32 height = (sl_uint32)(handle.frame.size.height * handle.contentScaleFactor);
				
				if (width > 0 && height > 0) {
					
					flagWorking = sl_true;
					
					sl_bool flagUpdate = sl_false;
					if (handle->m_flagRenderingContinuously) {
						flagUpdate = sl_true;
					} else {
						if (handle->m_flagRequestRender) {
							flagUpdate = sl_true;
						}
					}
					handle->m_flagRequestRender = sl_false;
					
					if (flagUpdate) {
						
						[EAGLContext setCurrentContext:desc.m_context];
						
						desc.m_engine->setViewport(0, 0, width, height);
						
						Ref<View> _view = instance->getView();
						if (RenderView* view = CastInstance<RenderView>(_view.get())) {
							view->dispatchFrame(desc.m_engine.get());
						}
						
						[handle display];

					}
				}
			}
			
		} else {
			break;
		}
		
		if (Thread::isNotStoppingCurrent()) {
			if (flagWorking) {
				sl_uint64 t = timer.getElapsedMilliseconds();
				if (t < 10) {
					Thread::sleep(10 - (sl_uint32)(t));
				}
			} else {
				Thread::sleep(1000);
			}
			timer.reset();
		} else {
			break;
		}
		
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_GLView
-(void)_init
{
	self.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
	self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	
	m_flagRenderingContinuously = sl_false;
	m_flagRequestRender = sl_true;
	m_thread = slib::Thread::start(slib::Function<void()>::bind(&(slib::_iOS_GLCallback), self));
}

-(void)dealloc
{
	slib::Ref<slib::Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->finishAndWait();
	}
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
