#include "../../../inc/slib/graphics/color.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

sl_uint8 Color::_zero[4] = {0, 0, 0, 0};
sl_uint8 Color::_transparent[4] = {255, 255, 255, 0};
sl_uint8 Color::_black[4] = {0, 0, 0, 255};
sl_uint8 Color::_white[4] = {255, 255, 255, 255};
sl_uint8 Color::_gray[4] = {128, 128, 128, 255};
sl_uint8 Color::_red[4] = {0, 0, 255, 255};
sl_uint8 Color::_green[4] = {0, 255, 0, 255};
sl_uint8 Color::_blue[4] = {255, 0, 0, 255};
sl_uint8 Color::_yellow[4] = {0, 255, 255, 255};
sl_uint8 Color::_magenta[4] = {255, 0, 255, 255};
sl_uint8 Color::_cyan[4] = {255, 255, 0, 255};

#define modelPlus_PAtoNPA 0x80000000
#define modelPlus_NPAtoPA 0x90000000

#define model_RGBA Color::RGBA
#define model_RGBA_PAtoNPA (Color::RGBA + modelPlus_PAtoNPA)
#define model_RGBA_NPAtoPA (Color::RGBA + modelPlus_NPAtoPA)
#define model_BGRA Color::BGRA
#define model_BGRA_PAtoNPA (Color::BGRA + modelPlus_PAtoNPA)
#define model_BGRA_NPAtoPA (Color::BGRA + modelPlus_NPAtoPA)
#define model_ARGB Color::ARGB
#define model_ARGB_PAtoNPA (Color::ARGB + modelPlus_PAtoNPA)
#define model_ARGB_NPAtoPA (Color::ARGB + modelPlus_NPAtoPA)
#define model_ABGR Color::ABGR
#define model_ABGR_PAtoNPA (Color::ABGR + modelPlus_PAtoNPA)
#define model_ABGR_NPAtoPA (Color::ABGR + modelPlus_NPAtoPA)
#define model_RGB Color::RGB
#define model_BGR Color::BGR
#define model_RGB565BE Color::RGB565BE
#define model_RGB565LE Color::RGB565LE
#define model_BGR565BE Color::BGR565BE
#define model_BGR565LE Color::BGR565LE
#define model_GRAY8 Color::GRAY8
#define model_ALPHA8 Color::ALPHA8
#define model_YUV444 Color::YUV444

#define _COLOR_SWITCH_MAIN_BODY_SWITCH(_model) \
	case model_##_model: \
		_COLOR_SWITCH_DEFINE(_model); \
		break;

#define _COLOR_SWITCH_MAIN_BODY \
	switch (model) { \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGBA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGBA_PAtoNPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGBA_NPAtoPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGRA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGRA_PAtoNPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGRA_NPAtoPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ARGB) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ARGB_PAtoNPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ARGB_NPAtoPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ABGR) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ABGR_PAtoNPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ABGR_NPAtoPA) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGB) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGR) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGB565BE) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(RGB565LE) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGR565BE) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(BGR565LE) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(GRAY8) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(ALPHA8) \
		_COLOR_SWITCH_MAIN_BODY_SWITCH(YUV444) \
		default: break; \
	}

#define _COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, _modelOut) \
	case model_##_modelOut: \
		_COLOR_SWITCH_DEFINE(_modelIn, _modelOut); \
		break;

#define _COLOR_SWITCH2_MAIN_BODY_SWITCH(_modelIn) \
	case model_##_modelIn: \
		switch (modelOut) { \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGBA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGBA_PAtoNPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGBA_NPAtoPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGRA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGRA_PAtoNPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGRA_NPAtoPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ARGB) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ARGB_PAtoNPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ARGB_NPAtoPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ABGR) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ABGR_PAtoNPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ABGR_NPAtoPA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB565BE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB565LE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR565BE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR565LE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, GRAY8) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ALPHA8) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, YUV444) \
			default: break; \
		} \
		break;

#define _COLOR_SWITCH2_MAIN_BODY32_SWITCH(_modelIn) \
	case model_##_modelIn: \
		switch (modelOut) { \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGBA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGRA) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ARGB) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ABGR) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB565BE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, RGB565LE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR565BE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, BGR565LE) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, GRAY8) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, ALPHA8) \
			_COLOR_SWITCH2_MAIN_BODY_SWITCH2(_modelIn, YUV444) \
			default: break; \
		} \
		break;


#define _COLOR_SWITCH2_MAIN_BODY \
	switch (modelIn) { \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(RGBA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(RGBA_PAtoNPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(RGBA_NPAtoPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(BGRA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(BGRA_PAtoNPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(BGRA_NPAtoPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ARGB) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ARGB_PAtoNPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ARGB_NPAtoPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ABGR) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ABGR_PAtoNPA) \
		_COLOR_SWITCH2_MAIN_BODY32_SWITCH(ABGR_NPAtoPA) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(RGB) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(BGR) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(RGB565BE) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(RGB565LE) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(BGR565BE) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(BGR565LE) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(GRAY8) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(ALPHA8) \
		_COLOR_SWITCH2_MAIN_BODY_SWITCH(YUV444) \
		default: break; \
	}


#define _COLOR_IMPL_OUT(_modelIn) \
	_COLOR_IMPL_DEFINE(_modelIn, RGBA) \
	_COLOR_IMPL_DEFINE(_modelIn, RGBA_PAtoNPA) \
	_COLOR_IMPL_DEFINE(_modelIn, RGBA_NPAtoPA) \
	_COLOR_IMPL_DEFINE(_modelIn, BGRA) \
	_COLOR_IMPL_DEFINE(_modelIn, BGRA_PAtoNPA) \
	_COLOR_IMPL_DEFINE(_modelIn, BGRA_NPAtoPA) \
	_COLOR_IMPL_DEFINE(_modelIn, ARGB) \
	_COLOR_IMPL_DEFINE(_modelIn, ARGB_PAtoNPA) \
	_COLOR_IMPL_DEFINE(_modelIn, ARGB_NPAtoPA) \
	_COLOR_IMPL_DEFINE(_modelIn, ABGR) \
	_COLOR_IMPL_DEFINE(_modelIn, ABGR_PAtoNPA) \
	_COLOR_IMPL_DEFINE(_modelIn, ABGR_NPAtoPA) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB565BE) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB565LE) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR565BE) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR565LE) \
	_COLOR_IMPL_DEFINE(_modelIn, GRAY8) \
	_COLOR_IMPL_DEFINE(_modelIn, ALPHA8) \
	_COLOR_IMPL_DEFINE(_modelIn, YUV444)


#define _COLOR_IMPL_OUT_FOR_32(_modelIn) \
	_COLOR_IMPL_DEFINE(_modelIn, RGBA) \
	_COLOR_IMPL_DEFINE(_modelIn, BGRA) \
	_COLOR_IMPL_DEFINE(_modelIn, ARGB) \
	_COLOR_IMPL_DEFINE(_modelIn, ABGR) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB565BE) \
	_COLOR_IMPL_DEFINE(_modelIn, RGB565LE) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR565BE) \
	_COLOR_IMPL_DEFINE(_modelIn, BGR565LE) \
	_COLOR_IMPL_DEFINE(_modelIn, GRAY8) \
	_COLOR_IMPL_DEFINE(_modelIn, ALPHA8) \
	_COLOR_IMPL_DEFINE(_modelIn, YUV444)

#define _COLOR_IMPL_BODY \
	_COLOR_IMPL_OUT_FOR_32(RGBA) \
	_COLOR_IMPL_OUT_FOR_32(RGBA_PAtoNPA) \
	_COLOR_IMPL_OUT_FOR_32(RGBA_NPAtoPA) \
	_COLOR_IMPL_OUT_FOR_32(BGRA) \
	_COLOR_IMPL_OUT_FOR_32(BGRA_PAtoNPA) \
	_COLOR_IMPL_OUT_FOR_32(BGRA_NPAtoPA) \
	_COLOR_IMPL_OUT_FOR_32(ARGB) \
	_COLOR_IMPL_OUT_FOR_32(ARGB_PAtoNPA) \
	_COLOR_IMPL_OUT_FOR_32(ARGB_NPAtoPA) \
	_COLOR_IMPL_OUT_FOR_32(ABGR) \
	_COLOR_IMPL_OUT_FOR_32(ABGR_PAtoNPA) \
	_COLOR_IMPL_OUT_FOR_32(ABGR_NPAtoPA) \
	_COLOR_IMPL_OUT(RGB) \
	_COLOR_IMPL_OUT(BGR) \
	_COLOR_IMPL_OUT(RGB565BE) \
	_COLOR_IMPL_OUT(RGB565LE) \
	_COLOR_IMPL_OUT(BGR565BE) \
	_COLOR_IMPL_OUT(BGR565LE) \
	_COLOR_IMPL_OUT(GRAY8) \
	_COLOR_IMPL_OUT(ALPHA8) \
	_COLOR_IMPL_OUT(YUV444)


#define _COLOR_SWITCH_DEFINE(model) \
	c.read##model(data);
SLIB_INLINE static void _Color_read(Color& c, sl_uint32 model, const void* data)
{
	_COLOR_SWITCH_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

void Color::read(ColorModel model, const void* data)
{
	_Color_read(*this, model, data);
}

void Color::read_PAtoNPA(ColorModel _model, const void* data)
{
	sl_uint32 model;
	if (Color::is32bitRGBModel(_model)) {
		model = _model + modelPlus_PAtoNPA;
	} else {
		model = _model;
	}
	_Color_read(*this, model, data);
}

void Color::read_NPAtoPA(ColorModel _model, const void* data)
{
	sl_uint32 model;
	if (Color::is32bitRGBModel(_model)) {
		model = _model + modelPlus_NPAtoPA;
	} else {
		model = _model;
	}
	_Color_read(*this, model, data);
}

#define _COLOR_SWITCH_DEFINE(model) \
	c.write##model(data);
SLIB_INLINE static void _Color_write(const Color& c, sl_uint32 model, void* data)
{
	_COLOR_SWITCH_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

void Color::write(ColorModel model, void* data) const
{
	_Color_write(*this, model, data);
}

void Color::write_PAtoNPA(ColorModel _model, void* data) const
{
	sl_uint32 model;
	if (Color::is32bitRGBModel(_model)) {
		model = _model + modelPlus_PAtoNPA;
	} else {
		model = _model;
	}
	_Color_write(*this, model, data);
}

void Color::write_NPAtoPA(ColorModel _model, void* data) const
{
	sl_uint32 model;
	if (Color::is32bitRGBModel(_model)) {
		model = _model + modelPlus_NPAtoPA;
	} else {
		model = _model;
	}
	_Color_write(*this, model, data);
}

#define bytesRGBA 4
#define bytesRGBA_PAtoNPA 4
#define bytesRGBA_NPAtoPA 4
#define bytesBGRA 4
#define bytesBGRA_PAtoNPA 4
#define bytesBGRA_NPAtoPA 4
#define bytesARGB 4
#define bytesARGB_PAtoNPA 4
#define bytesARGB_NPAtoPA 4
#define bytesABGR 4
#define bytesABGR_PAtoNPA 4
#define bytesABGR_NPAtoPA 4
#define bytesRGB 3
#define bytesBGR 3
#define bytesRGB565BE 2
#define bytesRGB565LE 2
#define bytesBGR565BE 2
#define bytesBGR565LE 2
#define bytesGRAY8 1
#define bytesALPHA8 1
#define bytesYUV444 3


#define _COLOR_SWITCH_DEFINE(model) \
	return bytes##model;

static sl_uint32 _Color_getModelBytes(sl_uint32 model)
{
	_COLOR_SWITCH_MAIN_BODY
	return 0;
}
#undef _COLOR_SWITCH_DEFINE

#define _COLOR_IMPL_DEFINE(modelIn, modelOut) \
static void _Color_convertLine__##modelIn##__##modelOut(sl_size count, sl_uint8* dataIn, sl_uint8* dataOut) { \
	Color c = Color::white(); \
	for (sl_size i = 0; i < count; i++) { \
		c.read##modelIn(dataIn); \
		c.write##modelOut(dataOut); \
		dataIn += bytes##modelIn; \
		dataOut += bytes##modelOut; \
	} \
}

_COLOR_IMPL_BODY

#undef _COLOR_IMPL_DEFINE

#define _COLOR_SWITCH_DEFINE(modelIn, modelOut) \
	_Color_convertLine__##modelIn##__##modelOut(count, dataIn, dataOut);

static void _Color_convertLine(sl_size count, sl_uint32 modelIn, const void* in, sl_uint32 modelOut, void* out)
{
	sl_uint8* dataIn = (sl_uint8*)in;
	sl_uint8* dataOut = (sl_uint8*)out;
	if (modelIn == modelOut) {
		if (dataIn != dataOut) {
			sl_size n = _Color_getModelBytes(modelIn) * count;
			Base::copyMemory(dataOut, dataIn, n);
		}
		return;
	}
	_COLOR_SWITCH2_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

void Color::convert(sl_size count, ColorModel modelIn, const void* in, ColorModel modelOut, void* out)
{
	if (count == 0) {
		return;
	}
	_Color_convertLine(count, modelIn, in, modelOut, out);
}

void Color::convertPAtoNPA(sl_size count, ColorModel _modelIn, const void* in, ColorModel _modelOut, void* out)
{
	if (count == 0) {
		return;
	}
	sl_uint32 modelIn, modelOut;
	if (Color::is32bitRGBModel(_modelIn)) {
		modelIn = _modelIn + modelPlus_PAtoNPA;
		modelOut = _modelOut;
	} else {
		modelIn = _modelIn;
		if (Color::is32bitRGBModel(_modelOut)) {
			modelOut = _modelOut + modelPlus_PAtoNPA;
		} else {
			modelOut = _modelOut;
		}
	}
	_Color_convertLine(count, modelIn, in, modelOut, out);
}

void Color::convertNPAtoPA(sl_size count, ColorModel _modelIn, const void* in, ColorModel _modelOut, void* out)
{
	if (count == 0) {
		return;
	}
	sl_uint32 modelIn, modelOut;
	if (Color::is32bitRGBModel(_modelIn)) {
		modelIn = _modelIn + modelPlus_NPAtoPA;
		modelOut = _modelOut;
	} else {
		modelIn = _modelIn;
		if (Color::is32bitRGBModel(_modelOut)) {
			modelOut = _modelOut + modelPlus_NPAtoPA;
		} else {
			modelOut = _modelOut;
		}
	}
	_Color_convertLine(count, modelIn, in, modelOut, out);

}

void Color::copy(sl_size count, const Color* in, Color* out)
{
	for (sl_size i = 0; i < count; i++) {
		out[i] = in[i];
	}
}

void Color::convertPAtoNPA(sl_size count, const Color* in, Color* out)
{
	for (sl_size i = 0; i < count; i++) {
		out[i] = in[i];
		out[i].convertPAtoNPA();
	}
}

void Color::convertNPAtoPA(sl_size count, const Color* in, Color* out)
{
	for (sl_size i = 0; i < count; i++) {
		out[i] = in[i];
		out[i].convertNPAtoPA();
	}
}

void Color::convert(sl_size count, ColorModel modelIn, const void* in, Color* out)
{
	Color::convert(count, modelIn, in, Color::defaultModel, out);
}

void Color::convertPAtoNPA(sl_size count, ColorModel modelIn, const void* in, Color* out)
{
	Color::convertPAtoNPA(count, modelIn, in, Color::defaultModel, out);
}

void Color::convertNPAtoPA(sl_size count, ColorModel modelIn, const void* in, Color* out)
{
	Color::convertNPAtoPA(count, modelIn, in, Color::defaultModel, out);
}

void Color::convert(sl_size count, const Color* in, ColorModel modelOut, void* out)
{
	Color::convert(count, Color::defaultModel, in, modelOut, out);
}

void Color::convertPAtoNPA(sl_size count, const Color* in, ColorModel modelOut, void* out)
{
	Color::convertPAtoNPA(count, Color::defaultModel, in, modelOut, out);
}

void Color::convertNPAtoPA(sl_size count, const Color* in, ColorModel modelOut, void* out)
{
	Color::convertNPAtoPA(count, Color::defaultModel, in, modelOut, out);
}


#define _COLOR_CONVERT_YUV_IMPL_HEADER \
	sl_uint8* dataIn = (sl_uint8*)in; \
	sl_uint8* dataOut = (sl_uint8*)out; \
	if (width & 1) { \
		return; \
	} \
	if (height & 1) { \
		return; \
	} \
	sl_size W = width; \
	sl_size H = height; \
	sl_size W2 = W >> 1; SLIB_UNUSED(W2); \
	sl_size H2 = H >> 1; SLIB_UNUSED(H2); \
	sl_size S1 = W * H; \
	sl_size S2 = S1 >> 1; SLIB_UNUSED(S2); \
	sl_size S4 = S2 >> 1; SLIB_UNUSED(S4);

#define _COLOR_YUV420_I420(p, pY, pU, pV, S) \
	pY = p; \
	pU = p + S1; \
	pV = pU + S4; \
	S = 1;

#define _COLOR_YUV420_YV12(p, pY, pU, pV, S) \
	pY = p; \
	pV = p + S1; \
	pU = pV + S4; \
	S = 1;

#define _COLOR_YUV420_NV21(p, pY, pU, pV, S) \
	pY = p; \
	pV = p + S1; \
	pU = pU + 1; \
	S = 2;

#define _COLOR_YUV420_NV12(p, pY, pU, pV, S) \
	pY = p; \
	pU = p + S1; \
	pV = pU + 1; \
	S = 2;

#define _COLOR_YUV420_DEFINE_PARAMS(model, p, pY, pU, pV, S) \
	switch (model) { \
	case Color::YUV_I420: \
		_COLOR_YUV420_I420(p, pY, pU, pV, S); \
		break; \
	case Color::YUV_YV12: \
		_COLOR_YUV420_YV12(p, pY, pU, pV, S); \
		break; \
	case Color::YUV_NV21: \
		_COLOR_YUV420_NV21(p, pY, pU, pV, S); \
		break; \
	case Color::YUV_NV12: \
		_COLOR_YUV420_NV12(p, pY, pU, pV, S); \
		break; \
	default: \
		return; \
	}

static void _Color_convert_from_to_yuv420(sl_size width, sl_size height, sl_uint32 modelIn, const void* in, sl_uint32 modelOut, void* out)
{
	_COLOR_CONVERT_YUV_IMPL_HEADER
	if (modelIn == modelOut) {
		if (dataIn != dataOut) {
			Base::copyMemory(dataOut, dataIn, S1 + S2);
		}
		return;
	}
	sl_uint8* pYIn = 0; sl_uint8* pUIn = 0; sl_uint8* pVIn = 0; sl_uint32 strideUVIn = 0;
	sl_uint8* pYOut = 0; sl_uint8* pUOut = 0; sl_uint8* pVOut = 0; sl_uint32 strideUVOut = 0;
	_COLOR_YUV420_DEFINE_PARAMS(modelIn, dataIn, pYIn, pUIn, pVIn, strideUVIn);
	_COLOR_YUV420_DEFINE_PARAMS(modelOut, dataOut, pYOut, pUOut, pVOut, strideUVOut);

	Base::copyMemory(pYOut, pYIn, S1);
	for (sl_size i = 0; i < S4; i++) {
		*pUOut = *pUIn;
		*pVOut = *pVIn;
		pUOut += strideUVOut;
		pVOut += strideUVOut;
		pUIn += strideUVIn;
		pVIn += strideUVIn;
	}
}

SLIB_INLINE static void _Color_set3(sl_uint8* d, sl_uint8 a, sl_uint8 b, sl_uint8 c)
{
	d[0] = a;
	d[1] = b;
	d[2] = c;
}

SLIB_INLINE static void _Color_get3(const sl_uint8* d, sl_uint8& a, sl_uint8& b, sl_uint8& c)
{
	a = d[0];
	b = d[1];
	c = d[2];
}

#define _COLOR_SWITCH_DEFINE(modelOut) { \
	Color c = Color::white(); \
	for (i = 0; i < H2; i++) { \
		sl_uint8* py = pYIn; \
		sl_uint8* po = dataOut; \
		for (j = 0; j < W2; j++) { \
			c.setYUV(*py, *pUIn, *pVIn); \
			c.write##modelOut(po); \
			c.setYUV(*(py + 1), *pUIn, *pVIn); \
			c.write##modelOut(po + bytes##modelOut); \
			c.setYUV(*(py + W), *pUIn, *pVIn); \
			c.write##modelOut(po + pitchOut); \
			c.setYUV(*(py + 1 + W), *pUIn, *pVIn); \
			c.write##modelOut(po + bytes##modelOut + pitchOut); \
			po += bytes##modelOut + bytes##modelOut; \
			py += 2; \
			pUIn += strideUVIn; \
			pVIn += strideUVIn; \
		} \
		dataOut += (pitchOut + pitchOut); \
		pYIn += (W + W); \
	} \
}

static void _Color_convert_from_yuv420(sl_size width, sl_size height, sl_uint32 modelIn, const void* in, sl_uint32 modelOut, void* out, sl_reg pitchOut)
{
	_COLOR_CONVERT_YUV_IMPL_HEADER
	sl_uint8* pYIn = 0; sl_uint8* pUIn = 0; sl_uint8* pVIn = 0; sl_uint32 strideUVIn = 0;
	_COLOR_YUV420_DEFINE_PARAMS(modelIn, dataIn, pYIn, pUIn, pVIn, strideUVIn);
	sl_size i, j;
	if (modelOut == Color::YUV444) {
		for (i = 0; i < H2; i++) {
			sl_uint8* py = pYIn;
			sl_uint8* po = dataOut;
			for (j = 0; j < W2; j++) {
				_Color_set3(po, *py, *pUIn, *pVIn);
				_Color_set3(po + 3, *(py + 1), *pUIn, *pVIn);
				_Color_set3(po + pitchOut, *(py + W), *pUIn, *pVIn);
				_Color_set3(po + 3 + pitchOut, *(py + 1 + W), *pUIn, *pVIn);
				po += 6;
				py += 2;
				pUIn += strideUVIn;
				pVIn += strideUVIn;
			}
			dataOut += (pitchOut + pitchOut);
			pYIn += (W + W);
		}
		return;
	}
	sl_uint32 model = modelOut;
	_COLOR_SWITCH_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

#define _COLOR_SWITCH_DEFINE(modelIn) { \
	Color c = Color::white(); \
	for (i = 0; i < H2; i++) { \
		sl_uint8* py = pYOut; \
		sl_uint8* pi = dataIn; \
		for (j = 0; j < W2; j++) { \
			sl_int32 U = 0, V = 0; \
			sl_uint8 _U, _V; \
			c.read##modelIn(pi); \
			c.getYUV(*py, _U, _V); U += _U; V += _V; \
			c.read##modelIn(pi + bytes##modelIn); \
			c.getYUV(*(py + 1), _U, _V); U += _U; V += _V; \
			c.read##modelIn(pi + pitchIn); \
			c.getYUV(*(py + W), _U, _V); U += _U; V += _V; \
			c.read##modelIn(pi + bytes##modelIn + pitchIn); \
			c.getYUV(*(py + 1 + W), _U, _V); U += _U; V += _V; \
			pi += bytes##modelIn + bytes##modelIn; \
			py += 2; \
			*pUOut = (sl_uint8)(Math::clamp0_255(U >> 2)); \
			*pVOut = (sl_uint8)(Math::clamp0_255(V >> 2)); \
			pUOut += strideUVOut; \
			pVOut += strideUVOut; \
		} \
		dataIn += (pitchIn + pitchIn); \
		pYOut += (W + W); \
	} \
}
static void _Color_convert_to_yuv420(sl_size width, sl_size height, sl_uint32 modelIn, const void* in, sl_reg pitchIn, sl_uint32 modelOut, void* out)
{
	_COLOR_CONVERT_YUV_IMPL_HEADER
	sl_uint8* pYOut = 0; sl_uint8* pUOut = 0; sl_uint8* pVOut = 0; sl_uint32 strideUVOut = 0;
	_COLOR_YUV420_DEFINE_PARAMS(modelOut, dataOut, pYOut, pUOut, pVOut, strideUVOut);
	sl_size i, j;
	if (modelIn == Color::YUV444) {
		for (i = 0; i < H2; i++) {
			sl_uint8* py = pYOut;
			sl_uint8* pi = dataIn;
			for (j = 0; j < W2; j++) {
				sl_int32 U = 0, V = 0;
				sl_uint8 _U, _V;
				_Color_get3(pi, *py, _U, _V); U += _U; V += _V;
				_Color_get3(pi + 3, *(py + 1), _U, _V); U += _U; V += _V;
				_Color_get3(pi + pitchIn, *(py + W), _U, _V); U += _U; V += _V;
				_Color_get3(pi + 3 + pitchIn, *(py + 1 + W), _U, _V); U += _U; V += _V;
				pi += 6;
				py += 2;
				*pUOut = (sl_uint8)(Math::clamp0_255(U >> 2));
				*pVOut = (sl_uint8)(Math::clamp0_255(V >> 2));
				pUOut += strideUVOut;
				pVOut += strideUVOut;
			}
			dataIn += (pitchIn + pitchIn);
			pYOut += (W + W);
		}
		return;
	}
	sl_uint32 model = modelIn;
	_COLOR_SWITCH_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

#define _COLOR_IMPL_DEFINE(modelIn, modelOut) \
static void _Color_convert__##modelIn##__##modelOut(sl_size width, sl_size height, sl_uint8* dataIn, sl_reg pitchIn, sl_uint8* dataOut, sl_reg pitchOut) { \
	sl_size i, j; \
	Color c = Color::white(); \
	for (i = 0; i < height; i++) { \
		sl_uint8* pi = dataIn; \
		sl_uint8* po = dataOut; \
		for (j = 0; j < width; j++) { \
			c.read##modelIn(pi); \
			c.write##modelOut(po); \
			pi += bytes##modelIn; \
			po += bytes##modelOut; \
		} \
		dataIn += pitchIn; \
		dataOut += pitchOut; \
	} \
}

_COLOR_IMPL_BODY

#undef _COLOR_IMPL_DEFINE

#define _COLOR_SWITCH_DEFINE(modelIn, modelOut) \
	_Color_convert__##modelIn##__##modelOut(width, height, dataIn, pitchIn, dataOut, pitchOut);

static void _Color_convert(sl_size width, sl_size height, sl_uint32 modelIn, const void* in, sl_reg pitchIn, sl_uint32 modelOut, void* out, sl_reg pitchOut)
{
	sl_uint8* dataIn = (sl_uint8*)in;
	sl_uint8* dataOut = (sl_uint8*)out;

	if (pitchIn == 0) {
		pitchIn = width * _Color_getModelBytes(modelIn);
	}
	if (pitchOut == 0) {
		pitchOut = width * _Color_getModelBytes(modelOut);
	}
	
	if (Color::isYUV420Model((ColorModel)modelOut)) {
		if (Color::isYUV420Model((ColorModel)modelIn)) {
			return _Color_convert_from_to_yuv420(width, height, modelIn, in, modelOut, out);
		} else {
			return _Color_convert_to_yuv420(width, height, modelIn, in, pitchIn, modelOut, out);
		}
	} else {
		if (Color::isYUV420Model((ColorModel)modelIn)) {
			return _Color_convert_from_yuv420(width, height, modelIn, in, modelOut, out, pitchOut);
		}
	}

	if (modelIn == modelOut) {
		if (dataIn != dataOut) {
			sl_size sizeCopy = width * _Color_getModelBytes(modelIn);
			for (sl_size i = 0; i < height; i++) {
				Base::copyMemory(dataOut, dataIn, sizeCopy);
				dataIn += pitchIn;
				dataOut += pitchOut;
			}
		}
		return;
	}
	_COLOR_SWITCH2_MAIN_BODY
}
#undef _COLOR_SWITCH_DEFINE

void Color::convert(sl_size width, sl_size height, ColorModel modelIn, const void* in, sl_reg pitchIn, ColorModel modelOut, void* out, sl_reg pitchOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	_Color_convert(width, height, modelIn, in, pitchIn, modelOut, out, pitchOut);
}

void Color::convertPAtoNPA(sl_size width, sl_size height, ColorModel _modelIn, const void* in, sl_reg pitchIn, ColorModel _modelOut, void* out, sl_reg pitchOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	sl_uint32 modelIn, modelOut;
	if (Color::is32bitRGBModel(_modelIn)) {
		modelIn = _modelIn + modelPlus_PAtoNPA;
		modelOut = _modelOut;
	} else {
		modelIn = _modelIn;
		if (Color::is32bitRGBModel(_modelOut)) {
			modelOut = _modelOut + modelPlus_PAtoNPA;
		} else {
			modelOut = _modelOut;
		}
	}
	_Color_convert(width, height, modelIn, in, pitchIn, modelOut, out, pitchOut);
}

void Color::convertNPAtoPA(sl_size width, sl_size height, ColorModel _modelIn, const void* in, sl_reg pitchIn, ColorModel _modelOut, void* out, sl_reg pitchOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	sl_uint32 modelIn, modelOut;
	if (Color::is32bitRGBModel(_modelIn)) {
		modelIn = _modelIn + modelPlus_NPAtoPA;
		modelOut = _modelOut;
	} else {
		modelIn = _modelIn;
		if (Color::is32bitRGBModel(_modelOut)) {
			modelOut = _modelOut + modelPlus_NPAtoPA;
		} else {
			modelOut = _modelOut;
		}
	}
	_Color_convert(width, height, modelIn, in, pitchIn, modelOut, out, pitchOut);
}

void Color::copy(sl_size width, sl_size height, const Color* in, sl_reg strideIn, Color* out, sl_reg strideOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	if (strideIn == 0) {
		strideIn = width;
	}
	if (strideOut == 0) {
		strideOut = width;
	}
	const Color* s = in;
	Color* d = out;
	for (sl_size y = 0; y < height; y++) {
		for (sl_size x = 0; x < width; x++) {
			d[x] = s[x];
		}
		s += strideIn;
		d += strideOut;
	}
}

void Color::convertPAtoNPA(sl_size width, sl_size height, const Color* in, sl_reg strideIn, Color* out, sl_reg strideOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	if (strideIn == 0) {
		strideIn = width;
	}
	if (strideOut == 0) {
		strideOut = width;
	}
	const Color* s = in;
	Color* d = out;
	for (sl_size y = 0; y < height; y++) {
		for (sl_size x = 0; x < width; x++) {
			d[x] = s[x];
			d[x].convertPAtoNPA();
		}
		s += strideIn;
		d += strideOut;
	}
}

void Color::convertNPAtoPA(sl_size width, sl_size height, const Color* in, sl_reg strideIn, Color* out, sl_reg strideOut)
{
	if (width == 0 || height == 0) {
		return;
	}
	if (strideIn == 0) {
		strideIn = width;
	}
	if (strideOut == 0) {
		strideOut = width;
	}
	const Color* s = in;
	Color* d = out;
	for (sl_size y = 0; y < height; y++) {
		for (sl_size x = 0; x < width; x++) {
			d[x] = s[x];
			d[x].convertNPAtoPA();
		}
		s += strideIn;
		d += strideOut;
	}
}

void Color::convert(sl_size width, sl_size height, ColorModel modelIn, const void* in, sl_reg pitchIn, Color* out, sl_reg strideOut)
{
	Color::convert(width, height, modelIn, in, pitchIn, defaultModel, out, strideOut * 4);
}

void Color::convertPAtoNPA(sl_size width, sl_size height, ColorModel modelIn, const void* in, sl_reg pitchIn, Color* out, sl_reg strideOut)
{
	Color::convertPAtoNPA(width, height, modelIn, in, pitchIn, defaultModel, out, strideOut * 4);
}

void Color::convertNPAtoPA(sl_size width, sl_size height, ColorModel modelIn, const void* in, sl_reg pitchIn, Color* out, sl_reg strideOut)
{
	Color::convertNPAtoPA(width, height, modelIn, in, pitchIn, defaultModel, out, strideOut * 4);
}

void Color::convert(sl_size width, sl_size height, const Color* in, sl_reg strideIn, ColorModel modelOut, void* out, sl_reg pitchOut)
{
	Color::convert(width, height, defaultModel, in, strideIn * 4, modelOut, out, pitchOut);
}

void Color::convertPAtoNPA(sl_size width, sl_size height, const Color* in, sl_reg strideIn, ColorModel modelOut, void* out, sl_reg pitchOut)
{
	Color::convertPAtoNPA(width, height, defaultModel, in, strideIn * 4, modelOut, out, pitchOut);
}

void Color::convertNPAtoPA(sl_size width, sl_size height, const Color* in, sl_reg strideIn, ColorModel modelOut, void* out, sl_reg pitchOut)
{
	Color::convertPAtoNPA(width, height, defaultModel, in, strideIn * 4, modelOut, out, pitchOut);
}
SLIB_GRAPHICS_NAMESPACE_END

