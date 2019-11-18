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

#include "slib/ui/render_view.h"

#include "slib/render/canvas.h"
#include "slib/core/thread.h"
#include "slib/core/dispatch.h"

namespace slib
{

	namespace priv
	{
		namespace render_view
		{

			class AnimationLoopImpl : public AnimationLoop
			{
			public:
				WeakRef<RenderView> m_view;
				
			public:
				AnimationLoopImpl(RenderView* view) : m_view(view)
				{
				}
				
			public:
				void _wake() override
				{
					Ref<RenderView> view(m_view);
					if (view.isNotNull()) {
						view->requestRender();
					}
				}
				
				void runStep()
				{
					sl_int32 n = _runStep();
					if (n >= 0) {
						_wake();
					}
				}
				
			};

			class DispatcherImpl : public Dispatcher
			{
			public:
				WeakRef<RenderView> m_view;
				
			public:
				sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms) override
				{
					Ref<RenderView> view(m_view);
					if (view.isNotNull()) {
						if (delay_ms > 0x7fffffff) {
							delay_ms = 0x7fffffff;
						}
						view->dispatchToDrawingThread(callback, (sl_uint32)delay_ms);
						return sl_true;
					}
					return sl_false;
				}
			};

		}
	}

	SLIB_DEFINE_OBJECT(RenderView, ViewGroup)

	RenderView::RenderView()
	{		
		setCreatingNativeWidget(sl_true);
		setCreatingChildInstances(sl_false);
		
		setPreferredEngineType(RenderEngineType::OpenGL_ES);
		
		m_redrawMode = RedrawMode::Continuously;
		m_flagDispatchEventsToRenderingThread = sl_false;
		
		m_lastRenderingThreadId = 0;
		
		m_flagDebugTextVisible = sl_false;
		m_flagDebugTextVisibleOnRelease = sl_false;
		
		setBackgroundColor(Color::Black, UIUpdateMode::Init);
	}

	RenderView::~RenderView()
	{
	}

	void RenderView::init()
	{
		ViewGroup::init();
		
		m_animationLoop = new priv::render_view::AnimationLoopImpl(this);
	}

	RedrawMode RenderView::getRedrawMode()
	{
		return m_redrawMode;
	}

	void RenderView::setRedrawMode(RedrawMode mode)
	{
		m_redrawMode = mode;
		Ptr<IRenderViewInstance> instance = getRenderViewInstance();
		if (instance.isNotNull()) {
			instance->setRedrawMode(this, mode);
		}
	}

	sl_bool RenderView::isDispatchingEventsToRenderingThread()
	{
		return m_flagDispatchEventsToRenderingThread;
	}

	void RenderView::setDispatchingEventsToRenderingThread(sl_bool flag)
	{
		m_flagDispatchEventsToRenderingThread = flag;
	}

	void RenderView::requestRender()
	{
		Ptr<IRenderViewInstance> instance = getRenderViewInstance();
		if (instance.isNotNull()) {
			instance->requestRender(this);
		}
	}

	void RenderView::invalidate(UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			requestRender();
		}
	}

	void RenderView::invalidate(const UIRect& rect, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			requestRender();
		}
	}

	void RenderView::renderViewContent(RenderEngine* engine)
	{
		engine->setDepthTest(sl_false);
		engine->setBlending(sl_true);
		Ref<RenderCanvas> canvas = RenderCanvas::create(engine, (sl_real)(getWidth()), (sl_real)(getHeight()));
		if (canvas.isNotNull()) {
			dispatchDraw(canvas.get());
		}
	}

	Size RenderView::measureText(const String& text, const Ref<Font>& _font, sl_bool flagMultiLine)
	{
		Ref<Font> font = _font;
		if (font.isNull()) {
			font = getFont();
			if (font.isNull()) {
				return Size::zero();
			}
		}
		return RenderCanvas::measureRenderingText(font, text, flagMultiLine);
	}

	Ref<AnimationLoop> RenderView::getAnimationLoop()
	{
		return m_animationLoop;
	}

	sl_bool RenderView::isDrawingThread()
	{
		if (isNativeWidget()) {
			return Thread::getCurrentThreadUniqueId() == m_lastRenderingThreadId;
		} else {
			return ViewGroup::isDrawingThread();
		}
	}

	void RenderView::dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (delayMillis) {
			Dispatch::setTimeout(SLIB_BIND_WEAKREF(void(), RenderView, dispatchToDrawingThread, this, callback, 0), delayMillis);
			return;
		}
		m_queuePostedCallbacks.push(callback);
		if (isNativeWidget()) {
			requestRender();
		} else {
			ViewGroup::dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), RenderView, _processPostedCallbacks, this), 0);
		}
	}

	void RenderView::runOnDrawingThread(const Function<void()>& callback)
	{
		if (isNativeWidget()) {
			if (Thread::getCurrentThreadUniqueId() == m_lastRenderingThreadId) {
				callback();
			} else {
				m_queuePostedCallbacks.push(callback);
				requestRender();
			}
		} else {
			dispatchToDrawingThread(callback);
		}
	}

	Ref<Dispatcher> RenderView::getDispatcher()
	{
		Ref<priv::render_view::DispatcherImpl> ret = new priv::render_view::DispatcherImpl;
		if (ret.isNotNull()) {
			ret->m_view = this;
			return ret;
		}
		return sl_null;
	}

	sl_bool RenderView::isDebugTextVisible()
	{
		return m_flagDebugTextVisible;
	}

	void RenderView::setDebugTextVisible(sl_bool flagVisible)
	{
		m_flagDebugTextVisible = flagVisible;
	}

	sl_bool RenderView::isDebugTextVisibleOnRelease()
	{
		return m_flagDebugTextVisible && m_flagDebugTextVisibleOnRelease;
	}

	void RenderView::setDebugTextVisibleOnRelease(sl_bool flagVisible)
	{
		m_flagDebugTextVisibleOnRelease = flagVisible;
		if (flagVisible) {
			m_flagDebugTextVisible = sl_true;
		}
	}

	void RenderView::onAttach()
	{
		requestRender();
	}

	void RenderView::onDrawBackground(Canvas* canvas)
	{
		if (isNativeWidget()) {
			Ref<Drawable> background = getCurrentBackground();
			if (background.isNotNull()) {
				if (!(background->isColor())) {
					drawBackground(canvas, background);
				}
			}
		} else {
			ViewGroup::onDrawBackground(canvas);
		}
	}

	SLIB_DEFINE_EVENT_HANDLER(RenderView, CreateEngine, RenderEngine* engine)
	
	void RenderView::dispatchCreateEngine(RenderEngine* engine)
	{
		SLIB_INVOKE_EVENT_HANDLER(CreateEngine, engine)
	}

	SLIB_DEFINE_EVENT_HANDLER_WITHOUT_ON(RenderView, Frame, RenderEngine* engine)
	
	void RenderView::onFrame(RenderEngine* engine)
	{
		renderViewContent(engine);
	}

	void RenderView::dispatchFrame(RenderEngine* engine)
	{
		MutexLocker lock(&m_lockRender);
		
		if (!engine) {
			return;
		}
		
		m_lastRenderingThreadId = Thread::getCurrentThreadUniqueId();
		
		if (m_animationLoop.isNotNull()) {
			priv::render_view::AnimationLoopImpl* l = static_cast<priv::render_view::AnimationLoopImpl*>(m_animationLoop.get());
			l->runStep();
		}
		_processPostedCallbacksNoLock();
		
		engine->beginScene();
		
		// clear
		{
			do {
				Color color;
				if (ColorDrawable::check(getCurrentBackground(), &color)) {
					if (color.a > 0) {
						color.a = 255;
						engine->clearColorDepth(color);
						break;
					}
				}
				engine->clearDepth();
			} while (0);
		}
		
		SLIB_INVOKE_EVENT_HANDLER(Frame, engine)
		
		if (m_flagDebugTextVisible) {
#if defined(SLIB_DEBUG)
			engine->drawDebugText();
#else
			if (m_flagDebugTextVisibleOnRelease) {
				engine->drawDebugText();
			}
#endif
		}
		
		engine->endScene();
		
	}

	void RenderView::dispatchDraw(Canvas* canvas)
	{
		MutexLocker lock(&m_lockRender);
		_processPostedCallbacksNoLock();
		ViewGroup::dispatchDraw(canvas);
	}

	void RenderView::dispatchMouseEvent(UIEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchMouseEvent, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchMouseEvent(ev);
	}

	void RenderView::dispatchTouchEvent(UIEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchTouchEvent, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchTouchEvent(ev);
	}

	void RenderView::dispatchMouseWheelEvent(UIEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchMouseWheelEvent, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchMouseWheelEvent(ev);
	}

	void RenderView::dispatchKeyEvent(UIEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchKeyEvent, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchKeyEvent(ev);
	}

	void RenderView::dispatchSetCursor(UIEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchSetCursor, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchSetCursor(ev);
	}

	void RenderView::dispatchSwipe(GestureEvent* ev)
	{
		if (m_flagDispatchEventsToRenderingThread) {
			if (isNativeWidget()) {
				m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchSwipe, this, ev->duplicate()));
				requestRender();
				return;
			}
		}
		ViewGroup::dispatchSwipe(ev);
	}

	void RenderView::_processPostedCallbacks()
	{
		MutexLocker lock(&m_lockRender);
		_processPostedCallbacksNoLock();
	}

	void RenderView::_processPostedCallbacksNoLock()
	{
		sl_size n = m_queuePostedCallbacks.getCount();
		Function<void()> callback;
		while (n > 0 && m_queuePostedCallbacks.pop(&callback)) {
			callback();
			n--;
		}
	}

	void RenderView::_dispatchMouseEvent(const Ref<UIEvent>& ev)
	{
		ViewGroup::dispatchMouseEvent(ev.get());
	}

	void RenderView::_dispatchTouchEvent(const Ref<UIEvent>& ev)
	{
		ViewGroup::dispatchTouchEvent(ev.get());
	}

	void RenderView::_dispatchMouseWheelEvent(const Ref<UIEvent>& ev)
	{
		ViewGroup::dispatchMouseWheelEvent(ev.get());
	}

	void RenderView::_dispatchKeyEvent(const Ref<UIEvent>& ev)
	{
		ViewGroup::dispatchKeyEvent(ev.get());
	}

	void RenderView::_dispatchSetCursor(const Ref<UIEvent>& ev)
	{
		ViewGroup::dispatchSetCursor(ev.get());
	}

	void RenderView::_dispatchSwipe(const Ref<GestureEvent>& ev)
	{
		ViewGroup::dispatchSwipe(ev.get());
	}

#if !defined(SLIB_UI)
	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	Ptr<IRenderViewInstance> RenderView::getRenderViewInstance()
	{
		return sl_null;
	}
#endif

}
