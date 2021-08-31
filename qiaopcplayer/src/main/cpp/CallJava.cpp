//
// Created by qiaopc on 2021/8/29.
//这个类用于 c++ 调用 java方法
//
#include <jni.h>
#include "CallJava.h"
#include "AndroidLog.h"


CallJava::CallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj) {
    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = env->NewGlobalRef(*obj);//NewGlobalRef：创建全局引用，传入一个局部引用参数 , 将局部引用转为全局引用

    jclass jlz = jniEnv->GetObjectClass(jobj); //获取Java层对应的类
    if(!jlz) {
        if (LOG_DEBUG) {
            LOGE("get jclass wrong");
        }
        return;
    }

    jmid_prepared = env->GetMethodID(jlz, "onCallPrepared", "()V");//获取Java层被回调的函数
}

void CallJava::onCallPrepared(int type) {
    if (type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
    } else if (type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) { //从全局的JavaVM中获取到环境变量，获取到当前线程中的JNIEnv指针
            if (LOG_DEBUG) {
                LOGE("get child thread jnienv wrong");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
        javaVM->DetachCurrentThread();
    }
}
