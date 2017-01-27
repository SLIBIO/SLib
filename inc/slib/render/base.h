#ifndef CHECKHEADER_SLIB_RENDER_BASE
#define CHECKHEADER_SLIB_RENDER_BASE

#include "definition.h"

#include "../core/object.h"
#include "../core/list.h"

SLIB_RENDER_NAMESPACE_BEGIN

class RenderEngine;
class RenderBaseObject;

#define SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT 4

class SLIB_EXPORT RenderBaseObjectInstance : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	RenderBaseObjectInstance();
	
	~RenderBaseObjectInstance();

public:
	void link(const Ref<RenderEngine>& engine, const Ref<RenderBaseObject>& object);

	Ref<RenderBaseObject> getObject();
	
	Ref<RenderEngine> getEngine();
	
protected:
	virtual void onUpdate(RenderBaseObject* object);

public:
	// should be called by only engine internally
	void _update(RenderBaseObject* object);
	
protected:
	AtomicWeakRef<RenderBaseObject> m_object;
	AtomicWeakRef<RenderEngine> m_engine;
	
	sl_bool m_flagUpdated;
	
	friend class RenderBaseObject;

};

class SLIB_EXPORT RenderBaseObject : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	RenderBaseObject();
	
	~RenderBaseObject();
	
public:
	void addInstance(const Ref<RenderBaseObjectInstance>& instance);
	
	void removeInstance(const Ref<RenderBaseObjectInstance>& instance);
	
	void removeAllInstances();

	Ref<RenderBaseObjectInstance> getInstance(RenderEngine* engine);
	
protected:
	AtomicRef<RenderBaseObjectInstance> m_instances[SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT];
	
};

SLIB_RENDER_NAMESPACE_END

#endif
