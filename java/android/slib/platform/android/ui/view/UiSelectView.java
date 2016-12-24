package slib.platform.android.ui.view;

import com.slib.R;
import android.content.Context;
import android.graphics.Rect;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.UiThread;

public class UiSelectView extends Spinner implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public static UiSelectView _create(Context context)
	{
		try {
			UiSelectView view = new UiSelectView(context);
			return view;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _applyList(final View _view, final String[] items) {
		if (!(UiThread.isUiThread())) {
			_view.post(new Runnable() {
				public void run() {
					_applyList(_view, items);
				}
			});
			return;
		}
		if (_view instanceof UiSelectView) {
			UiSelectView view = (UiSelectView)_view;
			MyAdapter adapter = view.new MyAdapter(items);
			view.setAdapter(adapter);
		}
	}
	
	public static int _getSelectedIndex(View _view) {
		if (_view instanceof UiSelectView) {
			UiSelectView view = (UiSelectView)_view;
			int n = view.getSelectedItemPosition();
			if (n < 0) {
				n = 0;
			}
			return n;
		}
		return 0;
	}
	
	public static void _select(final View _view, final int n) {
		if (!(UiThread.isUiThread())) {
			_view.post(new Runnable() {
				public void run() {
					_select(_view, n);
				}
			});
			return;
		}
		if (_view instanceof UiSelectView) {
			UiSelectView view = (UiSelectView)_view;
			view.setSelection(n);
		}
	}

	public static boolean _setFont(final View _view, final UiFont font) {
		if (!(UiThread.isUiThread())) {
			_view.post(new Runnable() {
				public void run() {
					_setFont(_view, font);
				}
			});
			return true;
		}
		if (_view instanceof UiSelectView) {
			UiSelectView view = (UiSelectView)_view;
			if (font != null) {
				view.font = font;
				return true;				
			}
		}
		return false;
	}
	
	private static native void nativeOnSelect(long instance, int n);
	public static void onEventSelect(IView view, int n) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnSelect(instance, n);
		}
	}

	UiFont font;

	public UiSelectView(Context context) {
		
		super(context);
		this.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
				if (position < 0) {
					position = 0;
				}
				onEventSelect(UiSelectView.this, position);
			}
			
			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});
	}
	
	class MyAdapter extends ArrayAdapter<String> {
		
		MyAdapter(String[] items) {
			super(UiSelectView.this.getContext(), R.layout.spinner_default_item, items);
			setDropDownViewResource(R.layout.spinner_default_dropdown_item);
		}
		
		public View getView(int position, View convertView, ViewGroup parent) {
			View ret = super.getView(position, convertView, parent);
			if (font != null) {
				if (ret != null) {
					if (ret instanceof TextView) {
						TextView tv = (TextView)ret;
						tv.setTypeface(font.getTypeface());
						tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
					}
				}				
			}
			return ret;
		}
		
		public View getDropDownView(int position, View convertView, ViewGroup parent) {
			View ret = super.getDropDownView(position, convertView, parent);
			if (font != null) {
				if (ret != null) {
					if (ret instanceof TextView) {
						TextView tv = (TextView)ret;
						tv.setTypeface(font.getTypeface());
						tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
					}
				}				
			}
			return ret;
		}
	}

}
