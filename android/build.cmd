::delete old files in case something happend
rmdir libs /S /Q
rmdir obj /S /Q

::copy all makefiles into jni folder
mkdir jni
copy Android.mk jni
copy Application.mk jni

cd jni
::be sure it's targeting crystax folder! eg '/home/user/crystax-ndk-10.3.2'
call %ANDROID_NDK%\ndk-build NDK_DEBUG=1

cd ..
rmdir jni /S /Q