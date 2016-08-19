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
	while (entry) {
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
		return minEntry->getImage();
	}
	sl_uint32 maxSize = 0;
	_ImageResourceEntry* maxEntry = sl_null;
	entry = entries;
	while (entry) {
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
		return maxEntry->getImage();
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
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
	{
		Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, getDrawableWidth(), getDrawableHeight()));
		sl_int32 width = (sl_int32)(rectDstWhole.getWidth());
		sl_int32 height = (sl_int32)(rectDstWhole.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = _ImageResource_getImage(m_entries, width, height);
			if (image.isNotNull()) {
				canvas->draw(rectDst, image, rectSrc);
			}
		}
	}

	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst)
	{
		sl_int32 width = (sl_int32)(rectDst.getWidth());
		sl_int32 height = (sl_int32)(rectDst.getHeight());
		if (width > 0 && height > 0) {
			Ref<Image> image = _ImageResource_getImage(m_entries, width, height);
			if (image.isNotNull()) {
				canvas->draw(rectDst, image);
			}
		}
	}
	
};

SLIB_DEFINE_OBJECT(_ImageResource_Drawable, Drawable)

Ref<Drawable> _ImageResource_get(_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height)
{
	if (entries->flagValid) {
		if (!(entries[1].flagValid)) {
			if (width == entries->width && height == entries->height) {
				return entries->getImage();
			} else {
				return Drawable::createScaledDrawable(entries->getImage(), (sl_real)width, (sl_real)height);
			}
		}
		return new _ImageResource_Drawable(entries, width, height);
	}
	return Ref<Drawable>::null();
}

SLIB_GRAPHICS_NAMESPACE_END
