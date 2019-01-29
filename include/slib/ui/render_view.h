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

#ifndef CHECKHEADER_SLIB_UI_RENDER_VIEW
#define CHECKHEADER_SLIB_UI_RENDER_VIEW

#include "definition.h"

#include "view.h"

#include "../render/engine.h"

namespace slib
{

	class SLIB_EXPORT RenderView : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		RenderView();
		
		~RenderView();
		
	public:
		RedrawMode getRedrawMode();
		
		void setRedrawMode(RedrawMode mode);
		
		sl_bool isDispatchingEventsToRenderingThread();
		
		void setDispatchingEventsToRenderingThread(sl_bool flag);
		
		void requestRender();
		
		void invalidate(UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void invalidate(const UIRect& rect, UIUpdateMode mode = UIUpdateMode::Redraw) override;
		
		void renderViewContent(RenderEngine* engine);
		
		Size measureText(const String& text, const Ref<Font>& font, sl_bool flagMultiLine = sl_false) override;
		
		Ref<AnimationLoop> getAnimationLoop() override;
		
		sl_bool isDrawingThread() override;
		
		void dispatchToDrawingThread(const Function<void()>& callback, sl_uint32 delayMillis = 0) override;
		
		void runOnDrawingThread(const Function<void()>& callback) override;
		
		Ref<Dispatcher> getDispatcher() override;

		sl_bool isDebugTextVisible();
		
		void setDebugTextVisible(sl_bool flagVisible);
		
		sl_bool isDebugTextVisibleOnRelease();
		
		void setDebugTextVisibleOnRelease(sl_bool flagVisible);
		
	public:
		SLIB_PROPERTY(RenderEngineType, PreferredEngineType)
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(RenderView, Frame, RenderEngine* engine)

	protected:
		void onAttach() override;
		
		void onDrawBackground(Canvas* canvas) override;
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* _parent) override;
		
		void dispatchMouseEvent(UIEvent* ev) override;
		
		void dispatchTouchEvent(UIEvent* ev) override;
		
		void dispatchMouseWheelEvent(UIEvent* ev) override;
		
		void dispatchKeyEvent(UIEvent* ev) override;
		
		void dispatchSetCursor(UIEvent* ev) override;
		
		void dispatchSwipe(GestureEvent* ev) override;

	private:
		void _dispatchMouseEvent(const Ref<UIEvent>& ev);
		
		void _dispatchTouchEvent(const Ref<UIEvent>& ev);
		
		void _dispatchMouseWheelEvent(const Ref<UIEvent>& ev);
		
		void _dispatchKeyEvent(const Ref<UIEvent>& ev);
		
		void _dispatchSetCursor(const Ref<UIEvent>& ev);
		
		void _dispatchSwipe(const Ref<GestureEvent>& ev);
		
		void _setRedrawMode_NW(RedrawMode mode);

		void _requestRender_NW();
		
	protected:
		RedrawMode m_redrawMode;
		sl_bool m_flagDispatchEventsToRenderingThread;
		
		Ref<AnimationLoop> m_animationLoop;
		sl_uint64 m_lastRenderingThreadId;
		Queue< Function<void()> > m_queuePostedCallbacks;

		sl_bool m_flagDebugTextVisible;
		sl_bool m_flagDebugTextVisibleOnRelease;
		
	};

}

#endif
