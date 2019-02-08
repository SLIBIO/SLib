/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_PEN
#define CHECKHEADER_SLIB_GRAPHICS_PEN

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"

namespace slib
{
	
	class SLIB_EXPORT PenDesc
	{
	public:
		PenStyle style;
		LineCap cap;
		LineJoin join;
		sl_real width;
		Color color;
		sl_real miterLimit;

	public:
		PenDesc();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PenDesc)
		
	};
	
	class SLIB_EXPORT Pen : public Object
	{
		SLIB_DECLARE_OBJECT;

	protected:
		Pen();

		~Pen();

	public:
		static Ref<Pen> getDefault();

		static Ref<Pen> create(const PenDesc& desc);

		static Ref<Pen> create(PenStyle style, sl_real width = 1, Color color = Color::Black);

		static Ref<Pen> createSolidPen(sl_real width = 1, Color color = Color::Black);

	public:
		void getDesc(PenDesc& desc);
	
		PenStyle getStyle();
	
		sl_real getWidth();
	
		Color getColor();
	
		LineCap getCap();
	
		LineJoin getJoin();
	
		sl_real getMiterLimit();
	
	protected:
		PenDesc m_desc;

		Ref<Referable> m_platformObject;
		SpinLock m_lock;

	};

}

#endif
