package com.thinking.skindetection;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Environment;

/**
 * Created by Yu Yong on 2017/3/3.
 */

public class SkinHandler {

    public static String FilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/thinking_skin";
    public static String[] detectors = new String[]{"lbpcascade_frontalface.xml", "haarcascade_eye.xml", "haarcascade_eye_tree_eyeglasses.xml"};

    static {
        System.loadLibrary("com_thinking_opencv");
    }

    public static boolean init(Context mContext) {
        for (int i = 0; i < detectors.length; i++) {
            FileTools.writeAssetsToSd("detectors/" + detectors[i], mContext, FilePath + "/" + detectors[i]);
        }
        return init(FilePath);
    }

    public static class Position {
        public int x = 0;
        public int y = 0;
        public int width = 0;
        public int height = 0;
    }

    public static class ImageData {
        public int width = 0;
        public int height = 0;
        public int[] image_datas = null;

        public ImageData(int w, int h) {
            width = w;
            height = h;
            image_datas = new int[width * height];
        }

        public ImageData(Bitmap data_source) {
            width = data_source.getWidth();
            height = data_source.getHeight();
            image_datas = new int[width * height];
            data_source.getPixels(image_datas, 0, width, 0, 0, width, height);
        }
    }

    public interface Listener {
        void onHandling(int[] img, int width, int height, int index);
    }

    public static native boolean init(String path);

    public static native void getFaseLocation(ImageData data, Position position, Listener listener);

    public static native void getEyesLocation(ImageData data, Position position_left, Position position_right, Listener listener);

    public static native void changeSkin(ImageData data, Position position, Listener listener);
}

//javah -d G:\OpenCVStu\20170306001\OpenCVStudy\SkinDetection\app\jni -classpath G:\OpenCVStu\20170306001\OpenCVStudy\SkinDetection\app\build\intermediates\classes\debug com.thinking.skindetection.SkinHandler

//javap -classpath . -s SkinHandler

/*
#include <jni.h>
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
*/
