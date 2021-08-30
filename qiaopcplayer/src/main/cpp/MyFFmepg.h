//
// Created by qiaopc on 2021/8/30.
//

#ifndef QIAOPCMUSIC_MYFFMEPG_H
#define QIAOPCMUSIC_MYFFMEPG_H

#include "CallJava.h"
#include "pthread.h"
#include "MyAudio.h"
extern "C"
{
#include "libavformat/avformat.h"
};

class MyFFmepg {
public:
    CallJava *callJava = NULL;
    const char *url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    MyAudio *audio = NULL;

public:
    MyFFmepg(CallJava *callJava, const char* url);
    ~MyFFmepg();
    void prepared();
    void decodeFFmpegThread();

    void start();
};


#endif //QIAOPCMUSIC_MYFFMEPG_H
