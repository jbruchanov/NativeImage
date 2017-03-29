//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_LOGHELPER_H
#define NATIVEIMAGEAPP_LOGHELPER_H

#ifdef DESKTOP

#include <stdio.h>

#define  LOGE(...)  printf("ERR:");printf(__VA_ARGS__);printf("\n");
#define  LOGW(...)  printf("WARNING:");printf(__VA_ARGS__);printf("\n");
#define  LOGD(...)  printf("DEBUG:");printf(__VA_ARGS__);printf("\n");
#define  LOGI(...)  printf("INFO:");printf(__VA_ARGS__);printf("\n");

#else

#include <android/log.h>
#define  LOG_TAG    "JNI"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

#endif //NATIVEIMAGEAPP_LOGHELPER_H