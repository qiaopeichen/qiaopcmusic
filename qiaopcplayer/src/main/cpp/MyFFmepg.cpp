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

    /**
     * int pthread_create(
                 pthread_t *restrict tidp,   //新创建的线程ID指向的内存单元。
                 const pthread_attr_t *restrict attr,  //线程属性，默认为NULL
                 void *(*start_rtn)(void *), //新创建的线程从start_rtn函数的地址开始运行
                 void *restrict arg //默认为NULL。若上述函数需要参数，将参数放入结构中并将地址作为arg传入。
                  );
     */
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);//它的功能是创建线程（实际上就是确定调用该线程函数的入口点），在线程创建以后，就开始运行相关的线程函数。
}

void MyFFmepg::decodeFFmpegThread() {
    av_register_all(); // 注册解码器
    avformat_network_init(); //网络初始化
    pFormatCtx = avformat_alloc_context(); // 上下文初始化
    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) { //该函数用于打开多媒体数据并且获得一些相关的信息。它的声明位于libavformat\avformat.h
        if (LOG_DEBUG) {
            LOGE("can not open url: %s", url);
        }
        return;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        //avformat_find_stream_info主要是读一些包（packets ），然后从中提取初流的信息。有一些文件格式没有头，比如说MPEG格式的，
        //这个时候，这个函数就很有用，因为它可以从读取到的包中获得到流的信息。在MPEG-2重复帧模式的情况下，该函数还计算真实的帧率。
        //逻辑文件位置不被此函数更改; 读出来的包会被缓存起来供以后处理。
        if (LOG_DEBUG) {
            LOGE("can not find streams from url: %s", url);
        }
        return;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) { //pFormatCtx->streams 是一个 AVStream 指针的数组，里面包含了媒体资源的每一路流信息，数组的大小为 pFormatCtx->nb_streams
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) { //得到音频流
            if (audio == NULL) {
                audio = new MyAudio(playstatus);
                audio->streamIndex = i; //设置索引
                audio->codecpar = pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codecpar->codec_id);//该方法用于寻找解码器；codec_id：编码数据类型
    if (!dec) {
        if (LOG_DEBUG) {
            LOGE("can not find decoder");
        }
        return;
    }
    audio->avCodecContext = avcodec_alloc_context3(dec);//分配解码器
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
    if (avcodec_open2(audio->avCodecContext, dec, 0) != 0) { //avcodec_open2 该函数用于初始化一个视音频编解码器的AVCodecContext
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
