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
	
	setPreferredEngineType(RenderEngineType::OpenGL_ES);
	
	m_redrawMode = RedrawMode::Continuously;
	m_flagDispatchEventsToRenderingThread = sl_false;
	
	m_animationLoop = new _RenderAnimationLoop(this);
	m_lastRenderingThreadId = 0;
	
	m_flagDebugTextVisible = sl_true;
	m_flagDebugTextVisibleOnRelease = sl_false;
	
	setBackgroundColor(Color::Black, UIUpdateMode::Init);
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
		return View::isDrawingThread();
	}
}

void RenderView::dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis)
{
	if (isNativeWidget()) {
		m_queuePostedCallbacks.push(callback);
	} else {
		View::dispatchToDrawingThread(callback, delayMillis);
	}
}

void RenderView::runOnDrawingThread(const Function<void()>& callback)
{
	if (isNativeWidget()) {
		if (Thread::getCurrentThreadUniqueId() == m_lastRenderingThreadId) {
			callback();
		} else {
			m_queuePostedCallbacks.push(callback);
		}
	} else {
		View::runOnDrawingThread(callback);
	}
}

class _RenderViewDispatcher : public Dispatcher
{
public:
	WeakRef<RenderView> m_view;
	
public:
	// override
	sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms)
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

Ref<Dispatcher> RenderView::getDispatcher()
{
	Ref<_RenderViewDispatcher> ret = new _RenderViewDispatcher;
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

void RenderView::onFrame(RenderEngine* engine)
{
	renderViewContent(engine);
}

void RenderView::onAttach()
{
	requestRender();
}

void RenderView::onDrawBackground(Canvas* canvas)
{
	if (isNativeWidget()) {
		Ref<DrawAttributes> attrs = m_drawAttributes;
		if (attrs.isNotNull()) {
			Ref<Drawable> background;
			if (isPressedState()) {
				background = attrs->backgroundPressed;
			} else if (isHoverState()) {
				background = attrs->backgroundHover;
			}
			if (background.isNull()) {
				background = attrs->background;
			}
			if (background.isNotNull()) {
				drawBackground(canvas, Color::zero(), background);
			}
		}
	} else {
		View::onDrawBackground(canvas);
	}
}

void RenderView::dispatchFrame(RenderEngine* engine)
{
	if (!engine) {
		return;
	}
	
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

	engine->beginScene();
	
	// clear
	{
		Color color = getBackgroundColor();
		if (color.a > 0) {
			color.a = 255;
			engine->clearColorDepth(color);
		}
	}
	
	onFrame(engine);
	
	getOnFrame()(this, engine);
	
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
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchTouchEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchTouchEvent(ev);
}

void RenderView::dispatchMouseWheelEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchMouseWheelEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchMouseWheelEvent(ev);
}

void RenderView::dispatchKeyEvent(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchKeyEvent, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchKeyEvent(ev);
}

void RenderView::dispatchClick(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchClick, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchClick(ev);
}

void RenderView::dispatchSetCursor(UIEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
			m_queuePostedCallbacks.push(SLIB_BIND_WEAKREF(void(), RenderView, _dispatchSetCursor, this, ev->duplicate()));
			return;
		}
	}
	View::dispatchSetCursor(ev);
}

void RenderView::dispatchSwipe(GestureEvent* ev)
{
	if (m_flagDispatchEventsToRenderingThread) {
		if (!(isDrawingThread())) {
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
