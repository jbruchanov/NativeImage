#include "JNI_NativeImage.h"

#ifdef DESKTOP

#include "fakers/AndroidBitmap.h"
#else
#include "android/bitmap.h"
#endif

#include "../json11/json11.hpp"
#include "Image.hpp"
#include "LogHelper.h"
#include "Errors.h"
#include "JNIHelper.h"
#include "JpegImageProcessor.h"

#define CLASS_NAME "com/scurab/andriod/nativeimage/NativeImage"
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

/**
 * Java Format enum - JPEG_RGB(1), PNG_RGB(2), PNG_RGBA(3);
 * @param value
 * @return
 */
ImageProcessor* getImageProcessor(int value) {
    switch (value) {
        case 1:
            return new JpegImageProcessor();
//        case 2:
//            break;
//        case 3:
//            break;
    }
}

/*
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _init
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1init
        (JNIEnv *env, jobject thiz, jint componentsPerPixel) {
    jclass clazz = env->FindClass(CLASS_NAME);
    jmethodID methodId = env->GetMethodID(clazz, METHOD_SET_NATIVE_REF, "(J)V");//void onSetNativeRef(long ref)
    // Call the method on the object

    Image *image = new Image(componentsPerPixel);
    LOGD("JPEGImagePointer:%ld", (long)image);
    env->CallVoidMethod(thiz, methodId, (jlong)image);
}

/*
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _loadImage
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1loadImage
        (JNIEnv *env, jobject obj, jstring jpath, jint processor) {
    int result = JNI_ERR;
    const char *path = nullptr;
    try {
        Image *image = getObject(env, obj);
        path = env->GetStringUTFChars(jpath, 0);
        FILE *infile;
        if ((infile = fopen(path, "rb")) == NULL) {
            LOGD("can't open %s\n", path);
            return (jint) CANT_OPEN_FILE;
        } else {
            fclose(infile);
        }
        unique_ptr<ImageProcessor> prc(getImageProcessor((int) processor));
        IOResult ior = image->loadImage(prc.get(), path);
        LOGD("LoadedResult:%d", ior.result);
        if (OUT_OF_MEMORY == ior.result) {
            string errMsg = string("Unable to load:'") + path + "'";
            result = throwOutOfMemoryError(env, errMsg.c_str());
        }
        env->ReleaseStringUTFChars(jpath, path);
    } catch (...) {
        LOGE("CRASH!!!!!!");
    }
    if (path != nullptr) {
        env->ReleaseStringUTFChars(jpath, path);
    }
    return (jint) result;
}

/*
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _saveImage
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1saveImage
        (JNIEnv *env, jobject obj, jstring jpath, jint processor, jstring jsonArgs) {
    int result = JNI_ERR;
    const char *path = nullptr;
    try {
        Json saveArgs = nullptr;
        if (jsonArgs != nullptr) {
            string err;
            const char *json = env->GetStringUTFChars(jsonArgs, 0);
            saveArgs = Json::parse(json, err);
            env->ReleaseStringUTFChars(jsonArgs, json);
            if (err.length() > 0) {
                return INVALID_JSON;
            }
        }
        Image *image = getObject(env, obj);
        path = env->GetStringUTFChars(jpath, 0);
        FILE *infile;
        if ((infile = fopen(path, "wb")) == NULL) {
            LOGD("can't open %s\n", path);
            return (jint) CANT_OPEN_FILE;
        } else {
            fclose(infile);
        }
        unique_ptr<ImageProcessor> prc(getImageProcessor((int) processor));
        IOResult ior = image->saveImage(prc.get(), path, &saveArgs);
        LOGD("SaveResult:%d", ior.result);
    } catch (...) {
        LOGE("CRASH!!!!!!");
    }
    if (path != nullptr) {
        env->ReleaseStringUTFChars(jpath, path);
    }
    return (jint) result;
}

/*
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _getMetaData
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1getMetaData
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
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1dispose
        (JNIEnv *env, jobject obj) {
    Image *image = getObject(env, obj);
    LOGD("JPEGImagePointer:%ld", (long)image);
    delete image;
}
/*
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _setPixels
 * Signature: (Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1setPixels
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
 * Class:     com_scurab_andriod_nativeimage_NativeImage
 * Method:    _rotate
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scurab_andriod_nativeimage_NativeImage__1rotate
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
        default:
            //do nothing JAVA will throw the exception before
            break;
    }
}