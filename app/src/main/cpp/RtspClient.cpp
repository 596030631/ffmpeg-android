#include "RtspClient.h"

RtspClient::RtspClient() {
    ifmt_ctx = nullptr;
    ofmt_ctx = nullptr;
    in_codecpar = nullptr;
    ofmt = nullptr;
}

RtspClient::~RtspClient() = default;


void RtspClient::log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    LOGI("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

bool RtspClient::open(const char *rtspUrl) {
    running = true;
    av_register_all();
    avformat_network_init();

    AVPacket *pkt = nullptr;
    int ret;

    pkt = av_packet_alloc();
    if (!pkt) {
        LOGD("Could not allocate AVPacket");
        return false;
    }

    AVDictionary *rtsp_option = nullptr;
    av_dict_set(&rtsp_option, "rtsp_transport", "udp", 0);
    ret = avformat_open_input(&ifmt_ctx, rtspUrl, nullptr, &rtsp_option);
    if (ret < 0) {
        LOGD("Could not open '%s' %d", rtspUrl, ret);
        return false;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        LOGD("Failed to retrieve input stream information");
        return false;
    }

    av_dump_format(ifmt_ctx, 0, rtspUrl, 0);

    for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *inStream = ifmt_ctx->streams[i];
        in_codecpar = inStream->codecpar;
        LOGW("codec_type %d", in_codecpar->codec_type);
        if (in_codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_trade_index = i;
            break;
        }
    }


    LOGD("摄像头连接完成");
    while (running) {
        AVStream *in_stream, *out_stream;
        ret = av_read_frame(ifmt_ctx, pkt);
        if (ret < 0) {
            LOGE("NO FRAME:%d", ret);
            break;
        }
        in_stream = ifmt_ctx->streams[pkt->stream_index];
        if (pkt->stream_index != video_trade_index) {
            av_packet_unref(pkt);
            continue;
        }

        av_usleep(20*1000);
        LOGI("stream:%d", video_trade_index);

        if (recording && ofmt_ctx) {
            pkt->stream_index = video_trade_index;
            out_stream = ofmt_ctx->streams[pkt->stream_index];
            log_packet(ifmt_ctx, pkt, "in");

            /* copy packet */
            av_packet_rescale_ts(pkt, in_stream->time_base, out_stream->time_base);
            pkt->pos = -1;
            log_packet(ofmt_ctx, pkt, "out");

            ret = av_interleaved_write_frame(ofmt_ctx, pkt);
            /* pkt is now blank (av_interleaved_write_frame() takes ownership of
             * its contents and resets pkt), so that no unreferencing is necessary.
             * This would be different if one used av_write_frame(). */
            if (ret < 0) {
                LOGD("Error muxing packet:%d", ret);
            }
        }
    }
    LOGW("摄像头即将释放");
    av_packet_free(&pkt);
    avformat_close_input(&ifmt_ctx);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE)) {
        avio_closep(&ofmt_ctx->pb);
    }
    if (ofmt_ctx) avformat_free_context(ofmt_ctx);
    if (ret < 0 && ret != AVERROR_EOF) {
        LOGD("Error occurred: %s", av_err2str(ret));
        return false;
    }
    return true;
}

void RtspClient::close() {
    running = false;
}

void RtspClient::start(const char *out_filename) {
    if (ofmt_ctx || video_trade_index < 0) {
        LOGE("NOT READY");
    }
    int ret;
    LOGE("创建输出格式");
    ret = avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, out_filename);
    if (!ofmt_ctx) {
        LOGD("Could not create output context %d %d", video_trade_index, ret);
        return;
    }

    ofmt = ofmt_ctx->oformat;
    av_dump_format(ofmt_ctx, 0, out_filename, 1);
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            LOGD("Could not open output file '%s'", out_filename);
            return;
        }
    }


    AVStream *outStream;
    outStream = avformat_new_stream(ofmt_ctx, nullptr);
    if (!outStream) {
        LOGD("Failed allocating output stream");
        return;
    }

    ret = avcodec_parameters_copy(outStream->codecpar, in_codecpar);
    if (ret < 0) {
        LOGD("Failed to copy codec parameters");
        return;
    }
    outStream->codecpar->codec_tag = 0;

    ret = avformat_write_header(ofmt_ctx, nullptr);
    if (ret < 0) {
        LOGE("Error occurred when opening output file:%d", ret);
        return;
    }

    recording = true;
    LOGD("文件路径 >>> %s", out_filename);
}

void RtspClient::stop() {
    recording = false;
    av_write_trailer(ofmt_ctx);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);
    ofmt_ctx = nullptr;
    LOGE("停止录制");
}
