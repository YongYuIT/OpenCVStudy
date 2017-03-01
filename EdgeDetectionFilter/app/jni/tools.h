#ifndef COM_THINKING_TOOLS
#define COM_THINKING_TOOLS
#include <opencv2/objdetect/objdetect.hpp>
using namespace std;
using namespace cv;
bool do_save_img(string save_path_name, Mat mat);
bool do_save_img(string save_path_name, IplImage img);
#endif