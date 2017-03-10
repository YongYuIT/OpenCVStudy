#include "com_thinking_imagesegmentation_ImageHandler.h"
#include "tools.h"

JNIEXPORT void JNICALL Java_com_thinking_imagesegmentation_ImageHandler_getContoursRect
(JNIEnv * env, jclass j_calss, jobject img_data_obj, jobject listener_obj)
{
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
	//转换灰度
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//高斯模糊
	GaussianBlur(hui_img, hui_img, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//二值化
	Mat erzhi_img = Mat::zeros(hui_img.rows + 2, hui_img.cols + 2, CV_8UC1);
	Mat s_erzhi_img = erzhi_img(Rect(1, 1, hui_img.cols, hui_img.rows));
	threshold(hui_img, s_erzhi_img, 80, 255, THRESH_BINARY_INV);
	out_put_img(erzhi_img, 0);
	//找出边缘
	vector< vector< Point > > lunkuos;//轮廓
	findContours(erzhi_img, lunkuos, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "findContours %i", lunkuos.size());
	/*
	第一个参数image表示目标图像，
	第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，
	第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，
	第四个参数color为轮廓的颜色，
	第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，
	第六个参数lineType为线型，
	第七个参数为轮廓结构信息，
	第八个参数为maxLevel
	*/
	drawContours(m_image_datas, lunkuos, -1, Scalar(0, 0, 255, 255), 2);
	out_put_img(m_image_datas, 1);
	//筛选边缘
}