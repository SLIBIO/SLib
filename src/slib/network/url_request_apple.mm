#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/network/url_request.h"

#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/platform_apple.h"

namespace slib
{
	class _UrlRequest;
}

@interface _Slib_UrlRequestListener : NSObject<NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate> {
}
@end

namespace slib
{

	class _UrlRequestShared
	{
	public:
		NSURLSession* defaultSession;
		NSURLSession* backgroundSession;
		_Slib_UrlRequestListener* listener;
		NSOperationQueue* operationQueue;
		NSFileManager* fileManager;
		HashMap< NSUInteger, WeakRef<_UrlRequest> > requests;
		
	public:
		_UrlRequestShared()
		{
			operationQueue = [[NSOperationQueue alloc] init];
			listener = [[_Slib_UrlRequestListener alloc] init];
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
	
	SLIB_SAFE_STATIC_GETTER(_UrlRequestShared, _getUrlRequestShared)
	
	class _UrlRequest : public UrlRequest
	{
	public:
		NSURLSessionTask* m_task;
		NSUInteger m_taskId;
		
	public:
		_UrlRequest()
		{
			m_task = nil;
			m_taskId = -1;
		}
		
		~_UrlRequest()
		{
			clean();
		}
		
		static Ref<_UrlRequest> create(const UrlRequestParam& param, const String& _url, const String& downloadFilePath)
		{
			_UrlRequestShared* shared = _getUrlRequestShared();
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
							Pair<String, String> pair;
							Iterator< Pair<String, String> > iterator = param.requestHeaders.toIterator();
							while (iterator.next(&pair)) {
								[req setValue:(Apple::getNSStringFromString(pair.value)) forHTTPHeaderField:(Apple::getNSStringFromString(pair.key))];
							}
						}
						{
							Pair<String, String> pair;
							Iterator< Pair<String, String> > iterator = param.additionalRequestHeaders.toIterator();
							while (iterator.next(&pair)) {
								[req addValue:(Apple::getNSStringFromString(pair.value)) forHTTPHeaderField:(Apple::getNSStringFromString(pair.key))];
							}
						}
						NSURLSessionTask* task;
						if (downloadFilePath.isNotEmpty()) {
							task = [session downloadTaskWithRequest:req];
						} else {
							if (param.requestBody.isNotEmpty()) {
								NSData* data = req.HTTPBody;
								req.HTTPBody = nil;
								task = [session uploadTaskWithRequest:req fromData:data];
							} else {
								task = [session dataTaskWithRequest:req];
							}
						}
						if (task != nil) {
							Ref<_UrlRequest> ret = new _UrlRequest;
							if (ret.isNotNull()) {
								ret->_init(param, _url, downloadFilePath);
								ret->m_task = task;
								NSUInteger taskId = task.taskIdentifier;
								ret->m_taskId = taskId;
								shared->requests.put(taskId, ret);
								[task resume];
								return ret;
							}
						}
					}
				}
			}
			return sl_null;
		}
		
		// override
		void _cancel()
		{
			clean();
		}
		
		void clean()
		{
			if (m_task != nil) {
				[m_task cancel];
				m_task = nil;
				_UrlRequestShared* shared = _getUrlRequestShared();
				if (shared) {
					shared->requests.remove(m_taskId);
				}
			}
		}
		
		static Ref<_UrlRequest> fromTask(NSURLSessionTask* task)
		{
			_UrlRequestShared* shared = _getUrlRequestShared();
			if (shared) {
				NSUInteger taskId = [task taskIdentifier];
				return shared->requests.getValue(taskId, WeakRef<_UrlRequest>::null());
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
					Map<String, String> map;
					map.initHash(0, HashIgnoreCaseString(), EqualsIgnoreCaseString());
					if (map.isNotNull()) {
						IMap<String, String>* pmap = map.ref.get();
						[dict enumerateKeysAndObjectsUsingBlock:^(id key, id value, BOOL *stop) {
							pmap->put_NoLock(Apple::getStringFromNSString((NSString*)key), Apple::getStringFromNSString((NSString*)value), MapPutMode::AddAlways);
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
			_UrlRequestShared* shared = _getUrlRequestShared();
			if (shared) {
				NSError* error = nil;
				NSURL* pathDst = [NSURL fileURLWithPath:Apple::getNSStringFromString(m_downloadFilePath)];
				[shared->fileManager removeItemAtURL:pathDst error:nil];
				if ([shared->fileManager moveItemAtURL:pathTempFile toURL:pathDst error:&error]) {
					onComplete();
				} else {
					if (error != nil) {
						NSLog(@"UrlRequest Error: %@", error.localizedDescription);
					}
					onError();
				}
			}
		}
		
		void dispatchComplete(NSError* error)
		{
			if (error != nil) {
				NSLog(@"UrlRequest Error: %@", error.localizedDescription);
				onError();
			} else {
				onComplete();
			}
		}
		
	};
	
	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		return Ref<UrlRequest>::from(_UrlRequest::create(param, url, downloadFilePath));
	}
	
}

@implementation _Slib_UrlRequestListener

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didSendBodyData:(int64_t)bytesSent totalBytesSent:(int64_t)totalBytesSent totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend
{
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(task);
	if (req.isNotNull()) {
		req->dispatchUploadBody(bytesSent, totalBytesSent);
	}
}

-(void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler
{
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(dataTask);
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
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(dataTask);
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
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(downloadTask);
	if (req.isNotNull()) {
		req->dispatchDownloadContent(bytesWritten, totalBytesWritten, totalBytesExpectedToWrite);
	}
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(downloadTask);
	if (req.isNotNull()) {
		req->dispatchFinishDownload(location);
	}
}

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
	slib::Ref<slib::_UrlRequest> req = slib::_UrlRequest::fromTask(task);
	if (req.isNotNull()) {
		req->dispatchComplete(error);
	}
}

@end

#endif
