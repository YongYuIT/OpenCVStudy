package com.thinking.imagesegmentation;

import android.graphics.Bitmap;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by Yu Yong on 2017/3/9.
 */

public class ImageHandler {

    private static ExecutorService pool = Executors.newFixedThreadPool(5);

    static {
        System.loadLibrary("com_thinking_opencv");
    }

    public static void doRectangleObj(ImageData data, final Listener listener) {
        pool.execute(new NdkTask(listener, data) {
            @Override
            public void run() {
                getRectangleObj((ImageData) mDatas[0], listener);
            }
        });
    }

    public static void doContoursRect(ImageData data, final Listener listener) {
        pool.execute(new NdkTask(listener, data) {
            @Override
            public void run() {
                getContoursRect((ImageData) mDatas[0], listener);
            }
        });
    }

    public static abstract class NdkTask implements Runnable {

        Object[] mDatas;
        Listener mListener;

        public NdkTask(Listener listener, Object... datas) {
            mDatas = datas;
            mListener = listener;
        }
    }

    public static class ImageData {
        private int width = 0;
        private int height = 0;
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

    private static native void getRectangleObj(ImageData data, Listener listener);


    private static native void getContoursRect(ImageData data, Listener listener);
}

//javah -d G:\OpenCVStu\20170309001\OpenCVStudy\ImageSegmentation\app\jni -classpath G:\OpenCVStu\20170309001\OpenCVStudy\ImageSegmentation\app\build\intermediates\classes\debug com.thinking.imagesegmentation.ImageHandler

/*
#include <jni.h>
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
*/