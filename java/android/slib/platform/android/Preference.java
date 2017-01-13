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
