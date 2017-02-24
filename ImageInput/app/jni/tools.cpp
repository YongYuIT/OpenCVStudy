#include"tools.h"
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>

bool do_save_img(string save_path_name, IplImage img){
	__android_log_print(ANDROID_LOG_INFO, "yuyong", "do_save_img to:%s", save_path_name.c_str());
	return cvSaveImage(save_path_name.c_str(), &img) == 1 ? true : false;
}

bool do_save_img(string save_path_name, Mat mat)
{
	IplImage img = mat;
	do_save_img(save_path_name, img);
}
