#include "stdafx.h"
#include <Windows.h>
#include "../HCodec/HMuxer.h"
#include "../HCodec/HDeMuxer.h"
#include "../HCodec/AACParser.h"
#include "../HCodec/H264Parser.h"


#ifdef _DEBUG
#pragma comment(lib, "HCodec_d.lib")
#else
#pragma comment(lib, "HCodec.lib")
#endif // _DEBUG



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


int test_HMux_1();
int test_HReMux_1();
int test_HDeMux_1();

//int main(int argc, char** argv)
//{
//	//test_HMux_1();
//	//test_HReMux_1();
//	test_HDeMux_1();
//
//	return 0;
//}


int test_HMux_1() {
	printf("########################test_HMux_1#################################\n");
	printf("###HMuxer\n");
	printf("###mux raw yuv420p and pcm data to mp4, mkv, mov, flv, avi .etc, and save it!\n");
	
	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fdc.pcm";
	const char* str_yuv420 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdc.yuv420";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hmx.mp4";

	int width = 960;
	int height = 540;
	FFVideoFormat vfmt;
	vfmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFAudioFormat afmt;
	afmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);

	FILE *infp_a = NULL;	
	FILE *infp_v = NULL;
	uint8_t *pcm_buf = NULL;
	uint8_t *yuv_buf = NULL;
	HMuxerBase *hmuxer = NULL;
	std::thread *work_thread_1 = NULL;
	std::thread *work_thread_2 = NULL;

	infp_a = fopen(str_pcm, "rb");
	if (infp_a == NULL) {
		printf("cannot open input pcm audio file fail: %s\n", str_pcm);
		goto __FAIL;
	}
	infp_v = fopen(str_yuv420, "rb");
	if (infp_v == NULL) {
		printf("cannot open input pcm video file fail: %s\n", str_yuv420);
		goto __FAIL;
	}	

	const int BUF_SIZE_A = afmt.frame_size_ * afmt.channels_ * 2;
	pcm_buf = new uint8_t[BUF_SIZE_A];
	const int BUF_SIZE_V = vfmt.width_ * vfmt.height_ * 3 >> 1;
	yuv_buf = new uint8_t[BUF_SIZE_V];

	hmuxer = new HMuxer(vfmt, afmt);
	int ret = hmuxer->Open(str_mp4);
	if (ret != 0) {
		printf("HMuxer open fail!");
		goto __FAIL;
	}

	work_thread_1 = new std::thread([&]() {
		while (feof(infp_a) == 0) {
			memset(pcm_buf, 0, BUF_SIZE_A);
			// 读取一帧数数据到缓冲区
			int ret = fread(pcm_buf, 1, BUF_SIZE_A, infp_a);
			if (ret > 0) {
				ret = hmuxer->Mux(pcm_buf, ret, 1);
				if (ret < 0) {
					printf("HMuxer::mux audio err!\n");
				}
			}

			Sleep(10);
		}
	});
	Sleep(10);
	work_thread_2 = new std::thread([&]() {
		while (feof(infp_v) == 0) {
			memset(yuv_buf, 0, BUF_SIZE_V);
			// 读取一帧数数据到缓冲区
			int ret = fread(yuv_buf, 1, BUF_SIZE_V, infp_v);
			if (ret > 0) {
				ret = hmuxer->Mux(yuv_buf, ret, 0);
				if (ret < 0) {
					printf("HMuxer::mux video err!\n");
				}
			}

			Sleep(10);
		}
	});

	Sleep(1000);

	if (work_thread_1->joinable()) {
		work_thread_1->join();
	}
	if (work_thread_2->joinable()) {
		work_thread_2->join();
	}
	hmuxer->Close();

__FAIL:
	// 释放资源
	fclose(infp_v);
	fclose(infp_a);
	SAFE_DEL(pcm_buf);
	SAFE_DEL(yuv_buf);
	SAFE_DEL(hmuxer);
	SAFE_DEL(work_thread_1);
	SAFE_DEL(work_thread_2);

	getchar();

	return 0;
}

int test_HReMux_1() {
	printf("########################test_HReMux_1#################################\n");
	printf("###HReMuxer\n");
	printf("###mux h264 and aac data to mp4, mkv, mov, flv, avi .etc, and save it!\n");

	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100.aac";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.h264";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hrmx.mp4";

	int width = 960;
	int height = 540;
	FFVideoFormat vfmt;
	vfmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFAudioFormat afmt;
	afmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);

	FILE *infp_a = NULL;
	FILE *infp_v = NULL;
	uint8_t *aac_buf = NULL;
	uint8_t *h264_buf = NULL;
	HMuxerBase *hremuxer = NULL;
	std::thread *work_thread_1 = NULL;
	std::thread *work_thread_2 = NULL;

	infp_a = fopen(str_aac, "rb");
	if (infp_a == NULL) {
		printf("cannot open input aac audio file fail: %s\n", str_aac);
		goto __FAIL;
	}
	infp_v = fopen(str_h264, "rb");
	if (infp_v == NULL) {
		printf("cannot open input h264 video file fail: %s\n", str_h264);
		goto __FAIL;
	}

	hremuxer = new HReMuxer(vfmt, afmt);
	int ret = hremuxer->Open(str_mp4);
	if (ret != 0) {
		printf("HReMuxer open fail!");
		goto __FAIL;
	}

	work_thread_1 = new std::thread([&]() {
		// 读取数据到缓冲区
		fseek(infp_a, 0, SEEK_END);
		int f_size = ftell(infp_a);
		fseek(infp_a, 0, SEEK_SET);

		aac_buf = new uint8_t[f_size + 1];
		memset(aac_buf, 0, f_size + 1);

		int ret = fread(aac_buf, 1, f_size, infp_a);
		if (ret <= 0) {
			printf("fread audio data err!\n");
		}
		int pos = 0;

		while (pos < f_size) {
			AAC_t aac = { 0 };
			int pos_t = AACParser().Parse(aac_buf + pos, f_size - pos, aac);
			int aac_header_size = sizeof(aac.fheader);

			ret = hremuxer->Mux(aac.data - aac_header_size, aac.size + aac_header_size, 1);
			if (ret < 0) {
				printf("HReMuxer::mux audio err!\n");
			}

			pos += pos_t + aac_header_size + aac.size;

			Sleep(10);
		}
	});
	Sleep(10);
	work_thread_2 = new std::thread([&]() {
		// 读取数据到缓冲区
		fseek(infp_v, 0, SEEK_END);
		int f_size = ftell(infp_v);
		fseek(infp_v, 0, SEEK_SET);

		h264_buf = new uint8_t[f_size + 1];
		memset(h264_buf, 0, f_size + 1);

		int ret = fread(h264_buf, 1, f_size, infp_v);
		if (ret <= 0) {
			printf("fread video data err!\n");
		}
		int pos = 0;

		while (pos < f_size) {
			uint8_t nalu[1024 * 16] = { 0 };
			int len = 0;
			int pos_t = H264Parser().Parse(h264_buf + pos, f_size - pos, nalu, len);

			ret = hremuxer->Mux(nalu, len, 0);
			if (ret < 0) {
				printf("HReMuxer::mux video err!\n");
			}

			pos += pos_t + len;

			Sleep(10);
		}
	});

	Sleep(1000);

	if (work_thread_1->joinable()) {
		work_thread_1->join();
	}
	if (work_thread_2->joinable()) {
		work_thread_2->join();
	}
	hremuxer->Close();

__FAIL:
	// 释放资源
	fclose(infp_v);
	fclose(infp_a);
	SAFE_DEL(aac_buf);
	SAFE_DEL(h264_buf);
	SAFE_DEL(hremuxer);
	SAFE_DEL(work_thread_1);
	SAFE_DEL(work_thread_2);

	getchar();

	return 0;
}


int test_HDeMux_1() {
	printf("########################test_HDeMux_1#################################\n");
	printf("###HDeMuxer\n");
	printf("###demux mp4, mkv, mov, flv, avi .etc to h264 and aac, and save it!\n");

	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hdmx.aac";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdmx.h264";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";

	FILE *outfp_a = NULL;
	FILE *outfp_v = NULL;
	HDeMuxer *hdemuxer = NULL;

	outfp_a = fopen(str_aac, "wb");
	if (outfp_a == NULL) {
		printf("cannot open aac audio output file fail: %s\n", str_aac);
		goto __FAIL;
	}
	outfp_v = fopen(str_h264, "wb");
	if (outfp_v == NULL) {
		printf("cannot open h264 video output file fail: %s\n", str_h264);
		goto __FAIL;
	}

	hdemuxer = new HDeMuxer();
	int ret = hdemuxer->Open(str_mp4);
	if (ret != 0) {
		printf("HDeMuxer open input media file fail!");
		goto __FAIL;
	}

	hdemuxer->DeMux([&](uint8_t *data, int size, int type) {
		int ret = -1;
		if (type == 0) {	// video data
			ret = fwrite(data, 1, size, outfp_v);
			if (ret <= 0) {
				printf("fwrite video data fail!");
			}
		}
		if (type == 1) {	// audio data
			//uint8_t aac_header[7];
			//AACParser().GetAdtsHeader(aac_header, size + 7);
			//ret = fwrite(aac_header, 1, 7, outfp_a);
			ret = fwrite(data, 1, size, outfp_a);
			if (ret <= 0) {
				printf("fwrite audio data fail!");
			}
		}

		return 0;
	});

	Sleep(1000);

	hdemuxer->Close();

__FAIL:
	// 释放资源
	fclose(outfp_v);
	fclose(outfp_a);
	SAFE_DEL(hdemuxer);

	printf("input one char\n");
	getchar();

	return 0;
}