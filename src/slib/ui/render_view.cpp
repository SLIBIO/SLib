#include "../../../inc/slib/ui/render_view.h"

#include "../../../inc/slib/render/canvas.h"
#include "../../../inc/slib/core/thread.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(RenderView, View)

class _RenderAnimationLoop : public AnimationLoop
{
public:
	WeakRef<RenderView> m_view;
	
public:
	_RenderAnimationLoop(RenderView* view) : m_view(view)
	{
	}
	
public:
	// override
	void _wake()
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

RenderView::RenderView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setCreatingNativeWidget(sl_true);
	setCreatingChildInstances(sl_false);
	setMakingLayout(sl_true, UIUpdateMode::Init);
	
	setPreferredEngineType(RenderEngineType::OpenGL_ES);
	
	m_redrawMode = RedrawMode::Continuously;
	m_flagDispatchEventsToRenderingThread = sl_false;
	
	m_animationLoop = new _RenderAnimationLoop(this);
	m_lastRenderingThreadId = 0;
	
	m_flagDebugTextVisible = sl_true;
	m_flagDebugTextVisibleOnRelease = sl_false;
}

RenderView::~RenderView()
{
}

RedrawMode RenderView::getRedrawMode()
{
	return m_redrawMode;
}

void RenderView::setRedrawMode(RedrawMode mode)
{
	m_redrawMode = mode;
	if (isNativeWidget()) {
		_setRedrawMode_NW(mode);
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
	if (isNativeWidget()) {
		_requestRender_NW();
	}
}

void RenderView::invalidate()
{
	requestRender();
}

void RenderView::invalidate(const UIRect& rect)
{
	requestRender();
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

Ref<AnimationLoop> RenderView::getAnimationLoop()
{
	return m_animationLoop;
}

sl_bool RenderView::isDrawingThread()
{
	return Thread::getCurrentThreadUniqueId() == m_lastRenderingThreadId;
}

void RenderView::dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis)
{
	m_queuePostedCallbacks.push(callback);
}

void RenderView::runOnDrawingThread(const Function<void()>& callback)
{
	if (Thread::getCurrentThreadUniqueId() == m_lastRenderingThreadId) {
		callback();
	} else {
		m_queuePostedCallbacks.push(callback);
	}
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

void RenderView::onFrame(RenderEngine* engine)
{
	renderViewContent(engine);
}

void RenderView::onAttach()
{
	requestRender();
}

void RenderView::dispatchFrame(RenderEngine* engine)
{
	m_lastRenderingThreadId = Thread::getCurrentThreadUniqueId();
	
	if (m_animationLoop.isNotNull()) {
		_RenderAnimationLoop* l = static_cast<_RenderAnimationLoop*>(m_animationLoop.get());
		l->runStep();
	}
	
	sl_size n = m_queuePostedCallbacks.getCount();
	Function<void()> callback;
	while (n > 0 && m_queuePostedCallbacks.pop(&callback)) {
		callback();
		n--;
	}

	if (engine) {
		engine->beginScene();
	}
	
	onFrame(engine);
	
	getOnFrame()(this, engine);
	
	if (m_flagDebugTextVisible) {
		if (engine) {
#if defined(SLIB_DEBUG)
			engine->drawDebugText();
#else
			if (m_flagDebugTextVisibleOnRelease) {
				engine->drawDebugText();
			}
#endif
		}
	}
	
	if (engine) {
		engine->endScene();
	}
}

void RenderView::dispatchMouseEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchMouseEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchMouseEvent(ev);
}

void RenderView::dispatchTouchEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchTouchEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchTouchEvent(ev);
}

void RenderView::dispatchMouseWheelEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchMouseWheelEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchMouseWheelEvent(ev);
}

void RenderView::dispatchKeyEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchKeyEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchKeyEvent(ev);
}

void RenderView::dispatchClick(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchClick, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchClick(ev);
}

void RenderView::dispatchSetCursor(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchSetCursor, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchSetCursor(ev);
}

void RenderView::dispatchSwipe(GestureEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (Thread::getCurrentThreadUniqueId() != m_lastRenderingThreadId) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchSwipe, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchSwipe(ev);
}

void RenderView::_dispatchMouseEvent(const Ref<UIEvent>& ev)
{
	View::dispatchMouseEvent(ev.get());
}

void RenderView::_dispatchTouchEvent(const Ref<UIEvent>& ev)
{
	View::dispatchTouchEvent(ev.get());
}

void RenderView::_dispatchMouseWheelEvent(const Ref<UIEvent>& ev)
{
	View::dispatchMouseWheelEvent(ev.get());
}

void RenderView::_dispatchKeyEvent(const Ref<UIEvent>& ev)
{
	View::dispatchKeyEvent(ev.get());
}

void RenderView::_dispatchClick(const Ref<UIEvent>& ev)
{
	View::dispatchClick(ev.get());
}

void RenderView::_dispatchSetCursor(const Ref<UIEvent>& ev)
{
	View::dispatchSetCursor(ev.get());
}

void RenderView::_dispatchSwipe(const Ref<GestureEvent>& ev)
{
	View::dispatchSwipe(ev.get());
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}

void RenderView::_setRedrawMode_NW(RedrawMode mode)
{
}

void RenderView::_requestRender_NW()
{
}

#endif

SLIB_UI_NAMESPACE_END
