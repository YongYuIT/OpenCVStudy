#include "com_thinking_edgedetectionfilter_ImageHandler.h"
#include "tools.h"
JNIEXPORT void JNICALL Java_com_thinking_edgedetectionfilter_ImageHandler_get_1image_1change_1bk
(JNIEnv * env, jclass j_class, jobject image_data_obj, jobject listener_obj){
	jint width;
	jint height;
	jintArray image_datas;
	//----------------------------------------------------------------------------------------------------------------------
	//解析ImageData
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "start jni call get_image_change_bk");
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
	//解析Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	//----------------------------------------------------------------------------------------------------------------------
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//转灰度图
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//回调
	out_put_img(env, listener_obj, onHandling_id, hui_img, width, height, 0);
	//----------------------------------------------------------------------------------------------------------------------
	//二值化，找出头发等颜色较深区域
	Mat result;
	threshold(hui_img, result, 80, 255, THRESH_BINARY_INV);
	//回调
	out_put_img(env, listener_obj, onHandling_id, result, width, height, 1);
	//----------------------------------------------------------------------------------------------------------------------
	//二值化，找出人体等颜色较浅区域
	Mat wake_result;
	threshold(hui_img, wake_result, 180, 255, THRESH_TOZERO);
	//回调
	out_put_img(env, listener_obj, onHandling_id, wake_result, width, height, 2);
	//----------------------------------------------------------------------------------------------------------------------
	//融合
	unsigned char* point;
	unsigned char* _point;
	unsigned char* __point;
	for (int i = 0; i < m_image_datas.rows; i++)
	for (int j = 0; j < m_image_datas.cols; j++) {
		point = m_image_datas.data + i*(m_image_datas.cols*m_image_datas.channels()) + j*m_image_datas.channels();
		_point = result.data + i*(result.cols*result.channels()) + j*result.channels();
		__point = wake_result.data + i*(wake_result.cols*wake_result.channels()) + j*wake_result.channels();

		if (!((_point[0])>0 || (__point[0])>0)){
			point[3] = 10;
		}
	}
	//----------------------------------------------------------------------------------------------------------------------
}
