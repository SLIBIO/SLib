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

package slib.platform.android.ui.view;

import android.app.Activity;
import android.content.Context;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

import slib.platform.android.Logger;

public class UiGestureDetector extends SimpleOnGestureListener {

    public interface GestureListener {

        void onSwipe(int direction, float posBegin);

        void onDoubleTap();
    }

    public final static int SWIPE_LEFT = 0;
    public final static int SWIPE_RIGHT = 1;
    public final static int SWIPE_UP = 2;
    public final static int SWIPE_DOWN = 3;

    private int swipe_Min_Distance = 50;
    private int swipe_Max_Distance = 2000;
    private int swipe_Min_Velocity = 50;

    private GestureDetector detector;
    private GestureListener listener;

    public UiGestureDetector(Context context, GestureListener listener) {
        this.detector = new GestureDetector(context, this);
        this.listener = listener;
    }

    public void onTouchEvent(MotionEvent event) {
        try {
            this.detector.onTouchEvent(event);
        } catch (Exception e) {
            Logger.exception(e);
        }
    }

    public void setSwipeMaxDistance(int distance) {
        this.swipe_Max_Distance = distance;
    }

    public void setSwipeMinDistance(int distance) {
        this.swipe_Min_Distance = distance;
    }

    public void setSwipeMinVelocity(int distance) {
        this.swipe_Min_Velocity = distance;
    }

    public int getSwipeMaxDistance() {
        return this.swipe_Max_Distance;
    }

    public int getSwipeMinDistance() {
        return this.swipe_Min_Distance;
    }

    public int getSwipeMinVelocity() {
        return this.swipe_Min_Velocity;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {

        final float xDistance = Math.abs(e1.getX() - e2.getX());
        final float yDistance = Math.abs(e1.getY() - e2.getY());

        if (xDistance > this.swipe_Max_Distance || yDistance > this.swipe_Max_Distance) {
            return false;
        }

        velocityX = Math.abs(velocityX);
        velocityY = Math.abs(velocityY);
        boolean result = false;

        if (velocityX > this.swipe_Min_Velocity && xDistance > this.swipe_Min_Distance) {
            if (e1.getX() > e2.getX()) // right to left
                this.listener.onSwipe(SWIPE_LEFT, e1.getX());
            else {
                this.listener.onSwipe(SWIPE_RIGHT, e1.getX());
            }

            result = true;
        } else if (velocityY > this.swipe_Min_Velocity && yDistance > this.swipe_Min_Distance) {
            if (e1.getY() > e2.getY()) // bottom to up
                this.listener.onSwipe(SWIPE_UP, e1.getY());
            else
                this.listener.onSwipe(SWIPE_DOWN, e1.getY());

            result = true;
        }

        return result;
    }

    @Override
    public boolean onDoubleTap(MotionEvent ev) {
        listener.onDoubleTap();
        return true;
    }

    @Override
    public boolean onSingleTapUp(MotionEvent ev) {
        return false;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent ev) {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent ev) {
        return false;
    }

}
