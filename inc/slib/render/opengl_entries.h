/*
This file is only for internal use in the engine.
Don't include this file directly from your own source
*/

SLIB_RENDER_NAMESPACE_BEGIN

class GLRenderEngine;

class SLIB_EXPORT _SLIB_RENDER_ENTRIES_CLASS
{
public:
	static Ref<GLRenderEngine> createEngine();
	
public:
	static void setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
	
	
	static void clear(const RenderClearParam& param);
	
	SLIB_INLINE void clearColor(const Color& color)
	{
		RenderClearParam param;
		param.flagColor = sl_true;
		param.color = color;
		param.flagDepth = sl_false;
		clear(param);
	}
	
	SLIB_INLINE void clearColorDepth(const Color& color, float depth = 1.0f)
	{
		RenderClearParam param;
		param.flagColor = sl_true;
		param.color = color;
		param.flagDepth = sl_true;
		param.depth = depth;
		clear(param);
	}
	
	SLIB_INLINE void clearDepth(float depth = 1.0f)
	{
		RenderClearParam param;
		param.flagColor = sl_false;
		param.flagDepth = sl_true;
		param.depth = depth;
		clear(param);
	}
	
	
	static void setDepthTest(sl_bool flagEnableDepthTest);
	
	static void setDepthWriteEnabled(sl_bool flagEnableDepthWrite);
	
	
	static void setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW = sl_true);
	
	
	static void setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param);
	
	SLIB_INLINE static void setBlending(sl_bool flagEnableBlending)
	{
		RenderBlendingParam param;
		setBlending(flagEnableBlending, param);
	}

	/* objects are represented as unsigned integers (zero means null object) */

	// draw
	static void drawTriangles(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawTriangles(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawTriangles(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawTriangleStrip(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawTriangleStrip(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawTriangleStrip(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawTriangleFan(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawTriangleFan(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawTriangleFan(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawLines(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawLines(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawLines(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawLineStrip(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawLineStrip(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawLineStrip(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawLineLoop(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawLineLoop(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawLineLoop(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);
	
	
	static void drawPoints(sl_uint32 countVertices, sl_uint32 startIndex = 0);
	
	static void drawPoints(sl_uint32 countIndices, const sl_uint16* indices);
	
	static void drawPoints(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes);

	
	static void setLineWidth(float width);
	
	static void flush();

	// textures
	static void setActiveSampler(sl_uint32 samplerNo);
	
	static void bindTexture2D(sl_uint32 texture);
	
	static void unbindTexture2D();
	
	static sl_uint32 createTexture2D(const BitmapData& bitmapData);
	
	static sl_uint32 createTexture2D(sl_uint32 width, sl_uint32 height, const Color* pixels = sl_null, sl_int32 stride = 0);
	
	static sl_uint32 createTexture2D(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
	
	static sl_uint32 createTexture2D(const Ref<Bitmap>& bitmap);
	
	static sl_uint32 createTexture2DFromMemory(const void* mem, sl_size size);
	
	static sl_uint32 createTexture2DFromFile(const String& filePath);
	
	static sl_uint32 createTexture2DFromResource(const String& path);
	
	static void updateTexture2D(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData);
	
	static void updateTexture2D(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride = 0);
	
	static void updateTexture2D(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Ref<Bitmap>& bitmap, sl_uint32 bitmapX = 0, sl_uint32 bitmapY = 0);
	
	static void setTexture2DFilterMode(TextureFilterMode minFilter, TextureFilterMode magFilter);
	
	static void setTexture2DWrapMode(TextureWrapMode wrapX, TextureWrapMode wrapY);
	
	static void deleteTexture(sl_uint32 texture);

	// shader
	static sl_uint32 createVertexShader(const String& source);
	
	static sl_uint32 createFragmentShader(const String& source);
	
	static void deleteShader(sl_uint32 shader);

	// program
	static sl_uint32 createProgram();
	
	static void attachShader(sl_uint32 program, sl_uint32 shader);
	
	static sl_bool linkProgram(sl_uint32 program);
	
	static sl_uint32 createProgram(sl_uint32 vertexShader, sl_uint32 fragmentShader); // also links
	
	static void useProgram(sl_uint32 program);
	
	static void deleteProgram(sl_uint32 program);

	// buffers
	static sl_uint32 createVertexBuffer(sl_bool flagStatic, sl_size size, const void* data = sl_null);
	
	static void updateVertexBuffer(sl_uint32 buffer, sl_size offset, const void* data, sl_size size);
	
	static void bindVertexBuffer(sl_uint32 buffer);
	
	static void unbindVertexBuffer();
	
	static sl_uint32 createIndexBuffer(sl_bool flagStatic, sl_size size, const void* data = sl_null);
	
	static void updateIndexBuffer(sl_uint32 buffer, sl_size offset, const void* data, sl_size size);
	
	static void deleteBuffer(sl_uint32 buffer);

	/*
		vertex array attribute

			flagDoNormalize is used for fixed data - convert to range [-1, 1] for signed, [0, 1] for unsigned
	*/
	// returns -1 on fail
	static sl_int32 getAttributeLocation(sl_uint32 program, const char* name);
	
	static void setVertexFloatArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexFloatArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexInt8ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexInt8ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexUint8ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexUint8ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexInt16ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexInt16ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexUint16ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void setVertexUint16ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes = 0, sl_bool flagDoNormalize = sl_false);
	
	static void disableVertexArrayAttribute(sl_int32 attributeLocation);

	// uniform
	// returns -1 on fail
	static sl_int32 getUniformLocation(sl_uint32 program, const char* name);
	
	static void setUniformFloatValue(sl_int32 uniformLocation, float value);
	
	static void setUniformFloatArray(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformIntValue(sl_int32 uniformLocation, sl_int32 value);
	
	static void setUniformIntArray(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformFloat2Value(sl_int32 uniformLocation, float v1, float v2);
	
	static void setUniformFloat2Value(sl_int32 uniformLocation, const Vector2& v);
	
	static void setUniformFloat2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformInt2Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2);
	
	static void setUniformInt2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformFloat3Value(sl_int32 uniformLocation, float v1, float v2, float v3);
	
	static void setUniformFloat3Value(sl_int32 uniformLocation, const Vector3& v);
	
	static void setUniformFloat3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformInt3Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3);
	
	static void setUniformInt3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformFloat4Value(sl_int32 uniformLocation, float v1, float v2, float v3, float v4);
	
	static void setUniformFloat4Value(sl_int32 uniformLocation, const Vector4& v);
	
	static void setUniformFloat4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformInt4Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3, sl_int32 v4);
	
	static void setUniformInt4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformMatrix2Value(sl_int32 uniformLocation, const Matrix2& value);
	
	static void setUniformMatrix2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformMatrix3Value(sl_int32 uniformLocation, const Matrix3& value);
	
	static void setUniformMatrix3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformMatrix4Value(sl_int32 uniformLocation, const Matrix4& value);
	
	static void setUniformMatrix4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count);
	
	static void setUniformTextureSampler(sl_int32 uniformLocation, sl_uint32 samplerNo);

public:
	static sl_bool isAvailable();
	
	static void loadEntries(const String& pathDll, sl_bool flagReload = sl_true);
	
	static void loadEntries(sl_bool flagReload = sl_false);

};
SLIB_RENDER_NAMESPACE_END
