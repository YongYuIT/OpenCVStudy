package com.thinking.imageinput;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View v) {
        //测试灰度转换
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
        //测试颜色过滤，这个实验充分说明Bitmap像素点的颜色编码是unsigned char[b,g,r,a,b,g,r,a...]
        if (v.getId() == R.id.btn_test_2) {
            Log.i("yuyong", "btn_test_2");
            Bitmap source = ((BitmapDrawable) getResources().getDrawable(
                    R.drawable.test)).getBitmap();
            NdkTools.ImageData data = new NdkTools.ImageData(source);
            NdkTools.image_change_color(data, Integer.parseInt(((EditText) findViewById(R.id.edt_index)).getText().toString()), (255 * Integer.parseInt(((EditText) findViewById(R.id.edt_index_alpha)).getText().toString())) / 100);
            Bitmap result = Bitmap.createBitmap(source.getWidth(), source.getHeight(), Bitmap.Config.ARGB_8888);
            result.setPixels(data.image_datas, 0, source.getWidth(), 0, 0, source.getWidth(), source.getHeight());
            ((ImageView) findViewById(R.id.img_2)).setImageBitmap(result);
        }
    }
}
