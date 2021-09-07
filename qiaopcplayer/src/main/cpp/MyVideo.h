//
// Created by qiaopc on 2021/9/7.
//

#ifndef QIAOPCMUSIC_MYVIDEO_H
#define QIAOPCMUSIC_MYVIDEO_H


#include "MyQueue.h"
#include "CallJava.h"

extern "C" {
#include "include/libavcodec/avcodec.h"

};

class MyVideo {
public:
    int streamIndex = -1; //流的索引
    AVCodecContext *avCodecContext = NULL;// 解码器上下文
    AVCodecParameters *codecpar = NULL;
    MyQueue *queue = NULL;
    Playstatus *playstatus = NULL;
    CallJava *callJava = NULL;
    AVRational time_base; //这个流的每一帧，持续的时间的分数表达式

    pthread_t thread_play;
public:
    MyVideo(Playstatus *playstatus, CallJava *callJava);
    ~MyVideo();
    void play();
};


#endif //QIAOPCMUSIC_MYVIDEO_H
