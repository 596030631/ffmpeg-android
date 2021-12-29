//
// Created by sjh on 2021/12/28.
//

#ifndef FFMPEG_ANDROID_RTSPCLIENT_H
#include "ALog.h"
#define FFMPEG_ANDROID_RTSPCLIENT_H


class RtspClient {
private:
    bool recording = false;
public:
    bool play(const char *rtspUrl, const char *string);
    void stop();
    RtspClient();
    ~RtspClient();
};


#endif //FFMPEG_ANDROID_RTSPCLIENT_H
