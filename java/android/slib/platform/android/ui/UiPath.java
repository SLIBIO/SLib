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

package slib.platform.android.ui;

import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.Region;

public class UiPath {
	
	public final static int FILL_WINDING = 0;
	public final static int FILL_ALTERNATE = 1;
	
	public static Path.FillType getFillType(int type) {
		if (type == FILL_WINDING) {
			return Path.FillType.WINDING;
		} else {
			return Path.FillType.EVEN_ODD;
		}
	}

	public Path path;
	float lastX = 0;
	float lastY = 0;
	boolean flagMoveAuto = false;
	
	public Region region;
	public RectF bounds;
	
	public UiPath() {
		path = new android.graphics.Path();
		path.setFillType(Path.FillType.WINDING);
	}
	
	public void setFillMode(int mode) {
		path.setFillType(getFillType(mode));
		invalidate();		
	}
	
	public void moveTo(float x, float y) {
		path.moveTo(x, y);
		lastX = x;
		lastY = y;
		flagMoveAuto = false;
		invalidate();
	}
	
	public void lineTo(float x, float y) {
		if (flagMoveAuto) {
			path.moveTo(lastX, lastY);
			flagMoveAuto = false;
		}
		path.lineTo(x, y);
		lastX = x;
		lastY = y;
		invalidate();
	}
	
	public void cubicTo(float xc1, float yc1, float xc2, float yc2, float x, float y) {
		if (flagMoveAuto) {
			path.moveTo(lastX, lastY);
			flagMoveAuto = false;
		}
		path.cubicTo(xc1, yc1, xc2, yc2, x, y);
		lastX = x;
		lastY = y;
		invalidate();
	}
	
	public void closeSubpath() {
		path.close();
		flagMoveAuto = true;
		invalidate();
	}

	public void invalidate() {
		region = null;
		bounds = null;
	}
	
	public RectF getBounds() {
		RectF ret = bounds;
		if (ret != null) {
			return ret;
		}
		ret = new RectF();
		path.computeBounds(ret, true);
		bounds = ret;
		return ret;
	}

	public Region getRegion() {
		Region ret = region;
		if (ret != null) {
			return ret;
		}
		ret = new Region();
		RectF bounds = getBounds();
		ret.setPath(path, new Region((int)(bounds.left), (int)(bounds.top), (int)(bounds.right), (int)(bounds.bottom)));
		region = ret;
		return ret;
	}
	
	public boolean containsPoint(float x, float y) {
		Region region = getRegion();
		return region.contains((int)x, (int)y);
	}
}
