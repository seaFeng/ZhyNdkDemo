#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

void queryRuntimeInfo(JNIEnv* env, jobject object);

static const char* kTAG = "jni-TAG";
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

typedef struct tick_context {
    JavaVM* javaVM;
    jclass jniHelperClz;
    jobject  jniHelperObj;
    jclass  mainActivityClz;
    jobject mainActivityObj;
    pthread_mutex_t lock;
    int done;
} TickContext;
TickContext g_ctx;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_ctx.javaVM = vm;

    if ((*vm).GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // 创建 JniHandler 对象
    jclass clz = env->FindClass("com/dahai/zhyndkdemo/JniHandler");
    //g_ctx.jniHelperClz = static_cast<jclass>(env->NewGlobalRef(clz));
    g_ctx.jniHelperClz = reinterpret_cast<jclass>(env->NewGlobalRef(clz));
    jmethodID jniHelperCtor = env->GetMethodID(g_ctx.jniHelperClz, "<init>", "()V");
    jobject helperObject = env->NewObject(g_ctx.jniHelperClz, jniHelperCtor);
    g_ctx.jniHelperObj = env->NewGlobalRef(helperObject);

    queryRuntimeInfo(env, g_ctx.jniHelperObj);

    return JNI_VERSION_1_6;
}

void queryRuntimeInfo(JNIEnv* env, jobject object) {
    // 获取 getBuildVersion 方法
    jmethodID versionFun = env->GetStaticMethodID(g_ctx.jniHelperClz, "getBuildVersion", "()Ljava/lang/String;");
    if (!versionFun) {  // 获取方法失败
        LOGE("Failed to retrieve getBuildVersion() methodID @ line %d",
            __LINE__);
        return;
    }

    LOGE("success to retrieve getBuildVersion() methodID @ line %d",
         __LINE__);

    // 调用 getBuildVersion 方法
    jstring buildVersion = static_cast<jstring>(env->CallStaticObjectMethod(g_ctx.jniHelperClz, versionFun));
    const char* version = env->GetStringUTFChars(buildVersion, nullptr);
    if (!version) {
        LOGE("Unable to get version string @ line %d", __LINE__);
        return;
    }
    LOGE("get version string @ line %d", __LINE__);

    LOGE("Android version - %s ", version);

    env->ReleaseStringUTFChars(buildVersion, version);

    //env->DeleteGlobalRef(buildVersion);

    // 調用 getRuntimeMemorySize 信息
    jmethodID memFunc = env->GetMethodID(g_ctx.jniHelperClz, "getRuntimeMemorySize", "()J");
    if (!memFunc) {
        LOGE("Failed to retrieve getRuntimeMemorySize() methodID @ line %d",
             __LINE__);
        return;
    }

    jlong result = env->CallLongMethod(g_ctx.jniHelperObj, memFunc);
    LOGE("Runtime free memory size: %lld" , result);

}

extern "C" JNIEXPORT jstring JNICALL
Java_com_dahai_zhyndkdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    #if defined(__arm__)
        #if defined(__ARM_ARCH_7A__)
        #if defined(__ARM_NEON__)
          #if defined(__ARM_PCS_VFP)
            #define ABI "armeabi-v7a/NEON (hard-float)"
          #else
            #define ABI "armeabi-v7a/NEON"
          #endif
        #else
          #if defined(__ARM_PCS_VFP)
            #define ABI "armeabi-v7a (hard-float)"
          #else
            #define ABI "armeabi-v7a"
          #endif
        #endif
      #else
       #define ABI "armeabi"
      #endif
    #elif defined(__i386__)
    #define ABI "x86"
    #elif defined(__x86_64__)
        #define ABI "x86_64"
    #elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
    #define ABI "mips64"
    #elif defined(__mips__)
    #define ABI "mips"
    #elif defined(__aarch64__)
    #define ABI "arm64-v8a"
    #else
    #define ABI "unknown"
    #endif

    std::string hello = "Hello from C++";
    LOGE("%s", hello.data());
    jint jniVersion = env->GetVersion();
    LOGE("%s%d", "version=",jniVersion);
    __android_log_print(ANDROID_LOG_ERROR, kTAG, "%s", hello.c_str());

    /* 查找Java类 */
    jclass myClass = env->FindClass("com/dahai/zhyndkdemo/MyClass");
    if(myClass == NULL) {
        printf("找不到Java类\n");
    }

    /* 创建Java对象并调用方法 */
    jmethodID myMethod = env->GetMethodID(myClass, "sayHello", "()V");
    jobject myObject = env->AllocObject(myClass);
    env->CallVoidMethod(myObject, myMethod);


    /* 获取Java类的超类 */
    jclass superClass = env->GetSuperclass(myClass);

    if(superClass == nullptr) {
        printf("Java类没有超类\n");
    }


    /* 获取Java对象的类 */
    jclass myObjectClass = env->GetObjectClass(myObject);
    if(myObjectClass == nullptr) {
        printf("无法获取Java对象的类\n");
    }

    return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
}

void sendJavaMsg(JNIEnv* env, jobject instance, jmethodID func, const char* msg) {
    LOGE("%s", "send java msg == ");
    LOGE("%s", msg);
    jstring javaMsg = env->NewStringUTF(msg);
    env->CallVoidMethod(instance, func, javaMsg);
    env->DeleteLocalRef(javaMsg);
}

/*
 * Main working thread function. From a pthread,
 *     calling back to MainActivity::updateTimer() to display ticks on UI
 *     calling back to JniHelper::updateStatus(String msg) for msg
 */
void*  updateTicks(void* context) {
    __android_log_print(ANDROID_LOG_ERROR, kTAG, "%s", "updateTicks");

    TickContext* pctx = (TickContext *)(context);
    JavaVM* javaVm = pctx->javaVM;
    JNIEnv* jniEnv;
    jint res = javaVm->GetEnv((void**)&jniEnv,JNI_VERSION_1_6);
    LOGE("获取 env %d", res);
    if (res != JNI_OK) {
        res = javaVm->AttachCurrentThread(&jniEnv, nullptr);
        if (res != JNI_OK) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return nullptr;
        }
    }

    // 获取 JniHandler 的 updateStatus 方法
    jmethodID updateStatusFun = jniEnv->GetMethodID(pctx->jniHelperClz, "updateStatus", "(Ljava/lang/String;)V");
    LOGE("%s", "TickerThread status: initializing...");
    sendJavaMsg(jniEnv, pctx->jniHelperObj, updateStatusFun, "TickerThread status: initializing...");

    //get MainActivity 的 updateTimer function 方法
    jmethodID updateTimerFun = jniEnv->GetMethodID(pctx->mainActivityClz, "updateTimer", "()V");

    struct timeval beginTime{}, curTime{}, usedTime, leftTime{};
    const struct timeval kOneSecond = {
            (__kernel_time_t)1,
            (__kernel_suseconds_t) 0
    };

    sendJavaMsg(jniEnv, pctx->jniHelperObj, updateStatusFun, "TickerThread status: start ticking ...");

    while (true) {
        gettimeofday(&beginTime, nullptr);
        pthread_mutex_lock(&pctx->lock);
        int done = pctx->done;
        if (pctx->done) {
            pctx->done = 0;
        }
        pthread_mutex_unlock(&pctx->lock);

        if (done) {
            break;
        }
        // 去更新 MainActivity 的UI显示。
        jniEnv->CallVoidMethod(pctx->mainActivityObj, updateTimerFun);

        gettimeofday(&curTime, nullptr);

        timersub(&curTime, &beginTime, &usedTime);
        timersub(&kOneSecond, &usedTime, &leftTime);

        struct timespec sleepTime;
        sleepTime.tv_sec = leftTime.tv_sec;
        sleepTime.tv_nsec = leftTime.tv_usec * 1000;
        LOGE("睡眠时间%ld", sleepTime.tv_nsec);
        if (sleepTime.tv_sec <= 1) {
            nanosleep(&sleepTime, nullptr);
        } else {
            sendJavaMsg(jniEnv, pctx->jniHelperObj, updateStatusFun,
                        "TickerThread error: processing too long!");
        }

    }

    sendJavaMsg(jniEnv, pctx->jniHelperObj, updateStatusFun, "TickerThread status: ticking stopped");

    javaVm->DetachCurrentThread();

    return context;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dahai_zhyndkdemo_MainActivity_startTicks(JNIEnv *env, jobject thiz) {
    pthread_t pthread;
    pthread_attr_t pthreadAttr;

    pthread_attr_init(&pthreadAttr);
    pthread_attr_setdetachstate(&pthreadAttr, PTHREAD_CREATE_DETACHED);

    // 初始化线程锁
    pthread_mutex_init(&g_ctx.lock, nullptr);

    jclass clz = env->GetObjectClass(thiz);
    g_ctx.mainActivityClz = static_cast<jclass>(env->NewGlobalRef(clz));
    g_ctx.mainActivityObj = env->NewGlobalRef(thiz);

    int result = pthread_create(&pthread, nullptr, updateTicks, &g_ctx);

    __android_log_print(ANDROID_LOG_ERROR, kTAG, "%d", result);

    if (result == 0) {
        assert(result == 0);
    }

    pthread_attr_destroy(&pthreadAttr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_dahai_zhyndkdemo_MainActivity_stopTicks(JNIEnv *env, jobject thiz) {
    pthread_mutex_lock(&g_ctx.lock);
    g_ctx.done = 1;
    pthread_mutex_unlock(&g_ctx.lock);

    struct timespec sleepTime;
    memset(&sleepTime, 0, sizeof(sleepTime));
    sleepTime.tv_nsec = 100000000;
    while (g_ctx.done) {
        nanosleep(&sleepTime, nullptr);
    }

    // release object we allocated from StartTicks() function
    env->DeleteGlobalRef(g_ctx.mainActivityClz);
    env->DeleteGlobalRef(g_ctx.mainActivityObj);
    g_ctx.mainActivityObj = nullptr;
    g_ctx.mainActivityClz = nullptr;

    pthread_mutex_destroy(&g_ctx.lock);
}