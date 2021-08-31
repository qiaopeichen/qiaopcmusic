//
// Created by qiaopc on 2021/8/30.
//

#ifndef QIAOPCMUSIC_MYAUDIO_H
#define QIAOPCMUSIC_MYAUDIO_H

#include "MyQueue.h"
#include "Playstatus.h"
extern "C" {
#include <libavcodec/avcodec.h>

};

class MyAudio {
public:
    int streamIndex = -1;
    AVCodecParameters *codecpar = NULL; // 包含音视频参数的结构体。很重要，可以用来获取音视频参数中的宽度、高度、采样率、编码格式等信息。
    AVCodecContext *avCodecContext = NULL; //关于编解码的结构体
    MyQueue *queue = NULL;
    Playstatus *playstatus = NULL;
public:
    MyAudio(Playstatus *playstatus);
    ~MyAudio();
};


#endif //QIAOPCMUSIC_MYAUDIO_H
