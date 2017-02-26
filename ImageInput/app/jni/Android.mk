LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
OPENCV_LIB_TYPE:=STATIC
OPENCV_INSTALL_MODULES:=on
include G:\OpenCV-2.4.11-android-sdk\OpenCV-android-sdk\sdk\native\jni\OpenCV.mk

LOCAL_MODULE    := com_thinking_opencv
LOCAL_SRC_FILES := com_thinking_imageinput_NdkTools.cpp \
    tools.cpp \

LOCAL_LDLIBS +=  -llog -ldl
include $(BUILD_SHARED_LIBRARY)
