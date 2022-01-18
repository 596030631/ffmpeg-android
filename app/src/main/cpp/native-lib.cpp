#include <jni.h>
#include <string>
#include <unistd.h>
#include "RtspClient.h"

extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_open(
        JNIEnv *env,
        jobject /* this */, jstring rtspUrl) {
    const char * c_rtspUrl = env->GetStringUTFChars(rtspUrl,JNI_FALSE);
    RtspClient::getInstance().open(c_rtspUrl);
    env->ReleaseStringUTFChars(rtspUrl, c_rtspUrl);
}


extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_close(
        JNIEnv *env,
        jobject /* this */) {
    RtspClient::getInstance().close();
}

extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_start(
        JNIEnv *env,
        jobject /* this */, jstring pathName) {
    const char * c_pathName = env->GetStringUTFChars(pathName,JNI_FALSE);
    RtspClient::getInstance().start(c_pathName);
    env->ReleaseStringUTFChars(pathName, c_pathName);
}


extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_stop(
        JNIEnv *env,
        jobject /* this */) {
    RtspClient::getInstance().stop();
}