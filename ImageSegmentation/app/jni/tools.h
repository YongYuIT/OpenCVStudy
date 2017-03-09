#ifndef COM_THINKING_TOOLS_H
#define COM_THINKING_TOOLS_H

#include "com_thinking_imagesegmentation_ImageHandler.h"

static JNIEnv * env;
static jobject listener_obj;
static jmethodID onHandling_id;

char* jstringTostring(JNIEnv* env, jstring jstr);
void set_out_params(JNIEnv * _env, jobject _listener_obj, jmethodID _onHandling_id);
void out_put_img(Mat out_put, int index);
//formate={1:YCrCb}
void out_put_img(Mat out_put, int formate, int index);


#endif