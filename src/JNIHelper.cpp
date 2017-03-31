//
// Created by scurab on 31/03/17.
//

#include "JNIHelper.h"


#define OUT_OF_MEMORY_EXCEPTION "java/lang/OutOfMemoryError"

jint throwOutOfMemoryError(JNIEnv *env, const char *message) {
    return env->ThrowNew(env->FindClass(OUT_OF_MEMORY_EXCEPTION), message);
}