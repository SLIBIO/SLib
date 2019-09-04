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

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/web_view.h"

#include "slib/core/win32_com.h"

#include "view_win32.h"

#include <exdisp.h>
#include <exdispid.h>
#include <mshtml.h>
#include <mshtmhst.h>

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace slib
{

	namespace priv
	{
		namespace web_view
		{

			class WebViewHelper : public WebView
			{
				friend class WebViewInstance;
			};

			class OleClient;

			class DocumentMoniker : public IMoniker
			{
			public:
				IStream* m_stream;
				String16 m_displayName;

				sl_int32 m_nRef;

			public:
				DocumentMoniker()
				{
					m_stream = NULL;
					m_nRef = 1;
				}

				~DocumentMoniker()
				{
					SLIB_WIN32_COM_SAFE_RELEASE(m_stream);
				}

			public:
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

			static void WriteHTML(IHTMLDocument2* doc, String content, String16 baseURL)
			{
				if (baseURL.isNotEmpty()) {
					HRESULT hr;
					IStream* stream = ::SHCreateMemStream((BYTE*)(content.getData()), (sl_uint32)(content.getLength()));
					if (stream) {
						IPersistMoniker* persistMoniker = NULL;
						hr = doc->QueryInterface(IID_IPersistMoniker, (void**)(&persistMoniker));
						if (hr == S_OK) {
							IBindCtx* ctx = NULL;
							hr = ::CreateBindCtx(0, &ctx);
							if (hr == S_OK) {
								DocumentMoniker* moniker = new DocumentMoniker;
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

			class WebViewInstance : public Win32_ViewInstance, public IWebViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				OleClient* m_oleClient;

				IOleObject* m_control;
				IWebBrowser2* m_browser;

				DWORD m_eventCookie;

			public:
				WebViewInstance();

				~WebViewInstance();

			public:
				void installExternal();

			public:
				sl_bool prepare(WebView* view);

				void refreshSize(WebView* view) override
				{
					HWND handle = getHandle();
					if (handle) {
						IWebBrowser2* browser = m_browser;
						if (browser) {
							browser->put_Left(0);
							browser->put_Top(0);
							browser->put_Width(view->getWidth());
							browser->put_Height(view->getHeight());
						}
					}
				}

				void load(WebView* view) override
				{
					WebViewHelper* helper = static_cast<WebViewHelper*>(view);
					IWebBrowser2* browser = m_browser;
					if (browser) {
						if (helper->m_flagOfflineContent) {
							IHTMLDocument2* doc2 = getDoc();
							if (doc2) {
								WriteHTML(doc2, helper->m_offlineContentHTML, helper->m_urlOrigin);
								doc2->Release();
							}
						} else {
							String16 url = helper->m_urlOrigin;
							if (url.isNotEmpty()) {
								VARIANT varURL;
								VariantInit(&varURL);
								varURL.vt = VT_BSTR;
								varURL.bstrVal = (BSTR)(url.getData());
								if (helper->m_customUserAgent.isNotEmpty()) {
									String16 headers = "User-Agent: " + helper->m_customUserAgent;
									VARIANT varHeaders;
									varHeaders.vt = VT_BSTR;
									varHeaders.bstrVal = (BSTR)(headers.getData());
									browser->Navigate2(&varURL, NULL, NULL, NULL, &varHeaders);
								} else {
									browser->Navigate2(&varURL, NULL, NULL, NULL, NULL);
								}
							}
						}
					}
				}

				sl_bool getURL(WebView* view, String& _out) override
				{
					IHTMLDocument2* doc2 = getDoc();
					if (doc2) {
						sl_bool bRet = sl_false;
						BSTR url = NULL;
						HRESULT hr = doc2->get_URL(&url);
						if (hr == S_OK) {
							_out = (sl_char16*)url;
							bRet = sl_true;
						}
						if (url) {
							SysFreeString(url);
						}
						doc2->Release();
						return bRet;
					}
					return sl_false;
				}

				sl_bool getPageTitle(WebView* view, String& _out) override
				{
					IHTMLDocument2* doc2 = getDoc();
					if (doc2) {
						sl_bool bRet = sl_false;
						BSTR title = NULL;
						HRESULT hr = doc2->get_title(&title);
						if (hr == S_OK) {
							_out = (sl_char16*)title;
							bRet = sl_true;
						}
						if (title) {
							::SysFreeString(title);
						}
						doc2->Release();
						return bRet;
					}
					return sl_null;
				}

				void goBack(WebView* view) override
				{
					IWebBrowser2* browser = m_browser;
					if (browser) {
						browser->GoBack();
					}
				}

				void goForward(WebView* view) override
				{
					IWebBrowser2* browser = m_browser;
					if (browser) {
						browser->GoBack();
					}
				}

				void reload(WebView* view) override
				{
					IWebBrowser2* browser = m_browser;
					if (browser) {
						browser->Refresh();
					}
				}

				void runJavaScript(WebView* view, const String& _script) override
				{
					String16 script = _script;
					if (script.isNotEmpty()) {
						IHTMLDocument2* doc2 = getDoc();
						if (doc2) {
							HRESULT hr;
							IHTMLWindow2* win = NULL;
							hr = doc2->get_parentWindow(&win);
							if (hr == S_OK) {
								BSTR s = (BSTR)(SysAllocString((OLECHAR*)(script.getData())));
								if (s) {
									VARIANT var;
									VariantInit(&var);
									// execScript always returns empty
									win->execScript(s, L"JScript", &var);
									VariantClear(&var);
									SysFreeString(s);
								}
								win->Release();
							}
							doc2->Release();
						}
					}
				}

				void setCustomUserAgent(WebView* view, const String& agent) override
				{
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

				Ref<WebViewHelper> getHelper()
				{
					return CastRef<WebViewHelper>(getView());
				}

				void onBeforeNavigate2(BSTR szURL, VARIANT_BOOL* pFlagCancel)
				{
					Ref<WebViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						helper->dispatchStartLoad(szURL);
						*pFlagCancel = 0;
					}
				}

				void onNavigateComplete(BSTR szURL)
				{
					installExternal();
					runJavaScript(sl_null, "window.slib = {send: slib_send};");
				}

				void onDocumentComplete(BSTR szURL)
				{
					Ref<WebViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						helper->dispatchFinishLoad(szURL, sl_false);
					}
				}

				void onExternalCall(DISPPARAMS* pDispParams)
				{
					Ref<WebViewHelper> helper = CastRef<WebViewHelper>(getView());
					if (helper.isNotNull()) {
						List<String> params;
						sl_uint32 n = (sl_uint32)(pDispParams->cArgs);
						for (sl_uint32 i = 0; i < n; i++) {
							VARIANT& var = pDispParams->rgvarg[n - 1 - i];
							String s;
							if (var.vt == VT_BSTR) {
								s = String((sl_char16*)(var.bstrVal));
							} else {
								VARIANT varStr;
								VariantInit(&varStr);
								VariantChangeType(&varStr, &var, 0, VT_BSTR);
								if (varStr.vt == VT_BSTR) {
									s = String((sl_char16*)(varStr.bstrVal));
								}
								VariantClear(&varStr);
							}
							params.add(s);
						}
						String msg = params.getValueAt(0);
						if (msg.isNotEmpty()) {
							String param = params.getValueAt(1);
							helper->dispatchMessageFromJavaScript(msg, param);
						}
					}
				}

				LRESULT processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
				{
					HWND handle = getHandle();
					if (handle) {
						switch (msg) {
						case WM_PAINT:
							if (m_browser) {
								PAINTSTRUCT ps;
								BeginPaint(m_handle, &ps);
								EndPaint(m_handle, &ps);
								return 0;
							}
							break;
						case WM_ERASEBKGND:
							return TRUE;
						}
					}
					return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam);
				}

			};

			class OleClient : public IOleClientSite, public IOleInPlaceSite, public IOleInPlaceFrame, public IDocHostUIHandler, public IDispatch
			{
			public:
				WebViewInstance* m_viewInstance;

			public:
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

					GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
					GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);
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
							place->SetObjectRects(lprcPosRect, lprcPosRect);
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
						m_viewInstance->onExternalCall(pDispParams);
						break;
					case DISPID_BEFORENAVIGATE2:
						m_viewInstance->onBeforeNavigate2(pDispParams->rgvarg[5].pvarVal->bstrVal, pDispParams->rgvarg[0].pboolVal);
						break;
					case DISPID_NAVIGATECOMPLETE2:
						m_viewInstance->onNavigateComplete(pDispParams->rgvarg[0].pvarVal->bstrVal);
						break;
					case DISPID_DOCUMENTCOMPLETE:
						m_viewInstance->onDocumentComplete(pDispParams->rgvarg[0].pvarVal->bstrVal);
						break;
					default:
						return DISP_E_MEMBERNOTFOUND;
					}
					return S_OK;
				}
			};

			SLIB_DEFINE_OBJECT(WebViewInstance, Win32_ViewInstance)

			WebViewInstance::WebViewInstance()
			{
				m_control = NULL;
				m_browser = NULL;

				m_oleClient = new OleClient;
				m_oleClient->m_viewInstance = this;
			}

			WebViewInstance::~WebViewInstance()
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

			sl_bool WebViewInstance::prepare(WebView* view)
			{
				HWND hWnd = m_handle;
				if (!hWnd) {
					return sl_false;
				}

				HRESULT hr;
				IOleObject* control;

				hr = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER, IID_IOleObject, (void**)(&control));
				if (hr == S_OK) {

					control->SetClientSite((IOleClientSite*)m_oleClient);

					hr = OleSetContainedObject((IUnknown*)(control), TRUE);
					if (hr == S_OK) {

						control->SetHostNames(L"SLIB_WebBrowser", NULL);

						RECT rect;
						GetClientRect(hWnd, &rect);

						hr = control->DoVerb(OLEIVERB_SHOW, NULL, (IOleClientSite*)m_oleClient, -1, hWnd, &rect);
						if (hr == S_OK) {

							IWebBrowser2* browser = NULL;
							hr = control->QueryInterface(IID_IWebBrowser2, (void**)(&browser));
							if (hr == S_OK) {

								m_control = control;
								m_browser = browser;

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

								refreshSize(view);

								VARIANT varURL;
								VariantInit(&varURL);
								varURL.vt = VT_BSTR;
								varURL.bstrVal = L"about:blank";
								browser->Navigate2(&varURL, NULL, NULL, NULL, NULL);

								return sl_true;
							}
						}
					}
					control->Release();
				}
				return sl_false;
			}

			void WebViewInstance::installExternal()
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

		}
	}

	using namespace priv::web_view;

	Ref<ViewInstance> WebView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}
		Ref<WebViewInstance> ret = Win32_ViewInstance::create<WebViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), sl_null, 0, 0);
		if (ret.isNotNull()) {
			ret->prepare(this);
			ret->load(this);
			return ret;
		}
		return sl_null;
	}

	Ptr<IWebViewInstance> WebView::getWebViewInstance()
	{
		return CastRef<WebViewInstance>(getViewInstance());
	}


	void DefaultWebViewProvider::clearCache()
	{
		GROUPID groupId;
		HANDLE handle = ::FindFirstUrlCacheGroup(0, CACHEGROUP_SEARCH_ALL, NULL, 0, &groupId, NULL);
		if (handle) {
			for (;;) {
				GROUPID next = groupId;
				BOOL bRet = ::FindNextUrlCacheGroup(handle, &next, NULL);
				::DeleteUrlCacheGroup(groupId, CACHEGROUP_FLAG_FLUSHURL_ONDELETE, NULL);
				if (bRet) {
					groupId = next;
				} else {
					break;
				}
			}
			::FindCloseUrlCache(handle);
		}
	}

	void DefaultWebViewProvider::clearCookie()
	{
	}

}

#endif
