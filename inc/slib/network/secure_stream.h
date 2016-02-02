#ifndef CHECKHEADER_SLIB_NETWORK_SECURE_TUNNEL
#define CHECKHEADER_SLIB_NETWORK_SECURE_TUNNEL

#include "definition.h"

#include "async.h"
#include "../core/map.h"
#include "../crypto/secure_stream.h"

SLIB_NETWORK_NAMESPACE_BEGIN

// gets only successfully connected secure connections
class SLIB_EXPORT AsyncTcpSecureStreamServer : public Object, public IAsyncTcpServerListener, public IAsyncSecureStreamListener
{
private:
	AsyncTcpSecureStreamServer();
	~AsyncTcpSecureStreamServer();
	
public:
	static Ref<AsyncTcpSecureStreamServer> create(
		const AsyncSecureStreamServerParam& param,
		const SocketAddress& addressListen,
		const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSecureStreamServer> create(
		const AsyncSecureStreamServerParam& param,
		const SocketAddress& addressListen);
	
	static Ref<AsyncTcpSecureStreamServer> create(
		const AsyncSecureStreamServerParam& param,
		sl_uint32 portListen,
		const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSecureStreamServer> create(
		const AsyncSecureStreamServerParam& param,
		sl_uint32 portListen);
	
	static Ref<AsyncTcpSecureStreamServer> createIPv6(
		const AsyncSecureStreamServerParam& param,
		sl_uint32 portListen,
		const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSecureStreamServer> createIPv6(
		const AsyncSecureStreamServerParam& param,
		sl_uint32 portListen);
	
public:
	void close();

protected:
	// override
	void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address);
	
	// override
	void onConnectedSecureStream(AsyncSecureStream* securedStream, sl_bool flagError);

private:
	Ref<AsyncTcpServer> m_server;
	AsyncSecureStreamServerParam m_param;
	HashMap< AsyncSecureStream*, Ref<AsyncSecureStream> > m_streams;
};

class SLIB_EXPORT AsyncTcpSecureStreamClient
{
public:
	static Ref<AsyncSecureStream> create(
		const Ref<AsyncTcpSocket>& socket,
		const AsyncSecureStreamClientParam& param,
		sl_bool flagConnect = sl_true);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param,
		const SocketAddress& addressBind, const SocketAddress& addressConnect,
		const Ref<AsyncIoLoop>& loop);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param,
		const SocketAddress& addressBind,
		const SocketAddress& addressConnect);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param,
		const SocketAddress& addressConnect,
		const Ref<AsyncIoLoop>& loop);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param,
		const SocketAddress& addressConnect);
	
};

SLIB_NETWORK_NAMESPACE_END

#endif

