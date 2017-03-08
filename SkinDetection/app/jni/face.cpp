#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"
JNIEXPORT jboolean JNICALL Java_com_thinking_skindetection_SkinHandler_init
(JNIEnv * env, jclass j_class, jstring base_path) {
	file_base_path = jstringTostring(env, base_path);
	//��ȡ��̬����
	jfieldID detectors_id = env->GetStaticFieldID(j_class, "detectors", "[Ljava/lang/String;");
	jobjectArray file_names = (jobjectArray)env->GetStaticObjectField(j_class, detectors_id);
	CascadeClassifier* fiers[3]{ &faceCascade, &eyeCascade1, &eyeCascade2 };
	for (int i = 0; i < env->GetArrayLength(file_names); i++) {
		jstring path = (jstring)env->GetObjectArrayElement(file_names, i);
		string tmp = file_base_path + "/" + jstringTostring(env, path);
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "get file from %s", tmp.c_str());
		//����Ŀ������
		fiers[i]->load(tmp);
	}
}

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_getFaseLocation
(JNIEnv * env, jclass j_class, jobject img_data, jobject position_img, jobject listener_obj) {
	//----------------------------------------------------------------------------------------------------------------------
	//�������
	if (faceCascade.empty()){
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "faceCascade is fuck empty");
	}
	//----------------------------------------------------------------------------------------------------------------------
	jint width;
	jint height;
	jintArray image_datas;
	//����ImageData
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "start jni call");
	jclass ImageData_class = env->GetObjectClass(img_data);
	jfieldID width_id = env->GetFieldID(ImageData_class, "width", "I");
	width = env->GetIntField(img_data, width_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get width is %i", width);
	jfieldID height_id = env->GetFieldID(ImageData_class, "height", "I");
	height = env->GetIntField(img_data, height_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get height is %i", height);
	jfieldID image_datas_id = env->GetFieldID(ImageData_class, "image_datas", "[I");
	image_datas = (jintArray)env->GetObjectField(img_data, image_datas_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get image_datas lenth is %i", env->GetArrayLength(image_datas));
	jint* _image_datas = env->GetIntArrayElements(image_datas, 0);
	Mat m_image_datas(height, width, CV_8UC4, (unsigned char*)_image_datas);
	//----------------------------------------------------------------------------------------------------------------------
	int p_x;
	int p_y;
	int p_width;
	int p_height;
	//����Position
	jclass Position_class = env->GetObjectClass(position_img);
	jfieldID p_x_id = env->GetFieldID(Position_class, "x", "I");
	jfieldID p_y_id = env->GetFieldID(Position_class, "y", "I");
	jfieldID p_width_id = env->GetFieldID(Position_class, "width", "I");
	jfieldID p_height_id = env->GetFieldID(Position_class, "height", "I");
	p_x = env->GetIntField(position_img, p_x_id);
	p_y = env->GetIntField(position_img, p_y_id);
	p_width = env->GetIntField(position_img, p_width_id);
	p_height = env->GetIntField(position_img, p_height_id);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "get position_img x is %i, y is %i, width is %i, height is %i", p_x, p_y, p_width, p_height);
	//----------------------------------------------------------------------------------------------------------------------
	//����Listener
	jclass Listener_class = env->GetObjectClass(listener_obj);
	jmethodID onHandling_id = env->GetMethodID(Listener_class, "onHandling", "([IIII)V");
	set_out_params(env, listener_obj, onHandling_id);
	//----------------------------------------------------------------------------------------------------------------------
	//�������λ�ò����
	//step1��ת���Ҷ�
	Mat hui_img;
	cvtColor(m_image_datas, hui_img, CV_BGRA2GRAY);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "step1");
	//step2������ͼ��
	Mat s_img;
	float scale_rate;
	const int s_width = 300;
	if (hui_img.cols > s_width){
		scale_rate = hui_img.cols / (float)s_width;
		int heidht = cvRound(hui_img.rows / scale_rate);//��������ȡ��
		resize(hui_img, s_img, Size(s_width, heidht));
	}
	else{
		s_img = hui_img;//ǳ����
	}
	//���
	//out_put_img(s_img, 0);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "step2");
	//step3��ֱ��ͼ���⻯������ͼ��ĶԱȶȺ�����
	Mat zhifang_img;
	equalizeHist(s_img, zhifang_img);
	//���
	//out_put_img(zhifang_img, 1);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "step3");
	//step4���������
	Size min_face_size(50, 50);//��С����ֵ��ֻ��ⲻС��50*50��������
	float search = 1.1f;//ɨ����Ծֵ��Խ����ζ����������ɨ��֮�����Ծ�߶�Խ��ɨ�����Խ�٣��ٶ�Խ�죬ͬʱҲԽ�п��ܶ�ʧĿ��
	int min_different = 4;//��������ֵ���������ֵ�����������ᱻȷ����С�����ֵ�����������ᱻ����
	int find_type = CASCADE_FIND_BIGGEST_OBJECT;//�������ͣ�CASCADE_FIND_BIGGEST_OBJECTֻ�����������
	vector<Rect> faces;
	faceCascade.detectMultiScale(zhifang_img, faces, search, min_different, find_type, min_face_size);
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "detectMultiScale %i", faces.size());
	// Ȧ����������
	if (faces.size() > 0){
		faces[0].x *= scale_rate;
		faces[0].y *= scale_rate;
		faces[0].width *= scale_rate;
		faces[0].height *= scale_rate;
		//������Բ
		rectangle(m_image_datas, faces[0], cvScalar(255, 0, 0, 255), CV_AA);
		int r = faces[0].height*1.1f / 2;
		ellipse(m_image_datas, Point(faces[0].width / 2 + faces[0].x, faces[0].height / 2 + faces[0].y), Size(r * 0.72f, r), 0, 0, 360, cvScalar(255, 0, 0, 255), 4, CV_AA);
	}
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "step4");
	//----------------------------------------------------------------------------------------------------------------------
	//�����������
	//���ڴ�ֵ������������û�õ�
	/*
	p_x = face_location.x;
	p_y = face_location.y;
	p_width = face_location.width;
	p_height = face_location.height;
	*/
	if (faces.size() > 0){
		env->SetIntField(position_img, p_x_id, faces[0].x);
		env->SetIntField(position_img, p_y_id, faces[0].y);
		env->SetIntField(position_img, p_width_id, faces[0].width);
		env->SetIntField(position_img, p_height_id, faces[0].height);
	}
}