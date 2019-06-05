/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/social/facebook.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(FacebookResolveUserUrlParam)
	
	FacebookResolveUserUrlParam::FacebookResolveUserUrlParam()
	{
	}
	
	void Facebook::resolveUserUrl(const FacebookResolveUserUrlParam& param)
	{
		auto onComplete = param.onComplete;
		auto dialog = param.dialog;
		if (dialog.isNull()) {
			dialog = OAuthWebRedirectDialog::getDefault();
		}
		OAuthWebRedirectDialogParam dialogParam;
		dialogParam.url = "https://www.facebook.com/me";
		dialogParam.options = param.dialogOptions;
		auto weakDialog = dialog.toWeak();
		dialogParam.onRedirect = [weakDialog, onComplete](const String& _url) {
			if (_url.isEmpty()) {
				onComplete(sl_null);
				return;
			}
			if (_url.startsWith("https://www.facebook.com/") || _url.startsWith("https://m.facebook.com/")) {
				Url url(_url);
				String path = url.path;
				sl_size n = path.getLength();
				if (n >= 6) {
					sl_char8* sz = path.getData();
					sl_bool flagValid = sz[0] == '/';
					if (flagValid) {
						for (sl_size i = 1; i < n; i++) {
							sl_char8 ch = sz[i];
							if (!(SLIB_CHAR_IS_ALNUM(ch) || ch == '.')) {
								flagValid = sl_false;
								break;
							}
						}
					}
					if (flagValid) {
						auto dialog = weakDialog.lock();
						if (dialog.isNotNull()) {
							dialog->close();
						}
						onComplete("https://www.facebook.com" + path);
					}
				}
			}
		};
		dialog->show(dialogParam);
	}
	
	void Facebook::resolveUserUrl(const Function<void(const String& url)>& onComplete)
	{
		FacebookResolveUserUrlParam param;
		param.onComplete = onComplete;
		resolveUserUrl(param);
	}
	
}
