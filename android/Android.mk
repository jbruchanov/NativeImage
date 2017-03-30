# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

CRYSTAX_LIBJPEG_PATH = $(ANDROID_NDK)

include $(CLEAR_VARS)
LOCAL_MODULE := jpeg
LOCAL_SRC_FILES := ../libjpeg/libs/$(TARGET_ARCH_ABI)/libjpeg.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := ../libjpeg/include
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp) $(wildcard $(LOCAL_PATH)/../../json11/*.cpp) 
LOCAL_LDLIBS := -llog -ljnigraphics
LOCAL_MODULE    := main
LOCAL_SHARED_LIBRARIES := jpeg
#LOCAL_SRC_FILES := main.cpp appjni.cpp json11.cpp
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
# LOCAL_SRC_FILES := ../../src/JNI_JpegImage.cpp

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
