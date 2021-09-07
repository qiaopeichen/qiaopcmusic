//
// Created by qiaopc on 2021/9/7.
//

#include "MyVideo.h"

MyVideo::MyVideo(Playstatus *playstatus, CallJava *callJava) {
    this->playstatus = playstatus;
    this->callJava = callJava;
    queue = new MyQueue(playstatus);
}

MyVideo::~MyVideo() {

}

void * playVideo(void *data) {
    MyVideo *video = static_cast<MyVideo *>(data);

    while (video->playstatus != NULL && !video->playstatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (video->queue->getAvpacket(avPacket) == 0) {
            //解码渲染
            LOGE("线程中获取视频AVpacket");
        }
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
    }

    pthread_exit(&video->thread_play);
}

void MyVideo::play() {
    pthread_create(&thread_play, NULL, playVideo, this);
}
