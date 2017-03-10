package com.thinking.imagesegmentation;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

public class ContoursActivity extends Activity implements ImageHandler.Listener {
    Handler mHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contours);
        mHandler = new Handler();
    }

    public void onClick(View v) {
        if (v.getId() == R.id.btn_test_1) {
            Bitmap source = ((BitmapDrawable) getResources().getDrawable(
                    R.drawable.test_000)).getBitmap();
            ImageHandler.ImageData data = new ImageHandler.ImageData(source);
            ImageHandler.doContoursRect(data, this);
        }
    }

    @Override
    public void onHandling(int[] img, int width, int height, final int index) {

        final Bitmap result = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        result.setPixels(img, 0, width, 0, 0, width, height);
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Log.i("yuyong", "index-----" + index);
                int id = 0;
                switch (index) {
                    case 0:
                        id = R.id.img_p_1;
                        break;
                    case 1:
                        id = R.id.img_p_2;
                        break;
                    case 2:
                        id = R.id.img_p_3;
                        break;
                    case 3:
                        id = R.id.img_p_4;
                        break;
                    case 4:
                        id = R.id.img_p_5;
                        break;
                }
                if (id == 0)
                    return;
                ((ImageView) findViewById(id)).setImageBitmap(result);
                findViewById(id).setVisibility(View.VISIBLE);
            }
        });

    }
}
