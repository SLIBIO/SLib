package slib.platform.android.ui.view;

import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.Util;
import android.content.Context;
import android.graphics.Rect;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.TypedValue;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;

public class UiEditView extends EditText implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	boolean flagPassword = false;

	public static View _create(Context context, int type)
	{
		try {
			if (type == 0) {
				// EditView
				UiEditView view = new UiEditView(context);
				view.setPadding(3, 0, 3, 0);
				return view;				
			} else if (type == 1) {
				// PasswordView
				UiEditView view = new UiEditView(context);
				view.flagPassword = true;
				view.setPadding(3, 0, 3, 0);
				view.setInputType(InputType.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_VARIATION_PASSWORD);
				return view;
			} else if (type == 2) {
				// TextArea
				UiTextArea view = new UiTextArea(context);
				return view;
			} else {
				// LabelView
				UiLabelView view = new UiLabelView(context);
				return view;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static String _getText(View view) {
		try {
			if (view instanceof TextView) {
				String text = ((TextView)view).getText().toString();
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
		if (view instanceof TextView) {
			if (text == null) {
				text = "";
			}
			TextView tv = (TextView)view;
			String old = tv.getText().toString();
			if (old == null) {
				old = "";
			}
			if (!(old.equals(text))) {
				tv.setText(text);				
			}
			return true;
		}
		return false;
	}
	
	public static boolean _isBorder(View view) {
		return true;
	}

	public static boolean _setBorder(View view, boolean flag) {
		return false;
	}
	
	public static int _getAlignment(View view) {
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			int gravity = tv.getGravity();
			return Util.getSlibAlignment(gravity);
		}
		return 0;
	}
	
	public static boolean _setAlignment(View view, int align) {
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setGravity(Util.getAndroidAlignment(align));
			return true;
		}
		return false;
	}

	public static String _getHintText(View view) {
		try {
			if (view instanceof TextView) {
				String s = ((TextView)view).getHint().toString();
				if (s == null) {
					s = "";
				}
				return s;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return "";
	}
	
	public static boolean _setHintText(View view, String text) {
		if (view instanceof TextView) {
			if (text == null) {
				text = "";
			}
			((TextView)view).setHint(text);
			return true;
		}
		return false;
	}
	
	public static boolean _isReadOnly(View view) {
		if (view instanceof TextView) {
			return ((TextView)view).isEnabled();
		}
		return false;
	}
	
	public static boolean _setReadOnly(View view, boolean flag) {
		if (view instanceof TextView) {
			((TextView)view).setEnabled(!flag);
			return true;
		}
		return false;
	}

	public static boolean _isMultiLine(View view) {
		if (view instanceof TextView) {
			return ((TextView)view).isEnabled();
		}
		return false;
	}
	public static boolean _setMultiLine(View view, boolean flag) {
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			int type = tv.getInputType();
			if (flag) {
				type |= EditorInfo.TYPE_TEXT_FLAG_MULTI_LINE;
			} else {
				type &= ~(EditorInfo.TYPE_TEXT_FLAG_MULTI_LINE);
			}
			tv.setInputType(type);
			return true;
		}
		return false;
	}

	public static boolean _setTextColor(View view, int color) {
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setTextColor(color);				
			return true;
		}
		return false;
	}

	public static boolean _setBackgroundColor(View view, int color) {
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setBackgroundColor(color);				
			return true;
		}
		return false;
	}
	
	public static boolean _setFont(View view, UiFont font) {
		if (view instanceof TextView) {
			if (font != null) {
				TextView tv = (TextView)view;
				tv.setTypeface(font.getTypeface());
				tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
				return true;				
			}
		}
		return false;
	}

	public static  boolean _setReturnKeyType(View view, int type) {
		if (view instanceof  TextView) {
			TextView tv = (TextView)view;
			switch (type) {
				case 0:
					tv.setImeOptions(EditorInfo.IME_ACTION_DONE);
					break;
				case 1:
					tv.setImeActionLabel("Return", KeyEvent.KEYCODE_ENTER);
					break;
				case 2:
					tv.setImeOptions(EditorInfo.IME_ACTION_DONE);
					break;
				case 3:
					tv.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
					break;
				case 4:
					tv.setImeOptions(EditorInfo.IME_ACTION_NEXT);
					break;
				case 5:
					tv.setImeActionLabel("Continue", KeyEvent.KEYCODE_ENTER);
					break;
				case 6:
					tv.setImeOptions(EditorInfo.IME_ACTION_GO);
					break;
				case 7:
					tv.setImeOptions(EditorInfo.IME_ACTION_SEND);
					break;
				case 8:
					tv.setImeActionLabel("Join", KeyEvent.KEYCODE_ENTER);
					break;
				case 9:
					tv.setImeActionLabel("Route", KeyEvent.KEYCODE_ENTER);
					break;
				case 10:
					tv.setImeActionLabel("EmergencyCall", KeyEvent.KEYCODE_ENTER);
					break;
				case 11:
					tv.setImeActionLabel("Google", KeyEvent.KEYCODE_ENTER);
					break;
				case 12:
					tv.setImeActionLabel("Yahoo", KeyEvent.KEYCODE_ENTER);
					break;
				default:
					tv.setImeOptions(EditorInfo.IME_ACTION_NONE);
					break;
			}
			return true;
		}
		return false;
	}

	public static boolean _setKeyboardType(View view, int type) {
		if (view instanceof UiEditView && ((UiEditView)view).flagPassword) {
			return true;
		}
		if (view instanceof  TextView) {
			TextView tv = (TextView)view;
			switch (type) {
				case 0:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case 1:
					tv.setInputType(InputType.TYPE_CLASS_NUMBER);
					break;
				case 2:
					tv.setInputType(InputType.TYPE_CLASS_PHONE);
					break;
				case 3:
					tv.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
					break;
				case 4:
					tv.setInputType(InputType.TYPE_NUMBER_FLAG_DECIMAL);
					break;
				case 5:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case 6:
					tv.setInputType(InputType.TYPE_TEXT_VARIATION_URI);
					break;
				case 7:
					tv.setInputType(InputType.TYPE_TEXT_VARIATION_WEB_EDIT_TEXT);
					break;
				case 8:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case  9:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case 10:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case 11:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				case 12:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
				default:
					tv.setInputType(InputType.TYPE_CLASS_TEXT);
					break;
			}
		}
		return false;
	}

	private static native void nativeOnChange(long instance);
	public static void onEventChange(IView view) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnChange(instance);
		}
	}
	
	public UiEditView(Context context) {
		super(context);
		this.addTextChangedListener(new TextWatcher() {
			public void beforeTextChanged(CharSequence s, int start, int count, int after) {
			}
			
			public void afterTextChanged(Editable s) {
				onEventChange(UiEditView.this);
			}
			
			public void onTextChanged(CharSequence s, int start, int before, int count) {
			}
		});
	}
}
