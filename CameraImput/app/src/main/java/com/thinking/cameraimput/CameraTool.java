package com.thinking.cameraimput;

import android.hardware.Camera;

import java.util.List;

/**
 * Created by Yu Yong on 2017/2/27.
 */

//负责相机数据采集，并向CameraView输出采集到的帧数据
public class CameraTool {
    private CameraTool() {

    }

    private static CameraTool thiz;
    private CameraView mView;
    private Camera mCamera;
    private Camera.Size mCameraSize;

    public Camera.Size getSize() {
        return mCameraSize;
    }

    synchronized static CameraTool getThiz() {
        if (thiz != null)
            return thiz;
        thiz = new CameraTool();
        return thiz;
    }


    public void setView(CameraView view) {
        mView = view;
    }

    public void initCamera() {
        int camera_num = Camera.getNumberOfCameras();
        releaseCamera();
        //优先前置
        mCamera = camera_num > 1 ? Camera.open(Camera.CameraInfo.CAMERA_FACING_FRONT) : Camera.open();
        mCamera.setPreviewCallback(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera) {
                mView.onGetFrame(data);
            }
        });
    }

    public synchronized void setCamera(int _width, int _height) {
        Camera.Parameters params = mCamera.getParameters();
        List<Camera.Size> supportSize = params.getSupportedPreviewSizes();

        //筛选出相机支持的，同时可被视图容纳的最大尺寸
        mCameraSize = mCamera.new Size(0, 0);
        for (Camera.Size s : supportSize) {
            if (s.height <= _height && s.width <= _width) {
                if (getSizeValue(s) > getSizeValue(mCameraSize))
                    mCameraSize = s;
            }
        }

        params.setPreviewSize(mCameraSize.width, mCameraSize.height);
        params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        mCamera.setParameters(params);
    }

    public void startFrame() {
        mCamera.startPreview();
    }

    private int getSizeValue(Camera.Size size) {
        return size.height * size.height + size.width + size.width;
    }

    public void releaseCamera() {
        if (mCamera == null)
            return;
        mCamera.setPreviewCallback(null);
        mCamera.stopPreview();
        mCamera.release();
        mCamera = null;
    }
}
