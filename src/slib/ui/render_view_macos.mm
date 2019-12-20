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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/render_view.h"

#include "slib/render/opengl.h"
#include "slib/core/thread.h"

#include "view_macos.h"

@interface SLIBGLViewHandle : SLIBViewHandle
{
	@public sl_bool m_flagRenderingContinuously;
	@public sl_bool m_flagRequestRender;
	@public sl_bool m_flagUpdate;
	@public sl_bool m_flagViewVisible;
	@public CGFloat m_viewportWidth;
	@public CGFloat m_viewportHeight;

	@public slib::Mutex m_lockRender;
	
	slib::AtomicRef<slib::Thread> m_thread;
}

-(void)queryViewStatus;

@end

namespace slib
{
	
	namespace priv
	{
		namespace render_view
		{
			
			class RenderViewInstance : public macOS_ViewInstance, public IRenderViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBGLViewHandle* getHandle()
				{
					return (SLIBGLViewHandle*)m_handle;
				}
				
				void setRedrawMode(RenderView* view, RedrawMode mode) override
				{
					SLIBGLViewHandle* handle = getHandle();
					if (handle != nil) {
						handle->m_flagRenderingContinuously = mode == RedrawMode::Continuously;
					}
				}
				
				void requestRender(RenderView* view) override
				{
					SLIBGLViewHandle* handle = getHandle();
					if (handle != nil) {
						handle->m_flagRequestRender = sl_true;
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(RenderViewInstance, ViewInstance)
			
		}
	}
	
	using namespace priv::render_view;

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
	{
		Ref<RenderViewInstance> ret = macOS_ViewInstance::create<RenderViewInstance, SLIBGLViewHandle>(this, parent);
		if (ret.isNotNull()) {
			ret->setRedrawMode(this, getRedrawMode());
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IRenderViewInstance> RenderView::getRenderViewInstance()
	{
		return CastRef<RenderViewInstance>(getViewInstance());
	}

	namespace priv
	{
		namespace gl_view
		{
			static void RenderThreadProc(__weak SLIBGLViewHandle* _handle)
			{
				Ref<RenderEngine> engine = GL::createEngine();
				if (engine.isNull()) {
					return;
				}
				
				sl_bool flagFirstFrame = sl_true;
				
				TimeCounter timer;
				NSOpenGLContext* context = nil;
				
				NSOpenGLPixelFormatAttribute attrs[] = {
					NSOpenGLPFADoubleBuffer,
					NSOpenGLPFAColorSize, 24,
					NSOpenGLPFAAlphaSize, 8,
					NSOpenGLPFADepthSize, 24,
					NSOpenGLPFAStencilSize, 8,
					0
				};
				NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
				if (pf == nil) {
					return;
				}
				
				int frameNumber = 0;
				
				while (1) {
					
					sl_bool flagWorking = sl_false;
					
					Ref<Thread> thread = Thread::getCurrent();
					if (thread.isNull() || thread->isNotStopping()) {
						
						SLIBGLViewHandle* handle = _handle;
						if (handle == nil) {
							return;
						}
						Ref<macOS_ViewInstance> instance = handle->m_viewInstance;
						if (instance.isNull()) {
							return;
						}
						
						if (frameNumber % 10 == 0) {
							dispatch_async(dispatch_get_main_queue(), ^{
								[handle queryViewStatus];
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
								dispatch_async(dispatch_get_main_queue(), ^{
									[context setView: handle];
								});
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
									
									Ref<macOS_ViewInstance> instance = handle->m_viewInstance;
									if (instance.isNotNull()) {
										Ref<View> _view = instance->getView();
										if (RenderView* view = CastInstance<RenderView>(_view.get())) {
											if (flagFirstFrame) {
												flagFirstFrame = sl_false;
												view->dispatchCreateEngine(engine.get());
											}
											engine->setViewport(0, 0, viewportWidth, viewportHeight);
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
					
					if (thread.isNull() || thread->isNotStopping()) {
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
	}

}

@implementation SLIBGLViewHandle

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
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(surfaceNeedsUpdate:) name:NSViewGlobalFrameDidChangeNotification object:self];
		__weak SLIBGLViewHandle* weak = self;
		m_thread = slib::Thread::start(slib::Function<void()>::bind(&(slib::priv::gl_view::RenderThreadProc), weak));
	}
	return self;
}

-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
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
	m_flagUpdate = sl_true;
	slib::Ref<slib::Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->wake();
	}
}

-(void)surfaceNeedsUpdate:(NSNotification*)notification
{
	m_flagUpdate = sl_true;
}

-(void)queryViewStatus
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
