#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_floodFlood
(JNIEnv * env, jclass j_class, jobject img_data_obj, jobject listener_obj)
{
	//----------------------------------------------------------------------------------------------------------------------
	//int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint, Scalar newVal, CV_OUT Rect* rect = 0, Scalar loDiff = Scalar(), Scalar upDiff = Scalar(), int flags = 4);
	//InputOutputArray������&�����1ͨ������3ͨ����8λ���߸���ͼ��
	//InputOutputArray��8λ��ͨ�����룬floodFill�㷨������㿪ʼ����������߽磨���뼴ȫ1����
	//Point��floodFill�㷨���
	//Scalar���㷨�����ɫֵ
	//Rect���㷨��С������������С�����ֵ�����򽫻ᱻ����
	//Scalar�����Ȼ�����ɫ�ĸ������ֵ
	//Scalar�����Ȼ�����ɫ���������ֵ
	//int����־���ɸ��е�����8λ����ͬ����
	jint width;
	jint height;
	jintArray image_datas;
	//����ImageData
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
	//����Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//����Laplacian��Ե�˲����õ�ȫ�ߴ�����
	Mat mask = getLaplacianMask(m_image_datas);
	//��ǿ����
	threshold(mask, mask, 80, 255, THRESH_BINARY);
	//�õ����߽��ȫ�ߴ�����
	//Mat b_mask = Mat::ones(mask.rows + 2, mask.cols + 2, CV_8UC1);
	Mat b_mask = Mat::zeros(mask.rows + 2, mask.cols + 2, CV_8UC1);
	threshold(b_mask, b_mask, 255, 255, THRESH_BINARY_INV);
	Mat tmp_mask = b_mask(Rect(1, 1, mask.cols, mask.rows));
	mask.copyTo(tmp_mask);
	//��̬ѧ�任��ɾ������ͼ���϶
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