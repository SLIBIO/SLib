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

#include "slib/service/chat.h"

#include "slib/core/io.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatMessageBody)
	
	ChatMessageBody::ChatMessageBody():
		contentType(ChatContentType::Text),
		flagEncrypted(sl_false),
		flagInlined(sl_true)
	{
	}

	Memory ChatMessageBody::packBody() const
	{
		MemoryWriter writer;
		writer.writeTime(time);
		sl_uint16 flags = (((sl_uint16)flagEncrypted) << 7) | (((sl_uint16)flagInlined) << 6) | ((sl_uint16)contentType);
		writer.writeUint16(flags);
		writer.writeStringSection(text);
		writer.writeSection(content);
		return writer.getData();
	}

	void ChatMessageBody::unpackBody(const Memory& mem)
	{
		MemoryReader reader(mem);
		time = reader.readTime();
		sl_uint16 flags = reader.readUint16();
		flagEncrypted = (sl_bool)((flags >> 7) & 1);
		flagInlined = (sl_bool)((flags >> 6) & 1);
		contentType = (ChatContentType)(flags & 31);
		text = reader.readStringSection();
		content = reader.readSection();
	}

	// supports signed 8-bit mono, little-endian signed 16-bit mono
	void ChatMessageBody::unpackAudio(const Memory& content, AudioData& audio)
	{
		sl_size size = content.getSize();
		if (size > 2) {
			sl_uint8* data = (sl_uint8*)(content.getData());
			sl_uint8 type = data[0];
			if (type == 0) {
				// 8-bit mono
				audio.format = AudioFormat::Int8_Mono;
				audio.data = data + 2;
				audio.count = size - 2;
				audio.ref = content.ref;
				return;
			} else if (type == 1) {
				// little-endian 16-bit mono
				audio.format = AudioFormat::Int16LE_Mono;
				audio.data = data + 2;
				audio.count = (size - 2) >> 1;
				audio.ref = content.ref;
				return;
			}
		}
		audio.format = AudioFormat::Int16LE_Mono;
		audio.data = sl_null;
		audio.count = 0;
	}

	Memory ChatMessageBody::packAudio(const AudioData& audio)
	{
		Memory content;
		AudioData t;
		sl_uint8 type = 0;
		if (AudioFormatHelper::getBitsPerSample(audio.format) == 8) {
			t.format = AudioFormat::Int8_Mono;
			content = Memory::create(2 + audio.count);
			type = 0;
		} else {
			t.format = AudioFormat::Int16_Mono;
			content = Memory::create(2 + (audio.count << 1));
			type = 1;
		}
		if (content.isNotNull()) {
			t.data = content.getData();
			*((sl_uint8*)(t.data)) = type;
			t.count = audio.count;
			t.copySamplesFrom(audio);
		}
		return content;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatMessage)

	ChatMessage::ChatMessage()
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatContact)

	SLIB_DEFINE_JSON_MEMBERS(ChatContact, userId, name, iconData, data)

	ChatContact::ChatContact()
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatRoom)

	SLIB_DEFINE_JSON_MEMBERS(ChatRoom, roomId, title, iconData, members, lastMessageId, lastReadMessageId)

	ChatRoom::ChatRoom()
	{
		lastMessageId = 0;
		lastReadMessageId = 0;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceResponse)

	ChatServiceResponse::ChatServiceResponse()
	{
		flagSuccess = sl_false;
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceMessageResponse)

	ChatServiceMessageResponse::ChatServiceMessageResponse()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceMessagesResponse)

	ChatServiceMessagesResponse::ChatServiceMessagesResponse()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceContactResponse)

	ChatServiceContactResponse::ChatServiceContactResponse()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceContactsResponse)

	ChatServiceContactsResponse::ChatServiceContactsResponse()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceRoomResponse)

	ChatServiceRoomResponse::ChatServiceRoomResponse()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatServiceRoomsResponse)

	ChatServiceRoomsResponse::ChatServiceRoomsResponse()
	{
	}


	SLIB_DEFINE_OBJECT(ChatClientDatabase, Object)

	ChatClientDatabase::ChatClientDatabase()
	{
	}

	ChatClientDatabase::~ChatClientDatabase()
	{
	}


	SLIB_DEFINE_OBJECT(ChatClientService, Object)

	ChatClientService::ChatClientService()
	{
	}

	ChatClientService::~ChatClientService()
	{
	}

}

