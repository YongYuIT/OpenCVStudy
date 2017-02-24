package com.thinking.imageinput;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View v) {
        if (v.getId() == R.id.btn_test_1) {
            Bitmap source = ((BitmapDrawable) getResources().getDrawable(
                    R.drawable.test)).getBitmap();
            NdkTools.ImageData data = new NdkTools.ImageData(source);
            int[] i_result = NdkTools.test_image_data_input(data);
            Log.i("yuyong", "success");
            Bitmap result = Bitmap.createBitmap(source.getWidth(), source.getHeight(), Bitmap.Config.ARGB_8888);
            //下面这两种方式都可以
            if (1 == 0) {
                Log.i("yuyong", "do 1");
                result.setPixels(i_result, 0, source.getWidth(), 0, 0, source.getWidth(), source.getHeight());
            } else {
                Log.i("yuyong", "do 2");
                result.setPixels(data.image_datas, 0, source.getWidth(), 0, 0, source.getWidth(), source.getHeight());
            }
            ((ImageView) findViewById(R.id.img_2)).setImageBitmap(result);
        }
    }
}
