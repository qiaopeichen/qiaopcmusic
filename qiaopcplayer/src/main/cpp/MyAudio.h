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
    AVCodecParameters *codecpar = NULL;
    AVCodecContext *avCodecContext = NULL;
    MyQueue *queue = NULL;
    Playstatus *playstatus = NULL;
public:
    MyAudio(Playstatus *playstatus);
    ~MyAudio();
};


#endif //QIAOPCMUSIC_MYAUDIO_H
