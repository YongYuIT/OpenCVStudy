#include "com_thinking_imagesegmentation_camera_FrameHandler.h"

JNIEXPORT void JNICALL Java_com_thinking_imagesegmentation_camera_FrameHandler_handle_1frame
(JNIEnv * env, jclass j_class, jint w, jint h, jboolean is_rotate, jbyteArray input, jintArray output){
	jbyte* _input = env->GetByteArrayElements(input, 0);
	Mat m_input(h + h / 2, w, CV_8UC1, (unsigned char *)_input);
	//至此，m_input存储的是从Android Camera输入的YUV420sp格式的矩阵。该矩阵是8位单通道的。高度比相同内容的8位4通道格式矩阵多150%。
	//总数据量（以unsigned char为单位）是：h*3/2*w=3hw/2
	//相同内容的8位4通道BGRA格式矩阵的数据量是4hw，即同内容的YUV420sp矩阵的数据量是Bitmap的37.5%
	Mat source;
	cvtColor(m_input, source, CV_YUV420sp2BGRA);
	source = source.t();
	IplImage img(source);
	cvFlip(&img, NULL, 1);
	//------------------------------------------------------------------
	//灰度转换
	Mat tmp;
	cvtColor(source, tmp, CV_BGRA2GRAY);
	//高斯模糊
	GaussianBlur(tmp, tmp, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//拉普拉斯边界滤波
	Mat lv_img;
	Laplacian(tmp, lv_img, CV_8U, 5);
	//二值化
	Mat erzhi_img;
	threshold(lv_img, erzhi_img, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//------------------------------------------------------------------
	//输出
	cvtColor(erzhi_img, source, CV_GRAY2BGRA);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "width:%i - %i, height:%i - %i, lenth:%i - %i", m_input.cols, source.cols, m_input.rows, source.rows, m_input.cols*m_input.rows*m_input.channels(), source.cols*source.rows*source.channels());
	env->ReleaseIntArrayElements(output, (jint*)source.data, 0);
	return;
}

