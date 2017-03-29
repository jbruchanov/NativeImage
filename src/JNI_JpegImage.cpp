//
// Created by jbruchanov on 29/03/17.
//

#include "JNI_JpegImage.h"
#include "../json11/json11.hpp"
#include "JPEGImage.hpp"
#include "LogHelper.h"

#define CLASS_NAME "com/scurab/andriod/nativeimage/JPEGImage"
#define METHOD_SET_NATIVE_REF "onSetNativeRef"
#define METHOD_GET_NATIVE_REF "getNativeRef"


using namespace json11;
using namespace std;

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage_init
        (JNIEnv *env, jobject obj) {
    jclass clazz = env->FindClass(CLASS_NAME);
    jmethodID methodId = env->GetMethodID(clazz, METHOD_SET_NATIVE_REF, "(J)V");//void onSetNativeRef(long ref)
    // Call the method on the object

    JPEGImage *image = new JPEGImage();
    LOGD("JPEGImagePointer:%ld", (long)image);
    env->CallVoidMethod(obj, methodId, (jlong)image);
}

JPEGImage* getObject(JNIEnv *env, jobject obj) {
    jclass clazz = env->FindClass(CLASS_NAME);
    jmethodID methodId = env->GetMethodID(clazz, METHOD_GET_NATIVE_REF, "()J");//long getNativeRef()
    // Call the method on the object
    jlong ptr = env->CallLongMethod(obj, methodId);
    return (JPEGImage*)ptr;
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    loadImage
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage_loadImage
(JNIEnv *env, jobject obj, jstring jpath) {
    JPEGImage *image = getObject(env, obj);
    const char *path = env->GetStringUTFChars(jpath, 0);
    LOGD("Trying reference\n");
    LOGD("Trying reference:%d\n", image->test(10, 20));
    LOGD("LoadingPath:%s\n", path);
    FILE * infile;
    if ((infile = fopen(path, "rb")) == NULL) {
        LOGD("can't open %s\n", path);
        return (jint)1;
    } else {
        LOGD("File opened:%s\n", path);
        fclose(infile);
    }

    int result = image->loadImage(path);
    LOGD("LoadedResult:%d", result);
    env->ReleaseStringUTFChars(jpath, path);
    return (jint)result;
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    getDetails
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage_getMetaData
        (JNIEnv *env, jobject obj) {
    JPEGImage *image = getObject(env, obj);
    const ImageMetaData metaData = image->getMetaData();

    Json result = Json::object {
            { "imageWidth", (int)metaData.imageWidth },
            { "imageHeight", (int)metaData.imageHeight }};

    string json = result.dump();
    return (*env).NewStringUTF(json.c_str());
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage_dispose
(JNIEnv *env, jobject obj) {
    JPEGImage *image = getObject(env, obj);
    LOGD("JPEGImagePointer:%ld", (long)image);
    delete image;
}



