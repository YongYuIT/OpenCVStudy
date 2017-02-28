package com.thinking.cameraimput;

import android.content.res.Resources;

/**
 * Created by Yu Yong on 2017/2/27.
 */
//负责帧数据处理
public class FrameHandler extends Thread {


    private byte[] mDataSource;
    private int mWidth;
    private int mHeight;
    private int[] mDataOutPut;
    private FrameListener mListener;
    private static FrameHandler thiz;
    private boolean isKeep = false;
    private Boolean isOutPut = false;
    private boolean isRotate = true;

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


    public void setDataSource(byte[] source, int width, int height) {
        if (mDataSource == null)
            mDataSource = new byte[source.length];
        System.arraycopy(source, 0, mDataSource, 0, source.length);
        mWidth = width;
        mHeight = height;
        if (mDataOutPut == null)
            mDataOutPut = new int[mWidth * mHeight];
    }

    public void setListener(FrameListener _l) {
        mListener = _l;
    }

    @Override
    public synchronized void start() {
        if (!isKeep && !this.isAlive()) {
            isKeep = true;
            super.start();
        }
    }

    public void stopHandle() {
        isKeep = false;
    }

    public void doOutPut() {
        synchronized (isOutPut) {
            isOutPut.notifyAll();
        }
    }

    @Override
    public void run() {
        super.run();
        while (isKeep) {
            if (mDataSource == null || mDataOutPut == null)
                continue;
            //Log.i("yuyong", mDataSource.length + "-->" + mDataOutPut.length + "-->" + mWidth + "-->" + mHeight);
            handle_frame(mWidth, mHeight, isRotate, mDataSource, mDataOutPut);
            //Log.i("yuyong", "run mDataOutPut");
            mListener.onFrame(mDataOutPut);
            synchronized (isOutPut) {
                try {
                    isOutPut.wait();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        mDataOutPut = null;
    }

    private static native void handle_frame(int width, int heigt, boolean is_roate, byte[] input_frame_data, int[] out_frame_data);
}

//javah -d G:\OpenCVStu\20170227002\OpenCVStudy\CameraImput\app\jni -classpath G:\OpenCVStu\20170227002\OpenCVStudy\CameraImput\app\build\intermediates\classes\debug com.thinking.cameraimput.FrameHandler

