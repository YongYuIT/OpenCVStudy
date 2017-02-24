#include"com_thinking_imageinput_NdkTools.h"
#include"tools.h"
#include <android/log.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace cv;

JNIEXPORT jintArray JNICALL Java_com_thinking_imageinput_NdkTools_test_1image_1data_1input
(JNIEnv * env, jclass j_class, jobject image_data_obj)
{
	jint width;
	jint height;
	jintArray image_datas;
	//----------------------------------------------------------------------------------------------------------------------
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
	//----------------------------------------------------------------------------------------------------------------------
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	Mat m_result;
	cvtColor(m_image_datas, m_result, CV_BGRA2GRAY);
	do_save_img("/storage/emulated/0/test.jpg", m_result);
	//----------------------------------------------------------------------------------------------------------------------
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "channers from %i to %i \n rows from %i to %i \n cols from %i to %i", m_image_datas.channels(), m_result.channels(), m_image_datas.rows, m_result.rows, m_image_datas.cols, m_result.cols);
	//����cvtColor��m_result��Ƚ�m_image_datas�Ѿ����������ݶ�ʧ���ﵽ�˻Ҷ�ת����Ŀ�ġ�����Ϊ��Android��ʾ������Ҫ��m_result�ָ���4ͨ����
	cvtColor(m_result, m_image_datas, CV_GRAY2BGRA);
	return image_datas;
}