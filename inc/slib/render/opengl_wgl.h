#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_WGL
#define CHECKHEADER_SLIB_RENDER_OPENGL_WGL

#include "definition.h"

#include "engine.h"

namespace slib
{

	class SLIB_EXPORT WGL
	{
	public:
		static Ref<Renderer> createRenderer(void* windowHandle, const RendererParam& param);
		
	};

}

#endif
