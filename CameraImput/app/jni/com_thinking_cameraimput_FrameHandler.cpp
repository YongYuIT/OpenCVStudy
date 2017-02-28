#include"com_thinking_cameraimput_FrameHandler.h"
#include <android/log.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>
#include"tools.h"
using namespace cv;
using namespace std;


JNIEXPORT void JNICALL Java_com_thinking_cameraimput_FrameHandler_handle_1frame
(JNIEnv * env, jclass j_class, jint w, jint h, jboolean is_rotate, jbyteArray input, jintArray output){
	jbyte* _input = env->GetByteArrayElements(input, 0);
	Mat m_input(h + h / 2, w, CV_8UC1, (unsigned char *)_input);
	//至此，m_input存储的是从Android Camera输入的YUV420sp格式的矩阵。该矩阵是8位单通道的。高度比相同内容的8位4通道格式矩阵多150%。
	//总数据量（以unsigned char为单位）是：h*3/2*w=3hw/2
	//相同内容的8位4通道BGRA格式矩阵的数据量是4hw，即同内容的YUV420sp矩阵的数据量是Bitmap的37.5%
	Mat source;
	//竖屏
	if (is_rotate){
		Mat tmp;
		cvtColor(m_input, tmp, CV_YUV420sp2BGRA);
		Point2f center = Point2f(tmp.cols / 2, tmp.rows / 2);
		double angle = 90;
		double scale = 1;
		Mat rotateMat = getRotationMatrix2D(center, angle, scale);
		warpAffine(tmp, source, rotateMat, tmp.size());
	}
	//横屏
	else {
		cvtColor(m_input, source, CV_YUV420sp2BGRA);
	}
	//------------------------------------------------------------------
	//灰度转换
	Mat tmp;
	cvtColor(source, tmp, CV_BGRA2GRAY);
	cvtColor(tmp, source, CV_GRAY2BGRA);
	//------------------------------------------------------------------
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "width:%i - %i, height:%i - %i, lenth:%i - %i", m_input.cols, source.cols, m_input.rows, source.rows, m_input.cols*m_input.rows*m_input.channels(), source.cols*source.rows*source.channels());
	env->ReleaseIntArrayElements(output, (jint*)source.data, 0);
	return;
}