package com.example.myapplication;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.qiaopcplayer.Demo;
import com.example.qiaopcplayer.TimeInfoBean;
import com.example.qiaopcplayer.listener.OnLoadListener;
import com.example.qiaopcplayer.listener.OnPauseResumeListener;
import com.example.qiaopcplayer.listener.OnPreparedListener;
import com.example.qiaopcplayer.listener.OnTimeInfoListener;
import com.example.qiaopcplayer.log.MyLog;
import com.example.qiaopcplayer.player.QiaopcPlayer;
import com.example.qiaopcplayer.util.TimeUtil;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.

    private QiaopcPlayer qiaopcPlayer;
    private TextView tvTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvTime = findViewById(R.id.tv_time);
        qiaopcPlayer = new QiaopcPlayer();
        qiaopcPlayer.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrerared() {
                MyLog.d("准备好了，可以开始播放声音了");
                qiaopcPlayer.start();
            }
        });
        qiaopcPlayer.setOnLoadListener(new OnLoadListener() {
            @Override
            public void onLoad(boolean load) {
                if (load) {
                    MyLog.d("加载中");
                } else {
                    MyLog.d("播放中");
                }
            }
        });
        qiaopcPlayer.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                if (pause) {
                    MyLog.d("暂停中");
                } else {
                    MyLog.d("播放中");
                }
            }
        });
        qiaopcPlayer.setOnTimeInfoListener(new OnTimeInfoListener() {
            @Override
            public void onTimeInfo(TimeInfoBean timeInfoBean) {
//                MyLog.d(timeInfoBean.toString());
                Message message = Message.obtain();
                message.what = 1;
                message.obj = timeInfoBean;
                handler.sendMessage(message);
            }
        });
    }


    public void begin(View view) {
        qiaopcPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
//        qiaopcPlayer.setSource("/mnt/shared/Other/Warriors.mp3");
        qiaopcPlayer.prepared();
    }

    public void pause(View view) {
        qiaopcPlayer.pause();
    }

    public void resume(View view) {
        qiaopcPlayer.resume();
    }

    Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
                TimeInfoBean timeInfoBean = (TimeInfoBean) msg.obj;
                tvTime.setText(TimeUtil.secdsToDateFormat(timeInfoBean.getCurrentTime(), timeInfoBean.getTotalTime())
                + "/" + TimeUtil.secdsToDateFormat(timeInfoBean.getTotalTime(), timeInfoBean.getTotalTime()));
            }
        }
    };
}
