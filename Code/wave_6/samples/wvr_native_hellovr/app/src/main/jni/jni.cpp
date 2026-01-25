// "WaveVR SDK 
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."

#define  LOG_TAG    "hellovr_jni"
#include <jni.h>
#include <log.h>
#include <Context.h>
#include <hellovr.h>
#include <unistd.h>
#include <wvr/wvr.h>

// DEBUG can be disabled when the Activity ask.
extern bool gDebug;
extern bool gDebugOld;
extern bool gMsaa;
extern bool gScene;
extern bool gSceneOld;

// --- 1. GLOBAL APP POINTER ---
MainApplication *app = nullptr;
std::string g_cachedPath = "";

int main(int argc, char *argv[]) {
    LOGENTRY();
    LOGI("HelloVR main, new MainApplication ");
    app = new MainApplication();
    if (!g_cachedPath.empty()) {
        LOGI("Main: Applying cached path to app: %s", g_cachedPath.c_str());
        app->setExportPath(g_cachedPath);
    }
    LOGI("HelloVR main, start call app->initVR()");
    if (!app) return 1;
    if (!app->initVR()) {
        std::cout << "HelloVR main, initVR fail,start call app->shutdownVR()";
        LOGW("HelloVR main, initVR fail,start call app->shutdownVR()");
        app->shutdownVR();
        delete app;
        return 1;
    }

    if (!app->initGL()) {
        std::cout << "HelloVR main, initGL failed, start call app->shutdownVR()";
        LOGW("HelloVR main, initGL failed, start call app->shutdownVR()");
        app->shutdownGL();
        app->shutdownVR();
        delete app;
        return 1;
    }

    while (true) {
        if (app->handleInput())
            break;

        if (app->renderFrame()) {
            std::cout << "Unknown render error. Quit.";
            LOGE("Unknown render error. Quit.");
            break;
        }

        app->updateHMDMatrixPose();
    }

    app->shutdownGL();
    app->shutdownVR();
    std::cout << "App Endet Normaly";

    delete app;
    app = nullptr;
    return 0;
}

// Add helper to convert Java string to C++ string
std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr) return "";
    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));
    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);
    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);
    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

// Native function implementation
extern "C"
JNIEXPORT void JNICALL
Java_com_htc_vr_samples_wvr_1hellovr_MainActivity_setExportPath(JNIEnv *env, jobject instance, jstring path_) {
    std::string path = jstring2string(env, path_);
    g_cachedPath = path;
    // Pass to your MainApplication instance (ensure 'app' is accessible here)
    if (app != nullptr) {
        app->setExportPath(path);
        LOGI("JNI: Export path set to %s", path.c_str());
    }
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_htc_vr_samples_wvr_1hellovr_MainActivity_init(JNIEnv * env, jobject act, jobject am);
    JNIEXPORT void JNICALL Java_com_htc_vr_samples_wvr_1hellovr_MainActivity_setFlag(JNIEnv * env, jclass clazz, jint flag);
};

JNIEXPORT void JNICALL Java_com_htc_vr_samples_wvr_1hellovr_MainActivity_init(JNIEnv * env, jobject activityInstance, jobject assetManagerInstance) {
    LOGI("MainActivity_init: call  Context::getInstance()->init");
    Context::getInstance()->init(env, assetManagerInstance);
    LOGI("register WVR main when library loading");
    WVR_RegisterMain(main);
}

JNIEXPORT void JNICALL Java_com_htc_vr_samples_wvr_1hellovr_MainActivity_setFlag(JNIEnv * env, jclass clazz, jint flag) {
    // Flags are defined in java Activity.
    gDebug = (flag & 0x1) != 0;
    LOGD("gDebug = %d", gDebug ? 1 : 0);
    gMsaa = (flag & 0x2) != 0;
    LOGD("gMsaa = %d", gMsaa ? 1 : 0);
    gScene = (flag & 0x4) != 0;
    LOGD("gScene = %d", gScene ? 1 : 0);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    Context *ctx = new Context(vm);
    if (!ctx) return JNI_VERSION_1_6;

    return JNI_VERSION_1_6;
}

jint JNI_OnUnLoad(JavaVM* vm, void* reserved) {
    delete Context::getInstance();
    return 0;
}

