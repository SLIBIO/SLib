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

#include "slib/ui/animation.h"

#include "slib/ui/view.h"

namespace slib
{
	class ViewTransformAnimationTarget : public AnimationTargetT<Matrix3>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewTransformAnimationTarget(const Ref<View>& view, const AnimationFrames<Matrix3>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const Matrix3& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewTranslateAnimationTarget : public AnimationTargetT<Vector2>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewTranslateAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const Vector2& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewScaleAnimationTarget : public AnimationTargetT<Vector2>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewScaleAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const Vector2& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewRotateAnimationTarget : public AnimationTargetT<sl_real>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewRotateAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const sl_real& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewFrameAnimationTarget : public AnimationTargetT<Rectangle>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewFrameAnimationTarget(const Ref<View>& view, const AnimationFrames<Rectangle>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const Rectangle& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewAlphaAnimationTarget : public AnimationTargetT<sl_real>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewAlphaAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const sl_real& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
	
	class ViewBackgroundColorAnimationTarget : public AnimationTargetT<Color4f>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewBackgroundColorAnimationTarget(const Ref<View>& view, const AnimationFrames<Color4f>& frames);
		
	public:
		Ref<View> getView();
		
		void update(float fraction, const Color4f& value) override;
		
	private:
		WeakRef<View> m_view;
		
	};
}
