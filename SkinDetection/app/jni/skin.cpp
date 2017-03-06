#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"
using namespace cv;

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_changeSkin
(JNIEnv * env, jclass j_calss, jobject img_data_obj, jobject position_obj, jobject listener_obj) {
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
	int p_x;
	int p_y;
	int p_width;
	int p_height;
	//解析Position
	jclass Position_class = env->GetObjectClass(position_obj);
	jfieldID p_x_id = env->GetFieldID(Position_class, "x", "I");
	jfieldID p_y_id = env->GetFieldID(Position_class, "y", "I");
	jfieldID p_width_id = env->GetFieldID(Position_class, "width", "I");
	jfieldID p_height_id = env->GetFieldID(Position_class, "height", "I");
	p_x = env->GetIntField(position_obj, p_x_id);
	p_y = env->GetIntField(position_obj, p_y_id);
	p_width = env->GetIntField(position_obj, p_width_id);
	p_height = env->GetIntField(position_obj, p_height_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get position_img x is %i, y is %i, width is %i, height is %i", p_x, p_y, p_width, p_height);
	//----------------------------------------------------------------------------------------------------------------------
	//解析Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//用拉普拉斯边缘滤波器得到边缘掩码
	//图片收缩
	Mat s_img;
	const int s_width = 300;
	const float scale_rate = m_image_datas.cols / (float)s_width;
	resize(m_image_datas, s_img, Size(s_width, m_image_datas.rows / scale_rate));
	p_x /= scale_rate;
	p_y /= scale_rate;
	p_width /= scale_rate;
	p_height /= scale_rate;
	//----------------------------------------------------------------------------------------------------------------------
	//转码
	Mat s_r_img;
	cvtColor(s_img, s_r_img, CV_BGRA2BGR);
	cvtColor(s_r_img, s_img, CV_BGR2YCrCb);
	//用拉普拉斯边缘滤波器得到全尺寸边缘掩码
	Mat l_mask = getLaplacianMask(m_image_datas);
	//得到带边界的缩放掩码
	Mat s_b_mask = Mat::zeros(s_img.rows + 2, s_img.cols + 2, CV_8UC1);
	Mat s_mask = s_b_mask(Rect(1, 1, s_img.cols, s_img.rows));
	resize(l_mask, s_mask, Size(s_img.cols, s_img.rows));
	out_put_img(s_b_mask, 0);
	//加强掩码
	threshold(s_mask, s_mask, 80, 255, THRESH_BINARY);
	out_put_img(s_b_mask, 1);
	//形态学变换，删除掩码图间间隙
	dilate(s_mask, s_mask, Mat());
	erode(s_mask, s_mask, Mat());
	out_put_img(s_b_mask, 2);
	//----------------------------------------------------------------------------------------------------------------------
	Point skin_p[6];
	skin_p[0] = Point(p_width / 2, p_height / 2 - p_height / 6);
	skin_p[1] = Point(p_width / 2 - p_width / 11, p_height / 2 - p_height / 6);
	skin_p[2] = Point(p_width / 2 + p_width / 11, p_height / 2 - p_height / 6);
	skin_p[3] = Point(p_width / 2, p_height / 2 + p_height / 16);
	skin_p[4] = Point(p_width / 2 - p_width / 9, p_height / 2 + p_height / 16);
	skin_p[5] = Point(p_width / 2 + p_width / 9, p_height / 2 + p_height / 16);

	Scalar lowerDiff = Scalar(60, 25, 20);
	Scalar upperDiff = Scalar(80, 15, 15);

	Mat c_s_mask = s_mask.clone();
	for (int i = 0; i < 6; i++) {
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "pi(%i,%i)", skin_p[i].x, skin_p[i].y);
		floodFill(s_img, s_b_mask, skin_p[i], Scalar(), NULL, lowerDiff, upperDiff, 4 | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY);
	}
	s_mask -= c_s_mask;
	out_put_img(s_b_mask, 3);
}