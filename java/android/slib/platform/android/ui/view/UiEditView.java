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

import io.slib.R;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.Util;

import android.app.Activity;
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

	boolean mFlagBorder = false;
	int mBackgroundColor = 0;

	public static View _create(Context context, int type)
	{
		try {
			if (type == 0) {
				// EditView
				UiEditView view = new UiEditView(context);
				view.setPadding(3, 0, 3, 0);
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
	
	public static boolean _setText(final View view, String text) {
		if (!(UiThread.isUiThread())) {
			final String t = text;
			view.post(new Runnable() {
				public void run() {
					_setText(view, t);
				}
			});
			return true;
		}
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
	
	public static boolean _setBorder(final View view, final boolean flag) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setBorder(view, flag);
				}
			});
			return true;
		}
		if (view instanceof UiEditView) {
			UiEditView tv = (UiEditView)view;
			tv.mFlagBorder = flag;
			tv.applyBackground();
			return true;
		}
		return false;
	}
	
	public static boolean _setAlignment(final View view, final int align) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setAlignment(view, align);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setGravity(Util.getAndroidAlignment(align));
			return true;
		}
		return false;
	}

	public static boolean _setHintText(final View view, String text) {
		if (!(UiThread.isUiThread())) {
			final String t = text;
			view.post(new Runnable() {
				public void run() {
					_setHintText(view, t);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			if (text == null) {
				text = "";
			}
			((TextView)view).setHint(text);
			return true;
		}
		return false;
	}
	
	public static boolean _setReadOnly(final View view, final boolean flag) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setReadOnly(view, flag);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			((TextView)view).setEnabled(!flag);
			return true;
		}
		return false;
	}

	public static boolean _setMultiLine(final View view, final boolean flag) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setMultiLine(view, flag);
				}
			});
			return true;
		}
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

	public static boolean _setTextColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setTextColor(view, color);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setTextColor(color);				
			return true;
		}
		return false;
	}

	public static boolean _setHintTextColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setHintTextColor(view, color);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setHintTextColor(color);
			return true;
		}
		return false;
	}
	
	public static boolean _setBackgroundColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setBackgroundColor(view, color);
				}
			});
			return true;
		}
		if (view instanceof UiEditView) {
			UiEditView tv = (UiEditView) view;
			tv.mBackgroundColor = color;
			tv.applyBackground();
		} else if (view instanceof TextView) {
			TextView tv = (TextView)view;
			tv.setBackgroundColor(color);				
			return true;
		}
		return false;
	}
	
	public static boolean _setFont(final View view, final UiFont font) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setFont(view, font);
				}
			});
			return true;
		}
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

	public static boolean _setReturnKeyType(final View view, final int type) {
		if (view instanceof UiTextArea) {
			return false;
		}
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setReturnKeyType(view, type);
				}
			});
			return true;
		}
		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			int defaultOption = EditorInfo.IME_FLAG_NO_FULLSCREEN;
			switch (type) {
				case 0:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_DONE);
					break;
				case 1:
					tv.setImeActionLabel("Return", KeyEvent.KEYCODE_ENTER);
					break;
				case 2:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_DONE);
					break;
				case 3:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_SEARCH);
					break;
				case 4:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_NEXT);
					break;
				case 5:
					tv.setImeActionLabel("Continue", KeyEvent.KEYCODE_ENTER);
					break;
				case 6:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_GO);
					break;
				case 7:
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_SEND);
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
					tv.setImeOptions(defaultOption | EditorInfo.IME_ACTION_NONE);
					break;
			}
			return true;
		}
		return false;
	}

	public static boolean _setInputType(final View view, final int keyboardType, final int autoCapType, final boolean flagPassword) {

		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setInputType(view, keyboardType, autoCapType, flagPassword);
				}
			});
			return true;
		}

		if (view instanceof TextView) {
			TextView tv = (TextView)view;
			int type;
			if (flagPassword) {
				type = InputType.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_VARIATION_PASSWORD;
			} else {
				if (view instanceof UiTextArea) {
					type = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_MULTI_LINE;
				} else {
					switch (keyboardType) {
						case 0:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 1:
							type = InputType.TYPE_CLASS_NUMBER;
							break;
						case 2:
							type = InputType.TYPE_CLASS_PHONE;
							break;
						case 3:
							type = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
							break;
						case 4:
							type = InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL;
							break;
						case 5:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 6:
							type = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_URI;
							break;
						case 7:
							type = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_WEB_EDIT_TEXT;
							break;
						case 8:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 9:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 10:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 11:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						case 12:
							type = InputType.TYPE_CLASS_TEXT;
							break;
						default:
							type = InputType.TYPE_CLASS_TEXT;
							break;
					}
				}
				if ((type & InputType.TYPE_MASK_CLASS) == InputType.TYPE_CLASS_TEXT) {
					switch (autoCapType) {
						case 1:
							type = type | InputType.TYPE_TEXT_FLAG_CAP_WORDS;
							break;
						case 2:
							type = type | InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
							break;
						case 3:
							type = type | InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS;
							break;
						default:
							break;
					}
				}
			}
			tv.setInputType(type);
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

	private static native void nativeOnDone(long instance);
	public static void onEventDone(IView view) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnDone(instance);
		}
	}

	private static native void nativeOnReturn(long instance);
	public static void onEventReturn(IView view) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnReturn(instance);
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
		this.setOnEditorActionListener(new OnEditorActionListener() {
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				if (event == null) {
					onEventDone(UiEditView.this);
					Util.dismissKeyboard((Activity)(getContext()));
					return true;
				} else {
					if (!(event.isShiftPressed())) {
						onEventReturn(UiEditView.this);
						return true;
					}
				}
				return false;
			}
		});
		this.setImeOptions(EditorInfo.IME_FLAG_NO_FULLSCREEN);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	void applyBackground() {
		applyBackground(this, mFlagBorder, mBackgroundColor);
	}

	public static void applyBackground(View view, boolean flag, int color) {
		if (flag && ((color & 0xFF000000) == 0 || (color & 0xFFFFFF) == 0xFFFFFF)) {
			view.setBackgroundResource(R.drawable.slib_rounded_border_background);
		} else {
			view.setBackgroundColor(color);
		}
	}

}
