//
// Created by sjh on 2021/12/28.
//

#ifndef FFMPEG_ANDROID_RTSPCLIENT_H
#include "ALog.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include <libavutil/time.h>
#include <libavutil/timestamp.h>
}
#define FFMPEG_ANDROID_RTSPCLIENT_H


class RtspClient {
private:
    bool running = true;
    bool recording = false;
    static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag);
    char* out_filename;
    RtspClient();
    ~RtspClient();
public:
    bool open(const char *rtspUrl);
    void close();

    void start(const char *pathName);
    void stop();

    static RtspClient& getInstance()
    {
        static RtspClient instance;
        return instance;
    }
    RtspClient(RtspClient const&) = delete;
    void operator=(RtspClient const&) = delete;
};


#endif //FFMPEG_ANDROID_RTSPCLIENT_H
