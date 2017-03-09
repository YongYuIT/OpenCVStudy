package com.thinking.imagesegmentation.camera;

import android.hardware.Camera;
import android.util.Log;

import java.util.List;

/**
 * Created by Yu Yong on 2017/2/27.
 */

//负责相机数据采集，并向CameraView输出采集到的帧数据
public class CameraTool {
    private CameraTool() {

    }

    private static CameraTool thiz;
    private CameraBaseView mView;
    private Camera mCamera;
    private boolean isRotate = true;


    synchronized static CameraTool getThiz() {
        if (thiz != null)
            return thiz;
        thiz = new CameraTool();
        return thiz;
    }


    public void setView(CameraBaseView view) {
        mView = view;
    }

    public void initCamera() {
        int camera_num = Camera.getNumberOfCameras();
        releaseCamera();
        //优先前置
        mCamera = camera_num > 1 ? Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK) : Camera.open();
        mCamera.setPreviewCallback(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera) {
                //Log.i("yuyong", "onPreviewFrame-->" + camera.getParameters().getPreviewSize().height + "-->" + camera.getParameters().getPreviewSize().width + "-->" + data.length);
                if (mView != null)
                    mView.onGetFrame(data, camera.getParameters().getPreviewSize());
            }
        });
    }

    public synchronized void setCamera(int _width, int _height) {

        if (isRotate) {
            _width = _width + _height;
            _height = _width - _height;
            _width = _width - _height;
        }

        Camera.Parameters params = mCamera.getParameters();
        List<Camera.Size> supportSize = params.getSupportedPreviewSizes();
        //筛选出相机支持的，同时可被视图容纳的最大尺寸
        Camera.Size max_size = mCamera.new Size(0, 0);
        for (Camera.Size s : supportSize) {
            Log.i("yuyong", s.width + "," + s.height);
            if (s.height <= _height && s.width <= _width) {
                if (getSizeValue(s) > getSizeValue(max_size))
                    max_size = s;
            }
        }
        Log.i("yuyong", "max_size-->" + max_size.width + "," + max_size.height);
        params.setPreviewSize(max_size.width, max_size.height);
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
        mView = null;
    }
}
