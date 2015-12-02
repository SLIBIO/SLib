package slib.platform.android.ui.view;

import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import android.content.Context;
import android.util.TypedValue;
import android.view.View;
import android.widget.Button;

public class UiButton extends Button implements Button.OnClickListener {
	
	public static UiButton _create(Context context)
	{
		try {
			UiButton button = new UiButton(context);
			return button;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static String _getText(View view) {
		try {
			if (view instanceof Button) {
				String text = ((Button)view).getText().toString();
				if (text == null) {
					text = "";
				}
				return text;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return "";
	}
	
	public static boolean _setText(View view, String text) {
		if (view instanceof Button) {
			if (text == null) {
				text = "";
			}
			((Button)view).setText(text);
			return true;
		}
		return false;
	}

	public static boolean _setFont(View view, UiFont font) {
		if (view instanceof Button) {
			if (font != null) {
				Button v = (Button)view;
				v.setTypeface(font.getTypeface());
				v.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
				return true;				
			}
		}
		return false;
	}
	
	
	public UiButton(Context context) {
		super(context);
		setOnClickListener(this);
		this.setPadding(0, -5, 0, 0);
	}

	@Override
	public void onClick(View v) {
		UiView.onEventClick(v);
	}
	
}
