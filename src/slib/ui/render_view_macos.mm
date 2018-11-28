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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"
#include "slib/core/thread.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_GLView : _priv_Slib_macOS_ViewHandle {
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	@public sl_bool m_flagUpdate;
	@public sl_bool m_flagViewVisible;
	@public CGFloat m_viewportWidth;
	@public CGFloat m_viewportHeight;

	@public slib::Mutex m_lockRender;
	
	slib::AtomicRef<slib::Thread> m_thread;
}

-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;
-(void)_queryViewStatus;

@end


namespace slib
{

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_GLView* handle = [[_priv_Slib_macOS_GLView alloc] initWithFrame:frame];
		if (handle != nil) {
			[handle _setRenderContinuously:(m_redrawMode == RedrawMode::Continuously)];
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		ObjectLocker lock(this);
		NSView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_priv_Slib_macOS_GLView class]]) {
			_priv_Slib_macOS_GLView* v = (_priv_Slib_macOS_GLView*)view;
			[v _setRenderContinuously:(mode == RedrawMode::Continuously)];
		}
	}

	void RenderView::_requestRender_NW()
	{
		NSView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_priv_Slib_macOS_GLView class]]) {
			_priv_Slib_macOS_GLView* v = (_priv_Slib_macOS_GLView*)view;
			[v _requestRender];
		}
	}

	void _priv_macOS_GLView_thread(__weak _priv_Slib_macOS_GLView* _handle)
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
		
		int frameNumber = 0;
		
		while (1) {
			
			sl_bool flagWorking = sl_false;
			
			if (Thread::isNotStoppingCurrent()) {
				
				_priv_Slib_macOS_GLView* handle = _handle;
				if (handle == nil) {
					return;
				}
				Ref<macOS_ViewInstance> instance = handle->m_viewInstance;
				if (instance.isNull()) {
					return;
				}
				
				if (frameNumber % 10 == 0) {
					dispatch_async(dispatch_get_main_queue(), ^{
						[handle _queryViewStatus];
					});
				}
				
				do {
					MutexLocker lock(&(handle->m_lockRender));
					if (!(handle->m_flagViewVisible)) {
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
					
					flagWorking = sl_true;
					if (flagUpdate) {
						sl_uint32 viewportWidth = (sl_uint32)(handle->m_viewportWidth);
						sl_uint32 viewportHeight = (sl_uint32)(handle->m_viewportHeight);
						if (viewportWidth != 0 && viewportHeight != 0) {
						
							[context makeCurrentContext];
							engine->setViewport(0, 0, viewportWidth, viewportHeight);
							
							Ref<macOS_ViewInstance> instance = handle->m_viewInstance;
							if (instance.isNotNull()) {
								Ref<View> _view = instance->getView();
								if (RenderView* view = CastInstance<RenderView>(_view.get())) {
									view->dispatchFrame(engine.get());
								}
							} else {
								return;
							}
					
							GL::flush();
							[context flushBuffer];
					
						}
					}
				} while(0);
				
				frameNumber++;
				
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

}

@implementation _priv_Slib_macOS_GLView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		m_flagRenderingContinuously = sl_false;
		m_flagRequestRender = sl_true;
		m_flagUpdate = sl_true;
		m_flagViewVisible = sl_false;
		m_viewportWidth = 1;
		m_viewportHeight = 1;
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_surfaceNeedsUpdate:) name:NSViewGlobalFrameDidChangeNotification object:self];
		m_thread = slib::Thread::start(slib::Function<void()>::bind(&(slib::_priv_macOS_GLView_thread), self));
	}
	return self;
}

-(void)dealloc
{
	slib::Ref<slib::Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->finishAndWait();
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
	slib::Ref<slib::Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->wake();
	}
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
}

-(void)_requestRender
{
	m_flagRequestRender = sl_true;
}

-(void)_queryViewStatus
{
	sl_bool flagVisible = sl_true;
	if ([self isHidden]) {
		flagVisible = sl_false;
	} else if (self.visibleRect.size.width < 1 || self.visibleRect.size.height < 1) {
		flagVisible = sl_false;
	} else if (self.window == nil) {
		flagVisible = sl_false;
	}
	m_flagViewVisible = flagVisible;
	NSRect rect = [self bounds];
	m_viewportWidth = rect.size.width;
	m_viewportHeight = rect.size.height;
}

@end

#endif
