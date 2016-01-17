#include "../../../inc/slib/render/texture.h"
#include "../../../inc/slib/render/engine.h"

#include "../../../inc/slib/graphics/image.h"

SLIB_RENDER_NAMESPACE_BEGIN
Texture::Texture()
{
	setMinFilter(textureFilterMode_Linear);
	setMagFilter(textureFilterMode_Linear);
	setWrapX(textureWrapMode_Clamp);
	setWrapY(textureWrapMode_Clamp);
	setFreeSourceOnUpdate(sl_false);
}

Ref<Texture> Texture::create(const Ref<Bitmap>& source)
{
	Ref<Texture> ret;
	if (source.isNotNull()) {
		sl_uint32 width = source->getWidth();
		sl_uint32 height = source->getHeight();
		if (width > 0 && height > 0) {
			ret = new Texture;
			if (ret.isNotNull()) {
				ret->m_source = source;
				ret->m_width = width;
				ret->m_height = height;
			}
		}
	}
	return ret;
}

Ref<Texture> Texture::create(const BitmapData& bitmapData)
{
	return create(Image::create(bitmapData));
}

Ref<Texture> Texture::create(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	return create(Image::create(width, height, pixels, stride));
}

Ref<Texture> Texture::loadFromMemory(const void* mem, sl_size size)
{
	if (size == 0) {
		return Ref<Texture>::null();
	}
	return create(Image::loadFromMemory(mem, size));
}

Ref<Texture> Texture::loadFromMemory(const Memory& mem)
{
	if (mem.isEmpty()) {
		return Ref<Texture>::null();
	}
	return loadFromMemory(mem.getBuf(), mem.getSize());
}

Ref<Texture> Texture::loadFromFile(const String& filePath)
{
	return create(Image::loadFromFile(filePath));
}

Ref<Texture> Texture::loadFromResource(const String& path)
{
	return create(Image::loadFromResource(path));
}

void Texture::freeSource()
{
	m_source.setNull();
}

sl_bool Texture::setSource(const Ref<Bitmap>& source)
{
	if (source.isNull()) {
		return sl_false;
	}
	if (source->getWidth() < m_width || source->getHeight() < m_height) {
		return sl_false;
	}
	m_source = source;
	update();
	return sl_true;
}

void Texture::update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	Ref<RenderEngine> engine = getEngine();
	if (engine.isNotNull()) {
		engine->_onUpdateTexture(this, x, y, width, height);
	}
}

void Texture::update()
{
	update(0, 0, m_width, m_height);
}

SLIB_RENDER_NAMESPACE_END
