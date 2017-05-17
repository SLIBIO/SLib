/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_IO
#define CHECKHEADER_SLIB_NETWORK_IO

#include "definition.h"

#include "socket.h"
#include "../core/io.h"

namespace slib
{

	class SLIB_EXPORT TcpStream : public Object, public IStream, public IClosable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TcpStream();
		
		TcpStream(const Ref<Socket>& socket);
		
		~TcpStream();
		
	public:
		void setSocket(const Ref<Socket>& socket);
		
		Ref<Socket> getSocket();
		
		void close() override;
		
		sl_int32 read32(void* buf, sl_uint32 size) override;
		
		sl_int32 write32(const void* buf, sl_uint32 size) override;
		
	protected:
		AtomicRef<Socket> m_socket;
		
	};

}

#endif
