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

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "slib/network/url_request.h"

#include "slib/core/safe_static.h"
#include "slib/core/platform_apple.h"

namespace slib
{
	class UrlRequest_Impl;
}

@interface _priv_Slib_UrlRequestListener : NSObject<NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate> {
}
@end

namespace slib
{

	class UrlRequest_Shared
	{
	public:
		NSURLSession* defaultSession;
		NSURLSession* backgroundSession;
		_priv_Slib_UrlRequestListener* listener;
		NSOperationQueue* operationQueue;
		NSFileManager* fileManager;
		CHashMap< NSUInteger, WeakRef<UrlRequest_Impl> > requests;
		
	public:
		UrlRequest_Shared()
		{
			operationQueue = [[NSOperationQueue alloc] init];
			listener = [[_priv_Slib_UrlRequestListener alloc] init];
			NSURLSessionConfiguration* configDefault = [NSURLSessionConfiguration defaultSessionConfiguration];
			defaultSession = [NSURLSession sessionWithConfiguration:configDefault delegate:listener delegateQueue:operationQueue];
			NSURLSessionConfiguration* configDownload = [NSURLSessionConfiguration backgroundSessionConfigurationWithIdentifier:@"slib_main_background_url_session"];
			if (configDownload != nil) {
				configDownload.discretionary = YES;
				backgroundSession = [NSURLSession sessionWithConfiguration:configDownload delegate:listener delegateQueue:operationQueue];
			}
			if (backgroundSession == nil) {
				backgroundSession = defaultSession;
			}
			fileManager = [[NSFileManager alloc] init];
		}
		
	};
	
	SLIB_SAFE_STATIC_GETTER(UrlRequest_Shared, Get_UrlRequestShared)
	
	class UrlRequest_Impl : public UrlRequest
	{
	public:
		NSURLSessionTask* m_task;
		NSUInteger m_taskId;
		
	public:
		UrlRequest_Impl()
		{
			m_task = nil;
			m_taskId = -1;
		}
		
		~UrlRequest_Impl()
		{
			clean();
		}
		
		static Ref<UrlRequest_Impl> create(const UrlRequestParam& param, const String& _url)
		{
			UrlRequest_Shared* shared = Get_UrlRequestShared();
			if (shared) {
				NSURLSession* session;
				if (param.flagUseBackgroundSession) {
					session = shared->backgroundSession;
				} else {
					session = shared->defaultSession;
				}
				NSURL* url = [NSURL URLWithString:(Apple::getNSStringFromString(_url))];
				if (url != nil) {
					NSMutableURLRequest* req = [[NSMutableURLRequest alloc] initWithURL:url];
					if (req != nil) {
						req.HTTPMethod = Apple::getNSStringFromString(HttpMethods::toString(param.method));
						req.HTTPBody = [NSData dataWithBytes:param.requestBody.getData() length:param.requestBody.getSize()];
						{
							for (auto& pair : param.requestHeaders) {
								[req setValue:(Apple::getNSStringFromString(pair.value)) forHTTPHeaderField:(Apple::getNSStringFromString(pair.key))];
							}
						}
						{
							for (auto& pair : param.additionalRequestHeaders) {
								[req addValue:(Apple::getNSStringFromString(pair.value)) forHTTPHeaderField:(Apple::getNSStringFromString(pair.key))];
							}
						}
						NSURLSessionTask* task;
						if (param.downloadFilePath.isNotEmpty()) {
							task = [session downloadTaskWithRequest:req];
						} else {
							if (param.requestBody.isNotNull()) {
								NSData* data = req.HTTPBody;
								req.HTTPBody = nil;
								task = [session uploadTaskWithRequest:req fromData:data];
							} else {
								task = [session dataTaskWithRequest:req];
							}
						}
						if (task != nil) {
							Ref<UrlRequest_Impl> ret = new UrlRequest_Impl;
							if (ret.isNotNull()) {
								ret->_init(param, _url);
								ret->m_task = task;
								NSUInteger taskId = task.taskIdentifier;
								ret->m_taskId = taskId;
								shared->requests.put(taskId, ret);
								return ret;
							}
						}
					}
				}
			}
			return sl_null;
		}
		
		void _sendAsync() override
		{
			[m_task resume];
		}
	
		void _cancel() override
		{
			clean();
		}
		
		void clean()
		{
			if (m_task != nil) {
				[m_task cancel];
				m_task = nil;
				UrlRequest_Shared* shared = Get_UrlRequestShared();
				if (shared) {
					shared->requests.remove(m_taskId);
				}
			}
		}
		
		static Ref<UrlRequest_Impl> fromTask(NSURLSessionTask* task)
		{
			UrlRequest_Shared* shared = Get_UrlRequestShared();
			if (shared) {
				NSUInteger taskId = [task taskIdentifier];
				return shared->requests.getValue(taskId, WeakRef<UrlRequest_Impl>::null());
			}
			return sl_null;
		}
		
		void dispatchUploadBody(sl_uint64 bytesSent, sl_uint64 totalBytesSent)
		{
			m_sizeBodySent = totalBytesSent;
			onUploadBody(bytesSent);
		}
		
		sl_bool dispatchReceiveResponse(NSURLResponse* response)
		{
			if ([response isKindOfClass:[NSHTTPURLResponse class]]) {
				NSHTTPURLResponse* http = (NSHTTPURLResponse*)response;
				m_responseStatus = (HttpStatus)(http.statusCode);
				
				NSDictionary* dict = http.allHeaderFields;
				if (dict != nil && [dict count] > 0) {
					HttpHeaderMap map;
					map.init();
					auto cmap = map.ref.get();
					if (cmap) {
						[dict enumerateKeysAndObjectsUsingBlock:^(id key, id value, BOOL *stop) {
							cmap->add_NoLock(Apple::getStringFromNSString((NSString*)key), Apple::getStringFromNSString((NSString*)value));
						}];
						m_responseHeaders = map;
					}
				}
			}
			m_sizeContentTotal = response.expectedContentLength;
			onResponse();
			if (m_flagClosed) {
				return sl_false;
			}
			return sl_true;
		}
		
		void dispatchReceiveContent(NSData* data)
		{
			Memory mem = Apple::getMemoryFromNSData(data);
			onReceiveContent(mem.getData(), mem.getSize(), mem);
		}
		
		void dispatchDownloadContent(sl_uint64 bytesReceived, sl_uint64 totalBytesReceived, sl_uint64 total)
		{
			if (m_sizeContentTotal == 0) {
				m_sizeContentTotal = total;
			}
			onDownloadContent(bytesReceived);
			m_sizeContentReceived = totalBytesReceived;
		}
		
		void dispatchFinishDownload(NSURL* pathTempFile)
		{
			UrlRequest_Shared* shared = Get_UrlRequestShared();
			if (shared) {
				NSError* error = nil;
				NSURL* pathDst = [NSURL fileURLWithPath:Apple::getNSStringFromString(m_downloadFilePath)];
				[shared->fileManager removeItemAtURL:pathDst error:nil];
				if ([shared->fileManager moveItemAtURL:pathTempFile toURL:pathDst error:&error]) {
					onComplete();
				} else {
					String strError;
					if (error != nil) {
						strError = Apple::getStringFromNSString(error.localizedDescription);
					} else {
						strError = String::format("Moving downloaded file failed: %s=>%s", Apple::getStringFromNSString(pathTempFile.absoluteString), m_downloadFilePath);
					}
					m_lastErrorMessage = strError;
					onError();
				}
			}
		}
		
		void dispatchComplete(NSError* error)
		{
			if (error != nil) {
				String strError = Apple::getStringFromNSString(error.localizedDescription);
				m_lastErrorMessage = strError;
				onError();
			} else {
				onComplete();
			}
		}
		
	};
	
	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url)
	{
		return Ref<UrlRequest>::from(UrlRequest_Impl::create(param, url));
	}
	
}

@implementation _priv_Slib_UrlRequestListener

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didSendBodyData:(int64_t)bytesSent totalBytesSent:(int64_t)totalBytesSent totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(task);
	if (req.isNotNull()) {
		req->dispatchUploadBody(bytesSent, totalBytesSent);
	}
}

-(void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(dataTask);
	if (req.isNotNull()) {
		if (req->dispatchReceiveResponse(response)) {
			completionHandler(NSURLSessionResponseAllow);
			return;
		}
	}
	completionHandler(NSURLSessionResponseCancel);
}

-(void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveData:(NSData *)data
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(dataTask);
	if (req.isNotNull()) {
		req->dispatchReceiveContent(data);
	}
}

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask willCacheResponse:(NSCachedURLResponse *)proposedResponse completionHandler:(void (^)(NSCachedURLResponse* cachedResponse))completionHandler
{
	completionHandler(nil);
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(downloadTask);
	if (req.isNotNull()) {
		req->dispatchDownloadContent(bytesWritten, totalBytesWritten, totalBytesExpectedToWrite);
	}
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(downloadTask);
	if (req.isNotNull()) {
		req->dispatchFinishDownload(location);
	}
}

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
	slib::Ref<slib::UrlRequest_Impl> req = slib::UrlRequest_Impl::fromTask(task);
	if (req.isNotNull()) {
		req->dispatchComplete(error);
	}
}

@end

#endif
