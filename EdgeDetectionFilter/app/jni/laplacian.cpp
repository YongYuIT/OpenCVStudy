#include "com_thinking_edgedetectionfilter_ImageHandler.h"
#include "tools.h"
JNIEXPORT void JNICALL Java_com_thinking_edgedetectionfilter_ImageHandler_get_1image_1laplacian
(JNIEnv * env, jclass j_class, jobject image_data_obj, jobject listener_obj){
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
	//对彩图执行双边滤波（平滑平坦区域，同时保持边缘锐化）
	//step1：缩放
	Mat color_img;
	cvtColor(m_image_datas, color_img, CV_BGRA2BGR);
	Size size = color_img.size();
	size.height = size.height / 2;
	size.width = size.width / 2;
	Mat s_img;
	resize(color_img, s_img, size, 0, 0, INTER_LINEAR);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "s_img height:%i -- weight:%i", s_img.cols, s_img.rows);
	//step2：双边滤波
	{
		Mat tmp;
		s_img.copyTo(tmp);
		int times = 7;//执行7*2次双边滤波
		int ksize = 9;//大小，即滤波器大小为ksize*ksize
		double s_color = 9;//色彩强度
		double s_space = 7;//位置强度
		for (int i = 0; i < times; i++){
			__android_log_print(ANDROID_LOG_INFO, "yuyong", "bilateralFilter %i", i);
			bilateralFilter(s_img, tmp, ksize, s_color, s_space);
			bilateralFilter(tmp, s_img, ksize, s_color, s_space);
		}
	}
	//step3：回复大小
	Mat b_img;
	resize(s_img, b_img, color_img.size(), 0, 0, INTER_LINEAR);
	//回调
	out_put_img(b_img, 0);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "b_img height:%i -- weight:%i", b_img.cols, b_img.rows);
	//----------------------------------------------------------------------------------------------------------------------
	//转灰度图
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//回调
	out_put_img(hui_img, 1);
	//----------------------------------------------------------------------------------------------------------------------
	//降噪
	//噪声类型：盐噪声（随机白点）；胡椒噪声（随机黑点）；椒盐噪声（随机黑白点）；高斯噪声（概率密度函数服从正态分布）
	//滤波器：高斯滤波
	//        中值滤波，降噪，边缘锐化
	//        均值滤波
	//        双边滤波

	//中值滤波
	medianBlur(hui_img, hui_img, 7);
	//回调
	out_put_img(hui_img, 2);
	//----------------------------------------------------------------------------------------------------------------------
	//Laplacian边缘滤波，生成不同亮度的边缘
	Mat lap_img;
	Laplacian(hui_img, lap_img, CV_8U, 5);
	//回调
	out_put_img(lap_img, 3);
	//----------------------------------------------------------------------------------------------------------------------
	//二值化（或者叫阈值二值化）
	Mat erzhi_img;
	//第三个参数：阈值
	//第四个参数：极值
	//第五个参数：比较方式，THRESH_BINARY：    当像素点值大于阈值时  像素点值取极值，否则取0
	//                      THRESH_BINARY_INV：当像素点值大于阈值时  像素点值取0，   否则取极值
	//                      THRESH_TRUNC：     当像素点值大于阈值时  像素点值取阈值，否则不变
	//                      THRESH_TOZERO：    当像素点值大于阈值时  像素点值不变，  否则取0
	//                      THRESH_TOZERO_INV：当像素点值大于阈值时  像素点值取0，   否则不变
	threshold(lap_img, erzhi_img, 80, 255, THRESH_BINARY_INV);
	//do_save_img("/storage/emulated/0/test.jpg", result);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "threshold result channels %i, width %i, height %i", erzhi_img.channels(), erzhi_img.cols, erzhi_img.rows);
	//回调
	out_put_img(erzhi_img, 4);
	//----------------------------------------------------------------------------------------------------------------------
	//合成
	Mat out_result;
	b_img.copyTo(out_result);
	out_result.setTo(0);
	b_img.copyTo(out_result, erzhi_img);
	//----------------------------------------------------------------------------------------------------------------------
	//输出
	cvtColor(out_result, m_image_datas, CV_BGR2BGRA);
}