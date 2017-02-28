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
	//���ˣ�m_input�洢���Ǵ�Android Camera�����YUV420sp��ʽ�ľ��󡣸þ�����8λ��ͨ���ġ��߶ȱ���ͬ���ݵ�8λ4ͨ����ʽ�����150%��
	//������������unsigned charΪ��λ���ǣ�h*3/2*w=3hw/2
	//��ͬ���ݵ�8λ4ͨ��BGRA��ʽ�������������4hw����ͬ���ݵ�YUV420sp�������������Bitmap��37.5%
	Mat source;
	//����
	if (is_rotate){
		Mat tmp;
		cvtColor(m_input, tmp, CV_YUV420sp2BGRA);
		Point2f center = Point2f(tmp.cols / 2, tmp.rows / 2);
		double angle = 90;
		double scale = 1;
		Mat rotateMat = getRotationMatrix2D(center, angle, scale);
		warpAffine(tmp, source, rotateMat, tmp.size());
	}
	//����
	else {
		cvtColor(m_input, source, CV_YUV420sp2BGRA);
	}
	//------------------------------------------------------------------
	//�Ҷ�ת��
	Mat tmp;
	cvtColor(source, tmp, CV_BGRA2GRAY);
	cvtColor(tmp, source, CV_GRAY2BGRA);
	//------------------------------------------------------------------
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "width:%i - %i, height:%i - %i, lenth:%i - %i", m_input.cols, source.cols, m_input.rows, source.rows, m_input.cols*m_input.rows*m_input.channels(), source.cols*source.rows*source.channels());
	env->ReleaseIntArrayElements(output, (jint*)source.data, 0);
	return;
}