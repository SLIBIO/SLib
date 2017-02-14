#include "../../../inc/slib/graphics/freetype.h"

#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/file.h"

#include "../../../inc/thirdparty/freetype/include/ft2build.h"
#include "../../../inc/thirdparty/freetype/include/freetype.h"
#include "../../../inc/thirdparty/freetype/include/ftstroke.h"

namespace slib
{

	class _FreeTypeLibrary : public Referable
	{
	public:
		FT_Library m_library;

	public:
		_FreeTypeLibrary()
		{
			FT_Error err = FT_Init_FreeType(&m_library);
			if (err || !m_library) {
				LogError("FreeType", "Failed to initialize FreeType");
			}
		}

		~_FreeTypeLibrary()
		{
			freeLibrary();
		}

	public:
		void freeLibrary()
		{
			FT_Library library = m_library;
			if (library) {
				m_library = sl_null;
				FT_Done_FreeType(library);
			}
		}

	};


	SLIB_DEFINE_OBJECT(FreeType, Object)

	FreeType::FreeType()
	{
		m_face = sl_null;
	}

	FreeType::~FreeType()
	{
		FT_Done_Face(m_face);
	}

	Ref<FreeType> FreeType::loadFromMemory(const Memory& mem, sl_uint32 index)
	{
		if (mem.isNotEmpty()) {
			Ref<_FreeTypeLibrary> libraryRef = new _FreeTypeLibrary;
			if (libraryRef.isNotNull()) {
				FT_Library library = libraryRef->m_library;
				if (library) {
					FT_Error err;
					FT_Face face;
					err = FT_New_Memory_Face(library, (FT_Byte*)(mem.getData()), (FT_Long)(mem.getSize()), index, &face);
					if (err == 0 && face) {
						Ref<FreeType> ret = new FreeType;
						if (ret.isNotNull()) {
							ret->m_libraryRef = libraryRef;
							ret->m_library = library;
							ret->m_face = face;
							ret->m_mem = mem;
							return ret;
						}
						FT_Done_Face(face);
					}
				}
			}
		}
		return sl_null;
	}

	Ref<FreeType> FreeType::loadFromFile(const String& fontFilePath, sl_uint32 index)
	{
		Memory mem = File::readAllBytes(fontFilePath);
		return loadFromMemory(mem, index);
	}

	sl_uint32 FreeType::getFacesCount()
	{
		return (sl_uint32)(m_face->num_faces);
	}

	sl_uint32 FreeType::getGlyphsCount()
	{
		return (sl_uint32)(m_face->num_glyphs);
	}

	sl_bool FreeType::setSize(sl_uint32 width, sl_uint32 height)
	{
		ObjectLocker lock(this);
		FT_Error err;
		err = FT_Set_Pixel_Sizes(m_face, (FT_UInt)width, (FT_UInt)height);
		if (err == 0) {
			return sl_true;
		}
		return sl_false;
	}

	void FreeType::setSize(sl_uint32 size)
	{
		setSize(size, size);
	}

	Size FreeType::getStringExtent(const sl_char16* sz, sl_uint32 len)
	{
		ObjectLocker lock(this);
		Size ret;
		if (len == 0) {
			ret.x = 0;
			ret.y = 0;
			return ret;
		}

		const sl_char16* chars = sz;
		sl_uint32 nChars = len;
		FT_Error err;
		sl_uint32 sizeX = 0;
		sl_uint32 sizeY = 0;
		for (sl_uint32 iChar = 0; iChar < nChars; iChar++) {
			sl_char16 ch = chars[iChar];
			FT_UInt glyph_index = FT_Get_Char_Index(m_face, ch);
			if (glyph_index > 0) {
				err = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
				if (err == 0) {
					sl_uint32 dx = (sl_uint32)(m_face->glyph->advance.x) >> 6;
					sl_uint32 dy = (sl_uint32)(m_face->glyph->metrics.height) >> 6;
					sizeX += dx;
					if (sizeY < dy) {
						sizeY = dy;
					}
				}
			}
		}
		ret.x = (sl_real)sizeX;
		ret.y = (sl_real)sizeY;
		return ret;
	}

	Size FreeType::getStringExtent(const String16& text)
	{
		return getStringExtent(text.getData(), (sl_uint32)(text.getLength()));
	}

	void FreeType::drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color)
	{
		ObjectLocker lock(this);
		if (len == 0) {
			return;
		}
		if (imageOutput.isNull()) {
			return;
		}
		
		sl_int32 widthImage = imageOutput->getWidth();
		sl_int32 heightImage = imageOutput->getHeight();
		if (widthImage <= 0 || heightImage <= 0) {
			return;
		}

		const sl_char16* chars = sz;
		sl_uint32 nChars = len;

		FT_Error err;
		FT_GlyphSlot slot = m_face->glyph;
		for (sl_uint32 iChar = 0; iChar < nChars; iChar++) {
			sl_char16 ch = chars[iChar];
			err = FT_Load_Char(m_face, ch, FT_LOAD_RENDER);
			if (err == 0) {
				sl_int32 dx = x + slot->bitmap_left;
				sl_int32 dy = y - slot->bitmap_top;
				sl_int32 widthChar = slot->bitmap.width;
				sl_int32 heightChar = slot->bitmap.rows;
				sl_int32 pitchChar = slot->bitmap.pitch;
				if (slot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY && slot->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) {
					return;
				}
				if (widthChar > 0 && heightChar > 0 && dx < widthImage && dy < heightImage && dx > -widthChar && dy > -heightChar) {
					int sx = 0;
					int sy = 0;
					if (dx < 0) {
						sx -= dx;
						widthChar += dx;
						dx = 0;
					}
					if (dy < 0) {
						sy -= dy;
						heightChar += dy;
						dy = 0;
					}
					if (dx + widthChar > widthImage) {
						widthChar = widthImage - dx;
					}
					if (dy + heightChar > heightImage) {
						heightChar = heightImage - dy;
					}
					
					sl_uint8* bitmapChar = (sl_uint8*)(slot->bitmap.buffer) + (sy * pitchChar + sx);
					Color* colorsOutput = imageOutput->getColorsAt(dx, dy);
					sl_uint32 strideImage = imageOutput->getStride();

					sl_uint32 rs = color.r;
					sl_uint32 gs = color.g;
					sl_uint32 bs = color.b;
					sl_uint32 as = color.a;

					if (slot->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
						for (int y = 0; y < heightChar; y++) {
							sl_uint8* ps = bitmapChar;
							Color* pd = colorsOutput;
							for (int x = 0; x < widthChar; x++) {
								sl_uint32 _as = (as * (sl_uint32)(*ps)) / 255;
								if (_as == 255) {
									*pd = Color(rs, gs, bs);
								} else if (_as > 0) {
									pd->blend_NPA_NPA(rs, gs, bs, _as);
								}
								pd++;
								ps++;
							}
							bitmapChar += pitchChar;
							colorsOutput += strideImage;
						}
					} else {
						for (int y = 0; y < heightChar; y++) {
							sl_uint8* ps = bitmapChar;
							Color* pd = colorsOutput;
							for (int x = 0; x < widthChar; x++) {
								sl_uint8 B = (ps[x >> 3] >> (7 - (x & 7))) & 1;
								if (B) {
									*pd = Color(rs, gs, bs);
								}
								pd++;
							}
							bitmapChar += pitchChar;
							colorsOutput += strideImage;
						}
					}

				}

				x += (slot->advance.x >> 6);
				y += (slot->advance.y >> 6);
			}
		}
	}

	void FreeType::drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color)
	{
		return drawString(imageOutput, x, y, text.getData(), (sl_uint32)(text.getLength()), color);
	}

	void FreeType::strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth)
	{
		_strokeString(imageOutput, x, y, sz, len, sl_false, sl_false, lineWidth, color);
	}

	void FreeType::strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeString(imageOutput, x, y, text.getData(), (sl_uint32)(text.getLength()), color, lineWidth);
	}

	void FreeType::strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth)
	{
		_strokeString(imageOutput, x, y, sz, len, sl_true, sl_false, lineWidth, color);
	}

	void FreeType::strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeStringInside(imageOutput, x, y, text.getData(), (sl_uint32)(text.getLength()), color, lineWidth);
	}

	void FreeType::strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth)
	{
		_strokeString(imageOutput, x, y, sz, len, sl_true, sl_true, lineWidth, color);
	}

	void FreeType::strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeString(imageOutput, x, y, text.getData(), (sl_uint32)(text.getLength()), color, lineWidth);
	}

	void FreeType::_strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len
		, sl_bool flagBorder, sl_bool flagOutside, sl_uint32 radius, const Color& color)
	{
		ObjectLocker lock(this);
		if (len == 0) {
			return;
		}
		if (imageOutput.isNull()) {
			return;
		}

		sl_int32 widthImage = imageOutput->getWidth();
		sl_int32 heightImage = imageOutput->getHeight();
		if (widthImage <= 0 || heightImage <= 0) {
			return;
		}

		FT_Stroker stroker = NULL;
		FT_Stroker_New(m_library, &stroker);
		if (stroker == NULL) {
			return;
		}
		FT_Stroker_Set(stroker, radius * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

		const sl_char16* chars = sz;
		sl_uint32 nChars = len;

		FT_Error err;
		for (sl_uint32 iChar = 0; iChar < nChars; iChar++) {
			sl_char16 ch = chars[iChar];
			err = FT_Load_Char(m_face, ch, FT_LOAD_DEFAULT);
			if (err == 0) {
				FT_Glyph glyph = NULL;
				err = FT_Get_Glyph(m_face->glyph, &glyph);
				if (err == 0 && glyph) {
					if (flagBorder) {
						if (flagOutside) {
							err = FT_Glyph_StrokeBorder(&glyph, stroker, sl_false, sl_true);
						} else {
							err = FT_Glyph_StrokeBorder(&glyph, stroker, sl_true, sl_true);
						}
					} else {
						err = FT_Glyph_Stroke(&glyph, stroker, sl_true);
					}
				}
				if (glyph) {
					if (err == 0) {
						err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, sl_true);
						if (err == 0) {
							FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
							sl_int32 dx = x + bitmapGlyph->left;
							sl_int32 dy = y - bitmapGlyph->top;
							sl_int32 widthChar = bitmapGlyph->bitmap.width;
							sl_int32 heightChar = bitmapGlyph->bitmap.rows;
							sl_int32 pitchChar = bitmapGlyph->bitmap.pitch;
							if (bitmapGlyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY && bitmapGlyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) {
								break;
							}
							if (widthChar > 0 && heightChar > 0 && dx < widthImage && dy < heightImage && dx > -widthChar && dy > -heightChar) {
								int sx = 0;
								int sy = 0;
								if (dx < 0) {
									sx -= dx;
									widthChar += dx;
									dx = 0;
								}
								if (dy < 0) {
									sy -= dy;
									heightChar += dy;
									dy = 0;
								}
								if (dx + widthChar > widthImage) {
									widthChar = widthImage - dx;
								}
								if (dy + heightChar > heightImage) {
									heightChar = heightImage - dy;
								}

								sl_uint8* bitmapChar = (sl_uint8*)(bitmapGlyph->bitmap.buffer) + (sy * pitchChar + sx);
								Color* colorsOutput = imageOutput->getColorsAt(dx, dy);
								sl_uint32 strideImage = imageOutput->getStride();

								sl_uint32 rs = color.r;
								sl_uint32 gs = color.g;
								sl_uint32 bs = color.b;
								sl_uint32 as = color.a;

								if (bitmapGlyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
									for (int y = 0; y < heightChar; y++) {
										sl_uint8* ps = bitmapChar;
										Color* pd = colorsOutput;
										for (int x = 0; x < widthChar; x++) {
											sl_uint32 _as = (as * (sl_uint32)(*ps)) / 255;
											if (_as == 255) {
												*pd = Color(rs, gs, bs);
											} else if (_as > 0) {
												pd->blend_NPA_NPA(rs, gs, bs, _as);
											}
											pd++;
											ps++;
										}
										bitmapChar += pitchChar;
										colorsOutput += strideImage;
									}
								} else {
									for (int y = 0; y < heightChar; y++) {
										sl_uint8* ps = bitmapChar;
										Color* pd = colorsOutput;
										for (int x = 0; x < widthChar; x++) {
											sl_uint8 B = (ps[x >> 3] >> (7 - (x & 7))) & 1;
											if (B) {
												*pd = Color(rs, gs, bs);
											}
											pd++;
										}
										bitmapChar += pitchChar;
										colorsOutput += strideImage;
									}
								}

							}
							x += (glyph->advance.x >> 16);
							y += (glyph->advance.y >> 16);
						}
					}
					if (glyph) {
						FT_Done_Glyph(glyph);
					}
				}
			}
		}

		FT_Stroker_Done(stroker);
	}

}
