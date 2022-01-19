#include "RtspClient.h"

RtspClient::RtspClient() {
    out_filename = nullptr;
}

RtspClient::~RtspClient() = default;


void RtspClient::log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

bool RtspClient::open(const char *rtspUrl) {
    running = true;


    /**
//    SwsContext *img_convert_ctx;
    AVFormatContext *context = avformat_alloc_context();
    AVCodecContext *pContext = avcodec_alloc_context3(nullptr);
    int video_stream_index = -1;

    avformat_network_init();

    AVDictionary *option = nullptr;
    av_dict_set(&option, "rtsp_transport", "tcp", 0);

    // Open RTSP
    if (int err = avformat_open_input(&context, rtspUrl, nullptr, &option) != 0) {
        LOGE("Cannot open input %s, error code: %d", rtspUrl, err);
        return false;
    }

    av_dict_free(&option);

    if (avformat_find_stream_info(context, nullptr) < 0) {
        LOGE("Cannot find stream info");
        return false;
    }

    // Search video stream
    for (int i = 0; i < context->nb_streams; i++) {
        if (context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = i;
    }

    if (video_stream_index == -1) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Video stream not found");
        return false;
    }

    AVPacket packet;
    av_init_packet(&packet);

    // Open output file
    AVFormatContext *oc = avformat_alloc_context();
    AVStream *stream = nullptr;

    // Start reading packets from stream and write them to file
    av_read_play(context);

    AVCodec *codec = NULL;
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        LOGE("Cannot find decoder H264");
        return false;
    }

    avcodec_get_context_defaults3(pContext, codec);
    avcodec_copy_context(pContext, context->streams[video_stream_index]->codec);

    if (avcodec_open2(pContext, codec, NULL) < 0) {
        LOGE("Cannot open codec");
        return false;
    }

//    img_convert_ctx = sws_getContext(pContext->width, pContext->height, pContext->pix_fmt,
//                                     pContext->width, pContext->height,
//                                     AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    auto size = (size_t) avpicture_get_size(AV_PIX_FMT_YUV420P, pContext->width,
                                            pContext->height);
    auto *picture_buf = (uint8_t *) (av_malloc(size));
    AVFrame *pic = av_frame_alloc();
    AVFrame *picrgb = av_frame_alloc();
    auto size2 = (size_t) avpicture_get_size(AV_PIX_FMT_RGB24, pContext->width, pContext->height);
    auto *picture_buf2 = (uint8_t *) (av_malloc(size2));
    avpicture_fill((AVPicture *) pic, picture_buf, AV_PIX_FMT_YUV420P, pContext->width,
                   pContext->height);
    avpicture_fill((AVPicture *) picrgb, picture_buf2, AV_PIX_FMT_RGB24, pContext->width,
                   pContext->height);


    timeval timeout{};
    const int millisecond = 10;
    timeout.tv_sec = millisecond / 1000;
    timeout.tv_usec = millisecond % 1000 * 1000;


    while (running && av_read_frame(context, &packet) >= 0) {
        if (!recording && f) {
            fflush(f);
            fclose(f);
            f = nullptr;
        }
        if (packet.stream_index == video_stream_index) { // Packet is video
            if (stream == NULL) {
                stream = avformat_new_stream(oc,
                                             context->streams[video_stream_index]->codec->codec);
                avcodec_copy_context(stream->codec, context->streams[video_stream_index]->codec);
                stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
            }

//            int check = 0;
//            packet.stream_index = stream->id;
//            LOGD("packet:%d", packet.size);

            if (f && recording) {
                fwrite(packet.data, 1, packet.size, f);
            }
//            avcodec_decode_video2(pContext, pic, &check, &packet);

//            sws_scale(img_convert_ctx, (const uint8_t *const *) pic->data, pic->linesize, 0,
//                      pContext->height, picrgb->data, picrgb->linesize);
//
//            LOGD("width:%d  height:%d", pContext->width, pContext->height);
//            callback(env, picture_buf2, 3, pContext->width, pContext->height);

            LOGD("*");
        }
//        select(0, NULL, NULL, NULL, &timeout);

        LOGD("_");

//        av_usleep(30 * 1000);
    }
    av_free_packet(&packet);
//    av_init_packet(&packet);
    fflush(f);
    fclose(f);

    av_free(pic);
    av_free(picrgb);
    av_free(picture_buf);
    av_free(picture_buf2);

    av_read_pause(context);
    avio_close(oc->pb);
    avformat_free_context(oc);
    avformat_close_input(&context);
//    return true;

*/










    const AVOutputFormat *ofmt = nullptr;
    AVFormatContext *ifmt_ctx = nullptr;
    AVFormatContext *ofmt_ctx = nullptr;
    AVPacket *pkt = nullptr;
    int ret, i;
    int stream_index = 0;
    int *stream_mapping = nullptr;
    unsigned int stream_mapping_size;

    pkt = av_packet_alloc();
    if (!pkt) {
        LOGD("Could not allocate AVPacket");
        return false;
    }

    AVDictionary *rtsp_option = nullptr;
    av_dict_set(&rtsp_option, "rtsp_transport", "tcp", 0);

    if ((ret = avformat_open_input(&ifmt_ctx, rtspUrl, nullptr, &rtsp_option)) < 0) {
        LOGD("Could not open '%s'", rtspUrl);
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        LOGD("Failed to retrieve input stream information");
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, rtspUrl, 0);

    stream_mapping_size = ifmt_ctx->nb_streams;
    stream_mapping = static_cast<int *>(av_calloc(stream_mapping_size, sizeof(*stream_mapping)));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, out_filename);
    if (!ofmt_ctx) {
        LOGD("Could not create output context");
        ret = AVERROR_UNKNOWN;
        goto end;
    }



    ofmt = ofmt_ctx->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *out_stream;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        out_stream = avformat_new_stream(ofmt_ctx, nullptr);
        if (!out_stream) {
            LOGD("Failed allocating output stream");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            LOGD("Failed to copy codec parameters");
            goto end;
        }
        out_stream->codecpar->codec_tag = 0;
    }
    av_dump_format(ofmt_ctx, 0, out_filename, 1);

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            LOGD("Could not open output file '%s'", out_filename);
            goto end;
        }
    }

    ret = avformat_write_header(ofmt_ctx, nullptr);
    if (ret < 0) {
        LOGE("Error occurred when opening output file:%d", ret);
        goto end;
    }

    while (running) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, pkt);
        if (ret < 0)
            break;

        in_stream = ifmt_ctx->streams[pkt->stream_index];
        if (pkt->stream_index >= stream_mapping_size ||
            stream_mapping[pkt->stream_index] < 0) {
            av_packet_unref(pkt);
            continue;
        }


        if ()



        pkt->stream_index = stream_mapping[pkt->stream_index];
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
            LOGD("Error muxing packet");
            break;
        }
    }

    av_write_trailer(ofmt_ctx);
    end:
    av_packet_free(&pkt);

    avformat_close_input(&ifmt_ctx);

    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);

    avformat_free_context(ofmt_ctx);

    av_freep(&stream_mapping);

    if (ret < 0 && ret != AVERROR_EOF) {
        LOGD("Error occurred: %s", av_err2str(ret));
        return false;
    }

    return true;
}

void RtspClient::close() {
    running = false;
}

void RtspClient::start(const char *_pathName) {
    recording = true;
    if (out_filename) delete(out_filename);
    out_filename = new char [strlen(_pathName) + 1];
    strcpy(out_filename, _pathName);
    LOGD("文件路径 >>> %s", out_filename);
}

void RtspClient::stop() {
    recording = false;
}
