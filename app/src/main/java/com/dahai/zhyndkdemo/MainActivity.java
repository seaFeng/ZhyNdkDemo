package com.dahai.zhyndkdemo;

import androidx.annotation.Keep;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.widget.TextView;

import com.dahai.zhyndkdemo.databinding.ActivityMainBinding;

/**
 *  时钟显示，一秒更新一下UI,
 */
public class MainActivity extends AppCompatActivity {
    int hour = 0;
    int minute = 0;
    int second = 0;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        initView();
    }

    private void initView() {
        // Example of a call to a native method
        binding.sampleText.setText(stringFromJNI());

        binding.sampleText2.setText(getStringFromJNI2());
    }

    @Override
    protected void onResume() {
        super.onResume();
        startTicks();
    }

    @Override
    protected void onStop() {
        super.onStop();
        stopTicks();
    }

    /*
     * A function calling from JNI to update current timer
     */
    @Keep
    private void updateTimer() {
        ++second;
        if(second >= 60) {
            ++minute;
            second -= 60;
            if(minute >= 60) {
                ++hour;
                minute -= 60;
            }
        }
        runOnUiThread(() -> {
            String ticks = "" + MainActivity.this.hour + ":" +
                    MainActivity.this.minute + ":" +
                    MainActivity.this.second;
            binding.sampleText.setText(ticks);
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void startTicks();
    public native void stopTicks();

    public native String getStringFromJNI2();
}