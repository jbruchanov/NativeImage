#~/bin/sh
DEBUG=0
if [ "$1" == "1" ]; then
	DEBUG=1
fi

if [ $DEBUG == 1 ]; then
	echo "APP_ABI := x86 armeabi-v7a" > Application.mk
else
	echo "APP_ABI := all" > Application.mk
fi

JNI="jni" #must be 'jni'

#delete old files in case something happend
rm -R $JNI 2> /dev/null 
rm -R libs 2> /dev/null 
rm -R obj 2> /dev/null 
rm -R libjpeg 2> /dev/null 

#copy all makefiles into jni folder
mkdir $JNI
cp Android.mk ./$JNI
cp Application.mk ./$JNI

cd $JNI
#be sure it's targeting crystax folder! eg '/home/user/crystax-ndk-10.3.2'
echo DEBUG=$DEBUG
eval "$ANDROID_NDK/ndk-build NDK_DEBUG=$DEBUG"

cd -
rm -R $JNI 2> /dev/null 

find . -name "libjpeg.so" -type f -delete
find . -name "libpng.so" -type f -delete
find . -name "libturbojpeg.so" -type f -delete
find . -name "gdbserver" -type f -delete
find . -name "gdb.setup" -type f -delete

if [ $DEBUG == 0 ]; then
	rm -R obj 2> /dev/null 
fi