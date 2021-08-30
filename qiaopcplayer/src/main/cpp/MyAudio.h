//
// Created by qiaopc on 2021/8/30.
//

#ifndef QIAOPCMUSIC_MYAUDIO_H
#define QIAOPCMUSIC_MYAUDIO_H

extern "C" {
#include <libavcodec/avcodec.h>

};

class MyAudio {
public:
    int streamIndex = -1;
    AVCodecParameters *codecpar = NULL;
    AVCodecContext *avCodecContext = NULL;
public:
    MyAudio();
    ~MyAudio();
};


#endif //QIAOPCMUSIC_MYAUDIO_H
