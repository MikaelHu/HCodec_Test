#include "stdafx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include <libavcodec/avcodec.h>

#include <libswresample/swresample.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
#include <libavutil/opt.h>
}

//ffmpeg libs
#if 0
#pragma comment(lib, "libavformat.dll.a")  
#pragma comment(lib, "libavcodec.dll.a")  
#pragma comment(lib, "libavutil.dll.a") 
#pragma comment(lib, "libswscale.dll.a") 
#else
#pragma comment(lib, "avformat.lib")  
#pragma comment(lib, "avcodec.lib")  
#pragma comment(lib, "avutil.lib") 
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
#endif


static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt);
static int select_sample_rate(const AVCodec *codec);
static int select_channel_layout(const AVCodec *codec);
void convert();


//int main() {
//	convert();
//
//	return 0;
//}


/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt)
{
	const enum AVSampleFormat *p = codec->sample_fmts;

	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == sample_fmt)
			return 1;
		p++;
	}
	return 0;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(const AVCodec *codec)
{
	const int *p;
	int best_samplerate = 0;

	if (!codec->supported_samplerates)
		return 44100;

	p = codec->supported_samplerates;
	while (*p) {
		if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate))
			best_samplerate = *p;
		p++;
	}
	return best_samplerate;
}

/* select layout with the highest channel count */
static int select_channel_layout(const AVCodec *codec)
{
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;

	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;

	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);

		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p++;
	}
	return best_ch_layout;
}


void convert()
{
	//avcodec_register_all();
	av_register_all();

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100.pcm";
	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.aac";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.mp3";
	const char* str_mp2 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.mp2";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.wav";
	const char* str_wma = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.wma";
	const char* str_alac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.alac";
	const char* str_ac3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_ec2.ac3";
	const char* out_file = str_ac3;

	int ret = -1;
	AVFormatContext *ofmt_ctx = NULL;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec = NULL;
	AVPacket *pkt = NULL;
	AVFrame *s16_frame = NULL;
	AVFrame *fltp_frame = NULL;
	SwrContext *swr_ctx = NULL;
	uint8_t *pcm_buffer = NULL;
	AVOutputFormat* fmt = NULL;

	FILE *pcm_f = fopen(str_pcm, "rb+");

	//Method 1.
	/*ofmt_ctx = avformat_alloc_context();
	fmt = av_guess_format(NULL, str_aac, NULL);
	ofmt_ctx->oformat = fmt;
	ret = 0;*/

	//Method 2.
	ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_file);
	//fmt = ofmt_ctx->oformat;

	if (ret<0) {
		printf("avformat_alloc_output_context2 fail \n");
		goto __FAIL;
	}

	dec = avcodec_find_encoder(ofmt_ctx->oformat->audio_codec);
	if (!dec) {
		printf("avcodec_find_encoder fail \n");
		goto __FAIL;
	}
	dec_ctx = avcodec_alloc_context3(dec);
	dec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP;
	if (!check_sample_fmt(dec, dec_ctx->sample_fmt)) {
		fprintf(stderr, "Encoder does not support sample format %s",
			av_get_sample_fmt_name(dec_ctx->sample_fmt));
		goto __FAIL;
	}
	dec_ctx->channel_layout = select_channel_layout(dec);
	dec_ctx->channels = av_get_channel_layout_nb_channels(dec_ctx->channel_layout);
	dec_ctx->sample_rate = select_sample_rate(dec);
	dec_ctx->bit_rate = 64000;

	ret = avio_open(&ofmt_ctx->pb, out_file, AVIO_FLAG_WRITE);
	if (ret<0) {
		printf("avio_open fail \n");
		goto __FAIL;
	}
	ret = avcodec_open2(dec_ctx, dec, NULL);
	if (ret<0) {
		printf("avcodec_open2 fail \n");
		goto __FAIL;
	}
	AVStream *st = avformat_new_stream(ofmt_ctx, dec);
	ret = avcodec_parameters_from_context(st->codecpar, dec_ctx);
	if (ret<0) {
		printf("avcodec_parameters_from_context fail \n");
		goto __FAIL;
	}

	ret = avformat_write_header(ofmt_ctx, NULL);
	if (ret<0) {
		printf("avformat_write_header fail \n");
		goto __FAIL;
	}

	s16_frame = av_frame_alloc();
	if (!s16_frame) {
		printf("av_frame_alloc fail \n");
		goto __FAIL;
	}
	s16_frame->nb_samples = dec_ctx->frame_size;
	s16_frame->format = AV_SAMPLE_FMT_S16;
	s16_frame->channel_layout = AV_CH_LAYOUT_STEREO;
	s16_frame->sample_rate = 44100;
	//    s16_frame->channels = av_get_channel_layout_nb_channels(s16_frame->channel_layout);
	ret = av_frame_get_buffer(s16_frame, 0);
	if (ret<0) {
		printf("av_frame_get_buffer fail \n");
		goto __FAIL;
	}
	pkt = av_packet_alloc();
	if (!pkt) {
		printf("av_packet_alloc fail \n");
		goto __FAIL;
	}
	int pts_i = 0;

	swr_ctx = swr_alloc_set_opts(NULL, dec_ctx->channel_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate, s16_frame->channel_layout, (AVSampleFormat)s16_frame->format, s16_frame->sample_rate, 0, NULL);
	if (!swr_ctx) {
		printf("swr_alloc_set_opts fail \n");
		goto __FAIL;
	}
	ret = swr_init(swr_ctx);
	if (ret<0) {
		printf("swr_init fail \n");
		goto __FAIL;
	}
	fltp_frame = av_frame_alloc();
	fltp_frame->nb_samples = dec_ctx->frame_size;
	fltp_frame->format = dec_ctx->sample_fmt;
	fltp_frame->channel_layout = dec_ctx->channel_layout;
	fltp_frame->sample_rate = dec_ctx->sample_rate;
	//    fltp_frame->channels = av_get_channel_layout_nb_channels(s16_frame->channel_layout);
	ret = av_frame_get_buffer(fltp_frame, 0);
	if (ret<0) {
		printf("av_frame_get_buffer fail \n");
		goto __FAIL;
	}
	uint64_t pcm_buffer_size = s16_frame->nb_samples*av_get_bytes_per_sample((AVSampleFormat)s16_frame->format)*s16_frame->channels;
	pcm_buffer = (uint8_t *)av_malloc(pcm_buffer_size);
	memset(pcm_buffer, 0, pcm_buffer_size);

	while (feof(pcm_f) == 0) {

		size_t size = fread(pcm_buffer, 1, pcm_buffer_size, pcm_f);

		int nb_samples = size / (av_get_bytes_per_sample((AVSampleFormat)s16_frame->format)*s16_frame->channels);
		s16_frame->nb_samples = nb_samples;
		fltp_frame->nb_samples = nb_samples;

		av_samples_fill_arrays(s16_frame->data, s16_frame->linesize, pcm_buffer, s16_frame->channels, s16_frame->nb_samples, (AVSampleFormat)s16_frame->format, 0);

		ret = swr_convert(swr_ctx, fltp_frame->data, fltp_frame->nb_samples, (const uint8_t**)s16_frame->data, s16_frame->nb_samples);

		if (size == 0) {
			printf("fread fail \n");
			break;
		}
		pts_i += fltp_frame->nb_samples;
		fltp_frame->pts = pts_i;
		ret = avcodec_send_frame(dec_ctx, fltp_frame);
		if (ret<0) {
			printf("avcodec_send_frame fail \n");
			break;
		}
		while (1) {
			ret = avcodec_receive_packet(dec_ctx, pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				break;
			}
			else if (ret<0) {
				printf("avcodec_receive_packet fail \n");
				break;
			}
			ret = av_interleaved_write_frame(ofmt_ctx, pkt);
			if (ret<0) {
				printf("av_interleaved_write_frame fail \n");
				break;
			}
			av_packet_unref(pkt);
		}
	}
	ret = avcodec_send_frame(dec_ctx, NULL);
	if (ret<0) {
		printf("avcodec_send_frame fail \n");
		goto __FAIL;
	}
	while (1) {
		ret = avcodec_receive_packet(dec_ctx, pkt);
		if (ret == AVERROR(EINVAL) || ret == AVERROR_EOF) {
			break;
		}
		else if (ret<0) {
			printf("avcodec_receive_packet fail \n");
			break;
		}
		ret = av_interleaved_write_frame(ofmt_ctx, pkt);
		if (ret<0) {
			printf("av_interleaved_write_frame fail \n");
			break;
		}
		av_packet_unref(pkt);
	}
	ret = av_write_trailer(ofmt_ctx);
	if (ret<0) {
		printf("av_write_trailer fail \n");
	}

__FAIL:
	if (ofmt_ctx && ofmt_ctx->pb) {
		avio_close(ofmt_ctx->pb);
	}
	if (dec_ctx) {
		avcodec_close(dec_ctx);
	}
	if (pcm_buffer) {
		av_free(pcm_buffer);
	}
	if (ofmt_ctx) {
		avformat_free_context(ofmt_ctx);
	}
	if (s16_frame) {
		av_frame_free(&s16_frame);
	}
	if (fltp_frame) {
		av_frame_free(&fltp_frame);
	}
	if (pkt) {
		av_packet_free(&pkt);
	}
}
