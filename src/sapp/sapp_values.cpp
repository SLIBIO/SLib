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

#include "sapp_values.h"

#include "sapp_util.h"

#include "slib/core/variant.h"
#include "slib/math/calculator.h"

namespace slib
{
	
	static sl_reg _priv_SApp_parseFloat(float* _out, const sl_char8* sz, sl_size start, sl_size end)
	{
		return Calculator::calculate(_out, sl_null, sz, start, end);
	}
	
	static sl_bool _priv_SApp_parseFloat(float* _out, const String& str)
	{
		return Calculator::calculate(str, _out);
	}

	/************************************************
					String
	************************************************/

	SAppStringValue::SAppStringValue()
	: flagDefined(sl_false), flagReferResource(sl_false)
	{
	}

	sl_bool SAppStringValue::parse(const String& _str)
	{
		String str = _str;
		if (str.isNull()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str.startsWith('@')) {
			str = str.substring(1);
			if (str == "null") {
				flagReferResource = sl_false;
				valueOrName = String::null();
				flagDefined = sl_true;
				return sl_true;
			}
			if (str.startsWith('@')) {
				flagReferResource = sl_false;
				valueOrName = str;
			} else {
				if (str.startsWith("string/")) {
					str = str.substring(7);
				} else {
					return sl_false;
				}
				str = str.trim();
				if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
					return sl_false;
				}
				flagReferResource = sl_true;
				valueOrName = str;
			}
		} else {
			flagReferResource = sl_false;
			valueOrName = str;
		}
		flagDefined = sl_true;
		return sl_true;
	}
	
	/************************************************
					Dimension
	************************************************/

	SAppDimensionValue::SAppDimensionValue()
	: flagDefined(sl_false), unit(PX), amount(0)
	{
	}

	String SAppDimensionValue::getAccessString() const
	{
		if (!flagDefined) {
			return "0";
		}
		switch (unit) {
			case PX:
				return String::format("%d", (int)amount);
			case SW:
				if (Math::isAlmostZero(amount - 1)) {
					return "slib::UIResource::getScreenWidth()";
				} else {
					return String::format("(sl_ui_pos)(%ff*slib::UIResource::getScreenWidth())", amount);
				}
			case SH:
				if (Math::isAlmostZero(amount - 1)) {
					return "slib::UIResource::getScreenHeight()";
				} else {
					return String::format("(sl_ui_pos)(%ff*slib::UIResource::getScreenHeight())", amount);
				}
			case SMIN:
				if (Math::isAlmostZero(amount - 1)) {
					return "slib::UIResource::getScreenMinimum()";
				} else {
					return String::format("(sl_ui_pos)(%ff*slib::UIResource::getScreenMinimum())", amount);
				}
			case SMAX:
				if (Math::isAlmostZero(amount - 1)) {
					return "slib::UIResource::getScreenMaximum()";
				} else {
					return String::format("(sl_ui_pos)(%ff*slib::UIResource::getScreenMaximum())", amount);
				}
			case VW:
				if (Math::isAlmostZero(amount - 1)) {
					return "CONTENT_WIDTH";
				} else {
					return String::format("(sl_ui_pos)(%ff*CONTENT_WIDTH)", amount);
				}
			case VH:
				if (Math::isAlmostZero(amount - 1)) {
					return "CONTENT_HEIGHT";
				} else {
					return String::format("(sl_ui_pos)(%ff*CONTENT_HEIGHT)", amount);
				}
			case VMIN:
				if (Math::isAlmostZero(amount - 1)) {
					return "SLIB_MIN(CONTENT_WIDTH, CONTENT_HEIGHT)";
				} else {
					return String::format("(sl_ui_pos)(%ff*SLIB_MIN(CONTENT_WIDTH, CONTENT_HEIGHT))", amount);
				}
			case VMAX:
				if (Math::isAlmostZero(amount - 1)) {
					return "SLIB_MAX(CONTENT_WIDTH, CONTENT_HEIGHT)";
				} else {
					return String::format("(sl_ui_pos)(%ff*SLIB_MAX(CONTENT_WIDTH, CONTENT_HEIGHT))", amount);
				}
			case SP:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_ui_pos)(getScaledPixel())";
				} else {
					return String::format("(sl_ui_pos)(%ff*getScaledPixel())", amount);
				}
			case DP:
				return String::format("(sl_ui_pos)(slib::UIResource::dpToPixel(%ff))", amount);
			case PT:
				return String::format("(sl_ui_pos)(slib::UIResource::pointToPixel(%ff))", amount);
			case M:
				return String::format("(sl_ui_pos)(slib::UIResource::meterToPixel(%ff))", amount);
			case CM:
				return String::format("(sl_ui_pos)(slib::UIResource::centimeterToPixel(%ff))", amount);
			case MM:
				return String::format("(sl_ui_pos)(slib::UIResource::millimeterToPixel(%ff))", amount);
			case INCH:
				return String::format("(sl_ui_pos)(slib::UIResource::inchToPixel(%ff))", amount);
		}
		return "0";
	}

	String SAppDimensionFloatValue::getAccessString() const
	{
		if (!flagDefined) {
			return "0";
		}
		switch (unit) {
			case PX:
				return String::format("%ff", amount);
			case SW:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(slib::UIResource::getScreenWidth())";
				} else {
					return String::format("%ff*(sl_real)(slib::UIResource::getScreenWidth())", amount);
				}
			case SH:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(slib::UIResource::getScreenHeight())";
				} else {
					return String::format("%ff*(sl_real)(slib::UIResource::getScreenHeight())", amount);
				}
			case SMIN:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(slib::UIResource::getScreenMinimum())";
				} else {
					return String::format("%ff*(sl_real)(slib::UIResource::getScreenMinimum())", amount);
				}
			case SMAX:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(slib::UIResource::getScreenMaximum())";
				} else {
					return String::format("%ff*(sl_real)(slib::UIResource::getScreenMaximum())", amount);
				}
			case VW:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(CONTENT_WIDTH)";
				} else {
					return String::format("%ff*(sl_real)(CONTENT_WIDTH)", amount);
				}
			case VH:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(CONTENT_HEIGHT)";
				} else {
					return String::format("%ff*(sl_real)(CONTENT_HEIGHT)", amount);
				}
			case VMIN:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(SLIB_MIN(CONTENT_WIDTH, CONTENT_HEIGHT))";
				} else {
					return String::format("%ff*(sl_real)(SLIB_MIN(CONTENT_WIDTH, CONTENT_HEIGHT))", amount);
				}
			case VMAX:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(SLIB_MAX(CONTENT_WIDTH, CONTENT_HEIGHT))";
				} else {
					return String::format("%ff*(sl_real)(SLIB_MAX(CONTENT_WIDTH, CONTENT_HEIGHT))", amount);
				}
			case SP:
				if (Math::isAlmostZero(amount - 1)) {
					return "(sl_real)(getScaledPixel())";
				} else {
					return String::format("%ff*getScaledPixel()", amount);
				}
			case DP:
				return String::format("slib::UIResource::dpToPixel(%ff)", amount);
			case PT:
				return String::format("slib::UIResource::pointToPixel(%ff)", amount);
			case M:
				return String::format("slib::UIResource::meterToPixel(%ff)", amount);
			case CM:
				return String::format("slib::UIResource::centimeterToPixel(%ff)", amount);
			case MM:
				return String::format("slib::UIResource::millimeterToPixel(%ff)", amount);
			case INCH:
				return String::format("slib::UIResource::inchToPixel(%ff)", amount);
		}
		return "0";
	}

	sl_bool SAppDimensionValue::parse(const String& _str)
	{
		do {
			String str = _str.trim();
			if (str.isEmpty()) {
				flagDefined = sl_false;
				unit = PX;
				amount = 0;
				return sl_true;
			}
			str = str.toLower();
			if (str == "fill" || str == "*") {
				amount = 1;
				unit = FILL;
				break;
			}
			if (str == "wrap") {
				amount = 1;
				unit = WRAP;
				break;
			}
			const sl_char8* sz = str.getData();
			sl_size len = str.getLength();
			float f;
			sl_reg ret = _priv_SApp_parseFloat(&f, sz, 0, len);
			if (ret == SLIB_PARSE_ERROR) {
				return sl_false;
			}
			sl_size pos = ret;
			while (pos < len) {
				if (SLIB_CHAR_IS_SPACE_TAB(sz[pos])) {
					pos++;
				} else {
					break;
				}
			}
			if (pos >= len) {
				amount = f;
				unit = PX;
				break;
			}
			sl_bool flagPercent = sl_false;
			if (sz[pos] == '%') {
				flagPercent = sl_true;
				f /= 100;
				pos++;
			}
			while (pos < len) {
				if (SLIB_CHAR_IS_SPACE_TAB(sz[pos])) {
					pos++;
				} else {
					break;
				}
			}
			if (pos == len) {
				amount = f;
				if (flagPercent) {
					unit = WEIGHT;
				} else {
					unit = PX;
				}
				break;
			} else if (pos + 1 == len) {
				if (sz[pos] == '*') {
					amount = f;
					unit = FILL;
					break;
				} else if (sz[pos] == 'm') {
					amount = f;
					unit = M;
					break;
				}
			} else if (pos + 2 == len) {
				if (sz[pos] == 's') {
					if (sz[pos + 1] == 'w') {
						amount = f;
						unit = SW;
						break;
					} else if (sz[pos + 1] == 'h') {
						amount = f;
						unit = SH;
						break;
					} else if (sz[pos + 1] == 'p') {
						amount = f;
						unit = SP;
						break;
					}
				} else if (sz[pos] == 'v') {
					if (sz[pos + 1] == 'w') {
						amount = f;
						unit = VW;
						break;
					} else if (sz[pos + 1] == 'h') {
						amount = f;
						unit = VH;
						break;
					}
				} else if (sz[pos] == 'p') {
					if (sz[pos + 1] == 'x') {
						if (!flagPercent) {
							amount = f;
							unit = PX;
							break;
						}
					} else if (sz[pos + 1] == 't') {
						if (!flagPercent) {
							amount = f;
							unit = PT;
							break;
						}
					}
				} else if (sz[pos] == 'd') {
					if (sz[pos + 1] == 'p') {
						if (!flagPercent) {
							amount = f;
							unit = DP;
							break;
						}
					}
				} else if (sz[pos] == 'm') {
					if (sz[pos + 1] == 'm') {
						if (!flagPercent) {
							amount = f;
							unit = MM;
							break;
						}
					}
				} else if (sz[pos] == 'c') {
					if (sz[pos + 1] == 'm') {
						if (!flagPercent) {
							amount = f;
							unit = CM;
							break;
						}
					}
				} else if (sz[pos] == 'i') {
					if (sz[pos + 1] == 'n') {
						if (!flagPercent) {
							amount = f;
							unit = INCH;
							break;
						}
					}
				}
			} else if (pos + 4 == len) {
				if (sz[pos] == 's') {
					if (sz[pos + 1] == 'm') {
						if (sz[pos + 2] == 'a' && sz[pos + 3] == 'x') {
							amount = f;
							unit = SMAX;
							break;
						} else if (sz[pos + 2] == 'i' && sz[pos + 3] == 'n') {
							amount = f;
							unit = SMIN;
							break;
						}
					}
				} else if (sz[pos] == 'v') {
					if (sz[pos + 1] == 'm') {
						if (sz[pos + 2] == 'a' && sz[pos + 3] == 'x') {
							amount = f;
							unit = VMAX;
							break;
						} else if (sz[pos + 2] == 'i' && sz[pos + 3] == 'n') {
							amount = f;
							unit = VMIN;
							break;
						}
					}
				}
			}
			return sl_false;
			
		} while (0);
		
		if (amount == 0) {
			unit = PX;
		}
		flagDefined = sl_true;
		return sl_true;
	}

	sl_bool SAppDimensionValue::checkGlobal()
	{
		if (!flagDefined) {
			return sl_true;
		}
		return isGlobalUnit(unit);
	}

	sl_bool SAppDimensionValue::checkSP()
	{
		if (!flagDefined) {
			return sl_true;
		}
		return amount > 0 && !isRelativeUnit(unit);
	}

	sl_bool SAppDimensionValue::checkPosition()
	{
		if (!flagDefined) {
			return sl_true;
		}
		return !isRelativeUnit(unit);
	}

	sl_bool SAppDimensionValue::checkSize()
	{
		if (!flagDefined) {
			return sl_true;
		}
		if (unit == FILL || unit == WRAP) {
			return sl_true;
		}
		return amount >= 0;
	}

	sl_bool SAppDimensionValue::checkScalarSize()
	{
		if (!flagDefined) {
			return sl_true;
		}
		return amount >= 0 && !isRelativeUnit(unit);
	}

	sl_bool SAppDimensionValue::checkMargin()
	{
		if (!flagDefined) {
			return sl_true;
		}
		if (unit == WEIGHT) {
			return sl_true;
		}
		return checkPosition();
	}

	sl_bool SAppDimensionValue::checkForWindow()
	{
		return checkGlobal();
	}

	sl_bool SAppDimensionValue::checkForWindowSize()
	{
		if (!flagDefined) {
			return sl_true;
		}
		return amount >= 0 && isGlobalUnit(unit);
	}

	sl_bool SAppDimensionValue::checkForRootViewPosition()
	{
		return checkGlobal();
	}

	sl_bool SAppDimensionValue::checkForRootViewSize()
	{
		if (!flagDefined) {
			return sl_true;
		}
		if (unit == FILL || unit == WRAP) {
			return sl_true;
		}
		if (unit == WEIGHT || isGlobalUnit(unit)) {
			return amount >= 0;
		}
		return sl_false;
	}

	sl_bool SAppDimensionValue::checkForRootViewScalarSize()
	{
		if (!flagDefined) {
			return sl_true;
		}
		if (isGlobalUnit(unit)) {
			return amount >= 0;
		}
		return sl_false;
	}

	sl_bool SAppDimensionValue::checkForRootViewMargin()
	{
		if (!flagDefined) {
			return sl_true;
		}
		if (unit == WEIGHT) {
			return sl_true;
		}
		return isGlobalUnit(unit);
	}

	sl_bool SAppDimensionValue::isNeededOnLayoutFunction()
	{
		if (!flagDefined) {
			return sl_false;
		}
		return isViewportUnit(unit);
	}

	sl_bool SAppDimensionValue::isRelativeUnit(int unit)
	{
		return unit == FILL || unit == WRAP || unit == WEIGHT;
	}

	sl_bool SAppDimensionValue::isGlobalUnit(int unit)
	{
		return unit == PX || unit == SW || unit == SH || unit == SMIN || unit == SMAX || unit == INCH || unit == M | unit == CM || unit == MM || unit == PT || unit == DP;
	}

	sl_bool SAppDimensionValue::isViewportUnit(int unit)
	{
		return unit == VW || unit == VH || unit == VMIN || unit == VMAX || unit == SP;
	}

	/************************************************
						Boolean
	************************************************/

	SAppBooleanValue::SAppBooleanValue()
	: flagDefined(sl_false), value(sl_false)
	{
	}

	String SAppBooleanValue::getAccessString()
	{
		if (!flagDefined) {
			return "sl_false";
		}
		if (value) {
			return "sl_true";
		} else {
			return "sl_false";
		}
	}

	sl_bool SAppBooleanValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "true") {
			value = sl_true;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "false") {
			value = sl_false;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					Float
	************************************************/

	SAppFloatValue::SAppFloatValue()
	: flagDefined(sl_false), value(0)
	{
	}

	String SAppFloatValue::getAccessString()
	{
		if (!flagDefined) {
			return "0";
		}
		return String::format("%ff", value);
	}

	sl_bool SAppFloatValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		float f;
		if (_priv_SApp_parseFloat(&f, str)) {
			value = f;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					Integer
	************************************************/

	SAppInt32Value::SAppInt32Value()
	: flagDefined(sl_false), value(0)
	{
	}

	String SAppInt32Value::getAccessString()
	{
		if (!flagDefined) {
			return "0";
		}
		return String::format("%d", value);
	}

	sl_bool SAppInt32Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_int32 f;
		if (Calculator::calculate(str, &f)) {
			value = f;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	SAppUint32Value::SAppUint32Value()
	: flagDefined(sl_false), value(0)
	{
	}

	String SAppUint32Value::getAccessString()
	{
		if (!flagDefined) {
			return "0";
		}
		return String::format("%d", value);
	}

	sl_bool SAppUint32Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_uint32 f;
		if (str.parseUint32(10, &f)) {
			value = f;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	SAppInt64Value::SAppInt64Value()
	: flagDefined(sl_false), value(0)
	{
	}

	String SAppInt64Value::getAccessString()
	{
		if (!flagDefined) {
			return "0";
		}
		return String::format("%d", value);
	}

	sl_bool SAppInt64Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_int64 f;
		if (Calculator::calculate(str, &f)) {
			value = f;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	SAppUint64Value::SAppUint64Value()
	: flagDefined(sl_false), value(0)
	{
	}

	String SAppUint64Value::getAccessString()
	{
		if (!flagDefined) {
			return "0";
		}
		return String::format("%d", value);
	}

	sl_bool SAppUint64Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_uint64 f;
		if (str.parseUint64(10, &f)) {
			value = f;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	/************************************************
					Vector2
	************************************************/

	SAppVector2Value::SAppVector2Value()
	: flagDefined(0)
	{
	}

	String SAppVector2Value::getAccessString()
	{
		if (!flagDefined) {
			return "slib::Vector2::zero()";
		}
		return String::format("slib::Vector2(%ff, %ff)", value.x, value.y);
	}

	sl_bool SAppVector2Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_size pos = 0;
		sl_size len = str.getLength();
		const sl_char8* sz = str.getData();
		float f[2];
		for (sl_size i = 0; i < 2; i++) {
			sl_reg iRet = _priv_SApp_parseFloat(f+i, sz, pos, len);
			if (iRet == SLIB_PARSE_ERROR) {
				return sl_false;
			}
			pos = iRet;
			for (; pos < len; pos++) {
				if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
					break;
				}
			}
			if (i == 1) {
				if (pos == len) {
					value.x = f[0];
					value.y = f[1];
					flagDefined = sl_true;
					return sl_true;
				} else {
					return sl_false;
				}
			} else {
				if (pos >= len) {
					return sl_false;
				}
				if (sz[pos] != ',') {
					return sl_false;
				}
				pos++;
				for (; pos < len; pos++) {
					if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
						break;
					}
				}
				if (pos >= len) {
					return sl_false;
				}
			}
		}
		return sl_false;
	}


	/************************************************
					Vector3
	************************************************/

	SAppVector3Value::SAppVector3Value()
	: flagDefined(sl_false)
	{
	}

	String SAppVector3Value::getAccessString()
	{
		if (!flagDefined) {
			return "slib::Vector3::zero()";
		}
		return String::format("slib::Vector3(%ff, %ff, %ff)", value.x, value.y, value.z);
	}

	sl_bool SAppVector3Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_size pos = 0;
		sl_size len = str.getLength();
		const sl_char8* sz = str.getData();
		float f[3];
		for (sl_size i = 0; i < 3; i++) {
			sl_reg iRet = _priv_SApp_parseFloat(f+i, sz, pos, len);
			if (iRet == SLIB_PARSE_ERROR) {
				return sl_false;
			}
			pos = iRet;
			for (; pos < len; pos++) {
				if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
					break;
				}
			}
			if (i == 2) {
				if (pos == len) {
					value.x = f[0];
					value.y = f[1];
					value.z = f[2];
					flagDefined = sl_true;
					return sl_true;
				} else {
					return sl_false;
				}
			} else {
				if (pos >= len) {
					return sl_false;
				}
				if (sz[pos] != ',') {
					return sl_false;
				}
				pos++;
				for (; pos < len; pos++) {
					if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
						break;
					}
				}
				if (pos >= len) {
					return sl_false;
				}
			}
		}
		return sl_false;
	}


	/************************************************
					Vector4
	************************************************/

	SAppVector4Value::SAppVector4Value()
	: flagDefined(sl_false)
	{
	}

	String SAppVector4Value::getAccessString()
	{
		if (!flagDefined) {
			return "slib::Vector4::zero()";
		}
		return String::format("slib::Vector4(%ff, %ff, %ff, %ff)", value.x, value.y, value.z, value.w);
	}

	sl_bool SAppVector4Value::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		sl_size pos = 0;
		sl_size len = str.getLength();
		const sl_char8* sz = str.getData();
		float f[4];
		for (sl_size i = 0; i < 4; i++) {
			sl_reg iRet = _priv_SApp_parseFloat(f+i, sz, pos, len);
			if (iRet == SLIB_PARSE_ERROR) {
				return sl_false;
			}
			pos = iRet;
			for (; pos < len; pos++) {
				if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
					break;
				}
			}
			if (i == 3) {
				if (pos == len) {
					value.x = f[0];
					value.y = f[1];
					value.z = f[2];
					value.w = f[3];
					flagDefined = sl_true;
					return sl_true;
				} else {
					return sl_false;
				}
			} else {
				if (pos >= len) {
					return sl_false;
				}
				if (sz[pos] != ',') {
					return sl_false;
				}
				pos++;
				for (; pos < len; pos++) {
					if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
						break;
					}
				}
				if (pos >= len) {
					return sl_false;
				}
			}
		}
		return sl_false;
	}


	/************************************************
					Visibility
	************************************************/

	SAppVisibilityValue::SAppVisibilityValue()
	: flagDefined(sl_false), value(Visibility::Visible)
	{
	}

	String SAppVisibilityValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::Visibility::Visible";
		}
		if (value == Visibility::Gone) {
			return "slib::Visibility::Gone";
		} else if (value == Visibility::Hidden) {
			return "slib::Visibility::Hidden";
		} else {
			return "slib::Visibility::Visible";
		}
	}

	sl_bool SAppVisibilityValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "visible") {
			value = Visibility::Visible;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "gone") {
			value = Visibility::Gone;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "hidden") {
			value = Visibility::Hidden;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
				PenStyle
	************************************************/

	SAppPenStyleValue::SAppPenStyleValue()
	: flagDefined(sl_false), value(PenStyle::Solid)
	{
	}

	String SAppPenStyleValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::PenStyle::Solid";
		}
		if (value == PenStyle::Dot) {
			return "slib::PenStyle::Dot";
		} else if (value == PenStyle::Dash) {
			return "slib::PenStyle::Dash";
		} else if (value == PenStyle::DashDot) {
			return "slib::PenStyle::DashDot";
		} else if (value == PenStyle::DashDotDot) {
			return "slib::PenStyle::DashDotDot";
		} else {
			return "slib::PenStyle::Solid";
		}
	}

	sl_bool SAppPenStyleValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "solid") {
			value = PenStyle::Solid;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "dot") {
			value = PenStyle::Dot;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "dash") {
			value = PenStyle::Dash;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "dashdot") {
			value = PenStyle::DashDot;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "dashdotdot") {
			value = PenStyle::DashDotDot;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					ScrollBars
	************************************************/

	SAppScrollBarsValue::SAppScrollBarsValue()
	: flagDefined(sl_false), horizontalScrollBar(sl_false), verticalScrollBar(sl_false)
	{
	}

	sl_bool SAppScrollBarsValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			horizontalScrollBar = sl_false;
			verticalScrollBar = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "none") {
			flagDefined = sl_true;
			horizontalScrollBar = sl_false;
			verticalScrollBar = sl_false;
			return sl_true;
		} else if (str == "horizontal") {
			flagDefined = sl_true;
			horizontalScrollBar = sl_true;
			verticalScrollBar = sl_false;
			return sl_true;
		} else if (str == "vertical") {
			flagDefined = sl_true;
			horizontalScrollBar = sl_false;
			verticalScrollBar = sl_true;
			return sl_true;
		} else if (str == "both") {
			flagDefined = sl_true;
			horizontalScrollBar = sl_true;
			verticalScrollBar = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					Name
	************************************************/

	SAppNameValue::SAppNameValue()
	: flagDefined(sl_false)
	{
	}

	String SAppNameValue::getAccessString()
	{
		return value;
	}

	sl_bool SAppNameValue::parse(const String& _str)
	{
		String str = _str.trim();
		str = str.trim();
		if (str.isEmpty()) {
			value.setEmpty();
			flagDefined = sl_false;
			return sl_true;
		}
		if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
			return sl_false;
		}
		value = str;
		flagDefined = sl_true;
		return sl_true;
	}
	
	/************************************************
						Color
	************************************************/
	
	SAppColorValue::SAppColorValue()
	: flagDefined(sl_false)
	{
	}
	
	sl_bool SAppColorValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str.startsWith('@')) {
			str = str.substring(1);
			if (str.startsWith("color/")) {
				str = str.substring(6);
			} else {
				return sl_false;
			}
			str = str.trim();
			if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
				return sl_false;
			}
			resourceName = str;
			flagDefined = sl_true;
			return sl_true;
		} else {
			Color c;
			if (Color::parse(str, &c)) {
				color = c;
				flagDefined = sl_true;
				return sl_true;
			}
			return sl_false;
		}
		
	}
	
	/************************************************
					Drawable
	************************************************/

	SAppDrawableValue::SAppDrawableValue()
	: flagNull(sl_false), flagColor(sl_false), flagWhole(sl_false), func(FUNC_NONE)
	{
	}

	sl_bool SAppDrawableValue::parse(const String& _str)
	{
		String str = _str;
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str == "@null") {
			flagDefined = sl_true;
			flagNull = sl_true;
			return sl_true;
		}
		
		if (SAppColorValue::parse(str)) {
			if (flagDefined) {
				flagNull = sl_false;
				flagColor = sl_true;
			}
			return sl_true;
		}
		
		if (!(str.startsWith("@drawable/"))) {
			return sl_false;
		}
		str = str.substring(10);
		
		sl_char8* sz = str.getData();
		sl_size len = str.getLength();
		sl_size pos = 0;
		
		while (pos < len) {
			if (SLIB_CHAR_IS_C_NAME(sz[pos])) {
				pos++;
			} else {
				break;
			}
		}
		if (!(SAppUtil::checkName(sz, pos))) {
			return sl_false;
		}
		
		resourceName = String(sz, pos);
		flagNull = sl_false;
		flagWhole = sl_true;
		func = FUNC_NONE;
		
		for (; pos < len; pos++) {
			if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
				break;
			}
		}
		
		if (pos < len) {
			
			if (sz[pos] == '[') {
				
				pos++;
				
				float f[4];
				for (sl_size i = 0; i < 4; i++) {
					for (; pos < len; pos++) {
						if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
							break;
						}
					}
					if (pos >= len) {
						return sl_false;
					}
					sl_bool flagPlus = sl_false;
					if (sz[pos] == '+') {
						if (i >= 2) {
							flagPlus = sl_true;
							pos++;
							for (; pos < len; pos++) {
								if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
									break;
								}
							}
							if (pos >= len) {
								return sl_false;
							}
						} else {
							return sl_false;
						}
					}
					sl_reg iRet = _priv_SApp_parseFloat(f+i, sz, pos, len);
					if (iRet == SLIB_PARSE_ERROR) {
						return sl_false;
					}
					if (i >= 2) {
						if (!flagPlus) {
							f[i] -= f[i-2];
						}
						if (f[i] < 0) {
							return sl_false;
						}
					}
					pos = iRet;
					for (; pos < len; pos++) {
						if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
							break;
						}
					}
					if (pos >= len) {
						return sl_false;
					}
					if (i == 3) {
						if (sz[pos] != ']') {
							return sl_false;
						}
					} else {
						if (sz[pos] != ',') {
							return sl_false;
						}
					}
					pos++;
				}
				
				flagWhole = sl_false;
				x = f[0];
				y = f[1];
				width = f[2];
				height = f[3];
			}
			
			for (; pos < len; pos++) {
				if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
					break;
				}
			}
			
			if (sz[pos] == ',') {
				pos++;
				for (; pos < len; pos++) {
					if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
						break;
					}
				}
				if (pos >= len) {
					return sl_false;
				}
				
				sl_uint32 nFuncParams = 0;
				if (pos + 10 < len && Base::equalsMemory(sz + pos, "nine-patch", 10)) {
					func = FUNC_NINEPATCH;
					nFuncParams = 8;
					pos += 10;
				} else if (pos + 22 < len && Base::equalsMemory(sz + pos, "horizontal-three-patch", 22)) {
					func = FUNC_THREEPATCH_HORIZONTAL;
					nFuncParams = 4;
					pos += 22;
				} else if (pos + 20 < len && Base::equalsMemory(sz + pos, "vertical-three-patch", 20)) {
					func = FUNC_THREEPATCH_VERTICAL;
					nFuncParams = 4;
					pos += 20;
				} else {
					return sl_false;
				}
				if (func == FUNC_NINEPATCH || func == FUNC_THREEPATCH_HORIZONTAL || func == FUNC_THREEPATCH_VERTICAL) {
					for (; pos < len; pos++) {
						if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
							break;
						}
					}
					if (pos >= len) {
						return sl_false;
					}
					if (sz[pos] != '(') {
						return sl_false;
					}
					pos++;
					
					SAppDimensionValue f[8];
					sl_size i = 0;
					for (; i < nFuncParams; i++) {
						for (; pos < len; pos++) {
							if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
								break;
							}
						}
						if (pos >= len) {
							return sl_false;
						}
						sl_size posStart = pos;
						for (; pos < len; pos++) {
							if (!(SLIB_CHAR_IS_ALNUM(sz[pos]) || sz[pos] == '.' || sz[pos] == '%' || sz[pos] == '\t' || sz[pos] == ' ')) {
								break;
							}
						}
						String s = String(sz + posStart, pos - posStart);
						if (s.isEmpty()) {
							return sl_false;
						}
						if (!(f[i].parse(s))) {
							return sl_false;
						}
						if (!(f[i].flagDefined)) {
							return sl_false;
						}
						for (; pos < len; pos++) {
							if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
								break;
							}
						}
						if (pos >= len) {
							return sl_false;
						}
						if (sz[pos] != ',') {
							i++;
							break;
						}
						pos++;
					}
					if (pos >= len) {
						return sl_false;
					}
					if (sz[pos] != ')') {
						return sl_false;
					}
					pos++;
					if (func == FUNC_NINEPATCH) {
						if (i != 4 && i != 8) {
							return sl_false;
						}
						if (i == 4) {
							f[4] = f[0];
							f[5] = f[1];
							f[6] = f[2];
							f[7] = f[3];
						}
						if (f[4].unit != SAppDimensionValue::PX || f[4].amount < 0) {
							return sl_false;
						}
						if (f[5].unit != SAppDimensionValue::PX || f[5].amount < 0) {
							return sl_false;
						}
						if (f[6].unit != SAppDimensionValue::PX || f[6].amount < 0) {
							return sl_false;
						}
						if (f[7].unit != SAppDimensionValue::PX || f[7].amount < 0) {
							return sl_false;
						}
						if (!(f[0].checkGlobal()) || f[0].amount < 0) {
							return sl_false;
						}
						if (!(f[1].checkGlobal()) || f[1].amount < 0) {
							return sl_false;
						}
						if (!(f[2].checkGlobal()) || f[2].amount < 0) {
							return sl_false;
						}
						if (!(f[3].checkGlobal()) || f[3].amount < 0) {
							return sl_false;
						}
						patchLeftWidthDst = f[0];
						patchRightWidthDst = f[1];
						patchTopHeightDst = f[2];
						patchBottomHeightDst = f[3];
						patchLeftWidth = f[4].amount;
						patchRightWidth = f[5].amount;
						patchTopHeight = f[6].amount;
						patchBottomHeight = f[7].amount;
					} else {
						if (i != 2 && i != 4) {
							return sl_false;
						}
						if (i == 2) {
							f[2] = f[0];
							f[3] = f[1];
						}
						if (f[2].unit != SAppDimensionValue::PX || f[2].amount < 0) {
							return sl_false;
						}
						if (f[3].unit != SAppDimensionValue::PX || f[3].amount < 0) {
							return sl_false;
						}
						if (!(f[0].checkGlobal()) || f[0].amount < 0) {
							return sl_false;
						}
						if (!(f[1].checkGlobal()) || f[1].amount < 0) {
							return sl_false;
						}
						if (func == FUNC_THREEPATCH_HORIZONTAL) {
							patchLeftWidthDst = f[0];
							patchRightWidthDst = f[1];
							patchLeftWidth = f[2].amount;
							patchRightWidth = f[3].amount;
						} else {
							patchTopHeightDst = f[0];
							patchBottomHeightDst = f[1];
							patchTopHeight = f[2].amount;
							patchBottomHeight = f[3].amount;
						}
					}
				}
			}
			
			for (; pos < len; pos++) {
				if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
					return sl_false;
				}
			}
			
			if (pos < len) {
				return sl_false;
			}
			
		}
		
		flagDefined = sl_true;
		
		return sl_true;
		
	}

	sl_bool SAppDrawableValue::parseWhole(const String& _str)
	{
		String str = _str;
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str == "@null") {
			flagDefined = sl_true;
			flagNull = sl_true;
			return sl_true;
		}
		
		if (SAppColorValue::parse(str)) {
			if (flagDefined) {
				flagNull = sl_false;
				flagColor = sl_true;
			}
			return sl_true;
		}
		
		if (!(str.startsWith("@drawable/"))) {
			return sl_false;
		}
		str = str.substring(10).trim();
		if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
			return sl_false;
		}
		func = FUNC_NONE;
		flagWhole = sl_true;
		resourceName = str;
		flagNull = sl_false;
		flagDefined = sl_true;
		return sl_true;
	}

	/************************************************
					Menu
	************************************************/

	SAppMenuValue::SAppMenuValue()
	: flagDefined(sl_false), flagNull(sl_false)
	{
	}

	sl_bool SAppMenuValue::parse(const String& _str)
	{
		String str = _str;
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str == "@null") {
			flagDefined = sl_true;
			flagNull = sl_true;
			return sl_true;
		}
		if (!(str.startsWith("@menu/"))) {
			return sl_false;
		}
		str = str.substring(6).trim();
		if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
			return sl_false;
		}

		resourceName = str;
		flagNull = sl_false;
		flagDefined = sl_true;
		return sl_true;
	}


	/************************************************
					AlignLayout
	************************************************/

	SAppAlignLayoutValue::SAppAlignLayoutValue()
	: flagDefined(sl_false), flagAlignParent(sl_false)
	{
	}

	sl_bool SAppAlignLayoutValue::parse(const String& _str)
	{
		String str = _str.trim();
		str = str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str == "false") {
			flagDefined = sl_false;
			return sl_true;
		}
		if (str == "true") {
			flagAlignParent = sl_true;
			flagDefined = sl_true;
			return sl_true;
		}
		if (!(SAppUtil::checkName(str.getData(), str.getLength()))) {
			return sl_false;
		}
		referingView = str;
		flagAlignParent = sl_false;
		flagDefined = sl_true;
		return sl_true;
	}


	/************************************************
					Scrolling
	************************************************/

	SAppScrollingValue::SAppScrollingValue()
	: flagDefined(sl_false), horizontal(sl_false), vertical(sl_false)
	{
	}

	sl_bool SAppScrollingValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			horizontal = sl_false;
			vertical = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "horizontal") {
			flagDefined = sl_true;
			horizontal = sl_true;
			vertical = sl_false;
			return sl_true;
		} else if (str == "vertical") {
			flagDefined = sl_true;
			horizontal = sl_false;
			vertical = sl_true;
			return sl_true;
		} else if (str == "both") {
			flagDefined = sl_true;
			horizontal = sl_true;
			vertical = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
				LayoutOrientation
	************************************************/

	SAppLayoutOrientationValue::SAppLayoutOrientationValue()
	: flagDefined(sl_false), value(LayoutOrientation::Vertical)
	{
	}

	String SAppLayoutOrientationValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::LayoutOrientation::Vertical";
		}
		if (value == LayoutOrientation::Horizontal) {
			return "slib::LayoutOrientation::Horizontal";
		} else {
			return "slib::LayoutOrientation::Vertical";
		}
	}

	sl_bool SAppLayoutOrientationValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "horizontal") {
			value = LayoutOrientation::Horizontal;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "vertical") {
			value = LayoutOrientation::Vertical;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
						Alignment
	************************************************/

	SAppAlignmentValue::SAppAlignmentValue()
	: flagDefined(sl_false), value(Alignment::Center)
	{
	}

	String SAppAlignmentValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::LayoutOrientation::Vertical";
		}
		switch (value) {
			case Alignment::TopLeft:
				return "slib::Alignment::TopLeft";
			case Alignment::TopCenter:
				return "slib::Alignment::TopCenter";
			case Alignment::TopRight:
				return "slib::Alignment::TopRight";
			case Alignment::MiddleLeft:
				return "slib::Alignment::MiddleLeft";
			case Alignment::MiddleCenter:
				return "slib::Alignment::MiddleCenter";
			case Alignment::MiddleRight:
				return "slib::Alignment::MiddleRight";
			case Alignment::BottomLeft:
				return "slib::Alignment::BottomLeft";
			case Alignment::BottomCenter:
				return "slib::Alignment::BottomCenter";
			case Alignment::BottomRight:
				return "slib::Alignment::BottomRight";
		}
		return "slib::Alignment::MiddleCenter";
	}

	sl_bool SAppAlignmentValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		Alignment v = 0;
		ListLocker<String> items(str.split("|"));
		for (sl_size i = 0; i < items.count; i++) {
			String item = items[i].trim();
			if (item.isEmpty()) {
				return sl_false;
			}
			if (item == "top") {
				v |= Alignment::Top;
			} else if (item == "middle") {
			} else if (item == "bottom") {
				v |= Alignment::Bottom;
			} else if (item == "left") {
				v |= Alignment::Left;
			} else if (item == "center") {
			} else if (item == "right") {
				v |= Alignment::Right;
			} else {
				return sl_false;
			}
		}
		flagDefined = sl_true;
		value = v;
		return sl_true;
	}


	/************************************************
				ScaleMode
	************************************************/

	SAppScaleModeValue::SAppScaleModeValue()
	: flagDefined(sl_false), value(ScaleMode::None)
	{
	}

	String SAppScaleModeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::ScaleMode::None";
		}
		switch (value) {
			case ScaleMode::Stretch:
				return "slib::ScaleMode::Stretch";
			case ScaleMode::Contain:
				return "slib::ScaleMode::Contain";
			case ScaleMode::Cover:
				return "slib::ScaleMode::Cover";
			default:
				break;
		}
		return "slib::ScaleMode::None";
	}

	sl_bool SAppScaleModeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "stretch") {
			value = ScaleMode::Stretch;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "contain") {
			value = ScaleMode::Contain;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "cover") {
			value = ScaleMode::Cover;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "none") {
			value = ScaleMode::None;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
				BoundShape
	************************************************/

	SAppBoundShapeValue::SAppBoundShapeValue()
	: flagDefined(sl_false), value(BoundShape::Rectangle)
	{
	}

	String SAppBoundShapeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::BoundShape::Rectangle";
		}
		switch (value) {
			case BoundShape::Ellipse:
				return "slib::BoundShape::Ellipse";
			case BoundShape::RoundRect:
				return "slib::BoundShape::RoundRect";
			default:
				break;
		}
		return "slib::BoundShape::Rectangle";
	}

	sl_bool SAppBoundShapeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "rectangle") {
			value = BoundShape::Rectangle;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "ellipse") {
			value = BoundShape::Ellipse;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "round-rect") {
			value = BoundShape::RoundRect;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					RedrawMode
	************************************************/

	SAppRedrawModeValue::SAppRedrawModeValue()
	: flagDefined(sl_false), value(RedrawMode::Continuously)
	{
	}

	String SAppRedrawModeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::RedrawMode::Continuously";
		}
		switch (value) {
			case RedrawMode::WhenDirty:
				return "slib::RedrawMode::WhenDirty";
			default:
				break;
		}
		return "slib::RedrawMode::Continuously";
	}

	sl_bool SAppRedrawModeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "continuously") {
			value = RedrawMode::Continuously;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "when-dirty") {
			value = RedrawMode::WhenDirty;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
					MultiLineMode
	************************************************/

	SAppMultiLineModeValue::SAppMultiLineModeValue()
	: flagDefined(sl_false), value(MultiLineMode::Single)
	{
	}

	String SAppMultiLineModeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::MultiLineMode::Single";
		}
		switch (value) {
			case MultiLineMode::Multiple:
				return "slib::MultiLineMode::Multiple";
			case MultiLineMode::WordWrap:
				return "slib::MultiLineMode::WordWrap";
			case MultiLineMode::BreakWord:
				return "slib::MultiLineMode::BreakWord";
			default:
				break;
		}
		return "slib::MultiLineMode::Single";
	}

	sl_bool SAppMultiLineModeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "single" || str == "false") {
			value = MultiLineMode::Single;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "multiple" || str == "true") {
			value = MultiLineMode::Multiple;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "word-wrap") {
			value = MultiLineMode::WordWrap;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "break-word") {
			value = MultiLineMode::BreakWord;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	/************************************************
					UIReturnKeyType
	************************************************/

	SAppUIReturnKeyTypeValue::SAppUIReturnKeyTypeValue()
	: flagDefined(sl_false), value(UIReturnKeyType::Default)
	{
	}

	String SAppUIReturnKeyTypeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::UIReturnKeyType::Default";
		}
		switch (value) {
			case UIReturnKeyType::Default:
				return "slib::UIReturnKeyType::Default";
			case UIReturnKeyType::Return:
				return "slib::UIReturnKeyType::Return";
			case UIReturnKeyType::Done:
				return "slib::UIReturnKeyType::Done";
			case UIReturnKeyType::Search:
				return "slib::UIReturnKeyType::Search";
			case UIReturnKeyType::Next:
				return "slib::UIReturnKeyType::Next";
			case UIReturnKeyType::Continue:
				return "slib::UIReturnKeyType::Continue";
			case UIReturnKeyType::Go:
				return "slib::UIReturnKeyType::Go";
			case UIReturnKeyType::Send:
				return "slib::UIReturnKeyType::Send";
			case UIReturnKeyType::Join:
				return "slib::UIReturnKeyType::Join";
			case UIReturnKeyType::Route:
				return "slib::UIReturnKeyType::Route";
			case UIReturnKeyType::EmergencyCall:
				return "slib::UIReturnKeyType::EmergencyCall";
			case UIReturnKeyType::Google:
				return "slib::UIReturnKeyType::Google";
			case UIReturnKeyType::Yahoo:
				return "slib::UIReturnKeyType::Yahoo";
			default:
				break;
		}
		return "slib::UIReturnKeyType::Default";
	}

	sl_bool SAppUIReturnKeyTypeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "default") {
			value = UIReturnKeyType::Default;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "return") {
			value = UIReturnKeyType::Return;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "done") {
			value = UIReturnKeyType::Done;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "search") {
			value = UIReturnKeyType::Search;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "next") {
			value = UIReturnKeyType::Next;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "continue") {
			value = UIReturnKeyType::Continue;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "go") {
			value = UIReturnKeyType::Go;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "send") {
			value = UIReturnKeyType::Send;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "route") {
			value = UIReturnKeyType::Route;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "emergency") {
			value = UIReturnKeyType::EmergencyCall;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "google") {
			value = UIReturnKeyType::Google;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "yahoo") {
			value = UIReturnKeyType::Yahoo;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
				UIKeyboardType
	************************************************/

	SAppUIKeyboardTypeValue::SAppUIKeyboardTypeValue()
	: flagDefined(sl_false), value(UIKeyboardType::Default)
	{
	}

	String SAppUIKeyboardTypeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::UIKeyboardType::Default";
		}
		switch (value) {
			case UIKeyboardType::Default:
				return "slib::UIKeyboardType::Default";
			case UIKeyboardType::Numpad:
				return "slib::UIKeyboardType::Numpad";
			case UIKeyboardType::Phone:
				return "slib::UIKeyboardType::Phone";
			case UIKeyboardType::Email:
				return "slib::UIKeyboardType::Email";
			case UIKeyboardType::Decimal:
				return "slib::UIKeyboardType::Decimal";
			case UIKeyboardType::Alphabet:
				return "slib::UIKeyboardType::Alphabet";
			case UIKeyboardType::Url:
				return "slib::UIKeyboardType::Url";
			case UIKeyboardType::WebSearch:
				return "slib::UIKeyboardType::WebSearch";
			case UIKeyboardType::Twitter:
				return "slib::UIKeyboardType::Twitter";
			case UIKeyboardType::NumbersAndPunctuation:
				return "slib::UIKeyboardType::NumbersAndPunctuation";
			case UIKeyboardType::NamePhone:
				return "slib::UIKeyboardType::NamePhone";
			case UIKeyboardType::Ascii:
				return "slib::UIKeyboardType::Ascii";
			case UIKeyboardType::AsciiNumpad:
				return "slib::UIKeyboardType::AsciiNumpad";
			default:
				break;
		}
		return "slib::UIKeyboardType::Default";
	}

	sl_bool SAppUIKeyboardTypeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "default") {
			value = UIKeyboardType::Default;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "numpad") {
			value = UIKeyboardType::Numpad;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "phone") {
			value = UIKeyboardType::Phone;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "email") {
			value = UIKeyboardType::Email;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "decimal") {
			value = UIKeyboardType::Decimal;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "alphabet") {
			value = UIKeyboardType::Alphabet;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "url") {
			value = UIKeyboardType::Url;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "web-search") {
			value = UIKeyboardType::WebSearch;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "twitter") {
			value = UIKeyboardType::Twitter;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "numbers-and-punctuation") {
			value = UIKeyboardType::NumbersAndPunctuation;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "name-phone") {
			value = UIKeyboardType::NamePhone;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "ascii") {
			value = UIKeyboardType::Ascii;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "ascii-numpad") {
			value = UIKeyboardType::AsciiNumpad;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}


	/************************************************
			UIAutoCapitalizationType
	************************************************/

	SAppUIAutoCapitalizationTypeValue::SAppUIAutoCapitalizationTypeValue()
	: flagDefined(sl_false), value(UIAutoCapitalizationType::None)
	{
	}

	String SAppUIAutoCapitalizationTypeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::UIAutoCapitalizationType::None";
		}
		switch (value) {
			case UIAutoCapitalizationType::None:
				return "slib::UIAutoCapitalizationType::None";
			case UIAutoCapitalizationType::Words:
				return "slib::UIAutoCapitalizationType::Words";
			case UIAutoCapitalizationType::Sentences:
				return "slib::UIAutoCapitalizationType::Sentences";
			case UIAutoCapitalizationType::AllCharacters:
				return "slib::UIAutoCapitalizationType::AllCharacters";
			default:
				break;
		}
		return "slib::UIAutoCapitalizationType::None";
	}

	sl_bool SAppUIAutoCapitalizationTypeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "none") {
			value = UIAutoCapitalizationType::None;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "words") {
			value = UIAutoCapitalizationType::Words;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "sentences") {
			value = UIAutoCapitalizationType::Sentences;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "all") {
			value = UIAutoCapitalizationType::AllCharacters;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}

	/************************************************
	 				RotationMode
	************************************************/
	
	SAppRotationModeValue::SAppRotationModeValue()
	: flagDefined(sl_false), value(RotationMode::Rotate0)
	{
	}
	
	String SAppRotationModeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::RotationMode::Rotate0";
		}
		switch (value) {
			case RotationMode::Rotate0:
				return "slib::RotationMode::Rotate0";
			case RotationMode::Rotate90:
				return "slib::RotationMode::Rotate90";
			case RotationMode::Rotate180:
				return "slib::RotationMode::Rotate180";
			case RotationMode::Rotate270:
				return "slib::RotationMode::Rotate270";
			default:
				break;
		}
		return "slib::RotationMode::Rotate0";
	}
	
	sl_bool SAppRotationModeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "0") {
			value = RotationMode::Rotate0;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "90") {
			value = RotationMode::Rotate90;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "180") {
			value = RotationMode::Rotate180;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "270") {
			value = RotationMode::Rotate270;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}
	
	/************************************************
	 				FlipMode
	************************************************/
	
	SAppFlipModeValue::SAppFlipModeValue()
	: flagDefined(sl_false), value(FlipMode::None)
	{
	}
	
	String SAppFlipModeValue::getAccessString()
	{
		if (!flagDefined) {
			return "slib::FlipMode::None";
		}
		switch (value) {
			case FlipMode::None:
				return "slib::FlipMode::None";
			case FlipMode::Horizontal:
				return "slib::FlipMode::Horizontal";
			case FlipMode::Vertical:
				return "slib::FlipMode::Vertical";
			case FlipMode::Both:
				return "slib::FlipMode::Both";
			default:
				break;
		}
		return "slib::FlipMode::None";
	}
	
	sl_bool SAppFlipModeValue::parse(const String& _str)
	{
		String str = _str.trim();
		if (str.isEmpty()) {
			flagDefined = sl_false;
			return sl_true;
		}
		str = str.toLower();
		if (str == "none") {
			value = FlipMode::None;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "horizontal") {
			value = FlipMode::Horizontal;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "vertical") {
			value = FlipMode::Vertical;
			flagDefined = sl_true;
			return sl_true;
		} else if (str == "both") {
			value = FlipMode::Both;
			flagDefined = sl_true;
			return sl_true;
		}
		return sl_false;
	}
	
}
