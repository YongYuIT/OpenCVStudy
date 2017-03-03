#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"
JNIEXPORT jboolean JNICALL Java_com_thinking_skindetection_SkinHandler_init
(JNIEnv * env, jclass j_class, jstring base_path) {
	file_base_path = jstringTostring(env, base_path);
	//获取静态数据
	jfieldID detectors_id = env->GetStaticFieldID(j_class, "detectors", "[Ljava/lang/String;");
	jobjectArray file_names = (jobjectArray)env->GetStaticObjectField(j_class, detectors_id);
	CascadeClassifier fiers[3]{ faceCascade,eyeCascade1,eyeCascade2 };
	for (int i = 0; i < env->GetArrayLength(file_names); i++) {
		jstring path = (jstring)env->GetObjectArrayElement(file_names, i);
		string tmp = file_base_path + "/" + jstringTostring(env, path);
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "get file from %s", tmp.c_str());
		//加载目标检测器
		fiers[i].load(tmp);
	}
}

JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_getFaseLocation
(JNIEnv * env, jclass j_class, jobject img_data, jobject position_img) {
	jint width;
	jint height;
	jintArray image_datas;
	//----------------------------------------------------------------------------------------------------------------------
	//解析ImageData
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
	//解析Position
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

}