package com.example.myapplication;

import android.support.v7.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.qiaopcplayer.Demo;
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
                MyLog.d("oncall prepared");
                qiaopcPlayer.start();
            }
        });
    }


    public void begin(View view) {
        qiaopcPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
        qiaopcPlayer.prepared();
    }
}
