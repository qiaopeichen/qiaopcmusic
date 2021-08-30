//
// Created by qiaopc on 2021/8/29.
//

#ifndef QIAOPCMUSIC_CALLJAVA_H
#define QIAOPCMUSIC_CALLJAVA_H

#include "AndroidLog.h"
#include <jni.h>
#include <cwchar>
#include "AndroidLog.h"


#define MAIN_THREAD 1
#define CHILD_THREAD 0

class CallJava {
public:
    _JavaVM *javaVM = NULL;
    _JNIEnv *jniEnv = NULL;
    jobject jobj;

    jmethodID jmid_prepared;
public:
    CallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj);
    ~CallJava();

    void onCallPrepared(int type);
};


#endif //QIAOPCMUSIC_CALLJAVA_H
