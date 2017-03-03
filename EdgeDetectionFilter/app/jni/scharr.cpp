#include "com_thinking_edgedetectionfilter_ImageHandler.h"
#include "tools.h"

JNIEXPORT void JNICALL Java_com_thinking_edgedetectionfilter_ImageHandler_get_1image_1scharr
(JNIEnv * env, jclass j_class, jobject image_data_obj, jobject listener_obj)
{
	jint width;
	jint height;
	jintArray image_datas;
	//----------------------------------------------------------------------------------------------------------------------
	//解析ImageData
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "start jni call");
	jclass ImageData_class = env->GetObjectClass(image_data_obj);
	jfieldID width_id = env->GetFieldID(ImageData_class, "width", "I");
	width = env->GetIntField(image_data_obj, width_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get width is %i", width);
	jfieldID height_id = env->GetFieldID(ImageData_class, "height", "I");
	height = env->GetIntField(image_data_obj, height_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get height is %i", height);
	jfieldID image_datas_id = env->GetFieldID(ImageData_class, "image_datas", "[I");
	image_datas = (jintArray)env->GetObjectField(image_data_obj, image_datas_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get image_datas lenth is %i", env->GetArrayLength(image_datas));
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//----------------------------------------------------------------------------------------------------------------------
	//解析Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//转灰度图
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//回调
	out_put_img(hui_img, 0);
	//----------------------------------------------------------------------------------------------------------------------
	//中值滤波
	medianBlur(hui_img, hui_img, 7);
	//回调
	out_put_img(hui_img, 1);
	//----------------------------------------------------------------------------------------------------------------------
	//梯度滤波
	//
	Mat ti_img_1, ti_img_2;
	Scharr(hui_img, ti_img_1, CV_8U, 1, 0);
	Scharr(hui_img, ti_img_2, CV_8U, 1, 0, -1);
	ti_img_1 += ti_img_2;
	//回调
	out_put_img(ti_img_1, 2);
	//----------------------------------------------------------------------------------------------------------------------
	//二值化
	Mat er_img;
	threshold(ti_img_1, er_img, 12, 255, THRESH_BINARY_INV);
	//回调
	out_put_img(er_img, 3);
	//----------------------------------------------------------------------------------------------------------------------
	//中值滤波
	medianBlur(er_img, er_img, 3);
	//回调
	out_put_img(er_img, 4);
	//----------------------------------------------------------------------------------------------------------------------
	//合成
	Mat out_put_result;
	cvtColor(m_image_datas, out_put_result, CV_BGRA2BGR);

	Mat tmp(out_put_result.size(), CV_8UC3);
	out_put_result.copyTo(tmp, er_img);
	//----------------------------------------------------------------------------------------------------------------------
	//输出
	cvtColor(tmp, m_image_datas, CV_BGR2BGRA);
}