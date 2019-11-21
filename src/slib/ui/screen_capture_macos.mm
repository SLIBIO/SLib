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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/screen_capture.h"

#include "slib/graphics/platform.h"
#include "slib/core/safe_static.h"

#include <AppKit/AppKit.h>

#include <libproc.h>

namespace slib
{
	
	namespace priv
	{
		namespace screen_capture
		{
			
			class Helper
			{
			public:
				Ref<Image> getImage(CGImageRef cgImage)
				{
					sl_uint32 width = (sl_uint32)(CGImageGetWidth(cgImage));
					sl_uint32 height = (sl_uint32)(CGImageGetHeight(cgImage));
					Ref<Bitmap>& bitmap = m_bitmapCache;
					Ref<Canvas>& canvas = m_canvasCache;
					CGContextRef& context = m_contextCache;
					if (bitmap.isNull() || canvas.isNull() || bitmap->getWidth() < width || bitmap->getHeight() < height) {
						bitmap = Bitmap::create(width, height);
						if (bitmap.isNull()) {
							return sl_null;
						}
						canvas = bitmap->getCanvas();
						if (canvas.isNull()) {
							return sl_null;
						}
						context = GraphicsPlatform::getCanvasHandle(canvas.get());
						CGContextTranslateCTM(context, 0, (CGFloat)height);
						CGContextScaleCTM(context, 1, -1);
					}
					if (context) {
						CGContextDrawImage(context, CGRectMake(0, 0, (CGFloat)width, (CGFloat)height), cgImage);
						return bitmap->toImage();
					}
					return sl_null;
				}
				
			private:
				Ref<Bitmap> m_bitmapCache;
				Ref<Canvas> m_canvasCache;
				CGContextRef m_contextCache;
				
			public:
				Mutex m_lock;
				
			};
			
			SLIB_SAFE_STATIC_GETTER(Helper, GetHelper)
			
			Ref<Image> TakeScreenshot(Helper* helper, NSScreen* screen)
			{
				NSRect rectScreen = [screen frame];
				CGImageRef cgImage = CGWindowListCreateImage(rectScreen, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
				if (cgImage) {
					Ref<Image> image = helper->getImage(cgImage);
					CGImageRelease(cgImage);
					return image;
				}
				return sl_null;
			}
			
		}
	}
	
	using namespace priv::screen_capture;

	Ref<Image> ScreenCapture::takeScreenshot()
	{
		Helper* helper = GetHelper();
		if (!helper) {
			return sl_null;
		}
		MutexLocker lock(&(helper->m_lock));
		return TakeScreenshot(helper, [[NSScreen screens] objectAtIndex:0]);
	}
	
	Ref<Image> ScreenCapture::takeScreenshotFromCurrentMonitor()
	{
		Helper* helper = GetHelper();
		if (!helper) {
			return sl_null;
		}
		MutexLocker lock(&(helper->m_lock));
		return TakeScreenshot(helper, [NSScreen mainScreen]);
	}

	List< Ref<Image> >  ScreenCapture::takeScreenshotsFromAllMonitors()
	{
		Helper* helper = GetHelper();
		if (!helper) {
			return sl_null;
		}
		MutexLocker lock(&(helper->m_lock));
		List< Ref<Image> > ret;
		for (NSScreen* screen in [NSScreen screens]) {
			Ref<Image> image = TakeScreenshot(helper, screen);
			if (image.isNotNull()) {
				ret.add_NoLock(Move(image));
			}
		}
		return ret;
	}
	
	sl_bool ScreenCapture::isScreenRecordingEnabled()
	{
		if (@available(macos 10.15, *)) {
			sl_bool bRet = sl_false;
			CFArrayRef list = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
			if (list) {
				sl_uint32 n = (sl_uint32)(CFArrayGetCount(list));
				for (sl_uint32 i = 0; i < n; i++) {
					NSDictionary* info = (NSDictionary*)(CFArrayGetValueAtIndex(list, (CFIndex)i));
					NSString* name = info[(id)kCGWindowName];
					NSNumber* pid = info[(id)kCGWindowOwnerPID];
					if (pid != nil && name != nil) {
						int nPid = [pid intValue];
						char path[PROC_PIDPATHINFO_MAXSIZE+1];
						int lenPath = proc_pidpath(nPid, path, PROC_PIDPATHINFO_MAXSIZE);
						if (lenPath > 0) {
							path[lenPath] = 0;
							if (strcmp(path, "/System/Library/CoreServices/SystemUIServer.app/Contents/MacOS/SystemUIServer") == 0) {
								bRet = sl_true;
								break;
							}
						}
					}
				}
				CFRelease(list);
			}
			return bRet;
		} else {
			return sl_true;
		}
	}

	void ScreenCapture::openSystemPreferencesForScreenRecording()
	{
		if (@available(macos 10.15, *)) {
			[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture"]];
		}
	}

}

#endif
