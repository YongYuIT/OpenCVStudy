package com.thinking.edgedetectionfilter;

import android.graphics.Bitmap;

/**
 * Created by Yu Yong on 2017/3/1.
 */
public class ImageHandler {
    static {
        System.loadLibrary("com_thinking_opencv");
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

    public interface Listener {
        void onHandling(int[] img, int width, int height, int index);
    }

    public static native void get_image_laplacian(ImageData data, Listener listener);
}

//javah -d G:\OpenCVStu\20170301001\OpenCVStudy\EdgeDetectionFilter\app\jni -classpath G:\OpenCVStu\20170301001\OpenCVStudy\EdgeDetectionFilter\app\build\intermediates\classes\debug com.thinking.edgedetectionfilter.ImageHandler

//javap -classpath . -s ImageHandler.Listener
/*
public interface com.thinking.edgedetectionfilter.ImageHandler$Listener {
  public abstract void onHandling(int[], int, int, int);
    descriptor: ([IIII)V
}
*/