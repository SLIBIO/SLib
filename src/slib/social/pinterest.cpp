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

#include "slib/social/pinterest.h"

#include "slib/core/safe_static.h"
#include "slib/core/log.h"

namespace slib
{
	
	namespace priv
	{
		namespace pinterest
		{
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Pinterest>, g_instance)
		}
	}
	
	using namespace priv::pinterest;
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestUser)
	
	SLIB_DEFINE_JSON(PinterestUser)
	{
		if (isFromJson) {
			this->json = json;
		}
		SLIB_JSON_ADD_MEMBERS(id, url, first_name, last_name)
	}
	
	PinterestUser::PinterestUser()
	{
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestBoard)
	
	SLIB_DEFINE_JSON(PinterestBoard)
	{
		if (isFromJson) {
			this->json = json;
		}
		SLIB_JSON_ADD_MEMBERS(id, name, url)
	}
	
	PinterestBoard::PinterestBoard()
	{
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestParam)
	
	PinterestParam::PinterestParam()
	{
		authorizeUrl = "https://api.pinterest.com/oauth/";
		accessTokenUrl = "https://api.pinterest.com/v1/oauth/token";
		defaultScopes.add_NoLock("read_public");
	}

	
	SLIB_DEFINE_OBJECT(Pinterest, OAuth2)
	
	Pinterest::Pinterest(const PinterestParam& param) : OAuth2(param)
	{
	}
	
	Pinterest::~Pinterest()
	{
	}
	
	Ref<Pinterest> Pinterest::create(const PinterestParam& param)
	{
		return new Pinterest(param);
	}
		
	void Pinterest::initialize(const PinterestParam& param)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return;
		}
		g_instance = create(param);
	}
	
	void Pinterest::initialize(const String& redirectUri, const String& appId, const String& appSecret)
	{
		PinterestParam param;
		param.preferenceName = "pinterest";
		param.clientId = appId;
		param.clientSecret = appSecret;
		param.redirectUri = redirectUri;
		initialize(param);
	}
	
	Ref<Pinterest> Pinterest::getInstance()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_instance)) {
			return sl_null;
		}
		return g_instance;
	}

	void Pinterest::authorizeRequest(UrlRequestParam& param, const OAuthAccessToken& token)
	{
		param.parameters.put("access_token", token.token);
	}
	
	String Pinterest::getRequestUrl(const String& path)
	{
		return "https://api.pinterest.com/v1/" + path;
	}
	
	void Pinterest::getUser(const String& userId, const Function<void(PinterestResult&, PinterestUser&)>& onComplete)
	{
		UrlRequestParam rp;
		if (userId.isNotEmpty()) {
			rp.url = getRequestUrl(String::format("users/%s", userId));
		} else {
			SLIB_STATIC_STRING(me, "me")
			rp.url = getRequestUrl(me);
		}
		rp.onComplete = [onComplete](UrlRequest* request) {
			PinterestResult result(request);
			PinterestUser user;
			if (!(request->isError())) {
				FromJson(result.response["data"], user);
				result.flagSuccess = user.id.isNotEmpty();
			}
			onComplete(result, user);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
	void Pinterest::getMyBoards(const Function<void(PinterestResult&, List<PinterestBoard>& boards)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = getRequestUrl("me/boards/");
		rp.onComplete = [onComplete](UrlRequest* request) {
			PinterestResult result(request);
			List<PinterestBoard> boards;
			if (!(request->isError()) && result.response["data"].isNotNull()) {
				FromJson(result.response["data"], boards);
				result.flagSuccess = sl_true;
			}
			onComplete(result, boards);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestCreateBoardResult)
	
	PinterestCreateBoardResult::PinterestCreateBoardResult(UrlRequest* request): PinterestResult(request)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestCreateBoardParam)
	
	PinterestCreateBoardParam::PinterestCreateBoardParam()
	{
	}
	
	void Pinterest::createBoard(const PinterestCreateBoardParam& param)
	{
		UrlRequestParam rp;
		rp.method = HttpMethod::POST;
		rp.url = getRequestUrl("boards/");
		HashMap<String, String> body;
		body.put_NoLock("name", param.name);
		if (param.description.isNotEmpty()) {
			body.put_NoLock("description", param.description);
		}
		rp.setFormData(body);
		rp.onComplete = [param](UrlRequest* request) {
			PinterestCreateBoardResult result(request);
			FromJson(result.response["data"], result.createdBoard);
			if (result.createdBoard.id.isEmpty()) {
				LogError("Pinterest CreateBoard", "%s", result.response);
				param.onComplete(result);
				return;
			}
			result.flagSuccess = sl_true;
			param.onComplete(result);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestCreatePinResult)
	
	PinterestCreatePinResult::PinterestCreatePinResult(UrlRequest* request): PinterestResult(request)
	{
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PinterestCreatePinParam)
	
	PinterestCreatePinParam::PinterestCreatePinParam()
	{
	}
	
	void Pinterest::createPin(const PinterestCreatePinParam& param)
	{
		UrlRequestParam rp;
		rp.method = HttpMethod::POST;
		rp.url = getRequestUrl("pins/");
		HashMap<String, String> body;
		body.put_NoLock("board", param.board);
		body.put_NoLock("note", param.note);
		if (param.link.isNotEmpty()) {
			body.put_NoLock("link", param.link);
		}
		body.put_NoLock("image_url", param.imageUrl);
		rp.setFormData(body);
		rp.onComplete = [param](UrlRequest* request) {
			PinterestCreatePinResult result(request);
			String id = result.response["data"]["id"].getString();
			if (id.isEmpty()) {
				LogError("Pinterest CreatePin", "%s", result.response);
				param.onComplete(result);
				return;
			}
			result.flagSuccess = sl_true;
			param.onComplete(result);
		};
		authorizeRequest(rp);
		UrlRequest::send(rp);
	}
	
}
