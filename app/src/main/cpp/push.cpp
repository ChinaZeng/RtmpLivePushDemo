#include <jni.h>

#include "RtmpPush.h"

CallJava *callJava = NULL;
JavaVM *jvm = NULL;

RtmpPush *rtmpPush;
bool  isExit= true;


extern "C"
JNIEXPORT void JNICALL
Java_com_zzw_live_rtmp_RtmpHelper_n_1init(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    if(!callJava)
        callJava = new CallJava(env, jvm, &instance);
    if(!rtmpPush)
        rtmpPush = new RtmpPush(url, callJava);

    isExit= false;
    rtmpPush->init();
    env->ReleaseStringUTFChars(url_, url);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_zzw_live_rtmp_RtmpHelper_n_1pushSPSPPS(JNIEnv *env, jobject instance, jbyteArray sps_,
                                                jint spsLen, jbyteArray pps_, jint ppsLen) {
    jbyte *sps = env->GetByteArrayElements(sps_, NULL);
    jbyte *pps = env->GetByteArrayElements(pps_, NULL);
    if (rtmpPush && !isExit) {
        rtmpPush->pushSPSPPS(reinterpret_cast<char *>(sps), spsLen, reinterpret_cast<char *>(pps),
                             ppsLen);
    }
    env->ReleaseByteArrayElements(sps_, sps, 0);
    env->ReleaseByteArrayElements(pps_, pps, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zzw_live_rtmp_RtmpHelper_n_1pushVideoData(JNIEnv *env, jobject instance, jbyteArray data_,
                                                   jint dataLen,jboolean keyFrame) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    if (rtmpPush && !isExit) {
        rtmpPush->pushVideoData(reinterpret_cast<char *>(data), dataLen,keyFrame);
    }
    env->ReleaseByteArrayElements(data_, data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zzw_live_rtmp_RtmpHelper_n_1pushAudioData(JNIEnv *env, jobject instance, jbyteArray data_,
                                                   jint dataLen) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);


    if (rtmpPush && !isExit) {
        rtmpPush->pushAudioData(reinterpret_cast<char *>(data), dataLen);
    }

    env->ReleaseByteArrayElements(data_, data, 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_zzw_live_rtmp_RtmpHelper_n_1stop(JNIEnv *env, jobject instance) {

    if (rtmpPush) {
        isExit = true;
        rtmpPush->pushStop();
        delete rtmpPush;
        delete callJava;

        rtmpPush=NULL;
        callJava =NULL;
    }
}


extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *javaVM, void *reserved) {
    jint result = -1;
    jvm = javaVM;
    JNIEnv *jniEnv = NULL;

    if ((result = javaVM->GetEnv((void **) (&jniEnv), JNI_VERSION_1_4)) != JNI_OK) {
        LOGE("GetEnv ERROR");
        return result;
    }
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    jvm = NULL;
}
