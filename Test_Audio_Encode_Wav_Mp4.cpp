#include "stdafx.h"
#include <stdio.h>

extern "C" {
//#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>

#include <libswresample/swresample.h>
#include <libavformat/avformat.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/common.h>
//#include <libavutil/frame.h>
//#include <libavutil/samplefmt.h>
//#include <libavutil/opt.h>
}

//ffmpeg libs
#pragma comment(lib, "avformat.lib")  
#pragma comment(lib, "avcodec.lib")  
#pragma comment(lib, "avutil.lib") 
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")


//int main() {
//	av_register_all();
//
//	//const char* inputFilename = "input.wav";
//	//const char* outputFilename = "output.mp4";
//	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100.wav";
//	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hw.mp4";
//
//	AVFormatContext* inputFormatContext = nullptr;
//	AVCodecContext* inputCodecContext = nullptr;
//	AVFormatContext* outputFormatContext = nullptr;
//	// �������ļ�
//	int ret = avformat_open_input(&inputFormatContext, str_wav, nullptr, nullptr);
//	if (ret != 0) {
//		fprintf(stderr, "�޷��������ļ�\n");
//		return -1;
//	}
//	// ������Ƶ����Ϣ
//	if (avformat_find_stream_info(inputFormatContext, NULL) < 0) {
//		fprintf(stderr, "�޷���ȡ��Ƶ����Ϣ\n");
//		return -1;
//	}
//	// ������Ƶ������
//	int audioStreamIndex = -1;
//	for (int i = 0; i < inputFormatContext->nb_streams; i++) {
//		if (inputFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//			audioStreamIndex = i;
//			break;
//		}
//	}
//	// ��������Ƶ������
//	if (audioStreamIndex >= 0) {
//		AVCodecParameters* audioCodecParameters = inputFormatContext->streams[audioStreamIndex]->codecpar;
//		AVCodec* audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
//		if (audioCodec == NULL) {
//			fprintf(stderr, "�Ҳ�����Ƶ������\n");
//			return -1;
//		}
//		inputCodecContext = avcodec_alloc_context3(audioCodec);
//		if (avcodec_parameters_to_context(inputCodecContext, audioCodecParameters) != 0) {
//			fprintf(stderr, "�޷�������Ƶ������������\n");
//			return -1;
//		}
//		if (avcodec_open2(inputCodecContext, audioCodec, NULL) != 0) {
//			fprintf(stderr, "�޷�����Ƶ������\n");
//			return -1;
//		}
//	}
//	else {
//		fprintf(stderr, "�޷��ҵ���Ƶ��\n");
//		return -1;
//	}
//	// ���������ʽ������
//	if (avformat_alloc_output_context2(&outputFormatContext, NULL, NULL, str_mp4) < 0) {
//		fprintf(stderr, "�޷����������ʽ������\n");
//		return -1;
//	}
//	// ������Ƶ��
//	AVStream* audioStream = avformat_new_stream(outputFormatContext, NULL);
//
//	// ������Ƶ�����������ĵ����������
//	if (avcodec_parameters_from_context(audioStream->codecpar, inputCodecContext) < 0) {
//		fprintf(stderr, "�޷����ƽ�����������\n");
//		return -1;
//	}
//	// ������ļ�
//	if (avio_open(&outputFormatContext->pb, str_mp4, AVIO_FLAG_WRITE) < 0) {
//		fprintf(stderr, "�޷�������ļ�\n");
//		return -1;
//	}
//	// д���ļ�ͷ
//	if (avformat_write_header(outputFormatContext, NULL) < 0) {
//		fprintf(stderr, "�޷�д���ļ�ͷ\n");
//		return -1;
//	}
//	// ������Ƶ������
//	AVPacket packet;
//	av_init_packet(&packet);
//	packet.data = NULL;
//	packet.size = 0;
//	while (av_read_frame(inputFormatContext, &packet) >= 0) {
//		if (packet.stream_index == audioStreamIndex) {
//			if (av_write_frame(outputFormatContext, &packet) < 0) {
//				fprintf(stderr, "�޷�д����Ƶ����\n");
//				return -1;
//			}
//		}
//		av_packet_unref(&packet);
//	}
//	// д���ļ�β
//	av_write_trailer(outputFormatContext);
//	// ������Դ
//	avformat_close_input(&inputFormatContext);
//	avcodec_free_context(&inputCodecContext);
//	avio_close(outputFormatContext->pb);
//	avformat_free_context(outputFormatContext);
//	return 0;
//}