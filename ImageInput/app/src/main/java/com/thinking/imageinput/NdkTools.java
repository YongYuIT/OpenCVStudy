package com.thinking.imageinput;

import android.graphics.Bitmap;

/**
 * Created by Yu Yong on 2017/2/24.
 */
public class NdkTools {

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

    //简单的灰度转换
    public static native int[] test_image_data_input(ImageData data);
}

//javah -d G:\OpenCVStu\20170223001\ImageInput\app\jni -classpath G:\OpenCVStu\20170223001\ImageInput\app\build\intermediates\classes\debug com.thinking.imageinput.NdkTools

//javap -classpath . -s NdkTools.ImageData
/*
public class com.thinking.imageinput.NdkTools$ImageData {
  public int width;
    descriptor: I
  public int height;
    descriptor: I
  public int[] image_datas;
    descriptor: [I
  final com.thinking.imageinput.NdkTools this$0;
    descriptor: Lcom/thinking/imageinput/NdkTools;
  public com.thinking.imageinput.NdkTools$ImageData(com.thinking.imageinput.NdkTools, int, int);
    descriptor: (Lcom/thinking/imageinput/NdkTools;II)V
}
*/