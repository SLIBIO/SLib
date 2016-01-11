#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/resource.h"

#include "image_soil2.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
Image::Image()
{
}

Ref<Image> Image::createStatic(const ImageDesc& desc)
{
	return createStatic(desc.width, desc.height, desc.colors, desc.stride, desc.ref.get());
}

Ref<Image> Image::createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	return createStatic(width, height, pixels, stride, sl_null);
}

Ref<Image> Image::createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride, const Referable* ref)
{
	Ref<Image> ret;
	if (width == 0 || height == 0 || pixels == sl_null) {
		return ret;
	}
	if (stride == 0) {
		stride = width;
	}
	ret = new Image;
	if (ret.isNotNull()) {
		ret->m_desc.colors = (Color*)pixels;
		ret->m_desc.width = width;
		ret->m_desc.height = height;
		ret->m_desc.stride = stride;
		ret->m_desc.ref = ref;
	}
	return ret;
}

Ref<Image> Image::create(const ImageDesc& desc)
{
	return create(desc.width, desc.height, desc.colors, desc.stride);
}

Ref<Image> Image::create(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	if (pixels) {
		BitmapData bitmapData(width, height, pixels, stride);
		return create(bitmapData);
	} else {
		Ref<Image> ret;
		sl_uint32 size = (width*height) << 2;
		Memory mem = Memory::create(size);
		if (mem.isNotEmpty()) {
			ret = new Image;
			if (ret.isNotNull()) {
				ret->m_desc.width = width;
				ret->m_desc.height = height;
				ret->m_desc.stride = width;
				ret->m_desc.ref = mem.getObject();
				ret->m_desc.colors = (Color*)(mem.getBuf());
				Base::zeroMemory(ret->m_desc.colors, size);
			}
		}
		return ret;

	}
}

Ref<Image> Image::create(const BitmapData& bitmapData)
{
	Ref<Image> ret;
	sl_uint32 width = bitmapData.width;
	sl_uint32 height = bitmapData.height;
	if (width == 0 || height == 0) {
		return ret;
	}
	sl_uint32 size = (width*height) << 2;
	Memory mem = Memory::create(size);
	if (mem.isNotEmpty()) {
		ret = new Image;
		if (ret.isNotNull()) {
			ret->m_desc.width = width;
			ret->m_desc.height = height;
			ret->m_desc.stride = width;
			ret->m_desc.ref = mem.getObject();
			ret->m_desc.colors = (Color*)(mem.getBuf());
			BitmapData dst(width, height, (Color*)(mem.getBuf()));
			if (bitmapData.format.isPrecomputedAlpha()) {
				dst.format = dst.format.getPrecomputedAlphaFormat();
			}
			dst.copyPixelsFrom(bitmapData);
		}
	}
	return ret;
}

Ref<Image> Image::createFromBitmap(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	Ref<Image> ret;
	if (bitmap.isNull()) {
		return ret;
	}
	sl_uint32 bw = bitmap->getWidth();
	sl_uint32 bh = bitmap->getHeight();
	if (x >= bw) {
		return ret;
	}
	if (y >= bh) {
		return ret;
	}
	if (width > bw - x) {
		return ret;
	}
	if (height > bh - y) {
		return ret;
	}
	Memory mem = Memory::create((width*height) << 2);
	if (mem.isNotEmpty()) {
		Color* buf = (Color*)(mem.getBuf());
		if (bitmap->readPixels(x, y, width, height, buf)) {
			ImageDesc desc;
			desc.width = width;
			desc.height = height;
			desc.stride = width;
			desc.colors = buf;
			desc.ref = mem.getObject();
			return Image::createStatic(desc);
		}
	}
	return ret;
}

Ref<Image> Image::createFromBitmap(const Ref<Bitmap>& bitmap)
{
	Ref<Image> ret;
	if (bitmap.isNull()) {
		return ret;
	}
	sl_uint32 width = bitmap->getWidth();
	sl_uint32 height = bitmap->getHeight();
	Memory mem = Memory::create((width*height) << 2);
	if (mem.isNotEmpty()) {
		Color* buf = (Color*)(mem.getBuf());
		if (bitmap->readPixels(0, 0, width, height, buf)) {
			ImageDesc desc;
			desc.width = width;
			desc.height = height;
			desc.stride = width;
			desc.colors = buf;
			desc.ref = mem.getObject();
			return Image::createStatic(desc);
		}
	}
	return ret;
}


sl_uint32 Image::getBitmapWidth()
{
	return m_desc.width;
}

sl_uint32 Image::getBitmapHeight()
{
	return m_desc.height;
}

sl_bool Image::readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bmd)
{
	ImageDesc& imd = m_desc;
	if (x >= imd.width || y >= imd.height) {
		return sl_false;
	}
	if (x + bmd.width > imd.width || y + bmd.height > imd.height) {
		return sl_false;
	}
	Color* colors = getColorsAt(x, y);
	BitmapData src(bmd.width, bmd.height, colors, imd.stride);
	bmd.copyPixelsFrom(src);
	return sl_true;
}

sl_bool Image::writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bmd)
{
	ImageDesc& imd = m_desc;
	if (x >= imd.width || y >= imd.height) {
		return sl_false;
	}
	if (x + bmd.width > imd.width || y + bmd.height > imd.height) {
		return sl_false;
	}
	Color* colors = getColorsAt(x, y);
	BitmapData src(bmd.width, bmd.height, colors, imd.stride);
	src.copyPixelsFrom(bmd);
	return sl_true;
}

sl_bool Image::resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color)
{
	ImageDesc& imd = m_desc;
	if (x >= imd.width || y >= imd.height) {
		return sl_false;
	}
	if (x + width > imd.width) {
		width = imd.width - x;
	}
	if (y + height > imd.height) {
		height = imd.height - y;
	}
	Color* colors = getColorsAt(x, y);
	for (sl_uint32 yi = 0; yi < height; yi++) {
		Color* colorsDst = colors;
		for (sl_uint32 xi = 0; xi < width; xi++) {
			colorsDst[xi] = color;
		}
		colors += m_desc.stride;
	}
	return sl_true;
}

sl_bool Image::readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride)
{
	BitmapData bitmapData(width, height, colors, stride);
	return readPixels(x, y, bitmapData);
}

sl_bool Image::writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
{
	BitmapData bitmapData(width, height, colors, stride);
	return writePixels(x, y, bitmapData);
}

sl_bool Image::resetPixels(const Color& color)
{
	return resetPixels(0, 0, getWidth(), getHeight(), color);
}


Ref<Canvas> Image::getCanvas()
{
	return Ref<Canvas>::null();
}

Ref<Image> Image::createSub(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) const
{
	Ref<Image> ret;
	if (x >= m_desc.width) {
		return ret;
	}
	if (y >= m_desc.height) {
		return ret;
	}
	if (width > m_desc.width - x) {
		return ret;
	}
	if (height > m_desc.height - y) {
		return ret;
	}
	ret = new Image;
	if (ret.isNotNull()) {
		ret->m_desc.colors = getColorsAt(x, y);
		ret->m_desc.width = width;
		ret->m_desc.height = height;
		ret->m_desc.stride = m_desc.stride;
		ret->m_desc.ref = m_desc.ref;
	}
	return ret;
}

Ref<Image> Image::loadFromFile(const String& filePath, sl_uint32 width, sl_uint32 height)
{
	Ref<Image> ret;
	Memory mem = File::readAllBytes(filePath);
	if (mem.isNotEmpty()) {
		ret = loadFromMemory(mem, width, height);
	}
	return ret;
}

Ref<Image> Image::loadFromResource(const String& path, sl_uint32 width, sl_uint32 height)
{
	Ref<Image> ret;
	Memory mem = Resource::readAllBytes(path);
	if (mem.isNotEmpty()) {
		ret = loadFromMemory(mem, width, height);
	}
	return ret;
}

void Image::fillColor(const Color& color)
{
	Color* colorsDst;
	Color* colorsDstLine = m_desc.colors;
	for (sl_uint32 y = 0; y < m_desc.height; y++) {
		colorsDst = colorsDstLine;
		for (sl_uint32 x = 0; x < m_desc.width; x++) {
			colorsDst[x] = color;
		}
		colorsDstLine += m_desc.stride;
	}
}

Ref<Image> Image::scale(sl_uint32 width, sl_uint32 height, StretchMode stretch) const
{
	Ref<Image> ret = Image::create(width, height);
	if (ret.isNotNull()) {
		draw(ret->m_desc, m_desc, blendCopy, stretch);
	}
	return ret;
}

void Image::draw(ImageDesc& dst, const ImageDesc& src, BlendMode blend, StretchMode stretch)
{
	if (src.width == 0 || src.height == 0 || src.stride == 0 || src.colors == sl_null) {
		return;
	}
	if (dst.width == 0 || dst.height == 0 || dst.stride == 0 || dst.colors == sl_null) {
		return;
	}
	// stretch - fast
	if (blend == blendSrcAlpha) {
		Color* colorsDst;
		Color* colorsDstLine = dst.colors;
		for (sl_uint32 y = 0; y < dst.height; y++) {
			colorsDst = colorsDstLine;
			const Color* colorsSrc = src.colors + ((y * src.height) / dst.height) * src.stride;
			sl_uint32 _x = 0;
			for (sl_uint32 x = 0; x < dst.width; x++) {
				colorsDst[x].blend_PA_NPA(colorsSrc[_x / dst.width]);
				_x += src.width;
			}
			colorsDstLine += dst.stride;
		}
	} else {
		Color* colorsDst;
		Color* colorsDstLine = dst.colors;
		for (sl_uint32 y = 0; y < dst.height; y++) {
			colorsDst = colorsDstLine;
			const Color* colorsSrc = src.colors + ((y * src.height) / dst.height) * src.stride;
			sl_uint32 _x = 0;
			for (sl_uint32 x = 0; x < dst.width; x++) {
				colorsDst[x] = colorsSrc[_x / dst.width];
				_x += src.width;
			}
			colorsDstLine += dst.stride;
		}
	}
}

void Image::drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh
	, const Ref<Image>& src, sl_int32 sx, sl_int32 sy, sl_int32 sw, sl_int32 sh
	, BlendMode blend, StretchMode stretch )
{
	if (src.isNull()) {
		return;
	}
	if (dw <= 0 || dh <= 0) {
		return;
	}
	if (sw <= 0 || sh <= 0) {
		return;
	}
	sl_int32 dwx = getWidth();
	sl_int32 dhx = getHeight();
	sl_int32 swx = src->getWidth();
	sl_int32 shx = src->getHeight();
	if (dwx <= 0 || dhx <= 0) {
		return;
	}
	if (swx <= 0 || shx <= 0) {
		return;
	}
	sl_int32 dx2 = dx + dw;
	sl_int32 dy2 = dy + dh;
	sl_int32 sx2 = sx + sw;
	sl_int32 sy2 = sy + sh;
	if (dx < 0) {
		sx -= dx * sw / dw;
		dx = 0;
	}
	if (dy < 0) {
		sy -= dy * sh / dh;
		dy = 0;
	}
	if (dx2 > dwx) {
		sx2 -= (dwx - dx2) * sw / dw;
		dx2 = dwx;
	}
	if (dy2 > dhx) {
		sy2 -= (dhx - dy2) * sh / dh;
		dy2 = dhx;
	}
	if (sx < 0) {
		dx -= sx * dw / sw;
		sx = 0;
	}
	if (sy < 0) {
		dy -= sy * dh / sh;
		sy = 0;
	}
	if (sx2 > swx) {
		dx2 -= (swx - sx2) * dw / sw;
		sx2 = swx;
	}
	if (sy2 > shx) {
		dy2 -= (shx - sy2) * dh / sh;
		sy2 = shx;
	}
	ImageDesc descDst, descSrc;
	descDst.colors = getColorsAt(dx, dy);
	descDst.width = dx2 - dx;
	descDst.height = dy2 - dy;
	descDst.stride = getStride();
	descSrc.colors = src->getColorsAt(sx, sy);
	descSrc.width = sx2 - sx;
	descSrc.height = sy2 - sy;
	descSrc.stride = src->getStride();
	draw(descDst, descSrc, blend, stretch);
}

void Image::drawImage(const Rectanglei& rectDst
					, const Ref<Image>& src, const Rectanglei& rectSrc
					, BlendMode blend, StretchMode stretch)
{
	drawImage(rectDst.left, rectDst.top, rectDst.getWidth(), rectDst.getHeight()
			  , src, rectSrc.left, rectSrc.top, rectSrc.getWidth(), rectSrc.getHeight()
			  , blend, stretch);
}

void Image::drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh
					, const Ref<Image>& src, sl_int32 sx, sl_int32 sy
					, BlendMode blend, StretchMode stretch)
{
	drawImage(dx, dy, dw, dh, src, sx, sy, dw, dh, blend, stretch);
}


ImageFileType Image::getFileType(const void* _mem, sl_size size)
{
	sl_uint8* mem = (sl_uint8*)_mem;
	if (size > 4 && mem[0] == 0xFF && mem[1] == 0xD8) {
		return imageFileTypeJPEG;
	}
	if (size > 0x08 && mem[0] == 0x89 && mem[1] == 0x50 && mem[2] == 0x4E && mem[3] == 0x47 && mem[4] == 0x0D && mem[5] == 0x0A && mem[6] == 0x1A && mem[7] == 0x0A) {
		return imageFileTypePNG;
	}
	if (size > 12 && mem[0] == 'B' && mem[1] == 'M') {
		return imageFileTypeBMP;
	}
	if (size > 4 && mem[0] == 'D' && mem[1] == 'D' && mem[2] == 'S' && mem[3] == ' ') {
		return imageFileTypeDDS;
	}
	return imageFileTypeUnknown;
}

Ref<Image> Image::loadFromMemory(const void* _mem, sl_size size, sl_uint32 width, sl_uint32 height)
{
	Ref<Image> ret;
	sl_uint8* mem = (sl_uint8*)_mem;
	ImageFileType type = getFileType(mem, size);
	do {
#ifdef SLIB_GRAPHICS_IMAGE_SUPPORT_JPEG
		if (type == imageFileTypeJPEG) {
			ret = loadFromJPEG(mem, size);
			break;
		}
#endif
#ifdef SLIB_GRAPHICS_IMAGE_SUPPORT_PNG
		if (type == imageFileTypePNG) {
			ret = loadFromPNG(mem, size);
			break;
		}
#endif
#ifdef SLIB_GRAPHICS_IMAGE_USE_SOIL2
		ret = Image_SOIL2::loadImage(mem, size);
		if (ret.isNotNull()) {
			break;
		}
#endif
	} while (0);
	if (ret.isNotNull()) {
		if (width == 0 || height == 0) {
			return ret;
		}
		if (ret->getWidth() != width && ret->getHeight() != height) {
			ret = ret->scale(width, height);
		}
		return ret;
	}
	return Ref<Image>::null();
}

SLIB_GRAPHICS_NAMESPACE_END

