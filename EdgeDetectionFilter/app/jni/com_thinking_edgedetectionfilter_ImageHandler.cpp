#include "com_thinking_edgedetectionfilter_ImageHandler.h"
#include <android/log.h>
#include"tools.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>
using namespace cv;

JNIEXPORT void JNICALL Java_com_thinking_edgedetectionfilter_ImageHandler_get_1image_1laplacian
(JNIEnv * env, jclass j_class, jobject image_data_obj, jobject listener_obj){
	jint width;
	jint height;
	jintArray image_datas;
	//----------------------------------------------------------------------------------------------------------------------
	//����ImageData
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
	//����Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	//----------------------------------------------------------------------------------------------------------------------
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//ת�Ҷ�ͼ
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);

	{
		Mat tmp;
		cvtColor(hui_img, tmp, CV_GRAY2BGRA);
		jintArray out = env->NewIntArray(tmp.rows*tmp.cols);
		env->ReleaseIntArrayElements(out, (jint*)tmp.data, 0);
		env->CallVoidMethod(listener_obj, onHandling_id, out, width, height, 0);
	}
	//----------------------------------------------------------------------------------------------------------------------
	//��ֵ�˲������룬��Ե��
	medianBlur(hui_img, hui_img, 7);

	{
		Mat tmp;
		cvtColor(hui_img, tmp, CV_GRAY2BGRA);
		jintArray out = env->NewIntArray(tmp.rows*tmp.cols);
		env->ReleaseIntArrayElements(out, (jint*)tmp.data, 0);
		env->CallVoidMethod(listener_obj, onHandling_id, out, width, height, 1);
	}
	//----------------------------------------------------------------------------------------------------------------------
	//Laplacian��Ե�˲������ɲ�ͬ���ȵı�Ե
	Mat lap_img;
	Laplacian(hui_img, lap_img, CV_8U, 5);

	{
		Mat tmp;
		cvtColor(lap_img, tmp, CV_GRAY2BGRA);
		jintArray out = env->NewIntArray(tmp.rows*tmp.cols);
		env->ReleaseIntArrayElements(out, (jint*)tmp.data, 0);
		env->CallVoidMethod(listener_obj, onHandling_id, out, width, height, 2);
	}
	//----------------------------------------------------------------------------------------------------------------------
	//��ֵ�������߽���ֵ��ֵ����
	Mat result;
	//��������������ֵ
	//���ĸ���������ֵ
	//������������ȽϷ�ʽ��THRESH_BINARY��    �����ص�ֵ������ֵʱ  ���ص�ֵȡ��ֵ������ȡ0
	//                      THRESH_BINARY_INV�������ص�ֵ������ֵʱ  ���ص�ֵȡ0��   ����ȡ��ֵ
	//                      THRESH_TRUNC��     �����ص�ֵ������ֵʱ  ���ص�ֵȡ��ֵ�����򲻱�
	//                      THRESH_TOZERO��    �����ص�ֵ������ֵʱ  ���ص�ֵ���䣬  ����ȡ0
	//                      THRESH_TOZERO_INV�������ص�ֵ������ֵʱ  ���ص�ֵȡ0��   ���򲻱�
	threshold(lap_img, result, 80, 255, THRESH_BINARY_INV);
	//do_save_img("/storage/emulated/0/test.jpg", result);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "threshold result channels %i, width %i, height %i", result.channels(), result.cols, result.rows);
	//----------------------------------------------------------------------------------------------------------------------
	//���
	cvtColor(result, m_image_datas, CV_GRAY2BGRA);
}