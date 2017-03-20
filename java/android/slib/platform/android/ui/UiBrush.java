/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android.ui;

import android.graphics.Paint;
import android.graphics.Paint.Style;

public class UiBrush {

	public int style;
	public static final int STYLE_SOLID = 0;
	
	public int color;
	
	public void applyPaint(Paint paint, int alpha) {
		paint.setColor(Graphics.applyAlphaToColor(color, alpha));
		paint.setStyle(Style.FILL);
	}
}
