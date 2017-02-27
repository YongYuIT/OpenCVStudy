package com.thinking.cameraimput;

import android.util.Log;

/**
 * Created by Yu Yong on 2017/2/27.
 */
//负责帧数据处理
public class FrameHandler extends Thread {


    private byte[] mDataSource;
    private int mWidth;
    private int mHeight;
    private int mOretation;
    private int[] mDataOutPut;
    private FrameListener mListener;
    private static FrameHandler thiz;
    private boolean isKeep = false;

    private FrameHandler() {
    }

    static {
        System.loadLibrary("com_thinking_opencv");
    }

    public static synchronized FrameHandler getThiz() {
        if (thiz == null)
            thiz = new FrameHandler();
        return thiz;
    }

    public interface FrameListener {
        void onFrame(int[] data);
    }


    public void setDataSource(byte[] source, int width, int height, int oretation) {
        if (mDataSource == null)
            mDataSource = new byte[source.length];
        System.arraycopy(source, 0, mDataSource, 0, source.length);
        mWidth = width;
        mHeight = height;
        mDataOutPut = new int[mWidth * mHeight];
        mOretation = oretation;
    }

    public void setListener(FrameListener _l) {
        mListener = _l;
    }

    @Override
    public synchronized void start() {
        if (!isKeep) {
            isKeep = true;
            super.start();
        }
    }

    public void stopHandle() {
        isKeep = false;
    }


    @Override
    public void run() {
        super.run();
        while (isKeep) {
            if (mDataSource == null || mDataOutPut == null)
                continue;
            //Log.i("yuyong", mDataSource.length + "-->" + mDataOutPut.length + "-->" + mWidth + "-->" + mHeight);
            handle_frame(mWidth, mHeight, mOretation, mDataSource, mDataOutPut);
            //Log.i("yuyong", "run mDataOutPut");
            mListener.onFrame(mDataOutPut);
            try {
                Thread.sleep(50);
            } catch (Exception e) {
            }

        }
        mDataOutPut = null;
    }

    //oretation 1：竖屏；2：横屏
    private static native void handle_frame(int width, int heigt, int oretation, byte[] input_frame_data, int[] out_frame_data);
}

//javah -d G:\OpenCVStu\20170227002\OpenCVStudy\CameraImput\app\jni -classpath G:\OpenCVStu\20170227002\OpenCVStudy\CameraImput\app\build\intermediates\classes\debug com.thinking.cameraimput.FrameHandler

