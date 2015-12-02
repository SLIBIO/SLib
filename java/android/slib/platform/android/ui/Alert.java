package slib.platform.android.ui;

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
	public static final int RESULT_FAIL = -1;
	public static final int RESULT_OK = 0;
	public static final int RESULT_CANCEL = 1;
	public static final int RESULT_YES = 2;
	public static final int RESULT_NO = 3;
}
