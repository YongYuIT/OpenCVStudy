package com.thinking.skindetection;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
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
                    R.drawable.test_000)).getBitmap();
            SkinHandler.ImageData data = new SkinHandler.ImageData(source);
            SkinHandler.getFaseLocation(data, new SkinHandler.Position());
            Bitmap result = Bitmap.createBitmap(source.getWidth(), source.getHeight(), Bitmap.Config.ARGB_8888);
            result.setPixels(data.image_datas, 0, source.getWidth(), 0, 0, source.getWidth(), source.getHeight());
            ((ImageView) findViewById(R.id.img_2)).setImageBitmap(result);
        }
    }
}
