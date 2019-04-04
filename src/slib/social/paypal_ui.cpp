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

#include "slib/social/paypal.h"
#include "slib/core/log.h"

namespace slib
{

	void PayPal::checkout(const PayPalCheckoutParam& param)
	{
		auto thiz = ToRef(this);
		if (m_accessToken.isNull()) {
			requestAccessToken([thiz, this, param](OAuthAccessTokenResult& result) {
				if (!(result.flagSuccess)) {
					PayPalCheckoutResult r;
					param.onComplete(r);
					return;
				}
				m_accessToken = MakeShared<OAuthAccessToken>(result.accessToken);
				checkout(param);
			});
			return;
		}
		PayPalCreateOrderParam orderParam = param;
		orderParam.onComplete = [thiz, this, param](PayPalCreateOrderResult& result) {
			if (!(result.flagSuccess)) {
				PayPalCheckoutResult r;
				param.onComplete(r);
				return;
			}
			auto dialog = param.dialog;
			if (dialog.isNull()) {
				dialog = OAuthWebRedirectDialog::getDefault();
			}
			OAuthWebRedirectDialogParam dialogParam;
			dialogParam.url = result.approveLink;
			dialogParam.options = param.dialogOptions;
			auto onComplete = param.onComplete;
			auto weakDialog = dialog.toWeak();
			String orderId = result.orderId;
			dialogParam.onRedirect = [thiz, this, weakDialog, param, orderId](const String& url) {
				PayPalCheckoutResult result;
				result.orderId = orderId;
				if (url.isEmpty()) {
					result.flagCancel = sl_true;
					param.onComplete(result);
					return;
				}
				if (url.startsWith(param.returnUrl)) {
					auto dialog = weakDialog.lock();
					if (dialog.isNotNull()) {
						dialog->close();
					}
					result.flagSuccess = sl_true;
					param.onComplete(result);
				} else if (url.startsWith(param.cancelUrl)) {
					auto dialog = weakDialog.lock();
					if (dialog.isNotNull()) {
						dialog->close();
					}
					result.flagCancel = sl_true;
					param.onComplete(result);
				}
			};
			dialog->show(dialogParam);
		};
		createOrder(orderParam);
	}

}
