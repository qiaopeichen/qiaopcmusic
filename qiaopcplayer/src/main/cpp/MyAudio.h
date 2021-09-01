//
// Created by qiaopc on 2021/8/30.
//

#ifndef QIAOPCMUSIC_MYAUDIO_H
#define QIAOPCMUSIC_MYAUDIO_H

#include "MyQueue.h"
#include "Playstatus.h"
#include "CallJava.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
};

class MyAudio {
public:
    int streamIndex = -1;
    AVCodecParameters *codecpar = NULL; // 包含音视频参数的结构体。很重要，可以用来获取音视频参数中的宽度、高度、采样率、编码格式等信息。
    AVCodecContext *avCodecContext = NULL; //关于编解码的结构体
    MyQueue *queue = NULL;
    Playstatus *playstatus = NULL;
    CallJava *callJava = NULL;

    pthread_t thread_play;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    int ret = -1;
    uint8_t *buffer = NULL;
    int data_size = NULL;
    int sample_rate = 0; //采样率

    int duration = 0; //播放时长
    AVRational time_base; //这个流的每一帧，持续的时间的分数表达式
    double now_time = 0;
    double clock = 0; //记录当前时间，递增
    double last_time = 0;

    //引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR; //石廊混音？

    //pcm
    SLObjectItf  pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf  pcmBufferQueue = NULL;


public:
    MyAudio(Playstatus *playstatus, int sample_rate, CallJava *callJava);
    ~MyAudio();

    void play();
    int resampleAudio();
    void initOpenSLES();
    int getCurrentSampleRateForOpensles(int sample_rate);

    void pause();
    void resume();
};


#endif //QIAOPCMUSIC_MYAUDIO_H
