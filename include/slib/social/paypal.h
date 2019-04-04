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

#ifndef CHECKHEADER_SLIB_SOCIAL_PAYPAL
#define CHECKHEADER_SLIB_SOCIAL_PAYPAL

#include "definition.h"

#include "oauth.h"

namespace slib
{
	
	typedef OAuthApiResult PayPalResult;
	
	enum class PayPalOrderIntent
	{
		CAPTURE,
		AUTHORIZE
	};
	
	enum class PayPalOrderStatus
	{
		None,
		CREATED,
		SAVED,
		APPROVED,
		VOIDED,
		COMPLETED
	};
	
	enum class PayPalLandingPage
	{
		Default,
		LOGIN,
		BILLING
	};
	
	enum class PayPalShippingPreference
	{
		Default,
		GET_FROM_FILE,
		NO_SHIPPING,
		SET_PROVIDED_ADDRESS
	};
	
	enum class PayPalOrderUserAction
	{
		Default,
		CONTINUE,
		PAY_NOW
	};
	
	class PayPalCreateOrderResult : public PayPalResult
	{
	public:
		String orderId;
		PayPalOrderStatus status;
		String approveLink;
		
	public:
		PayPalCreateOrderResult(UrlRequest*);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PayPalCreateOrderResult);
		
	};
	
	class PayPalCreateOrderParam
	{
	public:
		PayPalOrderIntent intent;
		double amount;
		String currencyCode;
		
		String brandName;
		String locale;
		PayPalLandingPage landingPage;
		PayPalShippingPreference shippingPreference;
		PayPalOrderUserAction userAction;
		String returnUrl;
		String cancelUrl;

		Function<void(PayPalCreateOrderResult&)> onComplete;
		
	public:
		PayPalCreateOrderParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PayPalCreateOrderParam)
		
	};
	
	
	class PayPalCheckoutResult
	{
	public:
		sl_bool flagSuccess;
		sl_bool flagCancel;
		
		String orderId;
		
	public:
		PayPalCheckoutResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PayPalCheckoutResult);
		
	};
	
	class PayPalCheckoutParam : public PayPalCreateOrderParam
	{
	public:
		OAuthWebRedirectDialogOptions dialogOptions;
		Ptr<OAuthWebRedirectDialog> dialog;
		
		Function<void(PayPalCheckoutResult&)> onComplete;

	public:
		PayPalCheckoutParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PayPalCheckoutParam)
		
	};
	
	class SLIB_EXPORT PayPalParam : public OAuthParam
	{
	public:
		sl_bool flagSandbox;
		
	public:
		PayPalParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PayPalParam)
		
	};
	
	class PayPal : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		PayPal(const PayPalParam& param);
		
		~PayPal();
		
	public:
		static Ref<PayPal> create(const PayPalParam& param);
		
		static void initialize(const PayPalParam& param);
		
		static void initialize(const String& clientId, const String& clientSecret);
		
		static void initializeSandbox(const String& clientId, const String& clientSecret);

		static void initializeWithAccessToken(const String& accessToken, sl_bool flagSandbox = sl_false);

		static Ref<PayPal> getInstance();
		
	public:
		String getRequestUrl(const String& path);

		String getRequestUrl_v2(const String& path);

		void createOrder(const PayPalCreateOrderParam& param);
		
		// Note that you should initialize PayPal instance with `accessToken` (which is granted on your secure server containing `clientSecret`) on mobile/desktop apps.
		void checkout(const PayPalCheckoutParam& param);
		
	public:
		sl_bool m_flagSandbox;
		
	};
	
	
	
}

#endif
