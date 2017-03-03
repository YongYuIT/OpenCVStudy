#include "com_thinking_skindetection_SkinHandler.h"
#include "tools.h"
JNIEXPORT jboolean JNICALL Java_com_thinking_skindetection_SkinHandler_init
(JNIEnv * env, jclass j_class, jstring base_path){
	file_base_path = jstringTostring(env, base_path);
	//获取静态数据
	jfieldID detectors_id = env->GetStaticFieldID(j_class, "detectors", "[Ljava/lang/String;");
	jobjectArray file_names = (jobjectArray)env->GetStaticObjectField(j_class, detectors_id);
	for (int i = 0; i < env->GetArrayLength(file_names); i++){
		jstring path = (jstring)env->GetObjectArrayElement(file_names, i);
		string tmp = file_base_path + "/" + jstringTostring(env, path);
		__android_log_print(ANDROID_LOG_INFO, "yuyong", "get file from %s", tmp.c_str());
	}
}