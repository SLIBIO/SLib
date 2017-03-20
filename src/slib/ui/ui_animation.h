/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/ui/animation.h"

#include "../../../inc/slib/ui/view.h"

namespace slib
{
	class ViewTransformAnimationTarget : public AnimationTargetT<Matrix3>
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewTransformAnimationTarget(const Ref<View>& view, const AnimationFrames<Matrix3>& frames);
		
	public:
		Ref<View> getView();
		
		// override
		void update(float fraction, const Matrix3& value);
		
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
		
		// override
		void update(float fraction, const Vector2& value);
		
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
		
		// override
		void update(float fraction, const Vector2& value);
		
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
		
		// override
		void update(float fraction, const sl_real& value);
		
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
		
		// override
		void update(float fraction, const Rectangle& value);
		
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
		
		// override
		void update(float fraction, const sl_real& value);
		
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
		
		// override
		void update(float fraction, const Color4f& value);
		
	private:
		WeakRef<View> m_view;
		
	};
}
