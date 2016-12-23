package slib.platform.android.ui.view;

import android.app.Activity;
import android.content.Context;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

public class UiGestureDetector extends SimpleOnGestureListener {

    public interface GestureListener {

        void onSwipe(int direction, float posBegin);

        void onDoubleTap();
    }

    public final static int SWIPE_LEFT = 0;
    public final static int SWIPE_RIGHT = 1;
    public final static int SWIPE_UP = 2;
    public final static int SWIPE_DOWN = 3;

    public final static int MODE_TRANSPARENT = 0;
    public final static int MODE_SOLID = 1;
    public final static int MODE_DYNAMIC = 2;

    private final static int ACTION_FAKE = -13; // just an unlikely number
    private int swipe_Min_Distance = 50;
    private int swipe_Max_Distance = 2000;
    private int swipe_Min_Velocity = 50;

    private int mode = MODE_DYNAMIC;
    private boolean bEnabled = true;
    private boolean tapIndicator = false;

    private GestureDetector detector;
    private GestureListener listener;

    private Context context;

    public UiGestureDetector(Context context, GestureListener listener) {
        this.context = context;
        this.detector = new GestureDetector(context, this);
        this.listener = listener;
    }

    public void onTouchEvent(MotionEvent event) {

        if (! this.bEnabled) {
            return;
        }

        try {
            boolean result = this.detector.onTouchEvent(event);

            if (this.mode == MODE_SOLID) {
                event.setAction(MotionEvent.ACTION_CANCEL);
            } else if (this.mode == MODE_DYNAMIC) {
                if (event.getAction() == ACTION_FAKE) {
                    event.setAction(MotionEvent.ACTION_UP);
                } else if (result) {
                    event.setAction(MotionEvent.ACTION_CANCEL);
                } else if (this.tapIndicator) {
                    event.setAction(MotionEvent.ACTION_DOWN);
                    this.tapIndicator = false;
                }
            }
        } catch (Exception e) {}
    }

    public void setMode(int mode) {
        this.mode = mode;
    }

    public int getMode() {
        return this.mode;
    }

    public void setEnabled(boolean status) {
        this.bEnabled = status;
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
    public boolean onSingleTapUp(MotionEvent ev) {
        this.tapIndicator = true;
        return false;
    }

    @Override
    public boolean onDoubleTap(MotionEvent ev) {
        listener.onDoubleTap();
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent ev) {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent ev) {

        if (mode == MODE_DYNAMIC) { // we owe an ACTION_UP, so we fake an action which will be converted to an ACTION_UP later
            ev.setAction(ACTION_FAKE);
            if (context instanceof Activity) {
                ((Activity)context).dispatchTouchEvent(ev);
            }
        }
        return false;
    }

}
