//
// Created by admin on 2018/9/18.
//

#include "CallJava.h"


CallJava::CallJava(JNIEnv *jniEnv, JavaVM *javaVM, const jobject *jobj) {
    this->jniEnv = jniEnv;
    this->javaVM = javaVM;
    //设置为全局
    this->jobj = jniEnv->NewGlobalRef(*jobj);
    jclass jcs = jniEnv->GetObjectClass(this->jobj);

    this->jmid_conn = jniEnv->GetMethodID(jcs, "onConntecting", "()V");
    this->jmid_conns = jniEnv->GetMethodID(jcs, "onConntectSuccess", "()V");
    this->jmid_connf = jniEnv->GetMethodID(jcs, "onConntectFail", "(Ljava/lang/String;)V");
}

CallJava::~CallJava() {
    this->jniEnv->DeleteGlobalRef(this->jobj);
    this->javaVM = NULL;
    this->jniEnv = NULL;
}

void CallJava::conn(int type) {
    if (type == THREAD_MAIN) {
        this->jniEnv->CallVoidMethod(jobj, jmid_conn);
    } else {
        JNIEnv *jniEnv;
        if (this->javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_conn);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::connf(int type, char *msg) {

    if (type == THREAD_MAIN) {
        jstring s = this->jniEnv->NewStringUTF(msg);
        this->jniEnv->CallVoidMethod(jobj, jmid_connf, s);
        this->jniEnv->ReleaseStringUTFChars(s, msg);
    } else {
        JNIEnv *jniEnv;
        if (this->javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            return;
        }
        jstring s = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid_connf, s);
        jniEnv->DeleteLocalRef(s);
        javaVM->DetachCurrentThread();
    }
}

void CallJava::conns(int type) {
    if (type == THREAD_MAIN) {
        this->jniEnv->CallVoidMethod(jobj, jmid_conns);
    } else {
        JNIEnv *jniEnv;
        if (this->javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_conns);
        javaVM->DetachCurrentThread();
    }
}