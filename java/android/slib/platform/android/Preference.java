/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package slib.platform.android;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;

/**
 * Created by strongman on 12/29/16.
 */

public class Preference {

    public static void setValue(SlibActivity activity, String key, String value) {
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(activity);
        SharedPreferences.Editor editor = sp.edit();
        editor.putString(key, value);
        editor.commit();
    }

    public static String getValue(SlibActivity activity, String key) {
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(activity);
        String value = sp.getString(key, null);
        return value;
    }

}
