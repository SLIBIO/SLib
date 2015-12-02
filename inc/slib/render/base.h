#ifndef CHECKHEADER_SLIB_RENDER_BASE
#define CHECKHEADER_SLIB_RENDER_BASE

#include "definition.h"

#include "../core/object.h"

SLIB_RENDER_NAMESPACE_BEGIN

class RenderEngine;
class RenderBaseObject;

class SLIB_EXPORT RenderBaseObjectInstance : public Object
{
	SLIB_DECLARE_OBJECT(RenderBaseObjectInstance, Object)
protected:
	RenderBaseObjectInstance();

public:
	void linkObject(RenderEngine* engine, RenderBaseObject* object);

	SLIB_INLINE Ref<RenderBaseObject> getObject()
	{
		return m_object.lock();
	}

	SLIB_INLINE Ref<RenderEngine> getEngine()
	{
		return m_engine.lock();
	}

protected:
	WeakRef<RenderBaseObject> m_object;
	WeakRef<RenderEngine> m_engine;

};

class SLIB_EXPORT RenderBaseObject : public Object
{
	SLIB_DECLARE_OBJECT(RenderBaseObject, Object)
protected:
	RenderBaseObject();
public:
	~RenderBaseObject();

public:
	void linkEngine(const Ref<RenderEngine>& engine, const Ref<RenderBaseObjectInstance>& instance);
	void unlinkEngine();

	SLIB_INLINE Ref<RenderEngine> getEngine()
	{
		return m_engine.lock();
	}

	SLIB_INLINE Ref<RenderBaseObjectInstance> getInstance()
	{
		return m_instance;
	}

protected:
	WeakRef<RenderEngine> m_engine;
	Ref<RenderBaseObjectInstance> m_instance;
};
SLIB_RENDER_NAMESPACE_END

#endif

