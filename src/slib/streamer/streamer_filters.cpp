#include "../../../inc/slib/streamer/definition.h"
#include "../../../inc/slib/streamer/filters.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/crypto/sha2.h"

#define STACK_BUFFER_SIZE 4096

SLIB_STREAMER_NAMESPACE_BEGIN
List<Packet> DatagramHashSHA256SendFilter::filter(const Packet& input)
{
	Packet output = input;
	MemoryBuffer buf;
	buf.add(SHA256::hash(input.data));
	buf.add(input.data);
	output.data = buf.merge();
	return List<Packet>::fromElement(output);
}

List<Packet> DatagramHashSHA256ReceiveFilter::filter(const Packet& input)
{
	Packet output = input;
	char* buf = (char*)(output.data.getBuf());
	sl_uint32 size = (sl_uint32)(output.data.getSize());
	if (size <= 32) {
		return List<Packet>::null();
	}
	char hash[32];
	SHA256::hash(buf + 32, size - 32, hash);
	if (Base::compareMemory(hash, buf, 32) != 0) {
		return List<Packet>::null();
	}
	output.data = Memory::create(buf + 32, size - 32);
	return List<Packet>::fromElement(output);
}
SLIB_STREAMER_NAMESPACE_END

SLIB_STREAMER_NAMESPACE_BEGIN
DatagramErrorCorrectionSendFilter::DatagramErrorCorrectionSendFilter(sl_uint32 level, sl_uint32 maxPacketSize)
{
	m_lastSentPacketNumber = 0;
	m_level = level;
	m_maxPacketSize = maxPacketSize;
}

DatagramErrorCorrectionSendFilter::~DatagramErrorCorrectionSendFilter()
{
}

List<Packet> DatagramErrorCorrectionSendFilter::filter(const Packet& input)
{
	void* buf = input.data.getBuf();
	sl_size size = input.data.getSize();
	if (size > m_maxPacketSize) {
		return List<Packet>::null();
	}
	SendPacket packetNew;
	packetNew.num = m_lastSentPacketNumber++;
	packetNew.mem = Memory::create(buf, size);
	m_packetsSend.push(packetNew, m_level + 1);
	MemoryWriter out;
	Array<SendPacket> packets = m_packetsSend.toArray();
	for (sl_size i = 0; i < packets.getCount(); i++) {
		out.writeUint64CVLI(packets[i].num);
		out.writeSizeCVLI(packets[i].mem.getSize());
		out.write(packets[i].mem.getBuf(), packets[i].mem.getSize());
	}
	Memory data = out.getData();

	List<Packet> ret;
	Packet output;
	output.format = Packet::formatRaw;
	output.data = data;
	ret.add(output);
	return ret;
}

DatagramErrorCorrectionReceiveFilter::DatagramErrorCorrectionReceiveFilter(sl_uint32 maxPacketSize)
{
	m_maxPacketSize = maxPacketSize;
	m_lastReceivedPacketNumber = 0;
}

DatagramErrorCorrectionReceiveFilter::~DatagramErrorCorrectionReceiveFilter()
{
}

List<Packet> DatagramErrorCorrectionReceiveFilter::filter(const Packet& input)
{
	List<Packet> ret;
	void* buf = input.data.getBuf();
	sl_size packetSize = input.data.getSize();
	MemoryReader in(buf, packetSize);
	SLIB_SCOPED_BUFFER(char, STACK_BUFFER_SIZE, buffer, m_maxPacketSize);
	if (!buffer) {
		return List<Packet>::null();
	}
	while (1) {
		sl_uint64 num;
		sl_size size;
		if (!in.readUint64CVLI(&num)) {
			break;
		}
		if (!in.readSizeCVLI(&size)) {
			break;
		}
		if (size == 0 || size > m_maxPacketSize) {
			break;
		}
		sl_size sizeRead = in.read(buffer, size);
		if (size == sizeRead) {
			if (num > m_lastReceivedPacketNumber || num + 100 < m_lastReceivedPacketNumber || m_lastReceivedPacketNumber == 0) {
				m_lastReceivedPacketNumber = num;
				Packet packet;
				packet.format = Packet::formatRaw;
				packet.data = Memory::create(buffer, size);
				ret.add(packet);
			}
		} else {
			break;
		}
	}
	return ret;
}
SLIB_STREAMER_NAMESPACE_END
