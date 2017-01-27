#include "../../../inc/slib/media/dshow.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/core/win32_com.h"
#include "../../../inc/slib/core/log.h"

#include <dshow.h>

EXTERN_C const IID IID_ISampleGrabberCB;

MIDL_INTERFACE("0579154A-2B53-4994-B0D0-E773148EFF85")
ISampleGrabberCB : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SampleCB(
		double SampleTime,
		IMediaSample *pSample) = 0;

	virtual HRESULT STDMETHODCALLTYPE BufferCB(
		double SampleTime,
		BYTE *pBuffer,
		long BufferLen) = 0;
};

EXTERN_C const IID IID_ISampleGrabber;

MIDL_INTERFACE("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")
ISampleGrabber : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetOneShot(
		BOOL OneShot) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetMediaType(
		const AM_MEDIA_TYPE *pType) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(
		AM_MEDIA_TYPE *pType) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(
		BOOL BufferThem) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(
		/* [out][in] */ long *pBufferSize,
		/* [out] */ long *pBuffer) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(
		/* [retval][out] */ IMediaSample **ppSample) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetCallback(
		ISampleGrabberCB *pCallback,
		long WhichMethodToCallback) = 0;
};

EXTERN_C const CLSID CLSID_SampleGrabber;
EXTERN_C const CLSID CLSID_NullRenderer;

#pragma comment(lib, "strmiids.lib")

SLIB_MEDIA_NAMESPACE_BEGIN

class _DShow_Camera : public Camera, public ISampleGrabberCB
{
public:
	ICaptureGraphBuilder2* m_capture;
	IGraphBuilder* m_graph;
	IMediaControl* m_control;
	ISampleGrabber* m_grabber;

	sl_bool m_flagRunning;

public:
	_DShow_Camera()
	{
		m_capture = sl_null;
		m_graph = sl_null;
		m_control = sl_null;
		m_grabber = sl_null;

		m_flagRunning = sl_false;
	}

	~_DShow_Camera()
	{
		release();
	}

public:
	static void logError(String error) {
		Log("Camera", error);
	}

	static void logError(String error, HRESULT hr) {
		Log("Camera", "%s (Result=%d)", error, (sl_int32)hr);
	}

	static Ref<_DShow_Camera> _create(const CameraParam& param)
	{
		Ref<_DShow_Camera> ret;
		HRESULT hr;

		IBaseFilter* filterSource = NULL;
		_queryDevices(param.deviceId, &filterSource);

		if (filterSource) {

			IGraphBuilder* graph = NULL;
			hr = ::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graph);
			if (graph) {
				ICaptureGraphBuilder2* capture = NULL;
				hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&capture);
				if (capture) {
					IMediaControl* control = NULL;
					hr = graph->QueryInterface(IID_IMediaControl, (LPVOID*)&control);
					if (control) {
						IBaseFilter* filterGrabber = NULL;
						hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&filterGrabber);
						if (filterGrabber) {
							IBaseFilter* filterNullRenderer = NULL;
							hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&filterNullRenderer);
							if (filterNullRenderer) {

								hr = capture->SetFiltergraph(graph);
								if (SUCCEEDED(hr)) {
									hr = graph->AddFilter(filterSource, L"Video Capture");
									if (SUCCEEDED(hr)) {
										hr = graph->AddFilter(filterGrabber, L"Sample Grabber");
										if (SUCCEEDED(hr)) {
											ISampleGrabber* grabber = NULL;
											hr = filterGrabber->QueryInterface(IID_ISampleGrabber, (void**)&grabber);
											if (grabber) {
												AM_MEDIA_TYPE mt;
												ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
												mt.majortype = MEDIATYPE_Video;
												mt.subtype = MEDIASUBTYPE_RGB24;
												hr = grabber->SetMediaType(&mt);
												if (SUCCEEDED(hr)) {
													hr = graph->AddFilter(filterNullRenderer, L"Null Renderer");
													if (SUCCEEDED(hr)) {
														hr = capture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, filterSource, filterGrabber, filterNullRenderer);
														if (SUCCEEDED(hr)) {
															ret = new _DShow_Camera();
															if (ret.isNotNull()) {
																hr = grabber->SetCallback(ret.get(), 0);
																if (SUCCEEDED(hr)) {
																	ret->m_capture = capture;
																	ret->m_graph = graph;
																	ret->m_control = control;
																	ret->m_grabber = grabber;
																	ret->_init(param);
																	if (param.flagAutoStart) {
																		ret->start();
																	}
																	return ret;
																} else {
																	logError("Failed to set capture callback", hr);
																	ret.setNull();
																}
															}
														} else {
															logError("Failed to render capture stream", hr);
														}
													} else {
														logError("Failed to add null rendering filter", hr);
													}
												} else {
													logError("Failed to set grabber media type", hr);
												}
											} else {
												logError("Failed to query sample grabber", hr);
											}
										} else {
											logError("Failed to add sample grabber filter", hr);
										}
									} else {
										logError("Failed to add source filter", hr);
									}
								} else {
									logError("Failed to set FilterGraph", hr);
								}

								filterNullRenderer->Release();
							} else {
								logError("Failed to create CLSID_NullRenderer", hr);
							}
							filterGrabber->Release();
						} else {
							logError("Failed to create CLSID_SampleGrabber", hr);
						}
						control->Release();
					} else {
						logError("Failed to query IMediaControl", hr);
					}
					capture->Release();
				} else {
					logError("Failed to create CLSID_CaptureGraphBuilder2", hr);
				}
				graph->Release();
			} else {
				logError("Failed to create CLSID_FilterGraph", hr);
			}

			filterSource->Release();

		} else {
			logError("Failed to find capture device: " + param.deviceId);
		}

		return ret;
	}

	void release()
	{
		ObjectLocker lock(this);
		if (m_capture) {
			stop();
			SLIB_WIN32_COM_SAFE_RELEASE(m_capture);
			SLIB_WIN32_COM_SAFE_RELEASE(m_graph);
			SLIB_WIN32_COM_SAFE_RELEASE(m_control);
			SLIB_WIN32_COM_SAFE_RELEASE(m_grabber);
		}
	}

	sl_bool isOpened()
	{
		return m_capture != sl_null;
	}

	void start()
	{
		ObjectLocker lock(this);
		if (m_flagRunning) {
			return;
		}
		if (m_capture && m_control) {
			HRESULT hr = m_control->Run();
			if (SUCCEEDED(hr)) {
				m_flagRunning = sl_true;
			} else {
				OAFilterState fs;
				if (SUCCEEDED(m_control->GetState(10, &fs))) {
					return logError("Device is already in use", hr);
				} else {
					return logError("Failed to start capture", hr);
				}
			}
		}
	}

	void stop()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		if (m_capture && m_control) {
			m_control->Stop();
		}
		m_flagRunning = sl_false;
	}

	sl_bool isRunning()
	{
		return m_flagRunning;
	}

	HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen) {
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample) {

		HRESULT hr;

		long cbBuffer = 0;
		char* pBuffer = NULL;

		pSample->GetPointer((BYTE**)&pBuffer);
		cbBuffer = pSample->GetSize();
		if (!pBuffer) {
			return E_FAIL;
		}
		AM_MEDIA_TYPE mt;
		hr = m_grabber->GetConnectedMediaType(&mt);
		if (FAILED(hr)) {
			return hr;
		}

		VideoCaptureFrame frame;
		if (mt.majortype == MEDIATYPE_Video) {
			if (mt.formattype == FORMAT_VideoInfo && mt.cbFormat >= sizeof(VIDEOINFOHEADER)) {
				VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)(mt.pbFormat);
				frame.image.width = vih->bmiHeader.biWidth;
				frame.image.height = vih->bmiHeader.biHeight;
				frame.image.format = BitmapFormat::BGR;
				frame.image.pitch =  - frame.image.calculatePitchAlign4(frame.image.width, 24);
				frame.image.data = pBuffer + cbBuffer + frame.image.pitch;
				if (frame.image.format != BitmapFormat::None) {
					onCaptureVideoFrame(&frame);
				}
			}
		}

		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) {
			*ppv = (void*)(ISampleGrabberCB*)(this);
			return NOERROR;
		}
		return E_NOINTERFACE;
	}

	static List<CameraInfo> _queryDevices(String deviceId, IBaseFilter** filter)
	{
		List<CameraInfo> ret;
		HRESULT hr;

		if (deviceId == "FRONT" || deviceId == "BACK") {
			deviceId.setNull();
		}

		ICreateDevEnum* pDevEnum = NULL;
		hr = ::CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pDevEnum);
		if (pDevEnum) {

			IEnumMoniker* pClassEnum = NULL;
			hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);

			if (pClassEnum) {
				
				IMoniker* pMoniker = NULL;
				ULONG cFetched = 0;

				while (S_OK == pClassEnum->Next(1, &pMoniker, &cFetched)) {

					CameraInfo dev;

					if (pMoniker) {

						IPropertyBag* prop = NULL;
						hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&prop);

						if (prop) {

							VARIANT var;

							::VariantInit(&var);
							hr = prop->Read(L"DevicePath", &var, 0);
							if (SUCCEEDED(hr)) {
								dev.id = var.bstrVal;
							}
							::VariantClear(&var);

							::VariantInit(&var);
							hr = prop->Read(L"FriendlyName", &var, 0);
							if (SUCCEEDED(hr)) {
								dev.name = var.bstrVal;
							}
							::VariantClear(&var);

							::VariantInit(&var);
							hr = prop->Read(L"Description", &var, 0);
							if (SUCCEEDED(hr)) {
								dev.description = var.bstrVal;
							}
							::VariantClear(&var);

							prop->Release();

						}

						if (dev.id.isNotEmpty()) {
							if (filter) {
								if (deviceId.isEmpty() || deviceId == dev.id) {
									hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)filter);
									if (FAILED(hr)) {
										logError("Failed to bind Filter", hr);
									}
									return ret;
								}
							} else {
								ret.add_NoLock(dev);
							}
						}
						
						pMoniker->Release();
						pMoniker = NULL;
					}
				}

				pClassEnum->Release();
			} else {
				logError("Failed to create CLSID_VideoInputDeviceCategory", hr);
			}

			pDevEnum->Release();

		} else {
			logError("Failed to create CLSID_SystemDeviceEnum", hr);
		}

		return ret;

	}
};

Ref<Camera> DirectShow::createCamera(const CameraParam& param)
{
	return _DShow_Camera::_create(param);
}

List<CameraInfo> DirectShow::getCamerasList()
{
	return _DShow_Camera::_queryDevices(String::null(), NULL);
}

SLIB_MEDIA_NAMESPACE_END

#else

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<Camera> DirectShow::createCamera(const CameraParam& param)
{
	return sl_null;
}

List<CameraInfo> DirectShow::getCamerasList()
{
	return sl_null;
}

SLIB_MEDIA_NAMESPACE_END

#endif
