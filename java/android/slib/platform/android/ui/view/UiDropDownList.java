package slib.platform.android.ui.view;

import com.slib.R;
import android.content.Context;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;

public class UiDropDownList extends Spinner {
	
	public static UiDropDownList _create(Context context)
	{
		try {
			UiDropDownList view = new UiDropDownList(context);
			return view;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _applyList(View _view, String[] items) {
		if (_view instanceof UiDropDownList) {
			UiDropDownList view = (UiDropDownList)_view;
			MyAdapter adapter = view.new MyAdapter(items);
			view.setAdapter(adapter);
		}
	}
	
	public static int _getSelectedIndex(View _view) {
		if (_view instanceof UiDropDownList) {
			UiDropDownList view = (UiDropDownList)_view;
			int n = view.getSelectedItemPosition();
			if (n < 0) {
				n = 0;
			}
			return n;
		}
		return 0;
	}
	
	public static void _select(View _view, int n) {
		if (_view instanceof UiDropDownList) {
			UiDropDownList view = (UiDropDownList)_view;
			view.setSelection(n);
		}
	}

	public static boolean _setFont(View _view, UiFont font) {
		if (_view instanceof UiDropDownList) {
			UiDropDownList view = (UiDropDownList)_view;
			if (font != null) {
				view.font = font;
				return true;				
			}
		}
		return false;
	}
	
	private static native void nativeOnSelect(long instance, int n);
	public static void onEventSelect(View view, int n) {
		long instance = UiView.getInstance(view);
		if (instance != 0) {
			nativeOnSelect(instance, n);
		}
	}

	UiFont font;

	public UiDropDownList(Context context) {
		
		super(context);
		this.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
				if (position < 0) {
					position = 0;
				}
				onEventSelect(UiDropDownList.this, position);
			}
			
			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});
	}
	
	class MyAdapter extends ArrayAdapter<String> {
		
		MyAdapter(String[] items) {
			super(UiDropDownList.this.getContext(), R.layout.spinner_default_item, items);
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
