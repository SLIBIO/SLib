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

package slib.platform.android.ui.helper;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

import slib.platform.android.Logger;

public class WebViewFileChooser implements FileChooserListener {

	private WebView webView;
	private int requestCode;
	private FileChooser fileChooser;
	private ValueCallback<Uri[]> callback;
	private ValueCallback<Uri> callbackOld;

	private boolean flagUseDefaultFileChooserParams = true;

	public WebViewFileChooser(WebView webView, int requestCode) {
		this.webView = webView;
		this.requestCode = requestCode;
		this.fileChooser = new FileChooser((Activity)(webView.getContext()), requestCode, this);
		this.fileChooser.setLoadingContent(false);
	}

	@TargetApi(Build.VERSION_CODES.LOLLIPOP)
	public void chooseFile(ValueCallback<Uri[]> callback, WebChromeClient.FileChooserParams fileChooserParams) {
		if (flagUseDefaultFileChooserParams) {
			this.callback = callback;
			this.callbackOld = null;
			((Activity)(webView.getContext())).startActivityForResult(fileChooserParams.createIntent(), requestCode);
		} else {
			String[] acceptTypes = fileChooserParams.getAcceptTypes();
			String acceptTypeTotal = "";
			for (int i = 0; i < acceptTypes.length; i++) {
				String acceptType = acceptTypes[i];
				if (acceptType != null && acceptType.length() > 0) {
					if (acceptTypeTotal.length() > 0) {
						acceptTypeTotal += ";";
					}
					acceptTypeTotal += acceptType;
				}
			}
			if (acceptTypeTotal.length() == 0) {
				acceptTypeTotal = "*";
			}
			chooseFile(acceptTypeTotal, "filesystem", null, callback);
		}
	}

	public void chooseFile(ValueCallback<Uri> callback) {
		chooseFile(callback, "");
	}

	public void chooseFile(ValueCallback<Uri> callback, String acceptType) {
		chooseFile(callback, acceptType, "filesystem");
	}

	public void chooseFile(ValueCallback<Uri> callback, String acceptType, String mediaSource) {
		chooseFile(acceptType, mediaSource, callback, null);
	}

	public void chooseFile(String acceptType, String mediaSource, ValueCallback<Uri> callbackOld, ValueCallback<Uri[]> callback) {
		this.callbackOld = callbackOld;
		this.callback = callback;

		try {
			boolean flagFileSystem = false;
			if (mediaSource == null || mediaSource.length() == 0) {
				flagFileSystem = true;
				mediaSource = "filesystem";
			} else if (mediaSource.equals("filesystem")) {
				flagFileSystem = true;
			}
			if (flagFileSystem) {
				int indexCapture = acceptType.indexOf("capture=");
				if (indexCapture >= 0) {
					indexCapture += 8;
					int lastCapture = acceptType.indexOf(';', indexCapture);
					if (lastCapture < 0) {
						mediaSource = acceptType.substring(indexCapture);
					} else {
						mediaSource = acceptType.substring(indexCapture, lastCapture);
					}
					if (!(mediaSource.equals("filesystem"))) {
						flagFileSystem = false;
					}
				}
			}
			if (acceptType.startsWith("image/")) {
				if (!flagFileSystem && mediaSource.equals("camera")) {
					fileChooser.captureImage();
				} else {
					fileChooser.chooseImage();
				}
			} else if (acceptType.startsWith("video/")) {
				if (!flagFileSystem && mediaSource.equals("camcorder")) {
					fileChooser.captureVideo();
				} else {
					fileChooser.chooseVideo();
				}
			} else if (acceptType.startsWith("audio/")) {
				if (!flagFileSystem && mediaSource.equals("microphone")) {
					fileChooser.captureAudio();
				} else {
					fileChooser.chooseAudio();
				}
			} else {
				fileChooser.chooseFile();
			}
		} catch (Exception e) {
			Logger.exception(e);
			if (callback != null) {
				callback.onReceiveValue(null);
			}
			if (callbackOld != null) {
				callbackOld.onReceiveValue(null);
			}
		}

	}

	@Override
	public void onChooseFile(Uri uri, Object content) {
		if (callback != null) {
			if (uri != null) {
				callback.onReceiveValue(new Uri[]{uri});
			} else {
				callback.onReceiveValue(null);
			}
		}
		if (callbackOld != null) {
			callbackOld.onReceiveValue(uri);
		}
	}

	public void processActivityResult(int resultCode, Intent intent) {
		if (flagUseDefaultFileChooserParams && Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
			if (callback != null) {
				callback.onReceiveValue(WebChromeClient.FileChooserParams.parseResult(resultCode, intent));
			}
		} else {
			fileChooser.processActivityResult(resultCode, intent);
		}
	}

}
