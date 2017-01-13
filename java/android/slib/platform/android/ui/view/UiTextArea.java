package slib.platform.android.ui.view;

import android.content.Context;

public class UiTextArea extends UiEditView {

	public UiTextArea(Context context) {
		super(context);
		_setMultiLine(this, true);
	}

}
