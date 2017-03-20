/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android.ui.view;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AbsoluteLayout;

@SuppressWarnings("deprecation")
public class UiGroupView extends AbsoluteLayout implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	UiGestureDetector gestureDetector;

	public UiGroupView(Context context) {
		super(context);
	}
	
	public void onDraw(Canvas canvas) {
		UiView.onEventDraw(this, canvas);
	}

	public boolean onKeyDown(int keycode, KeyEvent event) {
		UiView.onEventKey(this, true, keycode, event);
		return true;
	}
	
	public boolean onKeyUp(int keycode, KeyEvent event) {
		UiView.onEventKey(this, false, keycode, event);
		return true;
	}
	
	public boolean onInterceptTouchEvent(MotionEvent event) {
		if (UiView.onHitTestTouchEvent(this, (int)(event.getX()), (int)(event.getY()))) {
			return true;
		}
		return false;
	};
	
	@SuppressLint("ClickableViewAccessibility")
	public boolean onTouchEvent(MotionEvent event) {		
		UiView.onEventTouch(this, event);
		return true;
	}

	public boolean dispatchTouchEvent(MotionEvent event) {
		if (gestureDetector != null) {
			gestureDetector.onTouchEvent(event);
			super.dispatchTouchEvent(event);
			return true;
		} else {
			return super.dispatchTouchEvent(event);
		}
	}

	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		measureChildren(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		int count = getChildCount();
		for (int i = 0; i < count; i++) {
			View child = getChildAt(i);
			if (child.getVisibility() != GONE) {
				if (child instanceof IView) {
					IView view = (IView)child;
					Rect frame = view.getUIFrame();
					child.layout(frame.left, frame.top, frame.right, frame.bottom);
				}
			}
		}
	}

}
