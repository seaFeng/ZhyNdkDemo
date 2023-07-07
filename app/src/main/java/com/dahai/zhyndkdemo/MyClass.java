package com.dahai.zhyndkdemo;

import android.util.Log;

public class MyClass {
    private static final String TAG = "MyClass";

    public void sayHello() {

        Log.d(TAG, "my calss say Hello " + getClass().getName());
    }
}
