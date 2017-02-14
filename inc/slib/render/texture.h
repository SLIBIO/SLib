#ifndef CHECKHEADER_SLIB_RENDER_TEXTURE
#define CHECKHEADER_SLIB_RENDER_TEXTURE

#include "definition.h"

#include "base.h"

#include "../graphics/bitmap.h"

namespace slib
{

	enum class TextureFilterMode
	{
		Point,
		Linear
	};
	
	enum class TextureWrapMode
	{
		Repeat,
		Mirror,
		Clamp
	};
	
	class SLIB_EXPORT TextureInstance : public RenderBaseObjectInstance
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextureInstance();
		
		~TextureInstance();
		
	public:
		virtual void notifyUpdated(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
		
	protected:
		Rectanglei m_updatedRegion;
		
	};
	
	class SLIB_EXPORT Texture : public RenderBaseObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Texture();
		
		~Texture();
		
	public:
		static Ref<Texture> create(const Ref<Bitmap>& source);
		
		static Ref<Texture> create(const BitmapData& bitmapData);
		
		static Ref<Texture> create(sl_uint32 width, sl_uint32 height, const Color* colors = sl_null, sl_int32 stride = 0);
		
		static Ref<Texture> loadFromMemory(const void* mem, sl_size size);
		
		static Ref<Texture> loadFromMemory(const Memory& mem);
		
		static Ref<Texture> loadFromFile(const String& filePath);
		
		static Ref<Texture> loadFromAsset(const String& path);
		
		static Ref<Texture> getBitmapRenderingCache(const Ref<Bitmap>& source);
		
	public:
		Ref<Bitmap> getSource();
		
		sl_bool setSource(const Ref<Bitmap>& source);
		
		void freeSource();
		
		sl_uint32 getWidth();
		
		sl_uint32 getHeight();
		
		void update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
		
		void update();
		
		Ref<TextureInstance> getInstance(RenderEngine* engine);
		
	public:
		SLIB_PROPERTY(TextureFilterMode, MinFilter)
		SLIB_PROPERTY(TextureFilterMode, MagFilter)
		SLIB_PROPERTY(TextureWrapMode, WrapX)
		SLIB_PROPERTY(TextureWrapMode, WrapY)
		SLIB_BOOLEAN_PROPERTY(FreeSourceOnUpdate)
		
	protected:
		AtomicRef<Bitmap> m_source;
		AtomicWeakRef<Bitmap> m_sourceWeak;
		sl_uint32 m_width;
		sl_uint32 m_height;
		
	};
	
	class EngineTexture : public Texture
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		EngineTexture();
		
		~EngineTexture();
		
	public:
		Ref<Referable> getLinkedObject();
		
		void setLinkedObject(const Ref<Referable>& object);
		
	protected:
		AtomicRef<Referable> m_linkedObject;
		
	};

}
    
#endif
