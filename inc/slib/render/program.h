#ifndef CHECKHEADER_SLIB_RENDER_PROGRAM
#define CHECKHEADER_SLIB_RENDER_PROGRAM

#include "definition.h"

#include "base.h"
#include "texture.h"

#include "../math/matrix4.h"
#include "../graphics/color.h"

#define SLIB_RENDER_MAX_TEXTURE_SAMPLERS 16

SLIB_RENDER_NAMESPACE_BEGIN

class Primitive;
class RenderEngine;

class SLIB_EXPORT RenderProgramInfo : public Referable
{
public:
	sl_uint32 program_GL;
	void* effect_D3D9;
};

class SLIB_EXPORT RenderProgram : public RenderBaseObject
{
	SLIB_DECLARE_OBJECT

public:
	virtual Ref<RenderProgramInfo> create(RenderEngine* engine) = 0;
	
	virtual sl_bool onInit(RenderEngine* engine, RenderProgramInfo* info);

	virtual sl_bool onBeginProgram(RenderEngine* engine, RenderProgramInfo* info);
	
	virtual void onEndProgram(RenderEngine* engine, RenderProgramInfo* info);
	
	virtual sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	virtual void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);

	virtual String getGLSLVertexShader(RenderEngine* engine);
	
	virtual String getGLSLFragmentShader(RenderEngine* engine);

};


#define SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, location, VertexData, member) \
	engine->setVertexFloatArrayAttribute(location, (sl_uint32)(sl_size)(&(((VertexData*)0)->member)), sizeof(((VertexData*)0)->member) / sizeof(float), sizeof(VertexData));

// Base class for 2D RenderPrograms
class SLIB_EXPORT RenderProgram2D : public RenderProgram
{
public:
	RenderProgram2D();
	
public:
	// override
	Ref<RenderProgramInfo> create(RenderEngine* engine);
	
	// override
	sl_bool onInit(RenderEngine* engine, RenderProgramInfo* info);
	
	// override
	sl_bool onBeginProgram(RenderEngine* engine, RenderProgramInfo* info);
	
public:
	Ref<Texture> getTexture(sl_uint32 no);
	
	Ref<Texture> getTexture();
	
	void setTexture(sl_uint32 no, const Ref<Texture>& texture);
	
	void setTexture(const Ref<Texture>& texture);
	
public:
	SLIB_PROPERTY(Matrix3, Transform)
	
	SLIB_PROPERTY(Matrix3, TextureTransform)

	SLIB_PROPERTY(Color, Color)
	
	SLIB_PROPERTY(float, Alpha)

public:
	class Info_GL : public RenderProgramInfo
	{
	public:
		sl_int32 uniformTransform;	// Matrix3

		sl_int32 uniformTextures[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];	// samplers
		sl_int32 uniformTransformTexture; // Matrix3
		
		sl_int32 uniformColor;		// Vector3
		sl_int32 uniformAlpha;		// float

		sl_int32 attrPosition;		// Vector2
		sl_int32 attrColor;			// Vector4
		sl_int32 attrTexCoord;		// Vector2
	};

protected:
	SafeRef<Texture> m_textures[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
};

// Base class for 3D RenderPrograms
class SLIB_EXPORT RenderProgram3D : public RenderProgram
{
public:
	RenderProgram3D();

public:
	// override
	Ref<RenderProgramInfo> create(RenderEngine* engine);
	
	// override
	sl_bool onInit(RenderEngine* engine, RenderProgramInfo* info);
	
	// override
	sl_bool onBeginProgram(RenderEngine* engine, RenderProgramInfo* info);

public:
	const Matrix4& getModelMatrix();
	
	void setModelMatrix(const Matrix4& t);
	
	
	const Matrix4& getViewMatrix();
	
	void setViewMatrix(const Matrix4& t);
	
	
	const Matrix4& getProjectionMatrix();
	
	void setProjectionMatrix(const Matrix4& t, sl_bool flagUpdateTransform = sl_true);
	
	
	const Matrix4& getTransform();
	
	void setTransform(const Matrix4& t);
	
	
	const Matrix4& getModelViewMatrix();
	
	void setModelViewMatrix(const Matrix4& t);
	
	
	const Matrix4& getModelViewMatrixInverseTranspose();
	
	void setModelViewMatrixInverseTranspose(const Matrix4& t);
	
	
	const Matrix4& getViewProjectionMatrix();
	
	void setViewProjectionMatrix(const Matrix4& t);
	
	
	Ref<Texture> getTexture(sl_uint32 no);
	
	Ref<Texture> getTexture();
	
	void setTexture(sl_uint32 no, const Ref<Texture>& texture);
	
	void setTexture(const Ref<Texture>& texture);
	
	float getTextureAlpha(sl_uint32 no);
	
	void setTextureAlpha(sl_uint32 no, float alpha);
	
public:
	SLIB_PROPERTY(Vector3, DirectionalLight)
	
	SLIB_PROPERTY(Color, DiffuseColor)
	
	SLIB_PROPERTY(Color, AmbientColor)
	
	SLIB_PROPERTY(float, Alpha)
	
	SLIB_PROPERTY(Matrix3, TextureTransform)

public:
	class Info_GL : public RenderProgramInfo
	{
	public:
		sl_int32 uniformTransform;				// Matrix4
		sl_int32 uniformMatrixModel;			// Matrix4
		sl_int32 uniformMatrixView;				// Matrix4
		sl_int32 uniformMatrixProjection;		// Matrix4
		sl_int32 uniformMatrixModelView;		// Matrix4
		sl_int32 uniformMatrixModelViewIT;		// Matrix4
		sl_int32 uniformMatrixViewProjection;	// Matrix4

		sl_int32 uniformTextures[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];	// samplers
		sl_int32 uniformTextureAlphas[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];	// samplers
		sl_int32 uniformTransformTexture;		// Matrix3

		sl_int32 uniformDirectionalLight;		// Vector3
		sl_int32 uniformDiffuseColor;			// Vector3
		sl_int32 uniformAmbientColor;			// Vector3
		sl_int32 uniformAlpha;					// float

		sl_int32 attrPosition;	// Vector3
		sl_int32 attrNormal;	// Vector3
		sl_int32 attrColor;		// Vector3
		sl_int32 attrTexCoord;	// Vector2

	};

protected:
	Matrix4 m_matrixModel;
	Matrix4 m_matrixView;
	Matrix4 m_matrixProjection;

	sl_bool m_flagValidMatrixTransform;
	Matrix4 m_matrixTransform;
	sl_bool m_flagValidMatrixModelView;
	Matrix4 m_matrixModelView;
	sl_bool m_flagValidMatrixModelViewIT;
	Matrix4 m_matrixModelViewIT;
	sl_bool m_flagValidMatrixViewProjection;
	Matrix4 m_matrixViewProjection;

protected:
	SafeRef<Texture> m_textures[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];
	float m_textureAlphas[SLIB_RENDER_MAX_TEXTURE_SAMPLERS];
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
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
	sl_bool onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
	// override
	void onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive);
	
};

SLIB_RENDER_NAMESPACE_END

#endif

