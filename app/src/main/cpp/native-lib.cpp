#include <jni.h>
#include <string>
#include <unistd.h>
#include "RtspClient.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_et_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    return env->NewStringUTF(avcodec_configuration());
}

extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_play(
        JNIEnv *env,
        jobject /* this */, jstring rtspUrl, jstring pathName) {
    const char * c_rtspUrl = env->GetStringUTFChars(rtspUrl,JNI_FALSE);
    const char * c_pathName = env->GetStringUTFChars(pathName,JNI_FALSE);
    RtspClient::getInstance().play(c_rtspUrl, c_pathName);
    env->ReleaseStringUTFChars(rtspUrl, c_rtspUrl);
    env->ReleaseStringUTFChars(rtspUrl, c_pathName);
}


extern "C" JNIEXPORT void JNICALL
Java_com_et_ffmpeg_MainActivity_stop(
        JNIEnv *env,
        jobject /* this */) {
    RtspClient::getInstance().stop();
}