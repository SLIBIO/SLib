package slib.platform.android.ui;

import android.app.AlertDialog;
import android.content.DialogInterface;
import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class Alert {

	public int type;
	public static final int TYPE_OK = 0;
	public static final int TYPE_OKCANCEL = 1;
	public static final int TYPE_YESNO = 2;
	public static final int TYPE_YESNOCANCEL = 3;
	
	public String text;
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
				boolean flagRight = true;
				AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
				alertDialogBuilder.setTitle(caption);
				alertDialogBuilder.setMessage(text);
				String titleOk = this.titleOk;
				if (titleOk == null) {
					titleOk = "OK";
				}
				String titleCancel = this.titleCancel;
				if (titleCancel == null) {
					titleCancel = "Cancel";
				}
				String titleYes = this.titleYes;
				if (titleYes == null) {
					titleYes = "Yes";
				}
				String titleNo = this.titleNo;
				if (titleNo == null) {
					titleNo = "No";
				}
				switch (type) {
				case TYPE_OK:
					alertDialogBuilder.setPositiveButton(titleOk, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_OK);
						}
					});
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {						
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_OK);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				case TYPE_OKCANCEL:
					alertDialogBuilder.setPositiveButton(titleOk, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_OK);
						}
					});
					alertDialogBuilder.setNegativeButton(titleCancel, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {						
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				case TYPE_YESNO:
					alertDialogBuilder.setPositiveButton(titleYes, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_YES);
						}
					});
					alertDialogBuilder.setNegativeButton(titleNo, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_NO);
						}
					});
					alertDialogBuilder.setCancelable(false);
					break;
				case TYPE_YESNOCANCEL:
					alertDialogBuilder.setPositiveButton(titleYes, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_YES);
						}
					});
					alertDialogBuilder.setNeutralButton(titleNo, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_NO);
						}
					});
					alertDialogBuilder.setNegativeButton(titleCancel, new AlertDialog.OnClickListener() {						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setOnCancelListener(new AlertDialog.OnCancelListener() {						
						@Override
						public void onCancel(DialogInterface dialog) {
							nativeShowResult(nativeObject, RESULT_CANCEL);
						}
					});
					alertDialogBuilder.setCancelable(true);
					break;
				default:
					flagRight = false;
				}
				if (flagRight) {
					AlertDialog alertDialog = alertDialogBuilder.create();
					alertDialog.show();
					return true;
				}
			} catch (Throwable e) {
				Logger.exception(e);
			}
		}
		return false;
	}
	
	
}
