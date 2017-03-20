/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/render_view.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/core/thread.h"

#include "view_osx.h"

@interface _Slib_OSX_GLView : Slib_OSX_ViewHandle {
	
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	@public sl_bool m_flagUpdate;
	@public slib::Mutex m_lockRender;
	
	slib::AtomicRef<slib::Thread> m_thread;
}

-(void)_setRenderContinuously:(BOOL)flag;
-(void)_requestRender;

@end


namespace slib
{

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* _parent)
	{
		OSX_VIEW_CREATE_INSTANCE_BEGIN
		_Slib_OSX_GLView* handle = [[_Slib_OSX_GLView alloc] initWithFrame:frame];
		if (handle != nil) {
			[handle _setRenderContinuously:(m_redrawMode == RedrawMode::Continuously)];
		}
		OSX_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		ObjectLocker lock(this);
		NSView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_Slib_OSX_GLView class]]) {
			_Slib_OSX_GLView* v = (_Slib_OSX_GLView*)view;
			[v _setRenderContinuously:(mode == RedrawMode::Continuously)];
		}
	}

	void RenderView::_requestRender_NW()
	{
		NSView* view = UIPlatform::getViewHandle(this);
		if (view != nil && [view isKindOfClass:[_Slib_OSX_GLView class]]) {
			_Slib_OSX_GLView* v = (_Slib_OSX_GLView*)view;
			[v _requestRender];
		}
	}

	void _Ui_OSX_GLView_thread(__weak _Slib_OSX_GLView* _handle)
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
		
		while (1) {
			
			sl_bool flagWorking = sl_false;
			
			if (Thread::isNotStoppingCurrent()) {
				
				_Slib_OSX_GLView* handle = _handle;
				if (handle == nil) {
					return;
				}
				Ref<OSX_ViewInstance> instance = handle->m_viewInstance;
				if (instance.isNull()) {
					return;
				}
				
				do {
					MutexLocker lock(&(handle->m_lockRender));
					if ([handle isHidden] || handle.visibleRect.size.width < 1 || handle.visibleRect.size.height < 1 || handle.window == nil) {
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
						NSRect rect = [handle bounds];
						if (rect.size.width != 0 && rect.size.height != 0) {
						
							[context makeCurrentContext];
							engine->setViewport(0, 0, (sl_uint32)(rect.size.width), (sl_uint32)(rect.size.height));
							
							Ref<OSX_ViewInstance> instance = handle->m_viewInstance;
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

@implementation _Slib_OSX_GLView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		m_flagRenderingContinuously = sl_false;
		m_flagRequestRender = sl_true;
		m_flagUpdate = sl_true;
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_surfaceNeedsUpdate:) name:NSViewGlobalFrameDidChangeNotification object:self];
		m_thread = slib::Thread::start(slib::Function<void()>::bind(&(slib::_Ui_OSX_GLView_thread), self));
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

@end

#endif
