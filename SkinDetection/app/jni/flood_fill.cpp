#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_floodFlood
(JNIEnv * env, jclass j_class, jobject img_data_obj, jobject listener_obj)
{
	//----------------------------------------------------------------------------------------------------------------------
	//int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint, Scalar newVal, CV_OUT Rect* rect = 0, Scalar loDiff = Scalar(), Scalar upDiff = Scalar(), int flags = 4);
	//InputOutputArray：输入&输出，1通道或者3通道，8位或者浮点图像
	//InputOutputArray：8位单通道掩码，floodFill算法将从起点开始蔓延至掩码边界（掩码即全1）。
	//Point：floodFill算法起点
	//Scalar：算法填充颜色值
	//Rect：算法最小可填充矩形区域，小于这个值的区域将会被忽略
	//Scalar：亮度或者颜色的负差最大值
	//Scalar：亮度或者颜色的正差最大值
	//int：标志，由高中低三个8位符共同决定
	jint width;
	jint height;
	jintArray image_datas;
	//解析ImageData
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
	//利用Laplacian边缘滤波器得到全尺寸掩码
	Mat mask = getLaplacianMask(m_image_datas);
	//加强掩码
	threshold(mask, mask, 80, 255, THRESH_BINARY);
	//得到带边界的全尺寸掩码
	//Mat b_mask = Mat::ones(mask.rows + 2, mask.cols + 2, CV_8UC1);
	Mat b_mask = Mat::zeros(mask.rows + 2, mask.cols + 2, CV_8UC1);
	threshold(b_mask, b_mask, 255, 255, THRESH_BINARY_INV);
	Mat tmp_mask = b_mask(Rect(1, 1, mask.cols, mask.rows));
	mask.copyTo(tmp_mask);
	//形态学变换，删除掩码图间间隙
	dilate(tmp_mask, tmp_mask, Mat());
	erode(tmp_mask, tmp_mask, Mat());
	threshold(tmp_mask, tmp_mask, 80, 255, THRESH_BINARY);
	out_put_img(b_mask, 0);
	//----------------------------------------------------------------------------------------------------------------------
	Mat out_img;
	cvtColor(m_image_datas, out_img, CV_BGRA2BGR);
	cvtColor(out_img, m_image_datas, CV_BGR2YCrCb);
	floodFill(m_image_datas, b_mask, Point(10, 10), Scalar(100, 0, 0), NULL, Scalar(20, 20, 20), Scalar(20, 20, 20));
	floodFill(m_image_datas, b_mask, Point(m_image_datas.cols - 10, 10), Scalar(0, 100, 0), NULL, Scalar(20, 20, 20), Scalar(20, 20, 20));
	out_put_img(m_image_datas, 1, 1);
}