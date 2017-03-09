package com.thinking.imagesegmentation.camera;

import android.hardware.Camera;

/**
 * Created by Yu Yong on 2017/2/27.
 */
public interface CameraBaseView {
    void onGetFrame(byte[] data, Camera.Size size);
}
