package slib.platform.android.ui.view;

import android.graphics.Rect;

public interface IView {

    long getInstance();

    void setInstance(long instance);

    Rect getUIFrame();

    void setUIFrame(int left, int top, int right, int bottom);

}
