#ifndef COM_THINKING_TOOLS_H
#define COM_THINKING_TOOLS_H

#include "com_thinking_skindetection_SkinHandler.h"

//����ȫ�ֵľ�̬����ֻ���ڳ��������ʱ��Żᱻ����
static CascadeClassifier faceCascade;
static CascadeClassifier eyeCascade1;
static CascadeClassifier eyeCascade2;
static string file_base_path;

char* jstringTostring(JNIEnv* env, jstring jstr);

#endif