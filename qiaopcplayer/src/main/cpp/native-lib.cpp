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

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
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
        fFmepg = new MyFFmepg(callJava, source);
        fFmepg->prepared();
    }

//    env->ReleaseStringUTFChars(source_, source);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_qiaopcplayer_player_QiaopcPlayer_n_1start(JNIEnv *env, jobject instance) {

    // TODO
    if (fFmepg != NULL) {
        fFmepg->start();
    }
}