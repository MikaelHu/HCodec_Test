#include "stdafx.h"
#include "../HCodec/FFMpeg/ffdecoder.h"
#include "SDLSpeaker.h"
#include <Windows.h>


#undef main

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
//hnc
#pragma comment(lib, "swresample.lib")
#endif


void testAudio();

int main() {

	testAudio();

	return 0;
}

void addADTStoPacket(uint8_t* packet, int packetLen)
{
	int profile = 2;  //AAC LC
					  //39=MediaCodecInfo.CodecProfileLevel.AACObjectELD;
	int freqIdx = 4;  //44.1KHz
	int chanCfg = 1;  //CPE

					  // fill in ADTS data
	packet[0] = (uint8_t)0xFF;
	packet[1] = (uint8_t)0xF9;
	packet[2] = (uint8_t)(((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
	packet[3] = (uint8_t)(((chanCfg & 3) << 6) + (packetLen >> 11));
	packet[4] = (uint8_t)((packetLen & 0x7FF) >> 3);
	packet[5] = (uint8_t)(((packetLen & 7) << 5) + 0x1F);
	packet[6] = (uint8_t)0xFC;
}

void testAudio() {
	av_register_all();

	//封装格式上下文
	AVFormatContext * pFormatCtx = avformat_alloc_context();

	//2.打开输入音频文件
	if (avformat_open_input(&pFormatCtx, "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3", NULL, NULL) < 0) {
		printf("%s", "打开输入音频文件失败");
		return;
	}
	//3.获取音频信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("%s", "获取音频信息失败");
		return;
	}

	//音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
	int audio_stream_idx = -1;
	int i = 0;
	for (; i < pFormatCtx->nb_streams; i++) {
		//根据类型判断是否是音频流
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audio_stream_idx = i;
			break;
		}
	}

	SDLSpeaker* sdl_speaker = new SDLSpeaker();
	sdl_speaker->NewChannel("Left");
	sdl_speaker->NewChannel("Right");
	sdl_speaker->Start();

	FFAudioFormat a_fmt;
	a_fmt.set(128000, eFF_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 2, 44100, 1024);
	FFAudioDecoder* ff_audio_decoder = new FFAudioDecoder(eFF_CODEC_ID_MP3, a_fmt);

	bool bExit_ = 0;
	std::thread test_thread([&](int audio_stream_index) {
		//编码数据
		/*AVPacket packet;
		av_init_packet(&packet);*/

		AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
		uint8_t *out_data = nullptr;

		const int BUFFSIZE = MAX_AUDIO_FRAME_SIZE * 2;
		out_data = new uint8_t[BUFFSIZE];

		while (!bExit_ && av_read_frame(pFormatCtx, packet) >= 0) {
			if (packet->stream_index == audio_stream_index) {
				uint8_t *data_t = nullptr;

				if (out_data)
					memset(out_data, 0, BUFFSIZE * sizeof(uint8_t));

				data_t = new uint8_t[packet->size + 7];
				if (data_t)
					memset(data_t, 0, packet->size + 7);

				addADTStoPacket(data_t, 7);
				memcpy(data_t + 7, packet->data, packet->size);

				int out_size = BUFFSIZE;
				if (ff_audio_decoder->Decode(packet->data, packet->size/*,data_t, packet->size + 7*/, out_data, out_size) == 0) {
					int lret = 0;
					int out_buf_size = ff_audio_decoder->GetOutbufSize();
					lret = sdl_speaker->Write(out_data, out_buf_size, "Left");
					int rret = 0;
					rret = sdl_speaker->Write(out_data + out_buf_size, out_buf_size, "Right");
				}

				delete[] data_t;
				data_t = nullptr;

				av_packet_unref(packet);
			}
		}

		delete[] out_data;
		out_data = nullptr;

		av_packet_unref(packet);
	}, audio_stream_idx);

	Sleep(1000*160);

	sdl_speaker->Stop();

	delete sdl_speaker;
	delete ff_audio_decoder;
}