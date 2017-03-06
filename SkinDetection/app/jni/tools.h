#ifndef COM_THINKING_TOOLS_H
#define COM_THINKING_TOOLS_H

#include "com_thinking_skindetection_SkinHandler.h"

//这种全局的静态变量只有在程序结束的时候才会被析构
static CascadeClassifier faceCascade;
static CascadeClassifier eyeCascade1;
static CascadeClassifier eyeCascade2;
static string file_base_path;

static JNIEnv * env;
static jobject listener_obj;
static jmethodID onHandling_id;

char* jstringTostring(JNIEnv* env, jstring jstr);
void set_out_params(JNIEnv * _env, jobject _listener_obj, jmethodID _onHandling_id);
void out_put_img(Mat out_put, int index);

Mat getLaplacianMask(Mat& source_img);


#endif