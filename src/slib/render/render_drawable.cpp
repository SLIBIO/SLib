#include "../../../inc/slib/render/drawable.h"

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
	
	class _ShaderDrawable_Program : public RenderProgram2D_Position
	{
	public:
		String m_vertexShader;
		String m_fragmentShader;
		
	public:
		_ShaderDrawable_Program(const String& vs, const String fs):
	 m_vertexShader(vs), m_fragmentShader(fs)
		{
		}
		
		// override
		String getGLSLVertexShader(RenderEngine* engine)
		{
			return m_vertexShader;
		}
		
		// override
		String getGLSLFragmentShader(RenderEngine* engine)
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
			program = new _ShaderDrawable_Program(vs, fs);
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
