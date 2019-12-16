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

#ifndef CHECKHEADER_SLIB_SERVICE_CHAT
#define CHECKHEADER_SLIB_SERVICE_CHAT

#include "definition.h"

#include "../core/json.h"
#include "../db/database.h"
#include "../graphics/drawable.h"
#include "../media/audio_data.h"

namespace slib
{

	enum class ChatContentType
	{
		Unknown = 0,
		Text = 1,
		Image = 2,
		Audio = 3,
		Video = 4,
		Directory = 5,
		Sticker = 6
	};

	class SLIB_EXPORT ChatMessageBody
	{
	public:
		Time time;
		ChatContentType contentType;
		sl_bool flagEncrypted;
		sl_bool flagInlined;
		String text; // text or fileName
		Memory content; // content or thumbnail

	public:
		ChatMessageBody();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatMessageBody)

	public:
		Memory packBody() const;
		
		void unpackBody(const Memory& mem);
		
		static void unpackAudio(const Memory& content, AudioData& data);
		
		static Memory packAudio(const AudioData& data);
		
	};

	class SLIB_EXPORT ChatMessage : public ChatMessageBody
	{
	public:
		String senderId;
		sl_uint64 messageId;

	public:
		ChatMessage();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatMessage)

	};

	class SLIB_EXPORT ChatContact
	{
	public:
		String userId;
		String name;
		Ref<Drawable> icon;
		Memory iconData;
		Json data;
		
	public:
		ChatContact();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatContact)
		
		SLIB_DECLARE_JSON

	};

	class SLIB_EXPORT ChatRoom
	{
	public:
		String roomId;
		String title;
		Ref<Drawable> icon;
		Memory iconData;
		List<String> members;
		sl_uint64 lastMessageId;
		sl_uint64 lastReadMessageId;
		
	public:
		ChatRoom();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatRoom)
		
		SLIB_DECLARE_JSON
		
	};

	class ChatServiceResponse
	{
	public:
		sl_bool flagSuccess;
		String error;
		
	public:
		ChatServiceResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceResponse)

	};

	class ChatServiceMessageResponse : public ChatServiceResponse
	{
	public:
		ChatMessage message;
		
	public:
		ChatServiceMessageResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceMessageResponse)

	};

	class ChatServiceMessagesResponse : public ChatServiceResponse
	{
	public:
		List<ChatMessage> messages;
		
	public:
		ChatServiceMessagesResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceMessagesResponse)

	};

	class ChatServiceContactResponse : public ChatServiceResponse
	{
	public:
		ChatContact contact;
		
	public:
		ChatServiceContactResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceContactResponse)

	};

	class ChatServiceContactsResponse : public ChatServiceResponse
	{
	public:
		List<ChatContact> contacts;
		
	public:
		ChatServiceContactsResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceContactsResponse)

	};

	class ChatServiceRoomResponse : public ChatServiceResponse
	{
	public:
		ChatRoom room;
		
	public:
		ChatServiceRoomResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceRoomResponse)

	};

	class ChatServiceRoomsResponse : public ChatServiceResponse
	{
	public:
		List<ChatRoom> rooms;
		
	public:
		ChatServiceRoomsResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatServiceRoomsResponse)

	};

	class SLIB_EXPORT ChatClientService : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		ChatClientService();
		
		~ChatClientService();
		
	public:
		virtual void getContacts(const Function<void(ChatServiceContactsResponse&)>& callback) = 0;
		
		virtual void addContact(const ChatContact& contact, const Function<void(ChatServiceContactResponse&)>& callback) = 0;
		
		virtual void removeContact(const String& userId, const Function<void(ChatServiceContactResponse&)>& callback) = 0;
		
		virtual void setContactData(const String& userId, const Json& data, const Function<void(ChatServiceContactResponse&)>& callback) = 0;
		
		virtual void getRoom(const String& roomId, const Function<void(ChatServiceRoomResponse&)>& callback) = 0;
		
		virtual void getRoomWithUser(const String& userId, const Function<void(ChatServiceRoomResponse&)>& callback) = 0;
		
		virtual void getRooms(const Function<void(ChatServiceRoomsResponse&)>&) = 0;
		
		virtual void createRoom(const ChatRoom& room, const Function<void(ChatServiceRoomResponse&)>& callback) = 0;
		
		virtual void removeRoom(const String& roomId, const Function<void(ChatServiceRoomResponse&)>& callback) = 0;
		
		virtual void sendMessage(const String& roomId, const ChatMessage& message, const Function<void(ChatServiceMessageResponse&)>& callback) = 0;
		
		virtual void getMessagesFrom(const String& roomId, sl_uint64 start, sl_uint32 countLimit, const Function<void(ChatServiceMessagesResponse&)>& callback) = 0;
		
		virtual void getMessagesTo(const String& roomId, sl_uint64 end, sl_uint32 countLimit, const Function<void(ChatServiceMessagesResponse&)>& callback) = 0;
		
	public:
		SLIB_PROPERTY_FUNCTION(void(const String& roomId, ChatMessage&), OnReceiveMessage)

		SLIB_PROPERTY_FUNCTION(void(ChatRoom&), OnCreateRoom)
		
	};


	class SLIB_EXPORT ChatClientDatabase : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		ChatClientDatabase();
		
		~ChatClientDatabase();
		
	public:
		static Ref<ChatClientDatabase> createSQLite(const String& dbPath, const String& encryptionKey);
		
	public:
		virtual List<ChatContact> getContacts() = 0;
		
		virtual sl_bool getContact(const String& userId, ChatContact& outContact) = 0;
		
		virtual sl_bool addContact(const ChatContact& contact) = 0;
		
		virtual void updateContact(const ChatContact& contact) = 0;
		
		virtual void removeContact(const String& userId) = 0;
		
		virtual List<ChatRoom> getRooms() = 0;
		
		virtual sl_bool getRoom(const String& roomId, ChatRoom& outRoom) = 0;
		
		virtual sl_bool addRoom(const ChatRoom& room) = 0;
		
		virtual void updateRoom(const ChatRoom& room) = 0;
		
		virtual void removeRoom(const String& roomId) = 0;
		
		virtual sl_bool addMessage(const String& roomId, const ChatMessage& message) = 0;
		
		virtual List<ChatMessage> getMessagesFrom(const String& roomId, sl_uint64 start, sl_uint32 countLimit) = 0;
		
		virtual List<ChatMessage> getMessagesTo(const String& roomId, sl_uint64 end, sl_uint32 countLimit) = 0;
		
	};

}

#endif
