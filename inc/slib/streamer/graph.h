
#ifndef CHECKHEADER_SLIB_STREAMER_GRAPH
#define CHECKHEADER_SLIB_STREAMER_GRAPH

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/queue.h"
#include "../core/thread.h"

/***********************************

- Graph
Graph = Source -> Filter -> Filter -> ... -> Filter -> Sink
             Packet    Packet    Packet           Packet

Source should be attached for only one graph.

- Station
Station provides linked sink and sources.

                     -> Source1
				    /
	Sink -> Station  -> Source2
	                \
					 -> Source3


************************************/

SLIB_STREAMER_NAMESPACE_BEGIN
struct SLIB_EXPORT Packet
{
	enum Format {
		formatRaw = 0,
		formatAudio_PCM_S16 = 10,
		formatAudio_OPUS = 11
	};
	Format format;

	struct AudioParam
	{
		sl_uint32 nSamplesPerSecond;
		sl_uint32 nChannels;
	};
	AudioParam audioParam;
	struct NetworkParam
	{
		String addressFrom;
		String addressTo;
	};
	NetworkParam networkParam;

	Memory data;
};

class SLIB_EXPORT Source : public Object
{
	SLIB_DECLARE_OBJECT(Source, Object)
public:
	Source() {}
	virtual sl_bool receivePacket(Packet* out) = 0;

public:
	SLIB_PROPERTY(Ref<Event>, Event)
};

class SLIB_EXPORT Sink : public Object
{
	SLIB_DECLARE_OBJECT(Sink, Object)
public:
	SLIB_INLINE Sink() {}
	virtual sl_bool sendPacket(const Packet& packet) = 0;

public:
	SLIB_PROPERTY(Ref<Event>, Event)
};

class SLIB_EXPORT Filter : public Object
{
	SLIB_DECLARE_OBJECT(Filter, Object)
public:
	SLIB_INLINE Filter() {}
	virtual List<Packet> filter(const Packet& input) = 0;
};

class SLIB_EXPORT Graph : public Object
{
	SLIB_DECLARE_OBJECT(Graph, Object)
protected:
	Graph();
public:
	~Graph();

public:
	static Ref<Graph> create();

	sl_bool start();
	void release();

	virtual void feedPacket(const Packet& packet);

protected:
	virtual void run();

public:
	SLIB_INLINE const Ref<Source>& getSource()
	{
		return m_source;
	}
	SLIB_INLINE void setSource(const Ref<Source>& source)
	{
		m_source = source;
	}

	SLIB_INLINE const Ref<Sink>& getSink()
	{
		return m_sink;
	}
	SLIB_INLINE void setSink(const Ref<Sink>& sink)
	{
		m_sink = sink;
	}


	SLIB_INLINE const List< Ref<Filter> >& getFilters()
	{
		return m_filters;
	}
	SLIB_INLINE void addFilter(const Ref<Filter>& filter)
	{
		if (filter.isNotNull()) {
			m_filters.add(filter);
		}
	}

	SLIB_INLINE const Ref<Thread>& getThread()
	{
		return m_thread;
	}

private:
	Ref<Source> m_source;
	Ref<Sink> m_sink;
	List< Ref<Filter> > m_filters;
	Ref<Thread> m_thread;
};
SLIB_STREAMER_NAMESPACE_END

#endif
