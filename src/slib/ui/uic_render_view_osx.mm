#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/core/thread.h"

#include "ui_view_osx.h"

@interface _Slib_OSX_GLView : Slib_OSX_ViewHandle {
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	@public sl_bool m_flagUpdate;
	@public slib::Mutex m_lockRender;
	
	slib::Ref<slib::Thread> m_thread;
}

-(void)_setRenderContinuously:(BOOL)flag;

@end


SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> RenderView::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_GLView* handle = [[_Slib_OSX_GLView alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle _setRenderContinuously:(m_renderMode == RenderView::renderModeContinuously)];
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

void RenderView::setRenderMode(RenderView::RenderMode mode)
{
	MutexLocker lock(getLocker());
	NSView* view = UIPlatform::getViewHandle(this);
	if (view != nil && [view isKindOfClass:[_Slib_OSX_GLView class]]) {
		_Slib_OSX_GLView* v = (_Slib_OSX_GLView*)view;
		[v _setRenderContinuously:(mode == RenderView::renderModeContinuously)];
	}
	m_renderMode = mode;
}

void RenderView::requestRender()
{
	NSView* view = UIPlatform::getViewHandle(this);
	if (view != nil && [view isKindOfClass:[_Slib_OSX_GLView class]]) {
		_Slib_OSX_GLView* v = (_Slib_OSX_GLView*)view;
		v->m_flagRequestRender = sl_true;
	}
}

void _Ui_OSX_GLView_thread(_Slib_OSX_GLView* handle)
{
	Ref<RenderEngine> engine = GL::createEngine();
	if (engine.isNull()) {
		return;
	}
	
	TimeCounter timer;
	NSOpenGLContext* context = nil;
	
	NSOpenGLPixelFormatAttribute attrs[] = {
		NSOpenGLPFADoubleBuffer
		, NSOpenGLPFAColorSize, 24
		, NSOpenGLPFAAlphaSize, 8
		, NSOpenGLPFADepthSize, 24
		, NSOpenGLPFAStencilSize, 8
		, 0
	};
	NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	if (pf == nil) {
		return;
	}
	
	while (Thread::isNotStoppingCurrent()) {
		do {
			MutexLocker lock(&(handle->m_lockRender));
			if ([handle isHidden] || handle.window == nil) {
				if (context != nil) {
					[context clearDrawable];
					context = nil;
					handle->m_flagUpdate = sl_true;
				}
				break;
			}
			if (context == nil) {
				context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
				if (context == nil) {
					return;
				}
				[context setView: handle];
			}
			if (handle->m_flagUpdate) {
				[context update];
				handle->m_flagUpdate = sl_false;
			}
			sl_bool flagUpdate = sl_false;
			if (handle->m_flagRenderingContinuously) {
				flagUpdate = sl_true;
			} else {
				if (handle->m_flagRequestRender) {
					flagUpdate = sl_true;
				}
			}
			lock.unlock();
			handle->m_flagRequestRender = sl_false;
			if (flagUpdate) {
				NSRect rect = [handle bounds];
				if (rect.size.width != 0 && rect.size.height != 0) {
				
					[context makeCurrentContext];
					engine->setViewport(0, 0, (sl_uint32)(rect.size.width), (sl_uint32)(rect.size.height));
					
					Ref<OSX_ViewInstance> instance = handle->m_viewInstance.lock();
					if (instance.isNotNull()) {
						Ref<View> _view = instance->getView();
						if (RenderView::checkInstance(_view)) {
							RenderView* view = (RenderView*)(_view.get());
							view->dispatchOnFrame(engine.get());
						}
					} else {
						return;
					}
			
					GL::flush();
					[context flushBuffer];
			
				}
			}
		} while(0);
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

@implementation _Slib_OSX_GLView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		m_flagRenderingContinuously = sl_false;
		m_flagRequestRender = sl_true;
		m_flagUpdate = sl_true;
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_surfaceNeedsUpdate:) name:NSViewGlobalFrameDidChangeNotification object:self];
	}
	return self;
}

-(void)dealloc
{
	if (m_thread.isNotNull()) {
		m_thread->finishAndWait();
	}
}

- (BOOL)acceptsFirstResponder
{
	return TRUE;
}

- (void)drawRect:(NSRect)dirtyRect
{
	m_flagRequestRender = sl_true;
}

- (void)setFrame:(NSRect)frame
{
	slib::MutexLocker lock(&m_lockRender);
	[super setFrame: frame];
	[self updateTrackingAreas];
	m_flagUpdate = sl_true;
}

- (void)updateTrackingAreas
{
	if (m_trackingArea != nil) {
		[self removeTrackingArea:m_trackingArea];
	}
	m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingCursorUpdate| NSTrackingActiveInKeyWindow) owner:self userInfo:nil];
	if (m_trackingArea != nil) {
		[self addTrackingArea:m_trackingArea];
	}
}

-(void) _surfaceNeedsUpdate:(NSNotification*)notification
{
	m_flagUpdate = sl_true;
}

-(void)_setRenderContinuously:(BOOL)flag
{
	if (flag == m_flagRenderingContinuously) {
		return;
	}
	m_flagRenderingContinuously = flag;
	if (flag) {
		if (m_thread.isNull()) {
			m_thread = slib::Thread::start(SLIB_CALLBACK(slib::_Ui_OSX_GLView_thread, self));
		}
	}
}
@end

#endif
