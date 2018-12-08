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

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.os.Build;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.JavascriptInterface;
import android.webkit.JsPromptResult;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.EditText;

import slib.platform.android.Logger;
import slib.platform.android.ui.UiThread;

public class UiWebView extends WebView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public static UiWebView _create(Context context) {
		try {
			UiWebView ret = new UiWebView(context);
			return ret;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _load(final View view, final String url) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_load(view, url);
				}
			});
			return;
		}
		if (url == null || url.length() == 0) {
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.loadUrl(url);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}
	
	public static void _loadHTML(final View view, final String html, final String url) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_loadHTML(view, html, url);
				}
			});
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				if (url == null || url.length() == 0) {
					wv.loadData(url, "text/html", "utf-8");
				} else {
					wv.loadDataWithBaseURL(url, html, "text/html", "utf-8", null);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static String _getURL(View view) {
		try {
			if (view instanceof WebView) {
				WebView wv = (WebView)view;
				return wv.getUrl();
			}	
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static String _getTitle(View view) {
		try {
			if (view instanceof WebView) {
				WebView wv = (WebView)view;
				return wv.getTitle();
			}	
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _goBack(final View view) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_goBack(view);
				}
			});
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.goBack();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _goForward(final View view) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_goForward(view);
				}
			});
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.goForward();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _reload(final View view) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_reload(view);
				}
			});
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.reload();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _runJavaScript(final View view, final String script) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_runJavaScript(view, script);
				}
			});
			return;
		}
		if (script != null && script.length() > 0) {
			if (view instanceof WebView) {
				final WebView wv = (WebView)view;
				try {
					wv.loadUrl("javascript:" + script);
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		}
	}

	public static void _clearCache(final View view) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_clearCache(view);
				}
			});
			return;
		}

		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				Logger.info("WebView: Clear Cache");
				wv.clearHistory();
				wv.clearCache(true);
				wv.clearFormData();
				wv.clearMatches();
				wv.clearSslPreferences();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _setCustomUserAgent(final View view, final String userAgent) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setCustomUserAgent(view, userAgent);
				}
			});
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView) view;
			try {
				wv.getSettings().setUserAgentString(userAgent);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	private static native void nativeOnStartLoad(long instance, String url);
	public static void onStartLoad(IView view, String url) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnStartLoad(instance, url);
		}
	}
	
	private static native void nativeOnFinishLoad(long instance, String url);
	public static void onFinishLoad(IView view, String url) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnFinishLoad(instance, url);
		}
	}

	private static native void nativeOnErrorLoad(long instance, String url, String error);
	public static void onErrorLoad(IView view, String url, String error) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnErrorLoad(instance, url, error);
		}
	}

	private static native void nativeOnMessage(long instance, String msg, String param);
	public static void onMessage(IView view, String msg, String param) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnMessage(instance, msg, param);
		}
	}
	
	public UiWebView(Context context) {
		super(context);
		
		WebSettings webSettings = this.getSettings();
		webSettings.setJavaScriptEnabled(true);

		webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);
		webSettings.setBuiltInZoomControls(false); // Hide zoom control
		webSettings.setDisplayZoomControls(false);
		webSettings.setSupportZoom(false); // No scaling
		webSettings.setAllowFileAccess(true);
		if (Build.VERSION.SDK_INT >= 16) {
			webSettings.setAllowFileAccessFromFileURLs(true);
			webSettings.setAllowUniversalAccessFromFileURLs(true);
		}
		webSettings.setAllowContentAccess(true);
		webSettings.setDomStorageEnabled(true);
		webSettings.setDatabaseEnabled(true);
		String cacheDirPath = context.getFilesDir().getAbsolutePath()+ "/webcache";
		webSettings.setDatabasePath(cacheDirPath);
		webSettings.setAppCachePath(cacheDirPath);
		webSettings.setAppCacheEnabled(true);
		
		setWebViewClient(new MyWebViewClient());
		setWebChromeClient(new MyWebChromeClient());
		
		requestFocusFromTouch();
		
		addJavascriptInterface(new MyJavaScriptObject(), "slib");
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	class MyWebViewClient extends WebViewClient {
		
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			return false;
		}
		
		public void onPageStarted(WebView view, String url, Bitmap favicon) {
			onStartLoad(UiWebView.this, url);
		}
		
		public void onPageFinished(WebView view, String url) {
			onFinishLoad(UiWebView.this, url);
		}
		
		public void onReceivedError (WebView view, int errorCode, String description, String failingUrl) {
			onErrorLoad(UiWebView.this, failingUrl, description);
		}
		
	}
	
	class MyWebChromeClient extends WebChromeClient {

		@Override
		public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("").setMessage(message).setPositiveButton(android.R.string.ok, null);
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
						return true;
					}
				});
				builder.setCancelable(false);
				AlertDialog dialog = builder.create();
				dialog.show();
				result.confirm();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}

		@Override
		public boolean onJsConfirm(WebView view, String url, String message, final JsResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("")
						.setMessage(message)
						.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,int which) {
								result.confirm();
							}
						})
						.setNeutralButton(android.R.string.no, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.cancel();
							}
						});
				builder.setOnCancelListener(new DialogInterface.OnCancelListener() {
					@Override
					public void onCancel(DialogInterface dialog) {
						result.cancel();
					}
				});
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					@Override
					public boolean onKey(DialogInterface dialog, int keyCode,KeyEvent event) {
						return true;
					}
				});
				AlertDialog dialog = builder.create();
				dialog.show();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}

		@Override
		public boolean onJsPrompt(WebView view, String url, String message, String defaultValue, final JsPromptResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("").setMessage(message);
				final EditText et = new EditText(view.getContext());
				et.setSingleLine();
				et.setText(defaultValue);
				builder.setView(et)
						.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.confirm(et.getText().toString());
							}

						})
						.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.cancel();
							}
						});
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,KeyEvent event) {
						return true;
					}
				});
				AlertDialog dialog = builder.create();
				dialog.show();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}
	}
	
	class MyJavaScriptObject {
		
		@JavascriptInterface
		public void send(String msg, String param) {
			onMessage(UiWebView.this, msg, param);
		}
	}

}
