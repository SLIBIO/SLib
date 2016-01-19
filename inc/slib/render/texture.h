#ifndef CHECKHEADER_SLIB_RENDER_TEXTURE
#define CHECKHEADER_SLIB_RENDER_TEXTURE

#include "definition.h"

#include "base.h"

#include "../graphics/bitmap.h"

SLIB_RENDER_NAMESPACE_BEGIN
    
enum TextureFilterMode
{
    textureFilterMode_Point,
    textureFilterMode_Linear
};

enum TextureWrapMode
{
    textureWrapMode_Repeat,
    textureWrapMode_Mirror,
    textureWrapMode_Clamp
};

class SLIB_EXPORT Texture : public RenderBaseObject
{
	SLIB_DECLARE_OBJECT(Texture, RenderBaseObject)
protected:
	Texture();

public:
	static Ref<Texture> create(const Ref<Bitmap>& source);
    
	static Ref<Texture> create(const BitmapData& bitmapData);
    
	static Ref<Texture> create(sl_uint32 width, sl_uint32 height, const Color* colors = sl_null, sl_int32 stride = 0);
    
	static Ref<Texture> loadFromMemory(const void* mem, sl_size size);
    
	static Ref<Texture> loadFromMemory(const Memory& mem);
    
	static Ref<Texture> loadFromFile(const String& filePath);
    
	static Ref<Texture> loadFromResource(const String& path);
	
public:
	SLIB_INLINE Ref<Bitmap> getSource() const
	{
		return m_source;
	}

	SLIB_INLINE sl_uint32 getWidth() const
	{
		return m_width;
	}

	SLIB_INLINE sl_uint32 getHeight() const
	{
		return m_height;
	}
	
public:
	void update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
    
	void update();

	void freeSource();
    
	sl_bool setSource(const Ref<Bitmap>& source);

public:
	SLIB_PROPERTY_INLINE(TextureFilterMode, MinFilter)
	SLIB_PROPERTY_INLINE(TextureFilterMode, MagFilter)
	SLIB_PROPERTY_INLINE(TextureWrapMode, WrapX)
	SLIB_PROPERTY_INLINE(TextureWrapMode, WrapY)
	SLIB_BOOLEAN_PROPERTY_INLINE(FreeSourceOnUpdate)

protected:
	SafeRef<Bitmap> m_source;
	sl_uint32 m_width;
	sl_uint32 m_height;
};

SLIB_RENDER_NAMESPACE_END

#endif