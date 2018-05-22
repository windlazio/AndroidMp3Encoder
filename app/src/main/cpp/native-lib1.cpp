//
// Created by junpeng on 2018/5/22.
//

#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_windlazio_mp3encoder_MainActivity_stringFromJNI1(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ Pavelhan3";
    return env->NewStringUTF(hello.c_str());
}