package com.thinking.cameraimput;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Yu Yong on 2017/2/27.
 */
public class FaceView extends View implements CameraView, FrameHandler.FrameListener {

    private CameraTool mTools;
    private FrameHandler mFHandler;

    private byte[] mCameraFrame = null;
    private int[] mOutFrame;
    private int mWidth;
    private int mHeight;

    public FaceView(Context mContext) {
        super(mContext);
        init();
    }

    public FaceView(Context mContext, AttributeSet mAttrs) {
        super(mContext, mAttrs);
        init();
    }

    private void init() {
        mTools = CameraTool.getThiz();
        mTools.setView(this);
        mTools.initCamera();
        mFHandler = FrameHandler.getThiz();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        //将相对数值（例如dp）转化为绝对像素
        mWidth = MeasureSpec.getSize(widthMeasureSpec);
        mHeight = MeasureSpec.getSize(heightMeasureSpec);
        mTools.setCamera(mWidth, mHeight);
        mTools.startFrame();
        mFHandler.setDataSource(mCameraFrame, mTools.getSize().width, mTools.getSize().height, 1);
        mFHandler.setListener(this);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (mOutFrame == null)
            return;
        int x = (mWidth - mTools.getSize().width) / 2;
        int y = (mHeight - mTools.getSize().height) / 2;
        canvas.drawBitmap(mOutFrame, 0, mTools.getSize().width, x, y, mTools.getSize().width, mTools.getSize().height, true, null);
    }

    @Override
    public void onGetFrame(byte[] data) {
        if (mCameraFrame == null)
            mCameraFrame = new byte[data.length];
        System.arraycopy(data, 0, mCameraFrame, 0, data.length);
        data = null;
    }

    @Override
    public void onFrame(int[] data) {
        mOutFrame = data;
        //刷新界面
        this.invalidate();
    }
}
