#include"tools.h"

bool do_save_img(string save_path_name, IplImage img){
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "do_save_img to:%s", save_path_name.c_str());
	return cvSaveImage(save_path_name.c_str(), &img) == 1 ? true : false;
}

bool do_save_img(string save_path_name, Mat mat)
{
	IplImage img = mat;
	do_save_img(save_path_name, img);
}

void out_put_img(JNIEnv * env, jobject listener_obj, jmethodID onHandling_id, Mat out_put, int width, int height, int index){
	Mat tmp;
	cvtColor(out_put, tmp, CV_GRAY2BGRA);
	jintArray out = env->NewIntArray(tmp.rows*tmp.cols);
	env->ReleaseIntArrayElements(out, (jint*)tmp.data, 0);
	env->CallVoidMethod(listener_obj, onHandling_id, out, width, height, index);
}