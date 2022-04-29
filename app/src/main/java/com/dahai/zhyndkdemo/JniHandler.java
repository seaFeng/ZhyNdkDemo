package com.dahai.zhyndkdemo;

import android.os.Build;
import android.util.Log;

import androidx.annotation.Keep;

public class JniHandler {

    /*
     * Print out status to logcat
     */
    @Keep
    private void updateStatus(String msg) {
        if (msg.toLowerCase().contains("error")) {
            Log.e("JniHandler", "Native Err: " + msg);
        } else {
            Log.e("JniHandler", "Native Msg: " + msg);
        }
    }

    @Keep
    static public String getBuildVersion() {
        return Build.VERSION.RELEASE;
    }

    @Keep
    public long getRuntimeMemorySize() {
        return Runtime.getRuntime().freeMemory();
    }
}
