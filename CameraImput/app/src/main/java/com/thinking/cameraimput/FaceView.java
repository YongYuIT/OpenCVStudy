package com.thinking.cameraimput;

import android.content.Context;
import android.graphics.Canvas;
import android.hardware.Camera;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Yu Yong on 2017/2/27.
 */
public class FaceView extends View implements CameraView, FrameHandler.FrameListener {

    private CameraTool mTools;
    private FrameHandler mFHandler;
    private int[] mOutFrame;
    private int mWidth;
    private int mHeight;
    private Camera.Size mCurrentCameraSize;
    private Handler mHandler;

    public FaceView(Context mContext) {
        super(mContext);
        init();
    }

    public FaceView(Context mContext, AttributeSet mAttrs) {
        super(mContext, mAttrs);
        init();
    }

    private void init() {
        mHandler = new Handler();
        mTools = CameraTool.getThiz();
        mTools.setView(this);
        mTools.initCamera();
        mFHandler = FrameHandler.getThiz();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        //将相对数值（例如dp）转化为绝对像素
        mWidth = MeasureSpec.getSize(widthMeasureSpec);
        mHeight = MeasureSpec.getSize(heightMeasureSpec);
        mTools.setCamera(mWidth, mHeight);
        mTools.startFrame();
        mFHandler.setListener(this);
        //Log.i("yuyong", "onMeasure --> " + mWidth + " , " + mHeight);
        mFHandler.start();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (mOutFrame == null)
            return;
        int x = (mWidth - mCurrentCameraSize.width) / 2;
        int y = (mHeight - mCurrentCameraSize.height) / 2;
        canvas.drawBitmap(mOutFrame, 0, mCurrentCameraSize.width, x, y, mCurrentCameraSize.width, mCurrentCameraSize.height, true, null);
    }

    @Override
    protected void onDetachedFromWindow() {
        //Log.i("yuyong", "onDetachedFromWindow");
        super.onDetachedFromWindow();
        mFHandler.stopHandle();
        mTools.releaseCamera();
    }

    @Override
    public void onGetFrame(byte[] data, Camera.Size size) {
        mCurrentCameraSize = size;
        mFHandler.setDataSource(data, mCurrentCameraSize.width, mCurrentCameraSize.height, 2);
        data = null;
    }

    @Override
    public void onFrame(int[] data) {
        //Log.i("yuyong", "onFrame --> " + data[data.length / 2]);
        mOutFrame = data;
        //刷新界面
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                FaceView.this.invalidate();
            }
        });

    }
}
