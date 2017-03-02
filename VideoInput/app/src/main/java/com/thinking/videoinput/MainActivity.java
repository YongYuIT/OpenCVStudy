package com.thinking.videoinput;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View v) {
        if (v.getId() == R.id.btn_play) {
            ((MediaView) findViewById(R.id.mv_play)).setVideoFilePath(Environment.getExternalStorageDirectory().getAbsolutePath() + "/test.avi");
            ((MediaView) findViewById(R.id.mv_play)).startPlay();
        }
    }

}
