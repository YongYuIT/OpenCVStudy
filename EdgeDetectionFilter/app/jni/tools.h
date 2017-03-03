#ifndef COM_THINKING_TOOLS
#define COM_THINKING_TOOLS

#include "com_thinking_edgedetectionfilter_ImageHandler.h"

using namespace std;
using namespace cv;

bool do_save_img(string save_path_name, Mat mat);
bool do_save_img(string save_path_name, IplImage img);
void set_out_params(JNIEnv * _env, jobject _listener_obj, jmethodID _onHandling_id);
void out_put_img(Mat out_put, int index);

#endif