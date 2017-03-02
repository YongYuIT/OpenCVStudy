package com.thinking.videoinput;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;

/**
 * Created by Yu Yong on 2017/3/2.
 */

public class MediaView extends BaseMediaView {
    private MediaHandler mMediaHandler;
    private String mFilePath;
    private int mWidth;
    private int mHeight;
    private Handler mHandler;

    public MediaView(Context _context) {
        super(_context);
    }

    public MediaView(Context _context, AttributeSet mAttrs) {
        super(_context, mAttrs);
    }

    @Override
    protected void init() {
        mHandler = new Handler();
        mMediaHandler = MediaHandler.getThiz();
        mMediaHandler.setView(this);
    }

    public void setVideoFilePath(String filePath) {
        mFilePath = filePath;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        mWidth = MeasureSpec.getSize(widthMeasureSpec);
        mHeight = MeasureSpec.getSize(heightMeasureSpec);

    }

    public void startPlay() {
        mMediaHandler.getVideoFrame(mFilePath);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        //Log.i("yuyong", "onDraw-->" + (mFrame == null ? "null" : "not null-->" + mFrame.getWidth() + "," + mFrame.getHeight()));
        if (mFrame == null)
            return;
        int x = (mWidth - mFrame.getWidth()) / 2;
        int y = (mHeight - mFrame.getHeight()) / 2;
        int[] datas = new int[mFrame.getWidth() * mFrame.getHeight()];
        mFrame.getPixels(datas, 0, mFrame.getWidth(), 0, 0, mFrame.getWidth(), mFrame.getHeight());

        canvas.drawBitmap(datas, 0, mFrame.getWidth(), x, y, mFrame.getWidth(), mFrame.getHeight(), true, null);
        datas = null;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mMediaHandler.stop();
    }

    @Override
    public void onGetFrame(Bitmap frame) {
        super.onGetFrame(frame);
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                //Log.i("yuyong", "invalidate");
                MediaView.this.invalidate();
            }
        });
        mMediaHandler.doContinue();
    }
}
