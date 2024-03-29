#include <cstring>
#include <jni.h>
#include <cinttypes>
#include <android/log.h>
#include <string>
#include <gmath.h>
#include <gperf.h>

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, "hello-libs::", __VA_ARGS__))

void test() {

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dahai_zhyndkdemo_MainActivity_getStringFromJNI2(JNIEnv *env, jobject thiz) {
    auto ticks = GetTicks();

    for (int i = 0; i < 31; ++i) {
        volatile unsigned val = gpower(i);
        //LOGE("gpower(%d) = %d", i, val);
    }

    ticks = GetTicks() - ticks;
    LOGE("calculation time: %" PRIu64, ticks);
    return env->NewStringUTF("Hello from JNI LIBS! ");
}

