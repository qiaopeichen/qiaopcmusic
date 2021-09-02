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
    jmethodID jmid_load;
    jmethodID jmid_timeinfo;
    jmethodID jmid_error;
    jmethodID jmid_complete;
public:
    CallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj);
    ~CallJava();

    void onCallPrepared(int type);

    void onCallLoad(int type, bool load);

    void onCallTimeInfo(int type, int curr, int total);

    void CallError(int type, int code, char *msg);

    void onCallComplete(int type);
};


#endif //QIAOPCMUSIC_CALLJAVA_H
