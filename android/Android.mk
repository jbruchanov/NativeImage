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
include $(CLEAR_VARS)
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp) $(wildcard $(LOCAL_PATH)/../../json11/*.cpp) 
LOCAL_LDLIBS := -llog -ljnigraphics
LOCAL_MODULE    := main
LOCAL_SHARED_LIBRARIES := libjpeg_shared libturbojpeg_shared libpng_shared
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

#keep it after our source code
LIB_JPEG_TURBO := $(ANDROID_NDK)/sources/libjpeg-turbo/1.4.2
include $(LIB_JPEG_TURBO)/Android.mk

LIB_PNG := $(ANDROID_NDK)/sources/libpng/1.6.19
include $(LIB_PNG)/Android.mk