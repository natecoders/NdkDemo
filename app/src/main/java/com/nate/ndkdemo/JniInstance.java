package com.nate.ndkdemo;

import android.util.Log;

public class JniInstance {

    public static void callStaticMethod(String str) {
        Log.e("nate", "callStaticMethod called -->" + str);
    }

    public void callJavaInstanceMethod(String str) {
        Log.e("nate", "callJavaInstanceMethod called -->" + str);
    }
}
