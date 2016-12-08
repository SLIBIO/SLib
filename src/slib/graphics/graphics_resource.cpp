#include "../../../inc/slib/graphics/resource.h"

#include "../../../inc/slib/graphics/util.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

Ref<Image> _ImageResourceEntry::getImage()
{
	Ref<Image>& s = *((Ref<Image>*)((void*)&image));
	if (flag_load) {
		return s;
	}
	SpinLocker locker((SpinLock*)((void*)&lock));
	if (flag_load) {
		return s;
	}
	s = Image::loadFromMemory(source_bytes, source_size);
	flag_load = sl_true;
	return s;
}

Ref<Image> _ImageResourceEntry::getMatchingImage(sl_uint32 reqWidth, sl_uint32 reqHeight)
{
	if (reqWidth == 0 || reqHeight == 0) {
		return Ref<Image>::null();
	}
	Ref<Image>& s = *((Ref<Image>*)((void*)&image));
	if (flag_load) {
		return s;
	}
	SpinLocker locker((SpinLock*)((void*)&lock));
	if (flag_load) {
		return s;
	}
	if (s.isNotNull()) {
		if (s->getWidth() >= reqWidth && s->getHeight() >= reqHeight) {
			return s;
		}
	}
	sl_uint32 zoomLevel = 1;
	sl_uint32 widthZoom = width;
	sl_uint32 heightZoom = height;
	while (widthZoom >= reqWidth && heightZoom >= reqHeight) {
		widthZoom >>= 1;
		heightZoom >>= 1;
		zoomLevel <<= 1;
	}
	if (zoomLevel <= 2) {
		s = Image::loadFromMemory(source_bytes, source_size);
		flag_load = sl_true;
	} else {
		Ref<Image> t = Image::loadFromMemory(source_bytes, source_size);
		if (t.isNotNull()) {
			zoomLevel >>= 1;
			s = t->scale(width / zoomLevel, height / zoomLevel);
		}
	}
	return s;
}

_ImageResourceFreeStatic::_ImageResourceFreeStatic(_ImageResourceEntry* entries)
{
	m_entries = entries;
}

_ImageResourceFreeStatic::~_ImageResourceFreeStatic()
{
	_ImageResourceEntry* entry;
	entry = m_entries;
	while (entry->flagValid) {
		(*((Ref<Image>*)((void*)&(entry->image)))).Ref<Image>::~Ref();
		entry->flagValid = sl_false;
		entry++;
	}
}

Ref<Image> _ImageResource_getImage(_ImageResourceEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight)
{
	_ImageResourceEntry* entry;
	if (requiredWidth == 0 && requiredHeight == 0) {
		entry = entries;
		while (entry->flagValid) {
			return entry->getImage();
		}
		return Ref<Image>::null();
	}
	
	sl_uint32 minSize = 0;
	_ImageResourceEntry* minEntry = sl_null;
	entry = entries;
	while (entry->flagValid) {
		sl_uint32 width = entry->width;
		sl_uint32 height = entry->height;
		if (width >= requiredWidth && height >= requiredHeight) {
			sl_uint32 size = width * height;
			if (minSize == 0 || size < minSize) {
				minSize = size;
				minEntry = entry;
			}
		}
		entry++;
	}
	if (minEntry) {
		return minEntry->getMatchingImage(requiredWidth, requiredHeight);
	}
	sl_uint32 maxSize = 0;
	_ImageResourceEntry* maxEntry = sl_null;
	entry = entries;
	while (entry->flagValid) {
		sl_uint32 width = entry->width;
		sl_uint32 height = entry->height;
		if (width < requiredWidth || height < requiredHeight) {
			sl_uint32 size = width * height;
			if (maxSize == 0 || size > maxSize) {
				maxSize = size;
				maxEntry = entry;
			}
		}
		entry++;
	}
	if (maxEntry) {
		return maxEntry->getMatchingImage(requiredWidth, requiredHeight);
	}
	return Ref<Image>::null();
}

List< Ref<Image> > _ImageResource_getImages(_ImageResourceEntry* entries)
{
	List< Ref<Image> > ret;
	_ImageResourceEntry* entry;
	entry = entries;
	while (entry->flagValid) {
		Ref<Image> image = entry->getImage();
		if (image.isNotNull()) {
			ret.add_NoLock(image);
		}
		entry++;
	}
	return ret;
}


class _ImageResource_Drawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
private:
	_ImageResourceEntry* m_entries;
	sl_uint32 m_width;
	sl_uint32 m_height;

public:
	_ImageResource_Drawable(_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height)
	{
		m_entries = entries;
		m_width = width;
		m_height = height;
	}
	
public:
	// override
	sl_real getDrawableWidth()
	{
		return (sl_real)m_width;
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return (sl_real)m_height;
	}
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, m_width, m_height));
		sl_int32 width = (sl_int32)(rectDstWhole.getWidth());
		sl_int32 height = (sl_int32)(rectDstWhole.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = _ImageResource_getImage(m_entries, width, height);
			if (image.isNotNull()) {
				float fx = (float)(image->getWidth()) / (float)(m_width);
				float fy = (float)(image->getHeight()) / (float)(m_height);
				Rectangle r;
				r.left = rectSrc.left * fx;
				r.top = rectSrc.top * fy;
				r.right = rectSrc.right * fx;
				r.bottom = rectSrc.bottom * fy;
				canvas->draw(rectDst, image, r, param);
			}
		}
	}

	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		sl_int32 width = (sl_int32)(rectDst.getWidth());
		sl_int32 height = (sl_int32)(rectDst.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = _ImageResource_getImage(m_entries, width, height);
			if (image.isNotNull()) {
				canvas->draw(rectDst, image, param);
			}
		}
	}
	
};

SLIB_DEFINE_OBJECT(_ImageResource_Drawable, Drawable)


class _ImageResource_SimpleDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
private:
	_ImageResourceEntry* m_entry;
	sl_uint32 m_width;
	sl_uint32 m_height;
	
public:
	_ImageResource_SimpleDrawable(_ImageResourceEntry* entry, sl_uint32 width, sl_uint32 height)
	{
		m_entry = entry;
		m_width = width;
		m_height = height;
	}
	
public:
	// override
	sl_real getDrawableWidth()
	{
		return (sl_real)m_width;
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return (sl_real)m_height;
	}
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, m_width, m_height));
		sl_int32 width = (sl_int32)(rectDstWhole.getWidth());
		sl_int32 height = (sl_int32)(rectDstWhole.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = m_entry->getMatchingImage(width, height);
			if (image.isNotNull()) {
				float fx = (float)(image->getWidth()) / (float)(m_width);
				float fy = (float)(image->getHeight()) / (float)(m_height);
				Rectangle r;
				r.left = rectSrc.left * fx;
				r.top = rectSrc.top * fy;
				r.right = rectSrc.right * fx;
				r.bottom = rectSrc.bottom * fy;
				canvas->draw(rectDst, image, r, param);
			}
		}
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		sl_int32 width = (sl_int32)(rectDst.getWidth());
		sl_int32 height = (sl_int32)(rectDst.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = m_entry->getMatchingImage(width, height);
			if (image.isNotNull()) {
				canvas->draw(rectDst, image, param);
			}
		}
	}
	
};

SLIB_DEFINE_OBJECT(_ImageResource_SimpleDrawable, Drawable)


Ref<Drawable> _ImageResource_get(_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height)
{
	if (entries->flagValid) {
		if (!(entries[1].flagValid)) {
			return new _ImageResource_SimpleDrawable(entries, width, height);
		} else {
			return new _ImageResource_Drawable(entries, width, height);
		}
	}
	return Ref<Drawable>::null();
}

SLIB_GRAPHICS_NAMESPACE_END
