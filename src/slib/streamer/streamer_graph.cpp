#include "../../../inc/slib/streamer/definition.h"
#include "../../../inc/slib/streamer/graph.h"

SLIB_STREAMER_NAMESPACE_BEGIN

Graph::Graph()
{
}

Graph::~Graph()
{
	release();
}

sl_bool Graph::start()
{
	if (m_source.isNull()) {
		return sl_false;
	}
	if (m_sink.isNull()) {
		return sl_false;
	}
	m_thread = Thread::start(SLIB_CALLBACK_REF(Graph, run, this));
	if (m_thread.isNotNull()) {
		return sl_true;
	}
	return sl_false;
}

void Graph::release()
{
	m_source.setNull();
	m_sink.setNull();
	m_filters.clear();
	m_filters.setNull();
	Ref<Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->finishAndWait();
	}
}

void Graph::run()
{
	Ref<Source> source = m_source;
	if (source.isNull()) {
		return;
	}
	Ref<Event> ev = Event::create();
	if (ev.isNull()) {
		return;
	}
	source->setEvent(ev);

	while (Thread::isNotStoppingCurrent()) {
		Packet packet;
		if (ev->wait()) {
			while (Thread::isNotStoppingCurrent() && source->receivePacket(&packet)) {
				feedPacket(packet);
			}
		} else {
			Thread::sleep(50);
		}
	}
}

void Graph::feedPacket(const Packet& packet)
{
	Ref<Sink> sink = m_sink;
	if (sink.isNull()) {
		return;
	}
	List<Packet> packets;
	{
		packets.push(packet);
		ListLocker< Ref<Filter> > filters(m_filters.duplicate());
		for (sl_size i = 0; i < filters.count(); i++) {
			Ref<Filter> filter = filters[i];
			if (filter.isNotNull()) {
				List<Packet> outs;
				ListLocker<Packet> ins(packets);
				for (sl_size k = 0; k < ins.count(); k++) {
					List<Packet> out = filter->filter(ins[k]);
					outs.add(out);
				}
				packets = outs;
			}
		}
	}
	{
		ListLocker<Packet> outs(packets);
		for (sl_size i = 0; i < outs.count(); i++) {
			sink->sendPacket(outs[i]);
		}
	}
}

Ref<Graph> Graph::create()
{
	Ref<Graph> ret = new Graph;
	return ret;
}
SLIB_STREAMER_NAMESPACE_END
