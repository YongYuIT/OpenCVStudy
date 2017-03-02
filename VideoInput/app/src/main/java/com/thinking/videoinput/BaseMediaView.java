package com.thinking.videoinput;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

/**
 * Created by Yu Yong on 2017/3/2.
 */

public abstract class BaseMediaView extends View {

    protected Bitmap mFrame;
    private Context mContext;

    public BaseMediaView(Context _context) {
        super(_context);
        mContext = _context;
        init();
    }

    public BaseMediaView(Context _context, AttributeSet mAttrs) {
        super(_context, mAttrs);
        mContext = _context;
        init();
    }

    protected abstract void init();

    public void onGetFrame(Bitmap frame) {
        //Log.i("yuyong", "onGetFrame---->" + (frame == null ? "null" : "not null"));
        mFrame = frame;
    }
}
