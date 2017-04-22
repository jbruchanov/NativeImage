## NativeImage

IDE: [CLion](https://www.jetbrains.com/clion/)<br/>
[Android Lib](https://github.com/jbruchanov/NativeImageAndroid)<br/>

##### Windows
- Install cygwin
  - standard dev packages
  - libjpeg-devel
  - libpng-devel
- Setup CLion to use cygwin
- Be sure you have JAVA JDK
  - if necessary update %JAVA_HOME%/include/win32/[jni.md](https://github.com/jbruchanov/NativeImage/blob/master/java/jni_md.h)
  
##### Linux (Ubuntu)
- install libraries
  - libjpeg-devel
  - libpng-devel
  - oracle java jdk

##### Compilation for Android
- download [CrystaX](https://www.crystax.net/)
- set `ANDROID_NDK` system variable to CrystaX folder
- run `android/`[build.sh](https://github.com/jbruchanov/NativeImage/blob/master/android/build.sh) or [build.cmd](https://github.com/jbruchanov/NativeImage/blob/master/android/build.cmd)

##### Android Debug
- create simple project (not a library)
- copy all files into `./app/src/main/cpp/` so structure will look like
  - `./app/src/main/cpp/android/...`
  - `./app/src/main/cpp/json11/...`
  - `./app/src/main/cpp/src/...`
- compile it
- copy platform folders (x86, armeabi-v7a,...) from `android/obj/` into `./app/src/main/jniLibs/`. Structure will look like as [this](https://github.com/jbruchanov/NativeImageAndroid/tree/master/nativeimage/src/main/jniLibs).
- start app to debug (be sure you have enabled native/dual debugger)

License
-------

    Copyright 2017 Jiri Bruchanov

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
