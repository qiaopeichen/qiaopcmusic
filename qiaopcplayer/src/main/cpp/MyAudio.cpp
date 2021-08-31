//
// Created by qiaopc on 2021/8/30.
//

#include "MyAudio.h"

MyAudio::MyAudio(Playstatus *playstatus) {
    this->playstatus = playstatus;
    queue = new MyQueue(playstatus);
    buffer = (uint8_t *) av_malloc(44100 * 2 * 2);
}

MyAudio::~MyAudio() {

}

void *decodePlay(void *data) {
    MyAudio *audio = (MyAudio *)(data);
    audio->resampleAudio();
    pthread_exit(&audio->thread_play);
}

void MyAudio::play() {
    pthread_create(&thread_play, NULL, decodePlay, this);
}

//FILE *outFile = fopen("/mnt/shared/Other/mymusic.pcm", "w");
FILE *outFile = fopen("/storage/emulated/0/Download/mymusic.pcm", "w");

//实现重采样
int MyAudio::resampleAudio() {
    while (playstatus != NULL && !playstatus->exit) {
        avPacket = av_packet_alloc();
        if (queue->getAvpacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == 0) {
            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {//声道数: 比如立体声 channels = 2
                //根据声道数获取声道布局
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }
            SwrContext *swr_ctx = NULL; //重采样结构体
            swr_ctx = swr_alloc_set_opts(NULL,
                                         AV_CH_LAYOUT_STEREO, //声道布局，立体声
                                         AV_SAMPLE_FMT_S16, //采样位数 16位
                                         avFrame->sample_rate, //采样率 输入输出保持一致
                                         avFrame->channel_layout, //声道布局
                                         (AVSampleFormat) avFrame->format,//采样位数
                                         avFrame->sample_rate, //采样率 输入输出保持一致
                                         NULL,
                                         NULL);
            if(!swr_ctx || swr_init(swr_ctx) < 0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;
                if (swr_ctx != NULL) {
                    swr_free(&swr_ctx);
                    swr_ctx = NULL;
                }
                continue;
            }

            int nb = swr_convert(
                    swr_ctx,
                    &buffer, //输出数据
                    avFrame->nb_samples, //输出采样个数
                    (const uint8_t **) avFrame->data, //原始数据
                    avFrame->nb_samples // 输入采样个数
                    );

            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            // 数据个数 * 声道数（2） * 采样位数（16/8）
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
            fwrite(buffer, 1, data_size, outFile);

            LOGE("data size is %d", data_size);
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            swr_free(&swr_ctx);
            swr_ctx = NULL;
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }

    return 0;
}
