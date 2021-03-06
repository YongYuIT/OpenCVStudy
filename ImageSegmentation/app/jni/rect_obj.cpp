#include "com_thinking_imagesegmentation_ImageHandler.h"
#include "tools.h"

JNIEXPORT void JNICALL Java_com_thinking_imagesegmentation_ImageHandler_getRectangleObj
(JNIEnv * env, jclass j_calss, jobject img_data_obj, jobject listener_obj){

	//----------------------------------------------------------------------------------------------------------------------
	//解析ImageData
	jint width;
	jint height;
	jintArray image_datas;
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "start jni call");
	jclass ImageData_class = env->GetObjectClass(img_data_obj);
	jfieldID width_id = env->GetFieldID(ImageData_class, "width", "I");
	width = env->GetIntField(img_data_obj, width_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get width is %i", width);
	jfieldID height_id = env->GetFieldID(ImageData_class, "height", "I");
	height = env->GetIntField(img_data_obj, height_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get height is %i", height);
	jfieldID image_datas_id = env->GetFieldID(ImageData_class, "image_datas", "[I");
	image_datas = (jintArray)env->GetObjectField(img_data_obj, image_datas_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get image_datas lenth is %i", env->GetArrayLength(image_datas));
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//----------------------------------------------------------------------------------------------------------------------
	//解析Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//转换灰度图
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//高斯模糊，降噪，避免对竖边检测的干扰
	GaussianBlur(hui_img, hui_img, Size(5, 5), 0, 0, BORDER_DEFAULT);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "blur-----------do");
	out_put_img(hui_img, 0);
	//Sobel滤波
	Mat so_img;
	//从第四个参数开始
	//对x求导阶数
	//对y求导阶数
	//核的大小
	Sobel(hui_img, so_img, CV_8U, 1, 0, 3, 1, 0);
	out_put_img(so_img, 1);
	//二值化
	Mat erzhi_img;
	threshold(so_img, erzhi_img, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	out_put_img(erzhi_img, 2);
	//形态学运行，防止边界重影
	Mat element = getStructuringElement(MORPH_RECT, Size(17, 3));
	morphologyEx(erzhi_img, erzhi_img, CV_MOP_CLOSE, element);
	out_put_img(erzhi_img, 3);
	//找出轮廓
	vector< vector< Point > > lunkuos;//轮廓
	findContours(erzhi_img, lunkuos, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(m_image_datas, lunkuos, 0, Scalar(255, 0, 0, 255), CV_FILLED);
	out_put_img(m_image_datas, 4);
}