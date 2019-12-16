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

#ifndef CHECKHEADER_SLIB_SERVICE_CHAT_CLIENT
#define CHECKHEADER_SLIB_SERVICE_CHAT_CLIENT

#include "definition.h"

#include "chat.h"

#include "../ui/chat_view.h"

namespace slib
{

	class SLIB_EXPORT ChatClientParam
	{
	public:
		String myId;
		Ref<ListView> roomsView;
		Ref<ChatView> chatView;
		Ref<ChatClientDatabase> database;
		Ref<ChatClientService> service;
		
	public:
		ChatClientParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatClientParam)
		
	};

	class SLIB_EXPORT ChatClient : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		ChatClient();
		
		~ChatClient();

	public:
		static Ref<ChatClient> create(const ChatClientParam& param);
		
	public:
		void selectRoom(const String& roomId);
		
		void sendMessage(const String& receiverId, const String& message);
		
	public:
		void dispatchReceiveMessage(const String& roomId, ChatMessage& message);
		
	protected:
		sl_bool initialize(const ChatClientParam& param);
		
	protected:
		String m_myId;
		Ref<ListView> m_roomsView;
		Ref<ChatView> m_chatView;
		Ref<ChatClientDatabase> m_database;
		Ref<ChatClientService> m_service;
		
		String m_currentRoomId;
		
	};

}

#endif
