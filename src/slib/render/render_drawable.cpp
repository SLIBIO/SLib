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

#include "slib/render/drawable.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(RenderDrawable, Drawable)
	
	RenderDrawable::RenderDrawable()
	{
	}
	
	RenderDrawable::~RenderDrawable()
	{
	}
	
	void RenderDrawable::onRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
	}
	
	void RenderDrawable::dispatchRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		onRender(canvas, rectDst, param);
		getOnRender()(canvas, rectDst, param);
	}
	
	void RenderDrawable::onDrawAll(Canvas* _canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		RenderCanvas* canvas = CastInstance<RenderCanvas>(_canvas);
		if (canvas) {
			dispatchRender(canvas, rectDst, param);
		}
	}
	
	
	SLIB_DEFINE_OBJECT(ShaderDrawable, Drawable)
	
	ShaderDrawable::ShaderDrawable()
	{
		
	}
	
	ShaderDrawable::~ShaderDrawable()
	{
	}
	
	Ref<ShaderDrawable> ShaderDrawable::create(const String& vertexShader, const String& fragmentShader)
	{
		Ref<ShaderDrawable> ret = new ShaderDrawable;
		if (ret.isNotNull()) {
			ret->m_vertexShader = vertexShader;
			ret->m_fragmentShader = fragmentShader;
			return ret;
		}
		return sl_null;
	}
	
	Ref<ShaderDrawable> ShaderDrawable::createWithFragmentShader(const String& fragmentShader)
	{
		String vs =
		SLIB_STRINGIFY(
					   uniform mat3 u_Transform;
					   uniform vec4 u_Color;
					   attribute vec2 a_Position;
					   varying vec2 v_Position;
					   void main() {
						   vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
						   gl_Position = vec4(P.x, P.y, 0.0, 1.0);
						   v_Position = a_Position;
					   }
					   );
		return create(vs, fragmentShader);
	}
	
	String ShaderDrawable::getVertexShader()
	{
		return m_fragmentShader;
	}
	
	void ShaderDrawable::setVertexShader(const String& shader)
	{
		m_fragmentShader = shader;
		m_program.setNull();
	}
	
	String ShaderDrawable::getFragmentShader()
	{
		return m_fragmentShader;
	}
	
	void ShaderDrawable::setFragmentShader(const String& shader)
	{
		m_fragmentShader = shader;
		m_program.setNull();
	}
	
	class _priv_ShaderDrawable_Program : public RenderProgram2D_Position
	{
	public:
		String m_vertexShader;
		String m_fragmentShader;
		
	public:
		_priv_ShaderDrawable_Program(const String& vs, const String& fs)
		 : m_vertexShader(vs), m_fragmentShader(fs)
		{
		}
		
		String getGLSLVertexShader(RenderEngine* engine) override
		{
			return m_vertexShader;
		}
		
		String getGLSLFragmentShader(RenderEngine* engine) override
		{
			return m_fragmentShader;
		}
		
	};
	
	void ShaderDrawable::onRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Ref<RenderProgram2D_Position> program = m_program;
		if (program.isNull()) {
			String vs = m_vertexShader;
			String fs = m_fragmentShader;
			if (vs.isEmpty() || fs.isEmpty()) {
				return;
			}
			program = new _priv_ShaderDrawable_Program(vs, fs);
			if (program.isNull()) {
				return;
			}
			m_program = program;
		}
		Ref<RenderEngine> engine = canvas->getEngine();
		if (engine.isNull()) {
			return;
		}
		RenderProgramScope<RenderProgramState2D_Position> scope;
		if (scope.begin(engine, program)) {
			canvas->drawRectangle(rectDst, scope.getState(), param);
		}
	}
	
}
