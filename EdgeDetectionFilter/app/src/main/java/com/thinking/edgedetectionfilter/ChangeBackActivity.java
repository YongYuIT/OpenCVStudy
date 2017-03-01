package com.thinking.edgedetectionfilter;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class ChangeBackActivity extends Activity implements ImageHandler.Listener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_back);
    }

    public void onClick(View v) {
        if (v.getId() == R.id.btn_test_1) {
            Bitmap source = ((BitmapDrawable) getResources().getDrawable(
                    R.drawable.test_000)).getBitmap();
            ImageHandler.ImageData data = new ImageHandler.ImageData(source);
            ImageHandler.get_image_change_bk(data, this);
            Bitmap result = Bitmap.createBitmap(source.getWidth(), source.getHeight(), Bitmap.Config.ARGB_8888);
            result.setPixels(data.image_datas, 0, source.getWidth(), 0, 0, source.getWidth(), source.getHeight());
            ((ImageView) findViewById(R.id.img_2)).setImageBitmap(result);
        }
    }

    @Override
    public void onHandling(int[] img, int width, int height, int index) {
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
        }

        Bitmap result = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        result.setPixels(img, 0, width, 0, 0, width, height);
        ((ImageView) findViewById(id)).setImageBitmap(result);
        ((ImageView) findViewById(id)).setVisibility(View.VISIBLE);
    }
}
