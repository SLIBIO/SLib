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

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/ui/core.h"
#include "slib/ui/platform.h"

#include "slib/core/safe_static.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

@interface SLIBFacebookDelegate : NSObject<FBSDKSharingDelegate>
{
	@public slib::Function<void(slib::FacebookShareResult&)> onComplete;
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace facebook
		{
			
			class FacebookSDKContext : public Referable
			{
			public:
				FBSDKLoginManager* loginManager;
				SLIBFacebookDelegate* shareDelegate;
				
			public:
				FacebookSDKContext()
				{
					loginManager = [[FBSDKLoginManager alloc] init];
					shareDelegate = [[SLIBFacebookDelegate alloc] init];
				}
				
				static FacebookSDKContext* get()
				{
					SLIB_SAFE_STATIC(Ref<FacebookSDKContext>, s, new FacebookSDKContext)
					if (SLIB_SAFE_STATIC_CHECK_FREED(s)) {
						return sl_null;
					}
					return s.get();
				}
				
				static FBSDKLoginManager* getLoginManager()
				{
					FacebookSDKContext* sdk = get();
					if (sdk) {
						return sdk->loginManager;
					}
					return nil;
				}
				
				static SLIBFacebookDelegate* getShareDelegate()
				{
					FacebookSDKContext* sdk = get();
					if (sdk) {
						return sdk->shareDelegate;
					}
					return nil;
				}
			};
			
			static void getToken(OAuthAccessToken& _out, FBSDKAccessToken* _in)
			{
				_out.token = Apple::getStringFromNSString(_in.tokenString);
				_out.expirationTime = Apple::getTimeFromNSDate(_in.expirationDate);
				_out.refreshTime = Apple::getTimeFromNSDate(_in.refreshDate);
				{
					List<String> permissions;
					if (_in.permissions != nil) {
						for (id item in _in.permissions) {
							NSString* s = (NSString*)item;
							permissions.add_NoLock(Apple::getStringFromNSString(s));
						}
					}
					_out.scopes = permissions;
				}
			}
			
		}
	}
	
	using namespace priv::facebook;
	
	void FacebookSDK::initialize()
	{
		UIPlatform::registerDidFinishLaunchingCallback([](NSDictionary* launchOptions) {
			[[FBSDKApplicationDelegate sharedInstance] application:([::UIApplication sharedApplication]) didFinishLaunchingWithOptions:launchOptions];
		});
		UIPlatform::registerOpenUrlCallback([](NSURL* url, NSDictionary* options) {
			return [[FBSDKApplicationDelegate sharedInstance] application:([::UIApplication sharedApplication])
																  openURL:url
														sourceApplication:options[UIApplicationOpenURLOptionsSourceApplicationKey]
															   annotation:options[UIApplicationOpenURLOptionsAnnotationKey]
					];
		});
	}
	
	void FacebookSDK::_updateCurrentToken(Facebook* instance)
	{
		FBSDKAccessToken* token = [FBSDKAccessToken currentAccessToken];
		if (token != nil) {
			OAuthAccessToken oauthToken;
			getToken(oauthToken, token);
			instance->setAccessToken(oauthToken);
		}
	}
	
	void FacebookSDK::login(const FacebookLoginParam& param)
	{
		FBSDKLoginManager* manager = FacebookSDKContext::getLoginManager();
		if (manager == nil) {
			FacebookLoginResult result;
			param.onComplete(result);
			return;
		}
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread([param]() {
				FacebookSDK::login(param);
			});
			return;
		}
		
		NSMutableArray* array = [[NSMutableArray alloc] init];
		{
			List<String> permissions;
			if (param.authorization.scopes.isEmpty()) {
				permissions.add_NoLock("public_profile");
				permissions.add_NoLock("email");
			} else {
				permissions = param.authorization.scopes;
			}
			ListLocker<String> items(permissions);
			for (sl_size i = 0; i < items.count; i++) {
				NSString* s = Apple::getNSStringFromString(items[i]);
				[array addObject:s];
			}
		}
		Function<void(FacebookLoginResult&)> onComplete = param.onComplete;
		UIViewController* controller = UIPlatform::getCurrentViewController();
		if (controller == nil) {
			FacebookLoginResult result;
			param.onComplete(result);
			return;
		}
		[manager logInWithPermissions:array fromViewController:controller handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
			FacebookLoginResult login;
			if (error != nil) {
				NSString* message = [error localizedDescription];
				NSLog(@"Facebook Login Error: %@", message);
				login.errorDescription = Apple::getStringFromNSString(message);
				onComplete(login);
				return;
			}
			if (result.isCancelled) {
				login.flagCancel = sl_true;
				onComplete(login);
				return;
			}
			FBSDKAccessToken* token = result.token;
			if (token == nil) {
				onComplete(login);
				return;
			}
			login.flagSuccess = sl_true;
			getToken(login.accessToken, token);
			onComplete(login);
		}];
	}
	
	void FacebookSDK::share(const FacebookShareParam& param)
	{
		if (param.url.isEmpty()) {
			FacebookShareResult result;
			param.onComplete(result);
			return;
		}
		SLIBFacebookDelegate* delegate = FacebookSDKContext::getShareDelegate();
		if (delegate == nil) {
			FacebookShareResult result;
			param.onComplete(result);
			return;
		}
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread([param]() {
				FacebookSDK::share(param);
			});
			return;
		}
		
		FBSDKShareLinkContent *content = [FBSDKShareLinkContent new];
		content.contentURL = [NSURL URLWithString:Apple::getNSStringFromString(param.url)];
		if (param.quote.isNotEmpty()) {
			content.quote = Apple::getNSStringFromString(param.quote);
		}
		if (param.hashTag.isNotEmpty()) {
			content.hashtag = [FBSDKHashtag hashtagWithString:Apple::getNSStringFromString(param.hashTag)];
		}
		FBSDKShareDialog *dialog = [FBSDKShareDialog new];
		dialog.fromViewController = UIPlatform::getCurrentViewController();
		dialog.shareContent = content;
		dialog.mode = FBSDKShareDialogModeAutomatic;
		
		dialog.delegate = delegate;
		if (delegate->onComplete.isNotNull()) {
			FacebookShareResult result;
			delegate->onComplete(result);
		}
		delegate->onComplete = param.onComplete;
		
		[dialog show];
	}
	
	void FacebookSDK::clearAccessToken()
	{
		FBSDKLoginManager* manager = FacebookSDKContext::getLoginManager();
		if (manager == nil) {
			[manager logOut];
		}
	}
	
}

@implementation SLIBFacebookDelegate

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results
{
	slib::FacebookShareResult result;
	result.flagSuccess = sl_true;
	self->onComplete(result);
	self->onComplete.setNull();
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error
{
	NSLog(@"Facebook Sharing Error: %@", [error localizedDescription]);
	slib::FacebookShareResult result;
	self->onComplete(result);
	self->onComplete.setNull();
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer
{
	slib::FacebookShareResult result;
	result.flagCancel = sl_true;
	self->onComplete(result);
	self->onComplete.setNull();
}

@end

#endif
