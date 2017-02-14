#ifndef CHECKHEADER_SLIB_RENDER_DRAWABLE
#define CHECKHEADER_SLIB_RENDER_DRAWABLE

#include "definition.h"

#include "canvas.h"

#include "../graphics/drawable.h"
#include "../core/function.h"

namespace slib
{

	class RenderDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		RenderDrawable();
		
		~RenderDrawable();
		
	protected:
		virtual void onRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param);
		
	public:
		virtual void dispatchRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param);
		
		// override
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
		
	public:
		SLIB_PROPERTY(AtomicFunction<void(RenderCanvas*, Rectangle const&, DrawParam const&)>, OnRender)
		
	};
	
	class ShaderDrawable : public RenderDrawable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ShaderDrawable();
		
		~ShaderDrawable();
		
	public:
		static Ref<ShaderDrawable> create(const String& vertexShader, const String& fragmentShader);
		
		static Ref<ShaderDrawable> createWithFragmentShader(const String& fragmentShader);
		
	public:
		String getVertexShader();
		
		void setVertexShader(const String& shader);
		
		String getFragmentShader();
		
		void setFragmentShader(const String& shader);
		
	protected:
		// override
		void onRender(RenderCanvas* canvas, const Rectangle& rectDst, const DrawParam& param);
		
	public:
		AtomicString m_vertexShader;
		AtomicString m_fragmentShader;
		AtomicRef<RenderProgram2D_Position> m_program;
		
	};

}

#endif
