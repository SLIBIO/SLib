#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/platform.h"
#include "../../../inc/slib/graphics/bitmap.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

#if defined(SLIB_PLATFORM_IS_OSX)

CGImageRef GraphicsPlatform::loadCGImageFromMemory(const void* buf, sl_size size)
{
	CGImageRef ret;
	NSImage* image = GraphicsPlatform::loadNSImageFromMemory(buf, size);
	if (image == nil) {
		return NULL;
	}
	ret = [image CGImageForProposedRect:NULL context:NULL hints:NULL];
	return ret;
}

NSImage* GraphicsPlatform::loadNSImageFromMemory(const void* buf, sl_size size)
{
	NSData* data = [NSData dataWithBytesNoCopy:(void*)buf length:size freeWhenDone:FALSE];
	return [[NSImage alloc] initWithData:data];
}

NSColor* GraphicsPlatform::getNSColorFromColor(const Color& color)
{
	return [NSColor colorWithRed:color.getRedF() green:color.getGreenF() blue:color.getBlueF() alpha:color.getAlphaF()];
}

Color GraphicsPlatform::getColorFromNSColor(NSColor* color)
{
	if (color == nil) {
		return Color::zero();
	}
	color = [color colorUsingColorSpaceName:NSDeviceRGBColorSpace];
	CGFloat red, green, blue, alpha;
	[color getRed:&red green:&green blue:&blue alpha:&alpha];
	return Color4f((sl_real)red, (sl_real)green, (sl_real)blue, (sl_real)alpha);
}

#endif

#if defined(SLIB_PLATFORM_IS_IOS)

CGImageRef GraphicsPlatform::loadCGImageFromMemory(const void* buf, sl_size size)
{
	CGImageRef ret;
	UIImage* image = loadUIImageFromMemory(buf, size);
	if (image == nil) {
		return NULL;
	}
	ret = image.CGImage;
	return ret;
}

UIImage* GraphicsPlatform::loadUIImageFromMemory(const void* buf, sl_size size)
{
	NSData* data = [NSData dataWithBytesNoCopy:(void*)buf length:size freeWhenDone:FALSE];
	return [UIImage imageWithData:data];
}

UIColor* GraphicsPlatform::getUIColorFromColor(const Color& color)
{
	return [UIColor colorWithRed:color.getRedF() green:color.getGreenF() blue:color.getBlueF() alpha:color.getAlphaF()];
}

Color GraphicsPlatform::getColorFromUIColor(UIColor* color)
{
	if (color == nil) {
		return Color::zero();
	}
	CGFloat red, green, blue, alpha;
	[color getRed:&red green:&green blue:&blue alpha:&alpha];
	return Color4f((sl_real)red, (sl_real)green, (sl_real)blue, (sl_real)alpha);
}

#endif

void GraphicsPlatform::getCATransform(CATransform3D& _out, const Matrix3& mat)
{
	_out.m11 = mat.m00;
	_out.m12 = mat.m01;
	_out.m13 = 0;
	_out.m14 = 0;
	_out.m21 = mat.m10;
	_out.m22 = mat.m11;
	_out.m23 = 0;
	_out.m24 = 0;
	_out.m31 = 0;
	_out.m32 = 0;
	_out.m33 = 1;
	_out.m34 = 0;
	_out.m41 = mat.m20;
	_out.m42 = mat.m21;
	_out.m43 = 0;
	_out.m44 = 1;
}

void GraphicsPlatform::getCATransform(CATransform3D& _out, const Matrix4& mat)
{
	_out.m11 = mat.m00;
	_out.m12 = mat.m01;
	_out.m13 = mat.m02;
	_out.m14 = mat.m03;
	_out.m21 = mat.m10;
	_out.m22 = mat.m11;
	_out.m23 = mat.m12;
	_out.m24 = mat.m13;
	_out.m31 = mat.m20;
	_out.m32 = mat.m21;
	_out.m33 = mat.m22;
	_out.m34 = mat.m23;
	_out.m41 = mat.m30;
	_out.m42 = mat.m31;
	_out.m43 = mat.m32;
	_out.m44 = mat.m33;
}

void GraphicsPlatform::getCGAffineTransform(CGAffineTransform& t, const Matrix3& mat)
{
	t.a = mat.m00;
	t.b = mat.m01;
	t.c = mat.m10;
	t.d = mat.m11;
	t.tx = mat.m20;
	t.ty = mat.m21;
}

void GraphicsPlatform::getCGAffineTransform(CGAffineTransform& t, const Matrix3& mat, CGFloat f, CGFloat ax, CGFloat ay)
{
	t.a = mat.m00;
	t.b = mat.m01;
	t.c = mat.m10;
	t.d = mat.m11;
	t.tx = -ax * mat.m00 - ay * mat.m10 + mat.m20 / f + ax;
	t.ty = -ax * mat.m00 - ay * mat.m10 + mat.m21 / f + ay;
}

void GraphicsPlatform::getMatrix3FromCGAffineTransform(Matrix3& ret, const CGAffineTransform& t)
{
	ret.m00 = (sl_real)(t.a);
	ret.m01 = (sl_real)(t.b);
	ret.m02 = 0;
	ret.m10 = (sl_real)(t.c);
	ret.m11 = (sl_real)(t.d);
	ret.m12 = 0;
	ret.m20 = (sl_real)(t.tx);
	ret.m21 = (sl_real)(t.ty);
	ret.m22 = 1;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
