
#ifndef CHECKHEADER_SLIB_STREAMER_FILTERS
#define CHECKHEADER_SLIB_STREAMER_FILTERS

#include "definition.h"

#include "graph.h"

SLIB_STREAMER_NAMESPACE_BEGIN

class SLIB_EXPORT DatagramHashSHA256SendFilter : public Filter
{
public:
	DatagramHashSHA256SendFilter() {}
	~DatagramHashSHA256SendFilter() {}

	List<Packet> filter(const Packet& input);
};

class SLIB_EXPORT DatagramHashSHA256ReceiveFilter : public Filter
{
public:
	DatagramHashSHA256ReceiveFilter() {}
	~DatagramHashSHA256ReceiveFilter() {}

	List<Packet> filter(const Packet& input);
};

class SLIB_EXPORT DatagramErrorCorrectionSendFilter : public Filter
{
public:
	DatagramErrorCorrectionSendFilter(sl_uint32 level = 1, sl_uint32 maxPacketSize = 2000);
	~DatagramErrorCorrectionSendFilter();

	List<Packet> filter(const Packet& input);

private:
	struct SendPacket {
		sl_uint64 num;
		Memory mem;
	};
	Queue<SendPacket> m_packetsSend;
	sl_uint64 m_lastSentPacketNumber;
	sl_uint32 m_level;
	sl_uint32 m_maxPacketSize;
};

class SLIB_EXPORT DatagramErrorCorrectionReceiveFilter : public Filter
{
public:
	DatagramErrorCorrectionReceiveFilter(sl_uint32 maxPacketSize = 2000);
	~DatagramErrorCorrectionReceiveFilter();

	List<Packet> filter(const Packet& input);

private:
	sl_uint64 m_lastReceivedPacketNumber;
	sl_uint32 m_maxPacketSize;
};

SLIB_STREAMER_NAMESPACE_END

#endif
