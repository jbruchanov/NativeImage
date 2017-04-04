//
// Created by jbruchanov on 29/03/17.
//

#ifdef DESKTOP

#include "fakers/AndroidBitmap.h"
#else
#include "android/bitmap.h"
#endif

#include "JNI_JpegImage.h"
#include "../json11/json11.hpp"
#include "Image.hpp"
#include "LogHelper.h"
#include "Errors.h"
#include "JNIHelper.h"
#include "JpegImageProcessor.h"

#define CLASS_NAME "com/scurab/andriod/nativeimage/JPEGImage"
#define METHOD_SET_NATIVE_REF "onSetNativeRef"
#define METHOD_GET_NATIVE_REF "getNativeRef"


using namespace json11;
using namespace std;

Image* getObject(JNIEnv *env, jobject obj) {
    jclass clazz = env->FindClass(CLASS_NAME);
    jmethodID methodId = env->GetMethodID(clazz, METHOD_GET_NATIVE_REF, "()J");//long getNativeRef()
    // Call the method on the object
    jlong ptr = env->CallLongMethod(obj, methodId);
    return reinterpret_cast<Image*>(ptr);
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1init
        (JNIEnv *env, jobject obj, jint componentsPerPixel) {
    jclass clazz = env->FindClass(CLASS_NAME);
    jmethodID methodId = env->GetMethodID(clazz, METHOD_SET_NATIVE_REF, "(J)V");//void onSetNativeRef(long ref)
    // Call the method on the object

    Image *image = new Image(componentsPerPixel);
    LOGD("JPEGImagePointer:%ld", (long)image);
    env->CallVoidMethod(obj, methodId, (jlong)image);
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    loadImage
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1loadImage
(JNIEnv *env, jobject obj, jstring jpath) {
    int result = JNI_ERR;
    try {
        Image *image = getObject(env, obj);
        const char *path = env->GetStringUTFChars(jpath, 0);
        FILE *infile;
        if ((infile = fopen(path, "rb")) == NULL) {
            LOGD("can't open %s\n", path);
            return (jint) 1;
        } else {
            fclose(infile);
        }

        JpegImageProcessor prc;

        IOResult ior = image->loadImage(prc, path);
        LOGD("LoadedResult:%d", ior.result);
        if (OUT_OF_MEMORY == ior.result) {
            string errMsg = string("Unable to load:'") + path + "'";
            result = throwOutOfMemoryError(env, errMsg.c_str());
        }
        env->ReleaseStringUTFChars(jpath, path);
    } catch (...) {
        LOGE("CRASH!!!!!!");
    }
    return (jint)result;
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    getDetails
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1getMetaData
        (JNIEnv *env, jobject obj) {
    Image *image = getObject(env, obj);
    const ImageMetaData metaData = image->getMetaData();

    Json result = Json::object {
            {"imageWidth",    metaData.imageWidth},
            {"imageHeight",   metaData.imageHeight},
            {"componentSize", metaData.componentSize}};

    string json = result.dump();
    return (*env).NewStringUTF(json.c_str());
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1dispose
(JNIEnv *env, jobject obj) {
    Image *image = getObject(env, obj);
    LOGD("JPEGImagePointer:%ld", (long)image);
    delete image;
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    _setPixels
 * Signature: (Landroid/graphics/Bitmap;)V
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1setPixels
        (JNIEnv *env, jobject obj, jobject bitmap) {

    AndroidBitmapInfo info;
    int v;
    v = AndroidBitmap_getInfo(env, bitmap, &info);
    if (v != ANDROID_BITMAP_RESULT_SUCCESS) {
        return v;
    }

    Image *image = getObject(env, obj);
    ImageMetaData metaData = image->getMetaData();
    if (metaData.pixelCount() != (info.height * info.width)) {
        LOGE("Invalid resolution loadedImage:%dx%d vs bitmap:%dx%d", metaData.imageWidth, metaData.imageHeight, info.width, info.height);
        return NOT_SAME_RESOLUTION;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Invalid bitmap format, expected RGBA_8888 was:%d (<android/Bitmap.h>@AndroidBitmapFormat)", info.format);
        return INVALID_BITMAP_FORMAT;
    }

    int *ptr;
    v = AndroidBitmap_lockPixels(env, bitmap, (void **) &ptr);
    if (v != ANDROID_BITMAP_RESULT_SUCCESS) {
        return v;
    }

    image->setPixels(ptr, 4);

    v = AndroidBitmap_unlockPixels(env, bitmap);
    if(v != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("!!! Unable to unlock pixels, something BAD will happen soon !!!");
    }
    //well not sure what to do here if it's not unlocked ?
    return (jint) v;
}

/*
 * Class:     com_scurab_andriod_nativeimage_JPEGImage
 * Method:    _rotate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_JPEGImage__1rotate
        (JNIEnv *env, jobject obj, jint angle) {
    Image *image = getObject(env, obj);
    switch ((int) angle) {
        case 0:
            break;
        case 90:
            image->rotate90();
            break;
        case 180:
            image->rotate180();
            break;
        case 270:
            image->rotate180();
            image->rotate90();
            break;
    }
}


