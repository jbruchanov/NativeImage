#~/bin/sh

JNI="jni" #must be 'jni'

#delete old files in case something happend
rm -R $JNI 2> /dev/null 
rm -R libs 2> /dev/null 
rm -R obj 2> /dev/null 

#copy all makefiles into jni folder
mkdir $JNI
cp Android.mk ./$JNI
cp Application.mk ./$JNI

cd $JNI
#be sure it's targeting crystax folder! eg '/home/user/crystax-ndk-10.3.2'
eval "$ANDROID_NDK/ndk-build"

cd -
rm -R $JNI 2> /dev/null 