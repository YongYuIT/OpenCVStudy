#ifndef COM_THINKING_TOOLS
#define COM_THINKING_TOOLS

#include "com_thinking_edgedetectionfilter_ImageHandler.h"

using namespace std;
using namespace cv;

bool do_save_img(string save_path_name, Mat mat);
bool do_save_img(string save_path_name, IplImage img);
void out_put_img(JNIEnv * env, jobject listener_obj, jmethodID onHandling_id, Mat out_put, int width, int height, int index);
#endif