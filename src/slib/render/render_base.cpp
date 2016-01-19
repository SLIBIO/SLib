#include "../../../inc/slib/render/base.h"
#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN

void RenderBaseObjectInstance::linkObject(RenderEngine* engine, RenderBaseObject* object)
{
	object->linkEngine(engine, this);
	m_object = object;
	m_engine = engine;
}

Ref<RenderBaseObject> RenderBaseObjectInstance::getObject()
{
	return m_object;
}

Ref<RenderEngine> RenderBaseObjectInstance::getEngine()
{
	return m_engine;
}


RenderBaseObject::RenderBaseObject()
{
}

RenderBaseObject::~RenderBaseObject()
{
	unlinkEngine();
}

void RenderBaseObject::linkEngine(const Ref<RenderEngine>& engine, const Ref<RenderBaseObjectInstance>& instance)
{
	Ref<RenderEngine> engineOld(m_engine);
	if (engine != engineOld) {
		unlinkEngine();
		if (engine.isNotNull() && instance.isNotNull()) {
			m_engine = engine;
			m_instance = instance;
		}
	}
}

void RenderBaseObject::unlinkEngine()
{
	Ref<RenderEngine> engine(m_engine);
	Ref<RenderBaseObjectInstance> instance(m_instance);
	if (engine.isNotNull() && instance.isNotNull()) {
		engine->_onFreeInstance(instance.get());
	}
	m_engine.setNull();
	m_instance.setNull();
}

Ref<RenderEngine> RenderBaseObject::getEngine()
{
	return m_engine;
}

Ref<RenderBaseObjectInstance> RenderBaseObject::getInstance()
{
	return m_instance;
}

SLIB_RENDER_NAMESPACE_END
