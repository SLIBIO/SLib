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

#include <slib/social/facebook.h>

#if defined(SLIB_PLATFORM_IS_IOS)

#include <slib/ui/platform.h>
#include <slib/core/safe_static.h>

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

namespace slib
{
	
	void Facebook::initializeOnStartApp()
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
	
	class _priv_FacebookLoginManager : public Referable
	{
	public:
		FBSDKLoginManager* loginManager;
		
	public:
		_priv_FacebookLoginManager()
		{
			loginManager = [[FBSDKLoginManager alloc] init];
			loginManager.loginBehavior = FBSDKLoginBehaviorNative;
		}
		
		static FBSDKLoginManager* get()
		{
			SLIB_SAFE_STATIC(Ref<_priv_FacebookLoginManager>, s, new _priv_FacebookLoginManager)
			if (SLIB_SAFE_STATIC_CHECK_FREED(s)) {
				return nil;
			}
			return s->loginManager;
		}
		
	};
	
	static void _priv_Facebook_getToken(FacebookAccessToken& _out, FBSDKAccessToken* _in)
	{
		_out.userId = Apple::getStringFromNSString(_in.userID);
		_out.token = Apple::getStringFromNSString(_in.tokenString);
		_out.expirationDate = Apple::getTimeFromNSDate(_in.expirationDate);
		_out.refreshDate = Apple::getTimeFromNSDate(_in.refreshDate);
		{
			List<String> permissions;
			if (_in.permissions != nil) {
				for (id item in _in.permissions) {
					NSString* s = (NSString*)item;
					permissions.add_NoLock(Apple::getStringFromNSString(s));
				}
			}
			_out.permissions = permissions;
		}
		{
			List<String> permissions;
			if (_in.declinedPermissions != nil) {
				for (id item in _in.declinedPermissions) {
					NSString* s = (NSString*)item;
					permissions.add_NoLock(Apple::getStringFromNSString(s));
				}
			}
			_out.declinedPermissions = permissions;
		}
	}
	
	sl_bool Facebook::getCurrentToken(FacebookAccessToken* _out)
	{
		FBSDKAccessToken* token = [FBSDKAccessToken currentAccessToken];
		if (token != nil) {
			if (_out) {
				_priv_Facebook_getToken(*_out, token);
			}
			return sl_true;
		}
		return sl_false;
	}
	
	void Facebook::login(const FacebookLoginParam& param)
	{
		FBSDKLoginManager* manager = _priv_FacebookLoginManager::get();
		if (manager == nil) {
			FacebookLoginResult result;
			param.onComplete(result);
			return;
		}
		NSMutableArray* array = [[NSMutableArray alloc] init];
		{
			List<String> permissions = param.permissions;
			if (param.permissions.isEmpty()) {
				permissions.add_NoLock("public_profile");
				permissions.add_NoLock("email");
			} else {
				permissions = param.permissions;
			}
			ListLocker<String> items(permissions);
			for (sl_size i = 0; i < items.count; i++) {
				NSString* s = Apple::getNSStringFromString(items[i]);
				[array addObject:s];
			}
		}
		Function<void(FacebookLoginResult&)> callback = param.onComplete;
		UIViewController* controller = UIPlatform::getKeyWindow().rootViewController;
		auto func = ^(FBSDKLoginManagerLoginResult *result, NSError *error) {
			FacebookLoginResult login;
			if (error != nil) {
				NSString* message = [error localizedDescription];
				NSLog(@"Facebook Login Error: %@", message);
				login.errorMessage = Apple::getStringFromNSString(message);
				callback(login);
				return;
			}
			login.flagError = sl_false;
			if (result.isCancelled) {
				login.flagCancel = sl_true;
				callback(login);
				return;
			}
			{
				List<String> permissions;
				if (result.grantedPermissions != nil) {
					for (id item in result.grantedPermissions) {
						NSString* s = (NSString*)item;
						permissions.add_NoLock(Apple::getStringFromNSString(s));
					}
				}
				login.grantedPermissions = permissions;
			}
			{
				List<String> permissions;
				if (result.declinedPermissions != nil) {
					for (id item in result.declinedPermissions) {
						NSString* s = (NSString*)item;
						permissions.add_NoLock(Apple::getStringFromNSString(s));
					}
				}
				login.declinedPermissions = permissions;
			}
			FBSDKAccessToken* token = result.token;
			if (token == nil) {
				NSLog(@"Facebook Login Error: Token is not returned!");
				login.flagError = sl_true;
				callback(login);
				return;
			}
			_priv_Facebook_getToken(login.token, token);
			callback(login);
		};
		if (param.flagPublishPermissions) {
			[manager logInWithPublishPermissions:array fromViewController:controller handler:func];
		} else {
			[manager logInWithReadPermissions:array fromViewController:controller handler:func];
		}
	}
	
}

#endif
