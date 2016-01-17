#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/core/app.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"

#include "view_ios.h"

#include <GLKit/GLKit.h>

@interface _Slib_iOS_GLView : GLKView {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	
	slib::Ref<slib::Thread> m_thread;
}

-(void)_init;
-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;

@end


SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> RenderView::createInstance(ViewInstance* _parent)
{
	_Slib_iOS_GLView* handle = nil;
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	handle = [[_Slib_iOS_GLView alloc] initWithFrame:frame];
	IOS_VIEW_CREATE_INSTANCE_END
	if (handle != nil && ret.isNotNull()) {
		[handle _init];
		[handle _setRenderContinuously:(m_redrawMode == redrawMode_Continuously)];
	}
	return ret;
}

void RenderView::setRedrawMode(RedrawMode mode)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_GLView class]]) {
		_Slib_iOS_GLView* v = (_Slib_iOS_GLView*)handle;
		[v _setRenderContinuously:(mode == redrawMode_Continuously)];
	}
	m_redrawMode = mode;
}

void RenderView::requestRender()
{
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
	
	void clear()
	{
		m_context = nil;
		m_engine.setNull();
	}
};

void _iOS_GLCallback(_Slib_iOS_GLView* handle)
{
	TimeCounter timer;
	
	_iOS_GLDesc desc;
	
	while (Thread::isNotStoppingCurrent()) {
		
		Ref<iOS_ViewInstance> instance = handle->m_viewInstance;
		if (instance.isNull()) {
			return;
		}
		
		if (handle.superview != nil && handle.hidden == NO && !(UIApp::isMobilePaused())) {
			
			if (desc.m_context == nil) {
				if (!(desc.create())) {
					return;
				}
			}
			
			handle.context = desc.m_context;
			
			sl_uint32 width = (sl_uint32)(handle.frame.size.width) * (handle.contentScaleFactor);
			sl_uint32 height = (sl_uint32)(handle.frame.size.height) * (handle.contentScaleFactor);
			if (width > 0 && height > 0) {
				
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
					if (RenderView::checkInstance(_view)) {
						RenderView* view = (RenderView*)(_view.get());
						view->dispatchOnFrame(desc.m_engine.get());
					}
					
					[handle display];

				}
			}
		} else {
			desc.clear();
		}
		
		instance.setNull();
		if (Thread::isNotStoppingCurrent()) {
			sl_uint64 t = timer.getEllapsedMilliseconds();
			if (t < 20) {
				Thread::sleep((sl_uint32)(20-t));
			}
			timer.reset();
		}
	}
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_GLView
-(void)_init
{
	m_flagRenderingContinuously = sl_false;
	m_flagRequestRender = sl_true;
	m_thread = slib::Thread::start(SLIB_CALLBACK(slib::_iOS_GLCallback, self));
}

-(void)dealloc
{
	if (m_thread.isNotNull()) {
		m_thread->finishAndWait();
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

- (void)setFrame:(CGRect)frame
{
	[super setFrame:frame];
}

IOS_VIEW_EVENTS

@end

#endif
