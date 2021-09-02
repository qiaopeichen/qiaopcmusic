package com.example.qiaopcplayer.player;

import android.text.TextUtils;

import com.example.qiaopcplayer.TimeInfoBean;
import com.example.qiaopcplayer.listener.OnCompleteListener;
import com.example.qiaopcplayer.listener.OnErrorListener;
import com.example.qiaopcplayer.listener.OnLoadListener;
import com.example.qiaopcplayer.listener.OnPauseResumeListener;
import com.example.qiaopcplayer.listener.OnPreparedListener;
import com.example.qiaopcplayer.listener.OnTimeInfoListener;
import com.example.qiaopcplayer.log.MyLog;

public class QiaopcPlayer {

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    private String source;

    private OnPreparedListener onPreparedListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private OnTimeInfoListener onTimeInfoListener;
    private OnErrorListener onErrorListener;
    private OnCompleteListener onCompleteListener;
    private static TimeInfoBean timeInfoBean;

    public void setSource(String source) {
        this.source = source;
    }

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    public void setOnLoadListener(OnLoadListener onLoadListener) {
        this.onLoadListener = onLoadListener;
    }

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener) {
        this.onPauseResumeListener = onPauseResumeListener;
    }

    public void setOnTimeInfoListener(OnTimeInfoListener onTimeInfoListener) {
        this.onTimeInfoListener = onTimeInfoListener;
    }

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public void setOnCompleteListener(OnCompleteListener onCompleteListener) {
        this.onCompleteListener = onCompleteListener;
    }

    public QiaopcPlayer() {

    }

    public void prepared() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source not be empty");
            return;
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                n_prepared(source);
            }
        }).start();
    }

    public void onCallPrepared() {
        if (onPreparedListener != null) {
            onPreparedListener.onPrerared();
        }
    }

    public void start() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("source not be empty");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_start();
            }
        }).start();
    }

    public void pause() {
        n_pause();
        if(onPauseResumeListener != null) {
            onPauseResumeListener.onPause(true);
        }
    }

    public void resume() {
        n_resume();
        if(onPauseResumeListener != null) {
            onPauseResumeListener.onPause(false);
        }
    }

    public void stop() {
        timeInfoBean = null;
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_stop();
            }
        }).start();
    }

    public void seek(int secds) {
        n_seek(secds);
    }

    public void onCallLoad(boolean load) {
        if (onLoadListener != null) {
            onLoadListener.onLoad(load);
        }
    }

    public void onCallTimeInfo(int currentTime, int totalTime) {
        if (onTimeInfoListener != null) {
            if (timeInfoBean == null) {
                timeInfoBean = new TimeInfoBean();
            }
            timeInfoBean.setCurrentTime(currentTime);
            timeInfoBean.setTotalTime(totalTime);
            onTimeInfoListener.onTimeInfo(timeInfoBean);
        }
    }

    public void onCallError(int code, String msg) {
        stop();
        if (onErrorListener != null) {
            onErrorListener.onError(code, msg);
        }
    }

    public void onCallComplete() {
        stop();
        if (onCompleteListener != null) {
            onCompleteListener.onComplete();
        }
    }

    private native void n_prepared(String source);
    private native void n_start();
    private native void n_pause();
    private native void n_resume();
    private native void n_stop();
    private native void n_seek(int secds);
}
