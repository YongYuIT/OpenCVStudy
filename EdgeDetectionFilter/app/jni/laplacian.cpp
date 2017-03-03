#include "com_thinking_edgedetectionfilter_ImageHandler.h"
#include "tools.h"
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
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//----------------------------------------------------------------------------------------------------------------------
	//����Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//�Բ�ͼִ��˫���˲���ƽ��ƽ̹����ͬʱ���ֱ�Ե�񻯣�
	//step1������
	Mat color_img;
	cvtColor(m_image_datas, color_img, CV_BGRA2BGR);
	Size size = color_img.size();
	size.height = size.height / 2;
	size.width = size.width / 2;
	Mat s_img;
	resize(color_img, s_img, size, 0, 0, INTER_LINEAR);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "s_img height:%i -- weight:%i", s_img.cols, s_img.rows);
	//step2��˫���˲�
	{
		Mat tmp;
		s_img.copyTo(tmp);
		int times = 7;//ִ��7*2��˫���˲�
		int ksize = 9;//��С�����˲�����СΪksize*ksize
		double s_color = 9;//ɫ��ǿ��
		double s_space = 7;//λ��ǿ��
		for (int i = 0; i < times; i++){
			__android_log_print(ANDROID_LOG_INFO, "yuyong", "bilateralFilter %i", i);
			bilateralFilter(s_img, tmp, ksize, s_color, s_space);
			bilateralFilter(tmp, s_img, ksize, s_color, s_space);
		}
	}
	//step3���ظ���С
	Mat b_img;
	resize(s_img, b_img, color_img.size(), 0, 0, INTER_LINEAR);
	//�ص�
	out_put_img(b_img, 0);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "b_img height:%i -- weight:%i", b_img.cols, b_img.rows);
	//----------------------------------------------------------------------------------------------------------------------
	//ת�Ҷ�ͼ
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	//�ص�
	out_put_img(hui_img, 1);
	//----------------------------------------------------------------------------------------------------------------------
	//����
	//�������ͣ�������������׵㣩����������������ڵ㣩����������������ڰ׵㣩����˹�����������ܶȺ���������̬�ֲ���
	//�˲�������˹�˲�
	//        ��ֵ�˲������룬��Ե��
	//        ��ֵ�˲�
	//        ˫���˲�

	//��ֵ�˲�
	medianBlur(hui_img, hui_img, 7);
	//�ص�
	out_put_img(hui_img, 2);
	//----------------------------------------------------------------------------------------------------------------------
	//Laplacian��Ե�˲������ɲ�ͬ���ȵı�Ե
	Mat lap_img;
	Laplacian(hui_img, lap_img, CV_8U, 5);
	//�ص�
	out_put_img(lap_img, 3);
	//----------------------------------------------------------------------------------------------------------------------
	//��ֵ�������߽���ֵ��ֵ����
	Mat erzhi_img;
	//��������������ֵ
	//���ĸ���������ֵ
	//������������ȽϷ�ʽ��THRESH_BINARY��    �����ص�ֵ������ֵʱ  ���ص�ֵȡ��ֵ������ȡ0
	//                      THRESH_BINARY_INV�������ص�ֵ������ֵʱ  ���ص�ֵȡ0��   ����ȡ��ֵ
	//                      THRESH_TRUNC��     �����ص�ֵ������ֵʱ  ���ص�ֵȡ��ֵ�����򲻱�
	//                      THRESH_TOZERO��    �����ص�ֵ������ֵʱ  ���ص�ֵ���䣬  ����ȡ0
	//                      THRESH_TOZERO_INV�������ص�ֵ������ֵʱ  ���ص�ֵȡ0��   ���򲻱�
	threshold(lap_img, erzhi_img, 80, 255, THRESH_BINARY_INV);
	//do_save_img("/storage/emulated/0/test.jpg", result);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "threshold result channels %i, width %i, height %i", erzhi_img.channels(), erzhi_img.cols, erzhi_img.rows);
	//�ص�
	out_put_img(erzhi_img, 4);
	//----------------------------------------------------------------------------------------------------------------------
	//�ϳ�
	Mat out_result;
	b_img.copyTo(out_result);
	out_result.setTo(0);
	b_img.copyTo(out_result, erzhi_img);
	//----------------------------------------------------------------------------------------------------------------------
	//���
	cvtColor(out_result, m_image_datas, CV_BGR2BGRA);
}