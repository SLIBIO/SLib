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

#include "slib/network/http_server.h"

#include "slib/crypto/openssl.h"

namespace slib
{
	
	class _priv_OpenSSL_HttpServerConnectionProvider : public HttpServerConnectionProvider
	{
	public:
		Ref<AsyncTcpServer> m_server;
		Ref<AsyncIoLoop> m_loop;
		TlsAcceptStreamParam m_tlsParam;
		
		struct StreamDesc
		{
			Ref<OpenSSL_AsyncStream> stream;
			SocketAddress addressLocal;
			SocketAddress addressRemote;
		};
		HashMap< AsyncStream*, StreamDesc > m_streamsHandshaking;
		
	public:
		_priv_OpenSSL_HttpServerConnectionProvider()
		{
		}
		
		~_priv_OpenSSL_HttpServerConnectionProvider()
		{
			release();
		}
		
	public:
		static Ref<HttpServerConnectionProvider> create(HttpServer* server, const TlsAcceptStreamParam& tlsParam, const SocketAddress& addressListen)
		{
			Ref<OpenSSL_Context> context = Ref<OpenSSL_Context>::from(tlsParam.context);
			if (!(IsInstanceOf<OpenSSL_Context>(context))) {
				context = OpenSSL::createContext(tlsParam);
				if (context.isNull()) {
					return sl_null;
				}
			}
			Ref<AsyncIoLoop> loop = server->getAsyncIoLoop();
			if (loop.isNotNull()) {
				Ref<_priv_OpenSSL_HttpServerConnectionProvider> ret = new _priv_OpenSSL_HttpServerConnectionProvider;
				if (ret.isNotNull()) {
					ret->m_tlsParam = tlsParam;
					ret->m_tlsParam.context = context;
					ret->m_tlsParam.flagAutoStartHandshake = sl_false;
					ret->m_tlsParam.onHandshake = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_HttpServerConnectionProvider, onHandshake, ret);
					ret->m_loop = loop;
					ret->setServer(server);
					AsyncTcpServerParam sp;
					sp.bindAddress = addressListen;
					sp.onAccept = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_HttpServerConnectionProvider, onAccept, ret);
					sp.ioLoop = loop;
					Ref<AsyncTcpServer> server = AsyncTcpServer::create(sp);
					if (server.isNotNull()) {
						ret->m_server = server;
						return ret;
					}
				}
			}
			return sl_null;
		}
		
		void release() override
		{
			ObjectLocker lock(this);
			if (m_server.isNotNull()) {
				m_server->close();
			}
			m_streamsHandshaking.setNull();
		}
		
		void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address)
		{
			Ref<HttpServer> server = getServer();
			if (server.isNotNull()) {
				Ref<AsyncIoLoop> loop = m_loop;
				if (loop.isNull()) {
					return;
				}
				AsyncTcpSocketParam cp;
				cp.socket = socketAccept;
				cp.ioLoop = loop;
				Ref<AsyncTcpSocket> stream = AsyncTcpSocket::create(cp);
				if (stream.isNotNull()) {
					Ref<OpenSSL_AsyncStream> tlsStream = OpenSSL::acceptStream(stream, m_tlsParam);
					if (tlsStream.isNotNull()) {
						SocketAddress addrLocal;
						socketAccept->getLocalAddress(addrLocal);
						StreamDesc desc;
						desc.stream = tlsStream;
						desc.addressLocal = addrLocal;
						desc.addressRemote = address;
						m_streamsHandshaking.put(tlsStream.get(), desc);
						tlsStream->handshake();
					}
				}
			}
		}
		
		void onHandshake(TlsStreamResult& result)
		{
			StreamDesc desc;
			if (m_streamsHandshaking.remove(result.stream, &desc)) {
				if (!(result.flagError)) {
					Ref<HttpServer> server = getServer();
					if (server.isNotNull()) {
						server->addConnection(desc.stream, desc.addressRemote, desc.addressLocal);
					}
				}
			}
		}
		
	};


	sl_bool HttpServer::addHttpsBinding(const TlsAcceptStreamParam& param, const SocketAddress& addr)
	{
		Ref<HttpServerConnectionProvider> provider = _priv_OpenSSL_HttpServerConnectionProvider::create(this, param, addr);
		if (provider.isNotNull()) {
			addConnectionProvider(provider);
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool HttpServer::addHttpsBinding(const TlsAcceptStreamParam& param, sl_uint32 port)
	{
		return addHttpsBinding(param, SocketAddress(port));
	}
	
	sl_bool HttpServer::addHttpsBinding(const TlsAcceptStreamParam& param, const IPAddress& addr, sl_uint32 port)
	{
		return addHttpsBinding(param, SocketAddress(addr, port));
	}
	
}
