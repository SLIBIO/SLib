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

package slib.platform.android.ui;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Build;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.util.TypedValue;
import android.widget.TextView;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class Alert {

	public int type;
	public static final int TYPE_OK = 0;
	public static final int TYPE_OKCANCEL = 1;
	public static final int TYPE_YESNO = 2;
	public static final int TYPE_YESNOCANCEL = 3;
	
	public String text;
	public boolean flagHyperText;
	public String caption;
	
	public String titleOk;
	public String titleCancel;
	public String titleYes;
	public String titleNo;
	
	public long nativeObject;
	public static final int RESULT_OK = 0;
	public static final int RESULT_CANCEL = 1;
	public static final int RESULT_YES = 2;
	public static final int RESULT_NO = 3;
	
	public Alert()
	{
	}
	
	private static native void nativeShowResult(long ptr, int result);
	public boolean show(final SlibActivity activity) {
		
		if (!(UiThread.isUiThread())) {
			activity.runOnUiThread(new Runnable() {
				public void run() {
					show(activity);
				}
			});
		} else {
			try {
				AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity, AlertDialog.THEME_HOLO_LIGHT);
				alertDialogBuilder.setTitle(caption);
				if (flagHyperText) {
					TextView message = new TextView(activity);
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
						message.setText(Html.fromHtml(text, Html.FROM_HTML_MODE_COMPACT));
					} else {
						message.setText(Html.fromHtml(text));
					}
					int padding = (int)(TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 15, activity.getResources().getDisplayMetrics()));
					message.setPadding(padding, padding, padding, padding);
					message.setMovementMethod(LinkMovementMethod.getInstance());
					message.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 20);
					alertDialogBuilder.setView(message);
				} else {
					alertDialogBuilder.setMessage(text);
				}
				switch (type) {
				case TYPE_OK:
					if (titleOk != null) {
						alertDialogBuilder.setPositiveButton(titleOk, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_OK);
							}
						});
					} else {
						alertDialogBuilder.setPositiveButton(android.R.string.ok, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_OK);
							}
						});
					}
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_OK);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				case TYPE_OKCANCEL:
					if (titleOk != null) {
						alertDialogBuilder.setPositiveButton(titleOk, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_OK);
							}
						});
					} else {
						alertDialogBuilder.setPositiveButton(android.R.string.ok, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_OK);
							}
						});
					}
					if (titleCancel != null) {
						alertDialogBuilder.setNegativeButton(titleCancel, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_CANCEL);
							}
						});
					} else {
						alertDialogBuilder.setNegativeButton(android.R.string.cancel, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_CANCEL);
							}
						});
					}
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {						
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				case TYPE_YESNO:
					if (titleYes != null) {
						alertDialogBuilder.setPositiveButton(titleYes, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_YES);
							}
						});
					} else {
						alertDialogBuilder.setPositiveButton(android.R.string.yes, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_YES);
							}
						});
					}
					if (titleNo != null) {
						alertDialogBuilder.setNeutralButton(titleNo, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_NO);
							}
						});
					} else {
						alertDialogBuilder.setNeutralButton(android.R.string.no, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_NO);
							}
						});
					}
					alertDialogBuilder.setCancelable(false);
					break;
				case TYPE_YESNOCANCEL:
					if (titleYes != null) {
						alertDialogBuilder.setPositiveButton(titleYes, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_YES);
							}
						});
					} else {
						alertDialogBuilder.setPositiveButton(android.R.string.yes, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_YES);
							}
						});
					}
					if (titleNo != null) {
						alertDialogBuilder.setNeutralButton(titleNo, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_NO);
							}
						});
					} else {
						alertDialogBuilder.setNeutralButton(android.R.string.no, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_NO);
							}
						});
					}
					if (titleCancel != null) {
						alertDialogBuilder.setNegativeButton(titleCancel, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_CANCEL);
							}
						});
					} else {
						alertDialogBuilder.setNegativeButton(android.R.string.cancel, new AlertDialog.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								nativeShowResult(nativeObject, RESULT_CANCEL);
							}
						});
					}
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {						
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				default:
					return false;
				}

				AlertDialog alertDialog = alertDialogBuilder.create();
				alertDialog.show();
				return true;
			} catch (Throwable e) {
				Logger.exception(e);
			}
		}
		return false;
	}
	
	
}
