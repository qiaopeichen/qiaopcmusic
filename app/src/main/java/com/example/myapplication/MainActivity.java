package com.example.myapplication;

import android.support.v7.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.qiaopcplayer.Demo;
import com.example.qiaopcplayer.listener.OnLoadListener;
import com.example.qiaopcplayer.listener.OnPauseResumeListener;
import com.example.qiaopcplayer.listener.OnPreparedListener;
import com.example.qiaopcplayer.log.MyLog;
import com.example.qiaopcplayer.player.QiaopcPlayer;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.

    private QiaopcPlayer qiaopcPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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
}
