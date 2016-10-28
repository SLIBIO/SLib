#ifndef CHECKHEADER_SLIB_RENDER_PROGRAM
#define CHECKHEADER_SLIB_RENDER_PROGRAM

#include "definition.h"

#include "base.h"
#include "texture.h"

#include "../math/matrix4.h"
#include "../graphics/color.h"

SLIB_RENDER_NAMESPACE_BEGIN

class Primitive;
class RenderEngine;

class SLIB_EXPORT RenderProgramState : public Referable
{
public:
	sl_uint32 gl_program;

};

class SLIB_EXPORT RenderProgramInstance : public RenderBaseObjectInstance
{
	SLIB_DECLARE_OBJECT
};

class SLIB_EXPORT RenderProgram : public RenderBaseObject
{
	SLIB_DECLARE_OBJECT

public:
	virtual Ref<RenderProgramState> onCreate(RenderEngine* engine) = 0;
	
	virtual sl_bool onInit(RenderEngine* engine, RenderProgramState* state);

	virtual sl_bool onUpdate(RenderEngine* engine, RenderProgramState* state);
	
	virtual sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	virtual void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);

	virtual String getGLSLVertexShader(RenderEngine* engine);
	
	virtual String getGLSLFragmentShader(RenderEngine* engine);

	
	Ref<RenderProgramInstance> getInstance(RenderEngine* engine);
	
};

template <class BaseProgram, class StateType>
class SLIB_EXPORT RenderProgramT : public BaseProgram
{
	// override
	Ref<RenderProgramState> onCreate(RenderEngine* engine)
	{
		return new StateType;
	}
	
};


#define SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, location, VertexData, member) \
	engine->setVertexFloatArrayAttribute(location, (sl_uint32)(sl_size)(&(((VertexData*)0)->member)), sizeof(((VertexData*)0)->member) / sizeof(float), sizeof(VertexData));


class RenderProgramState2D : public RenderProgramState
{
public:
	Matrix3 transform;
	Ref<Texture> texture;
	Matrix3 textureTransform;
	Color3f color;
	float alpha;
	
	sl_int32 gl_uniformTransform;			// Matrix3
	sl_int32 gl_uniformTexture;				// sampler
	sl_int32 gl_uniformTextureTransform;	// sampler
	sl_int32 gl_uniformColor;				// Vector3
	sl_int32 gl_uniformAlpha;				// float
	
	sl_int32 gl_attrPosition;		// Vector2
	sl_int32 gl_attrColor;			// Vector4
	sl_int32 gl_attrTexCoord;		// Vector2
	
public:
	RenderProgramState2D();
	
};

// Base class for 2D RenderPrograms
class SLIB_EXPORT RenderProgram2D : public RenderProgramT<RenderProgram, RenderProgramState2D>
{
public:
	RenderProgram2D();
	
public:
	// override
	sl_bool onInit(RenderEngine* engine, RenderProgramState* state);
	
	// override
	sl_bool onUpdate(RenderEngine* engine, RenderProgramState* state);
	
};

// Position(2), TexCoord(2)
class SLIB_EXPORT RenderProgram2D_PositionTexture : public RenderProgram2D
{
public:
	struct VertexData
	{
		Vector2 position;
		Vector2 texCoord;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(2), TexCoord(2)
class SLIB_EXPORT RenderProgram2D_PositionTextureYUV : public RenderProgram2D_PositionTexture
{
public:
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
};

// Position(2), Color(4)
class SLIB_EXPORT RenderProgram2D_PositionColor : public RenderProgram2D
{
public:
	struct VertexData
	{
		Vector2 position;
		Color4f color;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(2)
class SLIB_EXPORT RenderProgram2D_Position : public RenderProgram2D
{
public:
	struct VertexData
	{
		Vector2 position;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};


class RenderProgramState3D : public RenderProgramState
{
public:
	Matrix4 transform;
	Matrix4 matrixModelViewIT;
	Ref<Texture> texture;
	Vector3 directionalLight;
	Color3f diffuseColor;
	Color3f ambientColor;
	float alpha;
	
	sl_int32 gl_uniformTransform;				// Matrix4
	sl_int32 gl_uniformMatrixModelViewIT;		// Matrix4
	sl_int32 gl_uniformTexture;					// sampler
	sl_int32 gl_uniformDirectionalLight;		// Vector3
	sl_int32 gl_uniformDiffuseColor;			// Vector3
	sl_int32 gl_uniformAmbientColor;			// Vector3
	sl_int32 gl_uniformAlpha;					// float
	
	sl_int32 gl_attrPosition;	// Vector3
	sl_int32 gl_attrNormal;		// Vector3
	sl_int32 gl_attrColor;		// Vector3
	sl_int32 gl_attrTexCoord;	// Vector2
	
public:
	RenderProgramState3D();
	
};

// Base class for 3D RenderPrograms
class SLIB_EXPORT RenderProgram3D : public RenderProgramT<RenderProgram, RenderProgramState3D>
{
public:
	RenderProgram3D();

public:
	// override
	sl_bool onInit(RenderEngine* engine, RenderProgramState* state);
	
	// override
	sl_bool onUpdate(RenderEngine* engine, RenderProgramState* state);

};

// Position(3), Normal(3), Color(4)
class SLIB_EXPORT RenderProgram3D_PositionNormalColor_Diffuse : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
		Vector3 normal;
		Color4f color;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(3), Color(4)
class SLIB_EXPORT RenderProgram3D_PositionColor : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
		Color4f color;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(3), Normal(3), TexCoord0(2)
class SLIB_EXPORT RenderProgram3D_PositionNormalTexture_Diffuse : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(3), TexCoord(2)
class SLIB_EXPORT RenderProgram3D_PositionTexture : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
		Vector2 texCoord;
	};

public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(3), Normal(3)
class SLIB_EXPORT RenderProgram3D_PositionNormal_Diffuse : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
		Vector3 normal;
	};
    
public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

// Position(3)
class SLIB_EXPORT RenderProgram3D_Position : public RenderProgram3D
{
public:
	struct VertexData
	{
		Vector3 position;
	};

public:
	// override
	String getGLSLVertexShader(RenderEngine* engine);
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine);
	
	// override
	sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive);
	
};

SLIB_RENDER_NAMESPACE_END

#endif

