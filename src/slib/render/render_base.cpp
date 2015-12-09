#include "../../../inc/slib/render/base.h"
#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN
RenderBaseObjectInstance::RenderBaseObjectInstance()
{
}

void RenderBaseObjectInstance::linkObject(RenderEngine* engine, RenderBaseObject* object)
{
	object->linkEngine(engine, this);
	m_object = (Ref<RenderBaseObject>)object;
	m_engine = (Ref<RenderEngine>)engine;
}


RenderBaseObject::RenderBaseObject()
{
}

RenderBaseObject::~RenderBaseObject()
{
	unlinkEngine();
}

void RenderBaseObject::linkEngine(const Ref<RenderEngine>& _engine, const Ref<RenderBaseObjectInstance>& _instance)
{
	Ref<RenderEngine> engine = _engine;
	if (engine != m_engine.lock()) {
		unlinkEngine();
		Ref<RenderBaseObjectInstance> instance = _instance;
		if (engine.isNotNull() && instance.isNotNull()) {
			m_engine = engine;
			m_instance = instance;
		}
	}
}

void RenderBaseObject::unlinkEngine()
{
	Ref<RenderEngine> engine = m_engine.lock();
	if (engine.isNotNull()) {
		engine->_onFreeInstance(m_instance.get());
	}
	m_engine.setNull();
	m_instance.setNull();
}
SLIB_RENDER_NAMESPACE_END
