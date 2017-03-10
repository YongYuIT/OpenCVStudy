#include "com_thinking_imagesegmentation_camera_FrameHandler.h"

JNIEXPORT void JNICALL Java_com_thinking_imagesegmentation_camera_FrameHandler_handle_1frame
(JNIEnv * env, jclass j_class, jint w, jint h, jboolean is_rotate, jbyteArray input, jintArray output){
	jbyte* _input = env->GetByteArrayElements(input, 0);
	Mat m_input(h + h / 2, w, CV_8UC1, (unsigned char *)_input);
	//���ˣ�m_input�洢���Ǵ�Android Camera�����YUV420sp��ʽ�ľ��󡣸þ�����8λ��ͨ���ġ��߶ȱ���ͬ���ݵ�8λ4ͨ����ʽ�����150%��
	//������������unsigned charΪ��λ���ǣ�h*3/2*w=3hw/2
	//��ͬ���ݵ�8λ4ͨ��BGRA��ʽ�������������4hw����ͬ���ݵ�YUV420sp�������������Bitmap��37.5%
	Mat source;
	cvtColor(m_input, source, CV_YUV420sp2BGRA);
	source = source.t();
	IplImage img(source);
	cvFlip(&img, NULL, 1);
	//------------------------------------------------------------------
	//�Ҷ�ת��
	Mat tmp;
	cvtColor(source, tmp, CV_BGRA2GRAY);
	//��˹ģ��
	GaussianBlur(tmp, tmp, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//������˹�߽��˲�
	Mat lv_img;
	Laplacian(tmp, lv_img, CV_8U, 5);
	//��ֵ��
	Mat erzhi_img;
	threshold(lv_img, erzhi_img, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//------------------------------------------------------------------
	//���
	cvtColor(erzhi_img, source, CV_GRAY2BGRA);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "width:%i - %i, height:%i - %i, lenth:%i - %i", m_input.cols, source.cols, m_input.rows, source.rows, m_input.cols*m_input.rows*m_input.channels(), source.cols*source.rows*source.channels());
	env->ReleaseIntArrayElements(output, (jint*)source.data, 0);
	return;
}

