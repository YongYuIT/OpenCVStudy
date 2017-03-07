#include"tools.h"
char* jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

void set_out_params(JNIEnv * _env, jobject _listener_obj, jmethodID _onHandling_id) {
	env = _env;
	listener_obj = _listener_obj;
	onHandling_id = _onHandling_id;
}

void out_put_img(Mat out_put, int index) {
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

//formate={1:YCrCb}
void out_put_img(Mat out_put, int formate, int index) {
	if (formate == 1) {
		Mat tmp;
		cvtColor(out_put, tmp, CV_YCrCb2BGR);
		out_put_img(tmp, index);
	}
}

Mat getLaplacianMask(Mat& source_img)
{
	Mat hui_img;
	cvtColor(source_img, hui_img, CV_BGRA2GRAY);
	medianBlur(hui_img, hui_img, 9);
	Mat lap_img;
	Laplacian(hui_img, lap_img, CV_8U, 5);
	return lap_img;
}
