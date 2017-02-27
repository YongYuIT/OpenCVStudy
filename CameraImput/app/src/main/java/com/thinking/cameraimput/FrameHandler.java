package com.thinking.cameraimput;

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

    private FrameHandler() {
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
        mDataSource = source;
        mWidth = width;
        mHeight = height;
        mDataOutPut = new int[mWidth * mHeight];
        mOretation = oretation;
    }

    public void setListener(FrameListener _l) {
        mListener = _l;
    }

    @Override
    public void run() {
        super.run();
        while (true) {
            handle_frame(mWidth, mHeight, mOretation, mDataSource, mDataOutPut);
            mListener.onFrame(mDataOutPut);
        }
    }

    //oretation 1：竖屏；2：横屏
    private static native void handle_frame(int width, int heigt, int oretation, byte[] input_frame_data, int[] out_frame_data);
}
