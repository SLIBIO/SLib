/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android.ui.view;

import android.graphics.Rect;

public interface IView {

    long getInstance();

    void setInstance(long instance);

    Rect getUIFrame();

    void setUIFrame(int left, int top, int right, int bottom);

}
