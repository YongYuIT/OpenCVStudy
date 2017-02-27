package com.thinking.cameraimput;

import android.hardware.Camera;

/**
 * Created by Yu Yong on 2017/2/27.
 */
public interface CameraView {
    void onGetFrame(byte[] data, Camera.Size size);
}
