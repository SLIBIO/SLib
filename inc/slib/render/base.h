#ifndef CHECKHEADER_SLIB_RENDER_BASE
#define CHECKHEADER_SLIB_RENDER_BASE

#include "definition.h"

#include "../core/object.h"

SLIB_RENDER_NAMESPACE_BEGIN

class RenderEngine;
class RenderBaseObject;

class SLIB_EXPORT RenderBaseObjectInstance : public Object
{
	SLIB_DECLARE_OBJECT

public:
	void linkObject(RenderEngine* engine, RenderBaseObject* object);

	Ref<RenderBaseObject> getObject();
	
	Ref<RenderEngine> getEngine();

protected:
	SafeWeakRef<RenderBaseObject> m_object;
	SafeWeakRef<RenderEngine> m_engine;

};

class SLIB_EXPORT RenderBaseObject : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	RenderBaseObject();
	
	~RenderBaseObject();

public:
	void linkEngine(const Ref<RenderEngine>& engine, const Ref<RenderBaseObjectInstance>& instance);
	
	void unlinkEngine();

	Ref<RenderEngine> getEngine();
	
	Ref<RenderBaseObjectInstance> getInstance();

protected:
	SafeWeakRef<RenderEngine> m_engine;
	SafeRef<RenderBaseObjectInstance> m_instance;
	
};

SLIB_RENDER_NAMESPACE_END

#endif
