#include <jni.h>
#include <string>
#include "CallJava.h"
#include "MyFFmepg.h"

extern "C"
{
#include <libavformat/avformat.h>
}

JavaVM *javaVM = NULL;
CallJava *callJava = NULL;
MyFFmepg *fFmepg = NULL;
Playstatus *playstatus = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {// 在加载动态链接库的时候，JVM会调用JNI_OnLoad(JavaVM* jvm, void* reserved)（如果定义了该函数）。
    jint result = -1;
    javaVM = vm; //JavaVM,英文全称是Java virtual machine，就是Java虚拟机。
    JNIEnv *env; //JNIEnv,英文全称是Java Native Interface Environment，就是Java本地接口环境。JNIEnv只在当前线程中有效。本地方法不能将JNIEnv从一个线程传递到另一个线程中
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1prepared(JNIEnv *env, jobject instance,
                                                              jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);

    // TODO
    if (fFmepg == NULL) {
        if (callJava == NULL) {
            callJava = new CallJava(javaVM, env, &instance);
        }
        playstatus = new Playstatus();
        fFmepg = new MyFFmepg(playstatus, callJava, source); // 把Java层被回调的函数传入FFmepg类中
        fFmepg->prepared();
    }

//    env->ReleaseStringUTFChars(source_, source);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1start(JNIEnv *env, jobject instance) {

    if (fFmepg != NULL) {
        fFmepg->start();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1pause(JNIEnv *env, jobject instance) {
    if(fFmepg != NULL) {
        fFmepg->pause();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1resume(JNIEnv *env, jobject instance) {
    if(fFmepg != NULL) {
        fFmepg->resume();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1stop(JNIEnv *env, jobject instance) {

    if (fFmepg != NULL) {
        fFmepg->release();
        delete(fFmepg);
        fFmepg = NULL;
        if (callJava != NULL) {
            delete(callJava);
            callJava = NULL;
        }
        if (playstatus != NULL) {
            delete(playstatus);
            playstatus = NULL;
        }
    }
}