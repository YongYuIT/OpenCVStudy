/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
/* Header for class com_thinking_skindetection_SkinHandler */

#ifndef _Included_com_thinking_skindetection_SkinHandler
#define _Included_com_thinking_skindetection_SkinHandler
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     com_thinking_skindetection_SkinHandler
	 * Method:    init
	 * Signature: (Ljava/lang/String;)Z
	 */
	JNIEXPORT jboolean JNICALL Java_com_thinking_skindetection_SkinHandler_init
		(JNIEnv *, jclass, jstring);

	/*
	 * Class:     com_thinking_skindetection_SkinHandler
	 * Method:    getFaseLocation
	 * Signature: (Lcom/thinking/skindetection/SkinHandler/ImageData;Lcom/thinking/skindetection/SkinHandler/Position;)V
	 */
	JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_getFaseLocation
		(JNIEnv *, jclass, jobject, jobject);

	/*
	 * Class:     com_thinking_skindetection_SkinHandler
	 * Method:    getEyesLocation
	 * Signature: (Lcom/thinking/skindetection/SkinHandler/ImageData;Lcom/thinking/skindetection/SkinHandler/Position;Lcom/thinking/skindetection/SkinHandler/Position;)V
	 */
	JNIEXPORT void JNICALL Java_com_thinking_skindetection_SkinHandler_getEyesLocation
		(JNIEnv *, jclass, jobject, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif