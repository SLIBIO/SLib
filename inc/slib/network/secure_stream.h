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
	void start();
	void close();

protected:
	void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address);
	void onConnectedSecureStream(AsyncSecureStream* securedStream, sl_bool flagError);

public:
	static Ref<AsyncTcpSecureStreamServer> create(const Ref<AsyncTcpServer>& server, const AsyncSecureStreamServerParam& param, sl_bool flagStart = sl_true);
	
	static Ref<AsyncTcpSecureStreamServer> create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen, const Ref<AsyncLoop>& loop, sl_bool flagStart = sl_true);
	static Ref<AsyncTcpSecureStreamServer> create(const AsyncSecureStreamServerParam& param, const SocketAddress& addressListen, sl_bool flagStart = sl_true);
	static Ref<AsyncTcpSecureStreamServer> create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, const Ref<AsyncLoop>& loop, sl_bool flagIPv6 = sl_false, sl_bool flagStart = sl_true);
	static Ref<AsyncTcpSecureStreamServer> create(const AsyncSecureStreamServerParam& param, sl_uint32 portListen, sl_bool flagIPv6 = sl_false, sl_bool flagStart = sl_true);

private:
	Ref<AsyncTcpServer> m_server;
	AsyncSecureStreamServerParam m_param;
	Map< AsyncSecureStream*, Ref<AsyncSecureStream> > m_streams;
};

class SLIB_EXPORT AsyncTcpSecureStreamClient
{
public:
	static Ref<AsyncSecureStream> create(const Ref<AsyncTcpSocket>& socket, const AsyncSecureStreamClientParam& param, sl_bool flagConnect = sl_true);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param
		, const SocketAddress& addressBind, const SocketAddress& addressConnect
		, const Ref<AsyncLoop>& loop);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param
		, const SocketAddress& addressBind, const SocketAddress& addressConnect);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param
		, const SocketAddress& addressConnect
		, const Ref<AsyncLoop>& loop);

	static Ref<AsyncSecureStream> create(
		const AsyncSecureStreamClientParam& param
		, const SocketAddress& addressConnect);
};

SLIB_NETWORK_NAMESPACE_END

#endif

