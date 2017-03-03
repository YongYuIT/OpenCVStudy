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

JNIEnv * env;
jobject listener_obj;
jmethodID onHandling_id;
void set_out_params(JNIEnv * _env, jobject _listener_obj, jmethodID _onHandling_id){
	env = _env;
	listener_obj = _listener_obj;
	onHandling_id = _onHandling_id;
}

void out_put_img(Mat out_put, int index){
	Mat tmp;
	if (out_put.channels() == 1)
		cvtColor(out_put, tmp, CV_GRAY2BGRA);
	else if (out_put.channels() == 3)
		cvtColor(out_put, tmp, CV_BGR2BGRA);
	else
		out_put.copyTo(tmp);
	jintArray out = env->NewIntArray(tmp.rows*tmp.cols);
	env->ReleaseIntArrayElements(out, (jint*)tmp.data, 0);
	env->CallVoidMethod(listener_obj, onHandling_id, out, out_put.cols, out_put.rows, index);
}