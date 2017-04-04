/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/ui/web_view.h"
#include "slib/core/win32_com.h"

#include "view_win32.h"

#include <exdisp.h>
#include <exdispid.h>
#include <mshtml.h>
#include <mshtmhst.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace slib
{

	class _Win32_WebView_OleClient;

	class _Win32_WebViewInstance : public Win32_ViewInstance
	{
	public:
		_Win32_WebView_OleClient* m_oleClient;

		IOleObject* m_control;
		IWebBrowser2* m_browser;

		DWORD m_eventCookie;

	public:
		_Win32_WebViewInstance();
		~_Win32_WebViewInstance();

	public:
		// override
		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
		{
			return sl_false;
		}

		// override
		sl_bool processCommand(SHORT code, LRESULT& result)
		{
			return sl_false;
		}

		// override
		sl_bool processNotify(NMHDR* nmhdr, LRESULT& result)
		{
			return sl_false;
		}

		sl_bool _initialize();

		void _resize()
		{
			IWebBrowser2* browser = m_browser;
			if (browser) {
				HWND hWnd = getHandle();
				RECT rc;
				::GetClientRect(hWnd, &rc);
				browser->put_Left(0);
				browser->put_Top(0);
				browser->put_Width(rc.right);
				browser->put_Width(rc.bottom);
			}
		}

		IHTMLDocument2* getDoc()
		{
			IWebBrowser2* browser = m_browser;
			if (!browser) {
				return NULL;
			}
			IDispatch* document = NULL;
			HRESULT hr = browser->get_Document(&document);
			IHTMLDocument2* ret = NULL;
			if (hr == S_OK) {
				hr = document->QueryInterface(IID_IHTMLDocument2, (void**)(&ret));
				document->Release();
			}
			return ret;
		}

		void __installExternal();

		void __onBeforeNavigate2(BSTR szURL, VARIANT_BOOL* pFlagCancel)
		{
			Ref<View> _view = getView();
			if (WebView* view = CastInstance<WebView>(_view.get())) {
				view->dispatchStartLoad(szURL);
				*pFlagCancel = 0;
			}
		}

		void __onNavigateComplete(BSTR szURL)
		{
			Ref<View> _view = getView();
			if (WebView* view = CastInstance<WebView>(_view.get())) {
				__installExternal();
			}
		}

		void __onDocumentComplete(BSTR szURL)
		{
			Ref<View> _view = getView();
			if (WebView* view = CastInstance<WebView>(_view.get())) {
				view->dispatchFinishLoad(szURL, sl_false);
			}
		}

	};

	class _Win32_WebView_OleClient : public IOleClientSite, public IOleInPlaceSite, public IOleInPlaceFrame, public IDocHostUIHandler, public IDispatch
	{
	public:
		_Win32_WebViewInstance* m_viewInstance;

		_Win32_WebView_OleClient()
		{
		}

		// IUnknown
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject)
		{
			*ppvObject = NULL;
			if (Base::equalsMemory(&iid, &IID_IUnknown, sizeof(IID))) {
				*ppvObject = (IUnknown*)(IOleClientSite*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IOleClientSite, sizeof(IID))) {
				*ppvObject = (IOleClientSite*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IOleInPlaceSite, sizeof(IID))) {
				*ppvObject = (IOleInPlaceSite*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IDocHostUIHandler, sizeof(IID))) {
				*ppvObject = (IDocHostUIHandler*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IOleInPlaceFrame, sizeof(IID))) {
				*ppvObject = (IOleInPlaceFrame*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IDispatch, sizeof(IID))) {
				*ppvObject = (IDispatch*)this;
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef()
		{
			return 1;
		}

		ULONG STDMETHODCALLTYPE Release()
		{
			return 1;
		}

		// IOleClientSite
		HRESULT STDMETHODCALLTYPE SaveObject()
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **ppContainer)
		{
			*ppContainer = NULL;
			return E_NOINTERFACE;
		}

		HRESULT STDMETHODCALLTYPE ShowObject()
		{
			return NOERROR;
		}

		HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE RequestNewObjectLayout()
		{
			return E_NOTIMPL;
		}

		// IOleWindow
		HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd)
		{
			*phwnd = m_viewInstance->getHandle();
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode)
		{
			return E_NOTIMPL;
		}

		// IOleInPlaceSite
		HRESULT STDMETHODCALLTYPE CanInPlaceActivate()
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceActivate()
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnUIActivate()
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
		{
			*ppFrame = (IOleInPlaceFrame*)this;
			AddRef();

			*ppDoc = NULL;

			lpFrameInfo->fMDIApp = FALSE;
			lpFrameInfo->hwndFrame = m_viewInstance->getHandle();
			lpFrameInfo->haccel = 0;
			lpFrameInfo->cAccelEntries = 0;

			::GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
			::GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate()
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE DiscardUndoState(void)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect)
		{
			IOleObject* control = m_viewInstance->m_control;
			if (control) {
				IOleInPlaceObject* place = NULL;
				HRESULT hr = control->QueryInterface(IID_IOleInPlaceObject, (void**)(&place));
				if (hr == S_OK) {
					//place->SetObjectRects(lprcPosRect, lprcPosRect);
					place->Release();
				}
			}
			return S_OK;
		}


		// IOleInPlaceUIWindow
		HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
		{
			return S_OK;
		}

		// IOleInPlaceFrame
		HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID)
		{
			return E_NOTIMPL;
		}

		// IDocHostUIHandler
		HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
		{
			// let the browser show its default context menu
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE GetHostInfo(DOCHOSTUIINFO *pInfo)
		{
			pInfo->cbSize = sizeof(DOCHOSTUIINFO);
			pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
			pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE ShowUI(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE HideUI(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE UpdateUI(void)
		{
			return S_OK;
		}

		/* Already Contained before
		HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable)
		{
			return S_OK;
		}
		*/

		HRESULT STDMETHODCALLTYPE OnDocWindowActivate(BOOL fActivate)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(BOOL fActivate)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE GetOptionKeyPath(LPOLESTR *pchKey, DWORD dw)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE GetDropTarget(IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE GetExternal(IDispatch **ppDispatch)
		{
			*ppDispatch = 0;
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE TranslateUrl(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
		{
			*ppchURLOut = 0;
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE FilterDataObject(IDataObject *pDO, IDataObject **ppDORet)
		{
			*ppDORet = 0;
			return S_FALSE;
		}

		// IDispatch
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo)
		{
			*pctinfo = 0;
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
		{
			for (UINT i = 0; i < cNames; i++) {
				rgDispId[i] = DISPID_UNKNOWN;
			}
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
		{
			switch (dispIdMember) {
			case DISPID_VALUE:
				// external call
				{
					sl_uint32 n = (sl_uint32)(pDispParams->cArgs);
					Ref<View> _view = m_viewInstance->getView();
					if (WebView* view = CastInstance<WebView>(_view.get())) {
						List<String> params;
						for (sl_uint32 i = 0; i < n; i++) {
							VARIANT& var = pDispParams->rgvarg[n - 1 - i];
							String s;
							if (var.vt == VT_BSTR) {
								s = String((sl_char16*)(var.bstrVal));
							} else {
								VARIANT varStr;
								::VariantInit(&varStr);
								::VariantChangeType(&varStr, &var, 0, VT_BSTR);
								if (varStr.vt == VT_BSTR) {
									s = String((sl_char16*)(varStr.bstrVal));
								}
								::VariantClear(&varStr);
							}
							params.add(s);
						}
						String msg = params.getValueAt(0);
						if (msg.isNotEmpty()) {
							String param = params.getValueAt(1);
							view->dispatchMessageFromJavaScript(msg, param);
						}
						/*
						::VariantInit(pVarResult);
						pVarResult->vt = VT_BSTR;
						pVarResult->bstrVal = ::SysAllocString(ret.getData());
						*/
					}
				}
				break;
			case DISPID_BEFORENAVIGATE2:
				m_viewInstance->__onBeforeNavigate2(pDispParams->rgvarg[5].pvarVal->bstrVal, pDispParams->rgvarg[0].pboolVal);
				break;
			case DISPID_NAVIGATECOMPLETE2:
				m_viewInstance->__onNavigateComplete(pDispParams->rgvarg[0].pvarVal->bstrVal);
				break;
			case DISPID_DOCUMENTCOMPLETE:
				m_viewInstance->__onDocumentComplete(pDispParams->rgvarg[0].pvarVal->bstrVal);
				break;
			default:
				return DISP_E_MEMBERNOTFOUND;
			}
			return S_OK;
		}
	};


	_Win32_WebViewInstance::_Win32_WebViewInstance()
	{
		m_control = NULL;
		m_browser = NULL;

		m_oleClient = new _Win32_WebView_OleClient;
		m_oleClient->m_viewInstance = this;
	}

	_Win32_WebViewInstance::~_Win32_WebViewInstance()
	{
		if (m_browser) {
			m_browser->Quit();
			SLIB_WIN32_COM_SAFE_RELEASE(m_browser);
		}
		if (m_control) {
			m_control->Close(OLECLOSE_NOSAVE);
			SLIB_WIN32_COM_SAFE_RELEASE(m_control);
		}

		delete m_oleClient;
	}

	sl_bool _Win32_WebViewInstance::_initialize()
	{
		HRESULT hr;
		IOleObject* control;
		hr = ::CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER, IID_IOleObject, (void**)(&control));
		if (hr != S_OK) {
			return sl_false;
		}
		control->SetClientSite((IOleClientSite*)m_oleClient);
		hr = ::OleSetContainedObject((IUnknown*)(control), TRUE);
		if (hr != S_OK) {
			control->Release();
			return sl_false;
		}

		control->SetHostNames(L"SLIB_WebBrowser", NULL);

		HWND hWnd = getHandle();
		RECT rect;
		::GetClientRect(hWnd, &rect);
		
		hr = control->DoVerb(OLEIVERB_SHOW, NULL, (IOleClientSite*)m_oleClient, -1, hWnd, &rect);
		if (hr != S_OK) {
			control->Release();
			return sl_false;
		}
		IWebBrowser2* browser = NULL;
		hr = control->QueryInterface(IID_IWebBrowser2, (void**)(&browser));
		if (hr != S_OK) {
			control->Release();
			return sl_false;
		}
		m_control = control;
		m_browser = browser;
		_resize();

		IConnectionPointContainer *cpc = NULL;
		hr = control->QueryInterface(IID_IConnectionPointContainer, (void**)&cpc);
		if (hr == S_OK) {
			IConnectionPoint *cp = NULL;
			hr = cpc->FindConnectionPoint(DIID_DWebBrowserEvents2, &cp);
			if (hr == S_OK) {
				cp->Advise((IDispatch*)m_oleClient, &m_eventCookie);
				cp->Release();
			}
			cpc->Release();
		}

		return sl_true;
	}

	void _Win32_WebViewInstance::__installExternal()
	{
		HRESULT hr;
		IHTMLDocument2* doc = getDoc();
		if (doc) {
			IHTMLWindow2* win = NULL;
			hr = doc->get_parentWindow(&win);
			if (win) {
				IDispatchEx* disp = NULL;
				hr = win->QueryInterface(IID_IDispatchEx, (void**)(&disp));
				if (disp) {
					DISPID dispid;
					hr = disp->GetDispID(L"slib_send", fdexNameEnsure, &dispid);
					if (hr == S_OK) {
						DISPID namedArgs[] = { DISPID_PROPERTYPUT };
						VARIANT vars[1];
						DISPPARAMS params;
						params.rgvarg = vars;
						::VariantInit(&(vars[0]));
						vars[0].vt = VT_DISPATCH;
						vars[0].pdispVal = m_oleClient;
						params.rgdispidNamedArgs = namedArgs;
						params.cArgs = 1;
						params.cNamedArgs = 1;
						hr = disp->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL);
						disp->Release();
					}
				}
				win->Release();
			}
			doc->Release();
		}
	}

	class _WebView_DocumentMoniker : public IMoniker
	{
	public:
		IStream* m_stream;
		String16 m_displayName;

		sl_int32 m_nRef;

		_WebView_DocumentMoniker()
		{
			m_stream = NULL;
			m_nRef = 1;
		}

		~_WebView_DocumentMoniker()
		{
			SLIB_WIN32_COM_SAFE_RELEASE(m_stream);
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject)
		{
			*ppvObject = NULL;
			if (Base::equalsMemory(&iid, &IID_IUnknown, sizeof(IID))) {
				*ppvObject = (IUnknown*)(IMoniker*)this;
				AddRef();
				return S_OK;
			}
			if (Base::equalsMemory(&iid, &IID_IMoniker, sizeof(IID))) {
				*ppvObject = (IMoniker*)this;
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef()
		{
			sl_int32 nRef = Base::interlockedIncrement32(&m_nRef);
			return nRef;
		}

		ULONG STDMETHODCALLTYPE Release()
		{
			sl_int32 nRef = Base::interlockedDecrement32(&m_nRef);
			if (nRef == 0) {
				delete this;
			}
			return nRef;
		}

		// IPersist
		HRESULT STDMETHODCALLTYPE GetClassID(CLSID *pClassID)
		{
			return E_NOTIMPL;
		}

		// IPersistStream
		HRESULT STDMETHODCALLTYPE IsDirty()
		{		
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE Load(IStream *pStm)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE Save(IStream *pStm, BOOL fClearDirty)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetSizeMax(ULARGE_INTEGER *pcbSize)
		{
			return E_NOTIMPL;
		}

		// IMoniker
		HRESULT STDMETHODCALLTYPE BindToObject(IBindCtx *pbc, IMoniker *pmkToLeft, REFIID riidResult, void **ppvResult)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE BindToStorage(IBindCtx *pbc, IMoniker *pmkToLeft, REFIID riid, void **ppvObj)
		{
			*ppvObj = NULL;
			if (Base::equalsMemory(&riid, &IID_IStream, sizeof(IID))) {
				*ppvObj = m_stream;
				m_stream->AddRef();
			} else {
				*ppvObj = NULL;
			}
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE Reduce(IBindCtx *pbc, DWORD dwReduceHowFar, IMoniker **ppmkToLeft, IMoniker **ppmkReduced)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE ComposeWith(IMoniker *pmkRight, BOOL fOnlyIfNotGeneric, IMoniker **ppmkComposite)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE Enum(BOOL fForward, IEnumMoniker **ppenumMoniker)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE IsEqual(IMoniker *pmkOtherMoniker)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE Hash(DWORD *pdwHash)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE IsRunning(IBindCtx *pbc, IMoniker *pmkToLeft, IMoniker *pmkNewlyRunning)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetTimeOfLastChange(IBindCtx *pbc, IMoniker *pmkToLeft, FILETIME *pFileTime)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE Inverse(IMoniker **ppmk)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE CommonPrefixWith(IMoniker *pmkOther, IMoniker **ppmkPrefix)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE RelativePathTo(IMoniker *pmkOther, IMoniker **ppmkRelPath)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE GetDisplayName(IBindCtx *pbc, IMoniker *pmkToLeft, LPOLESTR *ppszDisplayName)
		{
			String16 name = m_displayName;
			DWORD size = (DWORD)(name.getLength()) * 2 + 2;
			OLECHAR* buf = (OLECHAR*)(::CoTaskMemAlloc(size));
			if (buf) {
				Base::copyMemory(buf, name.getData(), size);
				*ppszDisplayName = buf;
				return S_OK;
			} else {
				*ppszDisplayName = NULL;
				return E_OUTOFMEMORY;
			}
		}

		HRESULT STDMETHODCALLTYPE ParseDisplayName(IBindCtx *pbc, IMoniker *pmkToLeft, LPOLESTR pszDisplayName, ULONG *pchEaten, IMoniker **ppmkOut)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE IsSystemMoniker(DWORD *pdwMksys)
		{
			return E_NOTIMPL;
		}
	};

	void _WebView_writeHTML(IHTMLDocument2* doc, String16 content, String16 baseURL)
	{
		if (baseURL.isNotEmpty()) {
			HRESULT hr;
			IStream* stream = ::SHCreateMemStream((BYTE*)(content.getData()), (sl_uint32)(content.getLength() * 2));
			if (stream) {
				IPersistMoniker* persistMoniker = NULL;
				hr = doc->QueryInterface(IID_IPersistMoniker, (void**)(&persistMoniker));
				if (hr == S_OK) {
					IBindCtx* ctx = NULL;
					hr = ::CreateBindCtx(0, &ctx);
					if (hr == S_OK) {
						_WebView_DocumentMoniker* moniker = new _WebView_DocumentMoniker;
						if (moniker) {
							moniker->m_stream = stream;
							if (stream) {
								moniker->m_displayName = baseURL;
								persistMoniker->Load(TRUE, moniker, ctx, STGM_READ);
							}
							moniker->Release();
							stream = NULL;
						}
						ctx->Release();
					}
					persistMoniker->Release();
				}
				if (stream) {
					stream->Release();
				}
			}
		} else {
			SAFEARRAYBOUND bound;
			bound.cElements = 1;
			bound.lLbound = 0;
			SAFEARRAY* sa = ::SafeArrayCreate(VT_VARIANT, 1, &bound);
			if (sa) {
				VARIANT* varArr;
				HRESULT hr = ::SafeArrayAccessData(sa, (void**)(&varArr));
				if (hr == S_OK) {
					varArr[0].vt = VT_BSTR;
					varArr[0].bstrVal = ::SysAllocString((BSTR)(content.getData()));
					doc->write(sa);
					doc->close();
				}
				::SafeArrayDestroy(sa);
			}
		}
	}

	String _WebView_getSource(IHTMLDocument2* doc)
	{
		String ret;
		HRESULT hr;

		IPersistStreamInit* streamInit = NULL;
		hr = doc->QueryInterface(IID_IPersistStreamInit, (void**)(&streamInit));
		if (hr == S_OK) {
			IStream* stream = NULL;
			hr = ::CreateStreamOnHGlobal(NULL, TRUE, &stream);
			if (hr == S_OK) {
				hr = streamInit->Save(stream, FALSE);
				if (hr == S_OK) {
					Memory mem = Win32_COM::readAllBytesFromStream(stream);
					String ret = String::fromUtf(mem);
					return ret;
				}
				stream->Release();
			}
			streamInit->Release();
		}
		return ret;
	}


	class _WebView : public WebView
	{
	public:
		void __init(_Win32_WebViewInstance* instance)
		{
			IWebBrowser2* browser = instance->m_browser;
			if (browser) {
				VARIANT varURL;
				::VariantInit(&varURL);
				varURL.vt = VT_BSTR;
				varURL.bstrVal = L"about:blank";
				browser->Navigate2(&varURL, NULL, NULL, NULL, NULL);
			}
		}

		void __load(_Win32_WebViewInstance* instance)
		{
			IWebBrowser2* browser = instance->m_browser;
			if (browser) {
				if (m_flagOfflineContent) {
					IHTMLDocument2* doc2 = instance->getDoc();
					if (doc2) {
						_WebView_writeHTML(doc2, m_offlineContentHTML, m_urlOrigin);
						doc2->Release();
					}
				} else {
					String16 url = m_urlOrigin;
					if (url.isNotEmpty()) {
						VARIANT varURL;
						::VariantInit(&varURL);
						varURL.vt = VT_BSTR;
						varURL.bstrVal = (BSTR)(url.getData());
						browser->Navigate2(&varURL, NULL, NULL, NULL, NULL);
					}
				}
			}
		}

	};

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		DWORD style = 0;
		DWORD styleEx = 0;
		Ref<_Win32_WebViewInstance> ret = Win32_ViewInstance::create<_Win32_WebViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		if (ret.isNotNull()) {
			ret->_initialize();
			((_WebView*)this)->__init(ret.get());
			((_WebView*)this)->__load(ret.get());
		}
		return ret;
	}

	void WebView::_refreshSize_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			instance->_resize();
		}
	}

	void WebView::_load_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			((_WebView*)this)->__load(instance);
		}
	}

	String WebView::_getURL_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			IHTMLDocument2* doc2 = instance->getDoc();
			if (doc2) {
				String ret;
				BSTR url = NULL;
				HRESULT hr = doc2->get_URL(&url);
				if (hr == S_OK) {
					ret = (sl_char16*)url;
				}
				if (url) {
					::SysFreeString(url);
				}
				doc2->Release();
				return ret;
			}
		}
		return m_urlOrigin;
	}

	String WebView::_getPageTitle_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			IHTMLDocument2* doc2 = instance->getDoc();
			if (doc2) {
				String ret;
				BSTR title = NULL;
				HRESULT hr = doc2->get_title(&title);
				if (hr == S_OK) {
					ret = (sl_char16*)title;
				}
				if (title) {
					::SysFreeString(title);
				}
				doc2->Release();
				return ret;
			}
		}
		return sl_null;
	}

	void WebView::_goBack_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			IWebBrowser2* browser = instance->m_browser;
			if (browser) {
				browser->GoBack();
			}
		}
	}

	void WebView::_goForward_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			IWebBrowser2* browser = instance->m_browser;
			if (browser) {
				browser->GoForward();
			}
		}
	}

	void WebView::_reload_NW()
	{
		Ref<ViewInstance> _instance = getViewInstance();
		if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
			IWebBrowser2* browser = instance->m_browser;
			if (browser) {
				browser->Refresh();
			}
		}
	}

	String _WebView_evalJavascript(IHTMLWindow2* win, const String& _script)
	{
		String ret;
		IDispatchEx* disp = NULL;
		HRESULT hr = win->QueryInterface(IID_IDispatchEx, (void**)(&disp));
		if (disp) {
			DISPID dispid;
			hr = disp->GetDispID(L"eval", fdexNameCaseSensitive, &dispid);
			if (hr == S_OK) {
				VARIANT var;
				::VariantInit(&var);
				DISPPARAMS params;
				VARIANT vars[1];
				params.cArgs = 1;
				params.rgvarg = vars;
				params.cNamedArgs = 0;
				params.rgdispidNamedArgs = NULL;
				String16 script = _script;
				::VariantInit(&(vars[0]));
				vars[0].vt = VT_BSTR;
				vars[0].bstrVal = ::SysAllocString((OLECHAR*)(script.getData()));
				disp->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &var, NULL, NULL);
				if (var.vt == VT_BSTR) {
					ret = String((sl_char16*)(var.bstrVal));
				} else {
					VARIANT varStr;
					::VariantInit(&varStr);
					::VariantChangeType(&varStr, &var, 0, VT_BSTR);
					if (varStr.vt == VT_BSTR) {
						ret = String((sl_char16*)(varStr.bstrVal));
					}
					::VariantClear(&varStr);
				}
				::VariantClear(&(vars[0]));
				::VariantClear(&var);
			}
			disp->Release();
		}
		return ret;
	}

	void WebView::_runJavaScript_NW(const String& _script)
	{
		String16 script = _script;
		if (script.isNotEmpty()) {
			Ref<ViewInstance> _instance = getViewInstance();
			if (_Win32_WebViewInstance* instance = CastInstance<_Win32_WebViewInstance>(_instance.get())) {
				IHTMLDocument2* doc2 = instance->getDoc();
				if (doc2) {
					HRESULT hr;
					IHTMLWindow2* win = NULL;
					hr = doc2->get_parentWindow(&win);
					if (hr == S_OK) {
						//_WebView_evalJavascript(win, _script);
						BSTR s = (BSTR)(::SysAllocString((OLECHAR*)(script.getData())));
						if (s) {
							VARIANT var;
							::VariantInit(&var);
							// execScript always returns empty
							win->execScript(s, L"JScript", &var);
							::VariantClear(&var);
							::SysFreeString(s);
						}
						win->Release();
					}
					doc2->Release();
				}
			}
		}
	}

}

#endif
