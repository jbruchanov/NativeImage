@Echo Off

set DEBUG=0
IF "%1"=="1" (set DEBUG=1)

IF %DEBUG%==1 (echo APP_ABI := x86 armeabi-v7a > Application.mk) ELSE (echo APP_ABI := all > Application.mk)

::delete old files in case something happend
rmdir libs /S /Q
rmdir obj /S /Q

::copy all makefiles into jni folder
mkdir jni
copy Android.mk jni
copy Application.mk jni

cd jni

::be sure it's targeting crystax folder! eg '/home/user/crystax-ndk-10.3.2'
rem call %ANDROID_NDK%\ndk-build NDK_DEBUG=1
echo DEBUG=%DEBUG%
call %ANDROID_NDK%\ndk-build NDK_DEBUG=%DEBUG%

cd ..
rmdir jni /S /Q
del libjpeg.so /S
del libpng.so /S
del libturbojpeg.so /S

IF %DEBUG%==0 (rmdir obj /S /Q)
pause