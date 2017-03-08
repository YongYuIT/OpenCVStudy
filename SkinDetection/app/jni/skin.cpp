#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"
using namespace cv;

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_changeSkin
(JNIEnv * env, jclass j_calss, jobject img_data_obj, jobject position_obj, jobject listener_obj) {
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
	//解析Position
	int p_x;
	int p_y;
	int p_width;
	int p_height;

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
	//图片收缩
	Mat s_img;
	const int s_width = 300;
	const float scale_rate = m_image_datas.cols / (float)s_width;
	resize(m_image_datas, s_img, Size(s_width, m_image_datas.rows / scale_rate));
	//缩放人脸位置
	p_x /= scale_rate;
	p_y /= scale_rate;
	p_width /= scale_rate;
	p_height /= scale_rate;
	//----------------------------------------------------------------------------------------------------------------------
	//用拉普拉斯边缘滤波器得到全尺寸边缘掩码
	Mat l_mask = getLaplacianMask(m_image_datas);
	//加强掩码
	threshold(l_mask, l_mask, 80, 255, THRESH_BINARY);
	Mat b_mask = Mat::zeros(s_img.rows + 2, s_img.cols + 2, CV_8UC1);
	threshold(b_mask, b_mask, 255, 255, THRESH_BINARY_INV);
	Mat tmp_mask = b_mask(Rect(1, 1, b_mask.cols - 2, b_mask.rows - 2));
	resize(l_mask, tmp_mask, Size(tmp_mask.cols, tmp_mask.rows));
	//输出
	//out_put_img(b_mask, 0);
	//----------------------------------------------------------------------------------------------------------------------
	//形态学变换，删除掩码图间间隙
	dilate(tmp_mask, tmp_mask, Mat());
	erode(tmp_mask, tmp_mask, Mat());
	//输出
	//out_put_img(b_mask, 1);
	//----------------------------------------------------------------------------------------------------------------------
	//转码
	Mat tmp;
	cvtColor(s_img, tmp, CV_BGRA2BGR);
	//YCrCb即YUV
	//Y：亮度
	//U：彩度
	//V：饱和度
	//Y = (B * 1868 + G * 9617 + R * 4899 + 8192) / 16384;
	//U = ((B - Y)* 9241 + 8192)/16384 + 128;
	//V = ((R - Y)*11682 + 8192)/16384 + 128;
	//R = Y + 1.14V
	//G = Y - 0.39U - 0.58V
	//B = Y + 2.03U
	cvtColor(tmp, s_img, CV_BGR2YCrCb);
	//----------------------------------------------------------------------------------------------------------------------
	//执行漫水填充
	Point skin_p[6];
	skin_p[0] = Point(p_x + p_width / 2, p_y + p_height / 2 - p_height / 6);
	skin_p[1] = Point(p_x + p_width / 2 - p_width / 11, p_y + p_height / 2 - p_height / 6);
	skin_p[2] = Point(p_x + p_width / 2 + p_width / 11, p_y + p_height / 2 - p_height / 6);
	skin_p[3] = Point(p_x + p_width / 2, p_y + p_height / 2 + p_height / 16);
	skin_p[4] = Point(p_x + p_width / 2 - p_width / 9, p_y + p_height / 2 + p_height / 16);
	skin_p[5] = Point(p_x + p_width / 2 + p_width / 9, p_y + p_height / 2 + p_height / 16);

	Mat tmp_img;
	s_img.copyTo(tmp_img);
	IplImage img(tmp_img);
	for (int i = 0; i < 6; i++) {
		cvCircle(&img, skin_p[i], 2, CV_RGB(255, 0, 0), 3);
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "size(%i,%i)-->pi(%i,%i)", img.width, img.height, skin_p[i].x, skin_p[i].y);
	}
	rectangle(tmp_img, Rect(p_x, p_y, p_width, p_height), CV_RGB(255, 0, 0), 1);
	//out_put_img(tmp_img, 1, 2);
	//亮度，色度，饱和度差异
	Scalar lowerDiff = Scalar(60, 25, 20);
	Scalar upperDiff = Scalar(80, 15, 15);
	Mat r_tmp_mask;
	tmp_mask.copyTo(r_tmp_mask);
	for (int i = 0; i < 6; i++) {
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "pi(%i,%i)", skin_p[i].x, skin_p[i].y);
		//floodFill(s_img, b_mask, skin_p[i], Scalar(255, 0, 0), NULL, lowerDiff, upperDiff, 4 | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY);
		floodFill(s_img, b_mask, skin_p[i], Scalar(255, 0, 0), NULL, lowerDiff, upperDiff, 8 + (255 << 8) + CV_FLOODFILL_FIXED_RANGE);
		//floodFill(s_img, b_mask, skin_p[i], Scalar(0, 0, 0), NULL, lowerDiff, upperDiff);
	}
	tmp_mask -= r_tmp_mask;
	//输出
	//out_put_img(tmp_mask, 3);
	//----------------------------------------------------------------------------------------------------------------------
	Mat out_tmp_mask(m_image_datas.size(), CV_8UC1);
	resize(tmp_mask, out_tmp_mask, out_tmp_mask.size());
	add(m_image_datas, Scalar(100, 0, 0, 0), m_image_datas, out_tmp_mask);
}