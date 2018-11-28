/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_RENDER_BASE
#define CHECKHEADER_SLIB_RENDER_BASE

#include "definition.h"

#include "../core/object.h"
#include "../core/list.h"

#define SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT 4

namespace slib
{

	class RenderEngine;
	class RenderBaseObject;
	
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
		
		sl_bool _isUpdated();
		
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

}

#endif
