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
        int x;
        int y;
        int width;
        int height;
    }

    public static class ImageData {
        public int width;
        public int height;
        public int[] image_datas;

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

    public static native boolean init(String path);

    public static native void getFaseLocation(ImageData data, Position position);

    public static native void getEyesLocation(ImageData data, Position position_left, Position position_right);
}

//javah -d G:\OpenCVStu\20170303002\OpenCVStudy\SkinDetection\app\jni -classpath G:\OpenCVStu\20170303002\OpenCVStudy\SkinDetection\app\build\intermediates\classes\debug com.thinking.skindetection.SkinHandler

/*
public class com.thinking.skindetection.SkinHandler {
  public static java.lang.String FilePath;
    descriptor: Ljava/lang/String;
  public com.thinking.skindetection.SkinHandler();
    descriptor: ()V

  public static boolean init(android.content.Context);
    descriptor: (Landroid/content/Context;)Z

  public static native boolean init(java.lang.String);
    descriptor: (Ljava/lang/String;)Z

  public static native void getFaseLocation(com.thinking.skindetection.SkinHandler$ImageData, com.thinking.skindetection.SkinHandler$Position);
    descriptor: (Lcom/thinking/skindetection/SkinHandler$ImageData;Lcom/thinking/skindetection/SkinHandler$Position;)V

  public static native void getEyesLocation(com.thinking.skindetection.SkinHandler$ImageData, com.thinking.skindetection.SkinHandler$Position, com.thinking.skindetection.SkinHandler$Position);
    descriptor: (Lcom/thinking/skindetection/SkinHandler$ImageData;Lcom/thinking/skindetection/SkinHandler$Position;Lcom/thinking/skindetection/SkinHandler$Position;)V

  static {};
    descriptor: ()V
}
*/

/*
#include <jni.h>
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
*/

//javap -classpath . -s SkinHandler