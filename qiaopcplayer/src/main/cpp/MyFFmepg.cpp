//
// Created by qiaopc on 2021/8/30.
//

#include "MyFFmepg.h"

MyFFmepg::MyFFmepg(Playstatus *playstatus, CallJava *callJava, const char *url) {
    this->callJava = callJava;
    this->url = url;
    this->playstatus = playstatus;
}

void *decodeFFmpeg(void *data) {
    MyFFmepg *myFFmepg = (MyFFmepg *) data;
    myFFmepg->decodeFFmpegThread();
    pthread_exit(&myFFmepg->decodeThread);
}

void MyFFmepg::prepared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

void MyFFmepg::decodeFFmpegThread() {
    av_register_all(); // 注册解码器
    avformat_network_init(); //网络初始化
    pFormatCtx = avformat_alloc_context(); // 上下文初始化
    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGE("can not open url: %s", url);
        }
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not find streams from url: %s", url);
        }
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) { //得到音频流
            if (audio == NULL) {
                audio = new MyAudio(playstatus);
                audio->streamIndex = i; //设置索引
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);
    if (!dec) {
        if (LOG_DEBUG) {
            LOGE("can not find decoder");
        }
        return;
    }
    audio->avCodecContext = avcodec_alloc_context3(dec);
    if (!audio->avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("can not alloc new decoderctx");
        }
        return;
    }

    //把解码器信息复制到解码器上下文中
    if (avcodec_parameters_to_context(audio->avCodecContext, audio->codecpar) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not find decoderctx");
        }
        return;
    }
    if (avcodec_open2(audio->avCodecContext, dec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("can not open audio streams");
        }
        return;
    }
    //回调java层
    callJava->onCallPrepared(CHILD_THREAD);
}

void MyFFmepg::start() {
    if (audio == NULL) {
        if (LOG_DEBUG) {
            LOGE("audio is null");
        }
        return;
    }
    int count = 0;
    while (1) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(pFormatCtx, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {
                count++;
                if (LOG_DEBUG) {
                    LOGE("解码第 %d 帧", count);
                }
                audio->queue->putAvpacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            break;
        }
    }

    //模拟出队
    while (audio->queue->getQueueSize() > 0) {
        AVPacket *avPacket = av_packet_alloc();
        audio->queue->getAvpacket(avPacket);
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
    }
    if (LOG_DEBUG) {
        LOGD("解码完成");
    }
}
