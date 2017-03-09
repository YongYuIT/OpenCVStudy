package com.thinking.cameraimput;

import android.content.Context;
import android.graphics.Canvas;
import android.hardware.Camera;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

/**
 * Created by Yu Yong on 2017/2/27.
 */
public class CameraView extends View implements CameraBaseView, FrameHandler.FrameListener {

    private CameraTool mTools;
    private FrameHandler mFHandler;
    private int[] mOutFrame;
    private int mWidth;
    private int mHeight;
    private Camera.Size mCurrentCameraSize;
    private Handler mHandler;
    private boolean isRotate = true;

    public CameraView(Context mContext) {
        super(mContext);
        init();
    }

    public CameraView(Context mContext, AttributeSet mAttrs) {
        super(mContext, mAttrs);
        init();
    }

    private void init() {
        mHandler = new Handler();
        mTools = CameraTool.getThiz();
        mTools.setView(this);
        mTools.initCamera();
        mFHandler = FrameHandler.getThiz();
        if (!isRotate) {
            setRotation(270);
        }
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
        Log.i("yuyong", "onMeasure --> " + mWidth + " , " + mHeight);
        mFHandler.start();
    }

    @Override
    protected synchronized void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (mOutFrame == null)
            return;
        int currentWidth = mCurrentCameraSize.width;
        int currentHeight = mCurrentCameraSize.height;
        if (isRotate) {
            currentWidth = mCurrentCameraSize.height;
            currentHeight = mCurrentCameraSize.width;
        }
        int x = (mWidth - currentWidth) / 2;
        int y = (mHeight - currentHeight) / 2;
        canvas.drawBitmap(mOutFrame, 0, currentWidth, x, y, currentWidth, currentHeight, true, null);
        // mFHandler.doOutPut();
        //Log.i("yuyong", "Current (" + mWidth + "," + mHeight + ") Camera (" + currentWidth + "," + currentHeight + ")");
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
        mFHandler.setDataSource(data, mCurrentCameraSize.width, mCurrentCameraSize.height);
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
                CameraView.this.invalidate();
                mFHandler.doOutPut();
            }
        });

    }
}
