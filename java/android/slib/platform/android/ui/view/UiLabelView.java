package slib.platform.android.ui.view;

import android.content.Context;
import android.graphics.Rect;
import android.view.View;
import android.widget.TextView;

public class UiLabelView extends TextView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public UiLabelView(Context context) {
		super(context);
		this.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				UiView.onEventClick(UiLabelView.this);
			}
		});
	}

}
