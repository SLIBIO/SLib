/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/render/base.h"

#include "slib/render/engine.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(RenderBaseObjectInstance, Object)
	
	RenderBaseObjectInstance::RenderBaseObjectInstance()
	{
		m_flagUpdated = sl_false;
	}
	
	RenderBaseObjectInstance::~RenderBaseObjectInstance()
	{
	}
	
	void RenderBaseObjectInstance::link(const Ref<RenderEngine>& engine, const Ref<RenderBaseObject>& object)
	{
		m_object = object;
		m_engine = engine;
		object->addInstance(this);
	}
	
	Ref<RenderBaseObject> RenderBaseObjectInstance::getObject()
	{
		return m_object;
	}
	
	Ref<RenderEngine> RenderBaseObjectInstance::getEngine()
	{
		return m_engine;
	}
	
	void RenderBaseObjectInstance::onUpdate(RenderBaseObject* object)
	{
	}
	
	void RenderBaseObjectInstance::_update(RenderBaseObject* object)
	{
		if (m_flagUpdated) {
			m_flagUpdated = sl_false;
			onUpdate(object);
		}
	}
	
	sl_bool RenderBaseObjectInstance::_isUpdated()
	{
		return m_flagUpdated;
	}
	
	
	SLIB_DEFINE_OBJECT(RenderBaseObject, Object)
	
	RenderBaseObject::RenderBaseObject()
	{
	}
	
	RenderBaseObject::~RenderBaseObject()
	{
	}
	
	void RenderBaseObject::addInstance(const Ref<RenderBaseObjectInstance>& instanceNew)
	{
		if (instanceNew.isNull()) {
			return;
		}
		Ref<RenderEngine> engineNew(instanceNew->m_engine);
		if (engineNew.isNull()) {
			return;
		}
		int indexEmpty = -1;
		int i;
		for (i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
			Ref<RenderBaseObjectInstance> instance = m_instances[i];
			if (instance.isNotNull()) {
				if (instance == instanceNew) {
					return;
				}
				Ref<RenderEngine> engine(instance->m_engine);
				if (engine.isNotNull()) {
					if (engine.get() == engineNew.get()) {
						return;
					}
				} else {
					indexEmpty = i;
					m_instances[i].setNull();
				}
			} else {
				indexEmpty = i;
			}
		}
		if (indexEmpty >= 0) {
			m_instances[indexEmpty] = instanceNew;
		} else {
			for (i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT - 1; i++) {
				m_instances[i] = m_instances[i+1];
			}
			m_instances[i] = instanceNew;
		}
	}
	
	void RenderBaseObject::removeInstance(const Ref<RenderBaseObjectInstance>& instanceRemove)
	{
		if (instanceRemove.isNull()) {
			return;
		}
		for (int i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
			if (m_instances[i] == instanceRemove) {
				m_instances[i].setNull();
				break;
			}
		}
	}
	
	void RenderBaseObject::removeAllInstances()
	{
		for (int i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
			m_instances[i].setNull();
		}
	}
	
	Ref<RenderBaseObjectInstance> RenderBaseObject::getInstance(RenderEngine* engine)
	{
		if (engine) {
			for (int i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
				Ref<RenderBaseObjectInstance> instance = m_instances[i];
				if (instance.isNotNull()) {
					Ref<RenderEngine> instanceEngine(instance->m_engine);
					if (instanceEngine.isNotNull()) {
						if (instanceEngine.get() == engine) {
							return instance;
						}
					} else {
						m_instances[i].setNull();
					}
				}
			}
		}
		return sl_null;
	}

}
