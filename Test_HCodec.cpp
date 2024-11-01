#include "stdafx.h"
#include <Windows.h>
#include "H264Convetor.h"
#include "AudioConvetor.h"
#include "HDecoder.h"
#include "HEncoder.h"
#include "OpenH264.h"
#include "FrameCodeConvert.h"
#include "SDLAudioPlayer.h"
#include "../HCodec/AACParser.h"

#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "HPlayer_d.lib")
#pragma comment(lib, "HCodec_d.lib")
#pragma comment(lib, "opencv_world310d.lib")
#else
#pragma comment(lib, "HPlayer.lib")
#pragma comment(lib, "HCodec.lib")
#pragma comment(lib, "opencv_world310.lib")
#endif // _DEBUG

#pragma comment(lib, "SDL2.lib")


//SDL 重新定义了main: #define main SDL_main
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
#pragma comment(lib, "swresample.lib")
#endif

int test_H264Convetor_Decode_1();
int test_H264Convetor_Decode_2();
int test_H264Convetor_Encode_Decode_1();
int test_H264Convetor_Encode_1();
int test_H264Convetor_Encode_2();

int test_HV_Decode_1();
int test_HV_Decode_2();
int test_HV_Encode_1();

int test_HA_Decode_1();
int test_HA_Decode_2();
int test_HA_Encode_1();


int test_FFV_Decode_1();
int test_FFV_Decode_2();
int test_FFV_Encode_1();
int test_FFV_Encode_2();
int test_FFV_Encode_3();

int test_FFA_Decode_1();
int test_FFA_Decode_2();
int test_FFA_Decode_3();
int test_FFA_Encode_1();
int test_FFA_Encode_2();

int test_OpenH264_1();

int test_audio_1();
int test_audio_2();
int test_audio_3();



int main(int argc, char** argv)
{
	//test_H264Convetor_Decode_1();
	test_H264Convetor_Decode_2();
	//test_H264Convetor_Encode_Decode_1();
	//test_H264Convetor_Encode_1();
	//test_H264Convetor_Encode_2();
	
	//test_HV_Decode_1();
	//test_HV_Decode_2();
	//test_HV_Encode_1();

	//test_HA_Decode_1();
	//test_HA_Decode_2();
	//test_HA_Encode_1();

	//test_FFV_Decode_1();
	//test_FFV_Decode_2();
	//test_FFV_Encode_1();
	//test_FFV_Encode_2();
	//test_FFV_Encode_3();

	//test_FFA_Decode_1();
	//test_FFA_Decode_2();
	//test_FFA_Decode_3();
	//test_FFA_Encode_1();
	//test_FFA_Encode_2();	

	//test_OpenH264_1();

	//test_audio_1();
	//test_audio_2();
	//test_audio_3();

	return 0;
}



int test_H264Convetor_Decode_1() {
	printf("########################test_H264Convetor_Decode_1#################################\n");
	printf("###decode h264, mp4, mov, mkv .etc and save yuv420 data to file!\n");

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.h264";
	const char* str_mkv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mkv";
	const char* str_mov = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mov";
	const char* str_yuv420 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hcvt.yuv420";

	int width = 960;
	int height = 540;
	FFVideoFormat enc_fmt;
	enc_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat dec_fmt;
	dec_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	H264Convetor(enc_fmt, dec_fmt).Decode(str_mov, str_yuv420);

	return 0;
}

int test_H264Convetor_Decode_2() {
	printf("########################test_H264Convetor_Decode_2#################################\n");
	printf("###decode h264 to bgr24, save and show!\n");

	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdmx.h264";
	const char* str_bgr24 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdmx.bgr24";

	int width = 960;
	int height = 540;
	FFVideoFormat enc_fmt;
	enc_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat dec_fmt;
	dec_fmt.set(width, height, eFF_PIX_FMT_BGR24, 400000, 25);
	H264Convetor h264_convetor(enc_fmt, dec_fmt);

	std::ofstream ofs(str_bgr24, std::ios::out | std::ios::binary);
	if (!ofs.is_open()) {
		std::cerr << "unable to open the output file:" << str_bgr24;
		return -1;
	}

	h264_convetor.DecodeH264(str_h264, [&](uint8_t* out_data, int out_size) {
		//save bgr24 data
		ofs.write((char*)out_data, out_size);

		//show img
		cv::Mat dframe = cv::Mat(cv::Size(dec_fmt.width_, dec_fmt.height_), CV_8UC3, out_data);
		cv::imshow("H264 Decode", dframe);  //显示图像
		cv::waitKey(2);

		return 0;
	});

	ofs.close();

	return 0;
}

int test_H264Convetor_Encode_Decode_1() {
	printf("########################test_H264Convetor_Encode_Decode_1#################################\n");
	printf("###encode yuv420 to h264 and decode to yuv420, then show!\n");

	int width = 960;
	int height = 540;
	FFVideoFormat enc_fmt;
	enc_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat dec_fmt;
	dec_fmt.set(width, height, eFF_PIX_FMT_BGR24, 400000, 25);
	H264Convetor h264_convetor(enc_fmt, dec_fmt);

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	cv::VideoCapture capture(str_mp4);
	uint8_t* yuv_buf = NULL;
	uint8_t* h264_buf = NULL;
	uint8_t* rgb_buf = NULL;
	int h264_buf_size = 1024 * 32;

	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;
		
		cv::Mat frame_resize;
		cv::resize(frame, frame_resize, cv::Size(width, height));

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame_resize, yuvImg, CV_BGR2YUV_I420);
		int buf_len = frame_resize.size().width * frame_resize.size().height * 3 >> 1;
		if (!yuv_buf) {
			yuv_buf = new uint8_t[buf_len];
		}

		memset(yuv_buf, 0, buf_len);
		memcpy(yuv_buf, yuvImg.data, buf_len);//pYuvBuf_即为所获取的YUV420数据 

		if (!h264_buf) {
			h264_buf = new uint8_t[h264_buf_size];
		}
		memset(h264_buf, 0, h264_buf_size);

		int out_size = 0;
		int ret = h264_convetor.Encode(yuv_buf, buf_len, h264_buf, out_size);
		if (ret == 0) {
			int rgb_buf_len = buf_len * 2;
			if (!rgb_buf) {
				rgb_buf = new uint8_t[rgb_buf_len];
			}
			memset(rgb_buf, 0, rgb_buf_len);
			int ret1 = h264_convetor.Decode(h264_buf, out_size, rgb_buf, rgb_buf_len);
			if (ret1 == 0) {
				cv::Mat dframe = cv::Mat(cv::Size(dec_fmt.width_, dec_fmt.height_), CV_8UC3, rgb_buf);
				cv::imshow("H264 Decode", dframe);  //显示图像
				cv::waitKey(2);
			}
		}		
	}

	cv::waitKey(6);
	capture.release();

	SAFE_DEL_A(h264_buf);
	SAFE_DEL_A(rgb_buf);
	SAFE_DEL_A(yuv_buf);

	return 0;
}

int test_H264Convetor_Encode_1() {
	printf("########################test_H264Convetor_Encode_1#################################\n");
	printf("###encode yuv420 to h264 and save to h264 file!\n");

	int width = 960;
	int height = 540;
	FFVideoFormat enc_fmt;
	enc_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat dec_fmt;
	dec_fmt.set(width, height, eFF_PIX_FMT_BGR24, 400000, 25);
	H264Convetor h264_convetor(enc_fmt, dec_fmt);

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hcvt.h264";

	cv::VideoCapture capture(str_mp4);
	uint8_t* yuv_buf = NULL;
	uint8_t* h264_buf = NULL;
	int h264_buf_size = 1024 * 32;


	std::ofstream* ofs = new std::ofstream(str_h264, std::ios::out | std::ios::binary);
	if (!ofs) {
		std::cerr << "unable to open the output file:" << str_h264;
		return -1;
	}

	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;

		cv::Mat frame_resize;
		cv::resize(frame, frame_resize, cv::Size(width, height));

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame_resize, yuvImg, CV_BGR2YUV_I420);
		int buf_len = frame_resize.size().width * frame_resize.size().height * 3 >> 1;
		if (!yuv_buf) {
			yuv_buf = new uint8_t[buf_len];
		}

		memset(yuv_buf, 0, buf_len);
		memcpy(yuv_buf, yuvImg.data, buf_len);//pYuvBuf_即为所获取的YUV420数据 

		if (!h264_buf) {
			h264_buf = new uint8_t[h264_buf_size];
		}
		memset(h264_buf, 0, h264_buf_size);

		int out_size = 0;
		int ret = h264_convetor.Encode(yuv_buf, buf_len, h264_buf, out_size);
		if (ret == 0) {
			ofs->write((char*)h264_buf, out_size);
		}
	}

	cv::waitKey(6);
	capture.release();

	ofs->close();

	SAFE_DEL_A(h264_buf);
	SAFE_DEL_A(yuv_buf);
	SAFE_DEL(ofs);

	return 0;
}

int test_H264Convetor_Encode_2() {
	printf("########################test_H264Convetor_Encode_2#################################\n");
	printf("###encode yuv420 to h264 and save to h264 file!\n");

	const char* str_yuv420p = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdc.yuv420";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hcvt.h264";

	FILE *fp = NULL;
	fp = fopen(str_yuv420p, "rb+");
	if (fp == NULL) {
		printf("cannot open this video file: %s\n", str_yuv420p);
		return -1;
	}

	int width = 960;
	int height = 540;
	FFVideoFormat enc_fmt;
	enc_fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat dec_fmt;
	dec_fmt.set(width, height, eFF_PIX_FMT_BGR24, 400000, 25);
	H264Convetor h264_convetor(enc_fmt, dec_fmt);

	const int BUFSIZE = width * height * 3 >> 1;
	uint8_t *yuv_buf = NULL;
	yuv_buf = new uint8_t[BUFSIZE];

	while (feof(fp) == 0) {
		memset(yuv_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int ret = fread(yuv_buf, 1, BUFSIZE, fp);
		if (ret > 0) {
			ret = h264_convetor.Encode(yuv_buf, ret, str_h264);
			if (ret != 0) {
				printf("HVideoEncoder:: Encode err!\n");
			}
		}
	}

	// 关闭文件和释放资源
	fclose(fp);
	SAFE_DEL_A(yuv_buf);

	return 0;
}


int test_HV_Decode_1() {
	printf("########################test_HV_Decode_1#################################\n");
	printf("###HVideoDecoder\n");
	printf("###decode mp4 and save yuv420 data to file!\n");

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_yuv420 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdc.yuv420";

	FFVideoFormat format;
	format.set(960, 540, eFF_PIX_FMT_YUV420P, 400000, 25);
	HDecoder* hdecoder = new HVideoDecoder(format);
	hdecoder->Open(str_mp4);
	int ret = hdecoder->Decode(str_yuv420);
	if (ret != 0) {
		printf("HVideoDecoder:: Decode err!\n");
	}

	hdecoder->Close();
	SAFE_DEL(hdecoder);

	return 0;
}

int test_HV_Decode_2() {
	printf("########################test_HV_Decode_2#################################\n");
	printf("###HVideoDecoder CB\n");
	printf("###decode mp4(h264) to bgr24, and show!\n");

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";

	FFVideoFormat format;
	format.set(960, 540, eFF_PIX_FMT_BGR24, 400000, 25);

	HVideoDecoder* hdecoder = new HVideoDecoder(format);
	hdecoder->Open(str_mp4);
	int ret = hdecoder->Decode([&](uint8_t* out_data, int out_size) {
		cv::Mat dframe = cv::Mat(cv::Size(format.width_, format.height_), CV_8UC3, out_data);
		cv::imshow("H264 Decode", dframe);  //显示图像
		cv::waitKey(2);

		return 0;
	});

	hdecoder->Close();
	SAFE_DEL(hdecoder);

	return 0;
}

int test_HV_Encode_1() {
	printf("########################test_HV_Encode_1#################################\n");
	printf("###HVideoEncoder\n");
	printf("###encode yuv420 to h264 and save to mp4, mkv, mov, m4v, avi .etc file!\n");

	const char* str_yuv420 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdc.yuv420";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hec.mp4";
	const char* str_mkv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hec.mkv";
	const char* str_mov = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hec.mov";
	const char* str_m4v = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hec.m4v";
	const char* str_avi = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hec.avi";

	FILE *fp = NULL;
	fp = fopen(str_yuv420, "rb+");
	if (fp == NULL) {
		printf("cannot open this video file: %s\n", str_yuv420);
		return -1;
	}

	const int width = 960;
	const int height = 540;
	const int BUFSIZE = width * height * 3 >> 1;
	uint8_t *yuv_buf = NULL;
	yuv_buf = new uint8_t[BUFSIZE];

	FFVideoFormat format;
	format.set(960, 540, eFF_PIX_FMT_YUV420P, 400000, 25);
	HEncoder* hencoder = new HVideoEncoder(format);
	hencoder->Open(str_mp4);

	while (feof(fp) == 0) {
		memset(yuv_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int ret = fread(yuv_buf, 1, BUFSIZE, fp);
		if (ret > 0) {
			ret = hencoder->Encode(yuv_buf, ret);
			if (ret != 0) {
				printf("HVideoEncoder:: Encode err!\n");
			}
		}
	}	

	hencoder->Close();
	SAFE_DEL(hencoder);

	// 关闭文件和释放资源
	fclose(fp);
	SAFE_DEL_A(yuv_buf);

	getchar();

	return 0;
}


int test_HA_Decode_1() {
	printf("########################test_HA_Decode_1#################################\n");
	printf("###HAudioDecoder\n");
	printf("###decode mp4(aac), mkv, mp3 to pcm, and play!\n");

	const char* str_mkv = "H:/movie/维京：王者之战.HD.720p.国俄双语中字.mkv";
	const char* str_mp4 = "H:/movie/芳华.1080p.HD国语中字.mp4";
	const char* str_rmvb = "H:/movie/功夫瑜伽BD国语中字1280高清.rmvb";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	HDecoder* hdecoder = new HAudioDecoder(fmt);
	hdecoder->Open(str_rmvb);

	SDLQueueAudioPlayer sdl_player;
	sdl_player.Start();

	hdecoder->Decode([&](uint8_t* out_data, int out_size) {
		int ret = sdl_player.Write(out_data, out_size);
		sdl_player.Delay(6);
		return 0;
	});

	Sleep(1000 * 20);
	sdl_player.Stop();
	hdecoder->Close();
	SAFE_DEL(hdecoder);

	return 0;
}

int test_HA_Decode_2() {
	printf("########################test_HA_Decode_2#################################\n");
	printf("###HAudioDecoder\n");
	printf("###decode wav, mp3, aac and save it to pcm file!\n");
	
	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hdc.pcm";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.wav";

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	HDecoder* hdecoder = new HAudioDecoder(fmt);
	hdecoder->Open(str_mp3);

	hdecoder->Decode(str_pcm);

	Sleep(1000 * 2);
	hdecoder->Close();
	SAFE_DEL(hdecoder);

	return 0;
}

int test_HA_Encode_1() {
	printf("########################test_HA_Encode_1#################################\n");
	printf("###HAudioEncoder\n");
	printf("###encode pcm to  wav, mp3, aac and save it to file!\n");

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fdc.pcm";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hec.mp3";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hec.wav";
	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hec.aac";

	FILE *fp = NULL;
	fp = fopen(str_pcm, "rb+");
	if (fp == NULL) {
		printf("cannot open this audio file: %s\n", str_pcm);
		return -1;
	}

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	HEncoder* hencoder = new HAudioEncoder(fmt);
	hencoder->Open(str_mp3);

	const int BUFSIZE = fmt.frame_size_ * fmt.channels_ * 2;
	uint8_t *pcm_buf = NULL;
	pcm_buf = new uint8_t[BUFSIZE];

	while (feof(fp) == 0) {
		memset(pcm_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int nret = fread(pcm_buf, 1, BUFSIZE, fp);
		if (nret > 0) {
			nret = hencoder->Encode(pcm_buf, nret);
			if (nret != 0) {
				printf("HAudioEncoder::Encode err!\n");
			}
		}
	}

	Sleep(1000 * 2);
	hencoder->Close();
	SAFE_DEL(hencoder);

	// 关闭文件和释放资源
	fclose(fp);
	SAFE_DEL_A(pcm_buf);

	getchar();

	return 0;
}


int test_FFV_Decode_1() {
	printf("########################test_FFV_Decode_1#################################\n");
	printf("###FFEncoder and FFDecoder\n");
	printf("###encode yuv420 to h264 and decode to bgr24, then show!\n");

	int width = 960;
	int height = 540;
	FFVideoFormat format1;
	format1.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFVideoFormat format2;
	format2.set(width, height, eFF_PIX_FMT_BGR24, 400000, 25);
	FFEncoder* ff_encoder = new FFVideoEncoder(eFF_CODEC_ID_H264, format1);
	FFDecoder* ff_decoder = new FFVideoDecoder(eFF_CODEC_ID_H264, format2);
	ff_encoder->Open();
	ff_decoder->Open();

	int BUFFSIZE = format2.width_ * format2.height_ * 3;
	uint8_t* pYuvBuf = nullptr;
	uint8_t* rgb24 = nullptr;
	uint8_t* dec_buf = nullptr;
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	cv::VideoCapture capture(str_mp4);
	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame, yuvImg, CV_BGR2YUV_I420);
		int bufLen = frame.size().width * frame.size().height * 3 >> 1;

		if (pYuvBuf == nullptr) {
			pYuvBuf = new unsigned char[bufLen];
		}
		if (rgb24 == nullptr) {
			rgb24 = new unsigned char[width * height * 3];
		}

		if (dec_buf == nullptr) {
			dec_buf = new uint8_t[BUFFSIZE];
		}
		memset(dec_buf, 0, BUFFSIZE);
		memset(pYuvBuf, 0, bufLen);
		memcpy(pYuvBuf, yuvImg.data, bufLen * sizeof(unsigned char));//pYuvBuf即为所获取的YUV420数据 

		FFVideoFormat video_format;
		video_format.height_ = frame.size().height;
		video_format.width_ = frame.size().width;
		video_format.fps_ = 25;
		video_format.bitrate_ = 400000;
		video_format.pix_fmt_ = eFF_PIX_FMT_YUV420P;
		dynamic_cast<FFVideoEncoder*>(ff_encoder)->setFFFmt(video_format);

		uint8_t ph264[1024 * 32];
		int out_size = -1;
		int ret = ff_encoder->Encode(pYuvBuf, bufLen, ph264, out_size);
		if (ret == 0) {
			int ret1 = ff_decoder->Decode(ph264, out_size, dec_buf, BUFFSIZE);
			if (ret1 == 0) {
				cv::Mat dframe = cv::Mat(cv::Size(format2.width_, format2.height_), CV_8UC3, dec_buf);
				cv::imshow("H264 Decode", dframe);  //显示图像
				cv::waitKey(25);
			}
		}
	}

	cv::waitKey(6);
	capture.release();

	ff_encoder->Close();
	ff_decoder->Close();
	SAFE_DEL(ff_encoder);
	SAFE_DEL(ff_decoder);
	SAFE_DEL(pYuvBuf);
	SAFE_DEL(dec_buf);

	return 0;
}

int test_FFV_Decode_2() {
	printf("########################test_FFV_Decode_2#################################\n");
	printf("###FFVideoDecoder\n");
	printf("###decode mp4(h264) to bgr24, and show!\n");

	const char* str_mkv = "H:/movie/维京：王者之战.HD.720p.国俄双语中字.mkv";
	const char* str_mp4_1 = "H:/movie/芳华.1080p.HD国语中字.mp4";
	const char* str_rmvb = "H:/movie/功夫瑜伽BD国语中字1280高清.rmvb";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_avi = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.avi";
	const char* str_mov = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mov";
	const char* str_flv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.flv";
	const char* str_m4v= "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.m4v";
	const char* str_wmv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.wmv";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.h264";

	FFVideoFormat fmt;
	fmt.set(960, 540, eFF_PIX_FMT_BGR24, 400000, 25);

	FFVideoDecoder* ffdecoder = new FFVideoDecoder(fmt);
	ffdecoder->Open(str_mp4);
	int ret = ffdecoder->Decode([&](uint8_t* out_data, int out_size) {
		//FFVideoFormat out_fmt;
		//out_fmt = ffdecoder->GetFFFmt();
		cv::Mat dframe = cv::Mat(cv::Size(fmt.width_, fmt.height_), CV_8UC3, out_data);
		cv::imshow("H264 Decode", dframe);  //显示图像
		cv::waitKey(25);

		return 0;
	});

	ffdecoder->Close();
	SAFE_DEL(ffdecoder);

	return 0;
}

int test_FFV_Encode_1() {
	printf("########################test_FFV_Encode_1#################################\n");
	printf("###FFVideoEncoder\n");
	printf("###encode yuv420 to h264 and save to mp4, mkv .etc file!\n");

	const char* str_infile = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mp4";
	const char* str_mkv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mkv";
	const char* str_mov = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mov";
	const char* str_m4v = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.m4v";
	const char* str_avi = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.avi";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.h264";

	int width = 960;
	int height = 540;
	FFVideoFormat fmt;
	fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFEncoder* ff_encoder = new FFVideoEncoder(fmt);
	ff_encoder->Open(str_mp4);
	
	uint8_t* yuv_buf = nullptr;

	cv::VideoCapture capture(str_infile);
	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;
		cv::Mat frame_resize;
		cv::resize(frame, frame_resize, cv::Size(width, height));

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame_resize, yuvImg, CV_BGR2YUV_I420);
		int bufLen = frame_resize.size().width * frame_resize.size().height * 3 >> 1;

		if (yuv_buf == nullptr) {
			yuv_buf = new unsigned char[bufLen];
		}

		memset(yuv_buf, 0, bufLen);
		memcpy(yuv_buf, yuvImg.data, bufLen * sizeof(unsigned char));//pYuvBuf即为所获取的YUV420数据 

		int ret = ff_encoder->Encode(yuv_buf, bufLen);
		if (ret != 0) {
			printf("FFVideoEncoder:Encode err\n");
		}
	}

	cv::waitKey(6);
	capture.release();

	ff_encoder->Close();
	SAFE_DEL(ff_encoder);
	SAFE_DEL(yuv_buf);

	return 0;
}

int test_FFV_Encode_2() {
	printf("########################test_FFV_Encode_2#################################\n");
	printf("###FFVideoEncoder\n");
	printf("###encode yuv420 to h264 and save to h264 file!\n");

	int width = 960;
	int height = 540;
	FFVideoFormat fmt;
	fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFEncoder* ff_encoder = new FFVideoEncoder(eFF_CODEC_ID_H264, fmt);
	ff_encoder->Open();

	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.h264";
	uint8_t* yuv_buf = nullptr;
	uint8_t* h264_buf = nullptr;

	int h264_buf_size = 1024 * 32;

	std::ofstream* ofs = new std::ofstream(str_h264, std::ios::out | std::ios::binary);
	if (!ofs) {
		std::cerr << "unable to open the output file:" << str_h264;
		return -1;
	}

	cv::VideoCapture capture(str_mp4);
	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;
		cv::Mat frame_resize;
		cv::resize(frame, frame_resize, cv::Size(width, height));

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame_resize, yuvImg, CV_BGR2YUV_I420);
		int bufLen = frame_resize.size().width * frame_resize.size().height * 3 >> 1;

		if (yuv_buf == nullptr) {
			yuv_buf = new unsigned char[bufLen];
		}

		memset(yuv_buf, 0, bufLen);
		memcpy(yuv_buf, yuvImg.data, bufLen * sizeof(unsigned char));//pYuvBuf即为所获取的YUV420数据 

		if (h264_buf == nullptr) {
			h264_buf = new uint8_t[h264_buf_size];
		}
		memset(h264_buf, 0, h264_buf_size);

		int ret = ff_encoder->Encode(yuv_buf, bufLen, h264_buf, h264_buf_size);
		if (ret == 0) {
			ofs->write((char*)h264_buf, h264_buf_size);
		}
	}

	cv::waitKey(6);
	capture.release();

	ff_encoder->Close();
	SAFE_DEL(ff_encoder);
	SAFE_DEL(yuv_buf);
	SAFE_DEL(h264_buf);
	ofs->close();
	SAFE_DEL(ofs);

	return 0;
}

int test_FFV_Encode_3() {
	printf("########################test_FFV_Encode_1#################################\n");
	printf("###FFVideoEncoder\n");
	printf("###encode yuv420 to h264 and save to mp4, mkv .etc file!\n");

	const char* str_yuv420 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_hdc.yuv420";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mp4";
	const char* str_mkv = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mkv";
	const char* str_mov = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.mov";
	const char* str_m4v = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.m4v";
	const char* str_avi = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.avi";
	const char* str_h264 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935_fec.h264";

	FILE *fp = NULL;
	fp = fopen(str_yuv420, "rb");
	if (fp == NULL) {
		printf("cannot open this video file: %s\n", str_yuv420);
		return -1;
	}

	int width = 960;
	int height = 540;
	FFVideoFormat fmt;
	fmt.set(width, height, eFF_PIX_FMT_YUV420P, 400000, 25);
	FFEncoder* ff_encoder = new FFVideoEncoder(fmt);
	ff_encoder->Open(str_mp4);

	const int BUFSIZE = width * height * 3 >> 1;
	uint8_t *yuv_buf = NULL;
	yuv_buf = new uint8_t[BUFSIZE];

	while (feof(fp) == 0) {
		memset(yuv_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int ret = fread(yuv_buf, 1, BUFSIZE, fp);
		if (ret > 0) {
			ret = ff_encoder->Encode(yuv_buf, ret);
			if (ret < 0) {
				printf("FFVideoEncoder encode yuv data err!\n");
			}
		}
	}

	// 关闭文件和释放资源
	ff_encoder->Close();
	SAFE_DEL(ff_encoder);
	fclose(fp);
	SAFE_DEL_A(yuv_buf);

	getchar();

	return 0;
}


int test_FFA_Decode_1() {
	printf("########################test_FFA_Decode_1#################################\n");
	printf("###FFAudioDecoder\n");
	printf("###decode mp4(aac), mkv, mp3 to pcm, save and play!\n");

	const char* str_mkv = "H:/movie/维京：王者之战.HD.720p.国俄双语中字.mkv";
	const char* str_mp4 = "H:/movie/芳华.1080p.HD国语中字.mp4";
	const char* str_rmvb = "H:/movie/功夫瑜伽BD国语中字1280高清.rmvb";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";
	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fdc.pcm";

	FILE *outfp = NULL;
	outfp = fopen(str_pcm, "wb");
	if (outfp == NULL) {
		printf("cannot open output pcm audio file: %s\n", str_pcm);
		return -1;
	}

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	FFAudioDecoder* ffdecoder = new FFAudioDecoder(fmt);
	ffdecoder->Open(str_mp3);

	SDLQueueAudioPlayer sdl_player;
	sdl_player.Start();

	ffdecoder->Decode([&](uint8_t* out_data, int out_size) {
		int nret = fwrite(out_data, 1, out_size, outfp);
		if (nret <= 0) {
			printf("Write data to encoded file err!\n");
		}

		nret = sdl_player.Write(out_data, out_size);
		sdl_player.Delay(6);
		return 0;
	});

	Sleep(1000 * 20);
	//释放资源
	sdl_player.Stop();
	ffdecoder->Close();
	SAFE_DEL(ffdecoder);
	fclose(outfp);

	return 0;
}

int test_FFA_Decode_2() {
	printf("########################test_FFA_Decode_2#################################\n");
	printf("###FFAudioDecoder\n");
	printf("###decode wav, mp3, aac to pcm, save and play!\n");

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fdc.pcm";
	const char* str_mp2 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp2";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.wav";
	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.aac";
	const char* str_wma = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.wma";
	const char* str_flac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.flac";
	const char* str_m4a = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.m4a";

	const char* str_aac_hdmx = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hdmx.aac";

	FILE *outfp = NULL;
	outfp = fopen(str_pcm, "wb");
	if (outfp == NULL) {
		printf("cannot open output pcm audio file: %s\n", str_pcm);
		return -1;

	}

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	FFAudioDecoder* ffdecoder = new FFAudioDecoder(fmt);
	int ret = ffdecoder->Open(str_aac_hdmx);

	SDLQueueAudioPlayer sdl_player;
	sdl_player.Start();

	ffdecoder->Decode([&](uint8_t* out_data, int out_size) {
		int nret = fwrite(out_data, 1, out_size, outfp);
		if (nret <= 0) {
			printf("Write data to encoded file err!\n");
		}

		nret = sdl_player.Write(out_data, out_size);
		sdl_player.Delay(6);
		return 0;
	});

	Sleep(1000 * 20);
	//释放资源
	sdl_player.Stop();
	ffdecoder->Close();
	SAFE_DEL(ffdecoder);
	fclose(outfp);

	return 0;
}

int test_FFA_Decode_3() {
	printf("########################test_FFA_Decode_3#################################\n");
	printf("###FFAudioDecoder\n");
	printf("###decode aac to pcm, and play!\n");

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hdmx.pcm";
	const char* str_aac_hdmx = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_hdmx.aac";

	FILE *infp_a = NULL;
	infp_a = fopen(str_aac_hdmx, "rb");
	if (infp_a == NULL) {
		printf("cannot open input aac audio file: %s\n", str_aac_hdmx);
		return -1;
	}

	// 读取数据到缓冲区
	fseek(infp_a, 0, SEEK_END);
	int f_size = ftell(infp_a);
	fseek(infp_a, 0, SEEK_SET);

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	FFAudioDecoder* ffdecoder = new FFAudioDecoder(eFF_CODEC_ID_AAC, fmt);
	int ret = ffdecoder->Open();

	SDLQueueAudioPlayer sdl_player;
	sdl_player.Start();

	uint8_t *aac_buf = NULL;
	aac_buf = new uint8_t[f_size + 1];
	memset(aac_buf, 0, f_size + 1);
	uint8_t *out_data = NULL;
	const int OUT_BUF_SIZE = 44100 * 16;
	out_data = new uint8_t[OUT_BUF_SIZE];

	ret = fread(aac_buf, 1, f_size, infp_a);
	if (ret <= 0) {
		printf("fread audio data err!\n");
	}
	int pos = 0;

	while (pos < f_size) {
		AAC_t aac = { 0 };
		int pos_t = AACParser().Parse(aac_buf + pos, f_size - pos, aac);
		int aac_header_size = sizeof(aac.fheader) + sizeof(aac.vheader);

		memset(out_data, 0, OUT_BUF_SIZE);
		int out_size = OUT_BUF_SIZE;

		ret = ffdecoder->Decode(aac.data - aac_header_size, aac.size + aac_header_size, out_data, out_size);
		if (ret < 0) {
			printf("FFAudioDecoder::Decode audio err!\n");
		}

		ret = sdl_player.Write(out_data, out_size);
		if (ret < 0) {
			printf("SDLQueueAudioPlayer::Write audio err!\n");
		}

		sdl_player.Delay(6);

		pos += pos_t + aac_header_size + aac.size;
	}

	Sleep(1000 * 1);
	//释放资源
	sdl_player.Stop();
	ffdecoder->Close();
	SAFE_DEL(aac_buf);
	SAFE_DEL(ffdecoder);
	fclose(infp_a);

	getchar();

	return 0;
}

int test_FFA_Encode_1() {
	printf("########################test_FFA_Encode_1#################################\n");
	printf("###FFAudioEncoder\n");
	printf("###encode pcm to wav, mp3, aac, and save to file!\n");

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100.pcm";
	const char* str_mp2 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.mp2";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.mp3";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.wav";
	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.aac";
	const char* str_wma = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.wma";
	const char* str_flac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.flac";
	const char* str_alac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.alac";
	const char* str_ac3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.ac3";

	FILE *infp = NULL;
	infp = fopen(str_pcm, "rb");
	if (infp == NULL) {
		printf("cannot open input pcm audio file: %s\n", str_pcm);
		return -1;
	}
	FILE *outfp = NULL;
	outfp = fopen(str_wav, "wb");
	if (outfp == NULL) {
		printf("cannot open output audio file: %s\n", str_mp3);
		return -1;
	}

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	FFAudioEncoder* ffencoder = new FFAudioEncoder(eFF_CODEC_ID_MP3, fmt);
	ffencoder->Open();

	const int BUFSIZE = fmt.frame_size_ * fmt.channels_ * 2;
	uint8_t *pcm_buf = NULL;
	pcm_buf = new uint8_t[BUFSIZE];
	const int EN_BUFSIZE = 1024 * 16;
	uint8_t *enc_buf = NULL;
	enc_buf = new uint8_t[EN_BUFSIZE];

	while (feof(infp) == 0) {
		memset(pcm_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int nret = fread(pcm_buf, 1, BUFSIZE, infp);
		if (nret > 0) {
			memset(enc_buf, 0, EN_BUFSIZE);
			int out_size = EN_BUFSIZE;
			nret = ffencoder->Encode(pcm_buf, nret, enc_buf, out_size);
			if (nret < 0) {
				printf("FFAudioEncoder::Encode err!\n");
				continue;
			}

			/*uint8_t adts_header[7] = { 0 };
			ffencoder->GetAdtsHeader(adts_header, 7 + out_size);
			nret = fwrite(adts_header, 1, 7, outfp);
			if (nret <= 0) {
				printf("Write adts header to file err!\n");
				break;
			}*/
			nret = fwrite(enc_buf, 1, out_size, outfp);
			if (nret <= 0) {
				printf("Write encoded data to file err!\n");
				break;
			}
		}
	}

	ffencoder->Close();
	SAFE_DEL(ffencoder);

	// 关闭文件和释放资源
	fclose(infp);
	SAFE_DEL_A(pcm_buf);
	fclose(outfp);
	SAFE_DEL_A(enc_buf);

	return 0;
}

int test_FFA_Encode_2() {
	printf("########################test_FFA_Encode_2#################################\n");
	printf("###FFAudioEncoder\n");
	printf("###encode pcm to wav, mp3, aac, and save to file!\n");

	const char* str_pcm = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100.pcm";
	const char* str_mp2 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.mp2";
	const char* str_mp3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.mp3";
	const char* str_wav = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.wav";
	const char* str_aac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.aac";
	const char* str_wma = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.wma";
	const char* str_flac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.flac";
	const char* str_alac = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.alac";
	const char* str_ac3 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.ac3";
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.mp4";
	const char* str_m4a = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s_nc_2_44100_fec.m4a";

	uint8_t *pcm_buf = NULL;
	FILE *infp = NULL;

	infp = fopen(str_pcm, "rb+");
	if (infp == NULL) {
		printf("cannot open input pcm audio file: %s\n", str_pcm);
		return -1;
	}

	FFAudioFormat fmt;
	fmt.set(44100, eFF_SAMPLE_FMT_S16, eFF_CH_LAYOUT_STEREO, 2, 128000, 1024);
	FFAudioEncoder* ffencoder = new FFAudioEncoder(fmt);
	int ret = ffencoder->Open(str_mp3);
	if (ret < 0) {
		printf("FFAudioEncoder::Open err!\n");
		goto __FAIL;
	}

	const int BUFSIZE = fmt.frame_size_ * fmt.channels_ * 2;
	pcm_buf = new uint8_t[BUFSIZE];

	while (feof(infp) == 0) {
		memset(pcm_buf, 0, BUFSIZE);
		// 读取一帧数数据到缓冲区
		int nret = fread(pcm_buf, 1, BUFSIZE, infp);
		if (nret > 0) {
			nret = ffencoder->Encode(pcm_buf, nret);
			if (nret < 0) {
				printf("FFAudioEncoder::Encode err!\n");
				continue;
			}
		}
	}

	ffencoder->Close();
	SAFE_DEL(ffencoder);

__FAIL:
	// 关闭文件和释放资源
	fclose(infp);
	SAFE_DEL_A(pcm_buf);

	getchar();

	return 0;
}


int test_OpenH264_1() {
	printf("########################test_OpenH264_1#################################\n");
	printf("###OpenH264Encoder & OpenH264Decoder\n");
	printf("###encode yuv420 to h264 and decode it to yuv420 and show it!\n");

	int pic_width = 960;
	int pic_height = 540;
	s_Enc_Param enc_param = { pic_width, pic_height, 5000000, 25 };
	OpenH264Decoder decoder;
	OpenH264Encoder encoder(enc_param);

	encoder.Open();
	decoder.Open();

	int BUFFSIZE = pic_width * pic_height * 3;

	uint8_t* yuv_buf = nullptr;
	uint8_t* rgb24_buf = nullptr;
	uint8_t* de_yuv_buf = nullptr;
	uint8_t* h264_buf = nullptr;
	const char* str_mp4 = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/video_data/VID_20240906_204935.mp4";
	cv::VideoCapture capture(str_mp4);
	while (true)
	{
		cv::Mat frame;			//定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;		//读取当前帧

		if (frame.empty())
			break;

		cv::Mat frame_rz;
		cv::resize(frame, frame_rz, cv::Size(pic_width, pic_height));
		int width = frame_rz.size().width;
		int height = frame_rz.size().height;

		//convert BGR24 to YUV420
		cv::Mat yuvImg;
		cv::cvtColor(frame_rz, yuvImg, CV_BGR2YUV_I420);
		int bufLen = width * height * 3;

		if (yuv_buf == nullptr) {
			yuv_buf = new unsigned char[bufLen];
		}
		if (rgb24_buf == nullptr) {
			rgb24_buf = new unsigned char[bufLen];
		}

		if (de_yuv_buf == nullptr) {
			de_yuv_buf = new uint8_t[bufLen];
		}

		if (h264_buf == nullptr) {
			h264_buf = new uint8_t[1024 * 32];
		}

		memset(yuv_buf, 0, bufLen);
		memcpy(yuv_buf, yuvImg.data, bufLen * sizeof(unsigned char) >> 1);//pYuvBuf即为所获取的YUV420数据 


		int out_size = -1;
		int err = 0;

		OpenH264Encoder::ST_Enc_CB enc_cb = [&](uint8_t* out_data, int out_size) {
			memset(de_yuv_buf, 0, BUFFSIZE);
			int out_width = 0;
			int out_height = 0;
			int nret = decoder.Decode(out_data, out_size, de_yuv_buf, out_width, out_height, 1, err);
			if (nret == 0) {
				//convert YUV420 to BGR24
				memset(rgb24_buf, 0, bufLen);
				FrameCodeConvert::GetInstance()->YUV420ToRGB24_Pinknoise(de_yuv_buf, rgb24_buf, out_width, out_height);
				cv::Mat dframe = cv::Mat(cv::Size(out_width, out_height), CV_8UC3, rgb24_buf);
				cv::imshow("H264 Decode", dframe);  //显示图像
				cv::waitKey(2);
			}

			return 0;
		};

		s_Pic_Info pic_info = { pic_width, pic_height, 0, videoFormatI420, 25, 0 };
		int nret = encoder.Encode(yuv_buf, bufLen, pic_info, enc_cb, err);
	}

	cv::waitKey(6);
	capture.release();

	SAFE_DEL(yuv_buf);
	SAFE_DEL(rgb24_buf);
	SAFE_DEL(de_yuv_buf);
	SAFE_DEL(h264_buf);

	encoder.Close();
	decoder.Close();

	return 0;
}



#include <iostream>
#include <string>
#include <fstream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

int test_audio_1() {
	printf("########################test_audio_1#################################\n");
	printf("###ffmpeg\n");
	printf("###decode mp4(aac), mkv, mp3 to pcm, save to file and play!\n");

	// 注册FFmpeg的所有组件
	av_register_all();

	std::string in_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";
	std::string out_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.pcm";

	// AVFormatContext 对象创建
	AVFormatContext *avFormatContext = avformat_alloc_context();
	// 打开音频文件
	int ret = avformat_open_input(&avFormatContext, in_file.c_str(), NULL, NULL);
	if (ret != 0) {
		printf("打开文件失败\n");
		return -1;
	}
	// 输出音频文件的信息
	av_dump_format(avFormatContext, 0, in_file.c_str(), 0);
	// 获取音频文件的流信息
	ret = avformat_find_stream_info(avFormatContext, NULL);
	if (ret < 0) {
		printf("获取流信息失败\n");
		return -1;
	}
	// 查找音频流在文件的所有流集合中的位置
	int streamIndex = 0;
	for (int i = 0; i < avFormatContext->nb_streams; ++i) {
		enum AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
		if (avMediaType == AVMEDIA_TYPE_AUDIO) {  //这边和视频不一样，是AUDIO
			streamIndex = i;
		}
	}
	// 拿到对应音频流的参数
	AVCodecParameters *avCodecParameters = avFormatContext->streams[streamIndex]->codecpar;
	// 获取解码器的标识ID
	enum AVCodecID avCodecId = avCodecParameters->codec_id;
	// 通过获取的ID，获取对应的解码器
	AVCodec *avCodec = avcodec_find_decoder(avCodecId);
	// 创建一个解码器上下文对象
	AVCodecContext *avCodecContext = avcodec_alloc_context3(NULL);
	if (avCodecContext == NULL) {
		//创建解码器上下文失败
		printf("创建解码器上下文失败\n");
		return -1;
	}
	// 将新的API中的 codecpar 转成 AVCodecContext
	avcodec_parameters_to_context(avCodecContext, avCodecParameters);
	ret = avcodec_open2(avCodecContext, avCodec, NULL);
	if (ret < 0) {
		printf("打开解码器失败\n");
		return -1;
	}
	printf("decodec name: %s\n", avCodec->name);

	//压缩数据包
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//解压缩后存放的数据帧的对象
	AVFrame *inFrame = av_frame_alloc();
	//frame->16bit 44100 PCM 统一音频采样格式与采样率
	//创建swrcontext上下文件
	SwrContext *swrContext = swr_alloc();
	//音频格式  输入的采样设置参数
	AVSampleFormat inFormat = avCodecContext->sample_fmt;
	// 出入的采样格式
	AVSampleFormat  outFormat = AV_SAMPLE_FMT_S16;
	// 输入采样率
	int inSampleRate = avCodecContext->sample_rate;
	// 输出采样率
	int outSampleRate = 44100;
	// 输入声道布局
	uint64_t in_ch_layout = avCodecContext->channel_layout;
	//输出声道布局
	uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
	//给Swrcontext 分配空间，设置公共参数
	swr_alloc_set_opts(swrContext, out_ch_layout, outFormat, outSampleRate,
		in_ch_layout, inFormat, inSampleRate, 0, NULL
	);
	// 初始化
	swr_init(swrContext);
	// 获取声道数量
	int outChannelCount = av_get_channel_layout_nb_channels(out_ch_layout);

	int currentIndex = 0;
	printf("声道数量%d\n", outChannelCount);
	// 设置音频缓冲区间 16bit   44100  PCM数据, 双声道
	int out_size = outSampleRate / 8 /** av_get_bytes_per_sample(outFormat)*/;
	uint8_t *out_buffer = (uint8_t *)av_malloc(out_size);
	// 创建pcm的文件对象
	FILE *fp_pcm = fopen(out_file.c_str(), "wb");


	SDLQueueAudioPlayer sdl_speaker;
	sdl_speaker.Start();
	

	//开始读取源文件，进行解码
	while (av_read_frame(avFormatContext, packet) >= 0) {
		if (packet->stream_index == streamIndex) {
			avcodec_send_packet(avCodecContext, packet);
			//解码
			ret = avcodec_receive_frame(avCodecContext, inFrame);
			if (ret == 0) {
				//将每一帧数据转换成pcm
				ret = swr_convert(swrContext, &out_buffer, out_size,
					(const uint8_t **)inFrame->data, inFrame->nb_samples);
				if (ret < 0) {
					printf("swr_convert failed %d\n", ret);
					continue;
				}

				//获取实际的缓存大小
				int out_buffer_size = av_samples_get_buffer_size(NULL, outChannelCount, inFrame->nb_samples, outFormat, 1);

				int numBytes = av_get_bytes_per_sample(outFormat);
				int out_size = inFrame->nb_samples * numBytes * outChannelCount;

				// 写入文件
				fwrite(out_buffer, 1, out_buffer_size, fp_pcm);

				//SDL fedd data
				int out_samples = ret;
				sdl_speaker.Write(out_buffer, out_samples * outChannelCount * numBytes);
			}
			printf("正在解码%d\n", currentIndex++);
		}
	}

	Sleep(1000 * 30);
	sdl_speaker.Stop();

	// 及时释放
	fclose(fp_pcm);
	av_frame_free(&inFrame);
	av_free(out_buffer);
	swr_free(&swrContext);
	avcodec_close(avCodecContext);
	avformat_close_input(&avFormatContext);

	return 0;
}

int test_audio_2() {
	printf("########################test_audio_2#################################\n");
	printf("###ffmpeg\n");
	printf("###decode mp4(aac), mkv, mp3 to pcm, save to file!\n");

	// 注册FFmpeg的所有组件
	av_register_all();

	std::string in_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.mp3";
	std::string out_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.pcm";

	// AVFormatContext 对象创建
	AVFormatContext *avFormatContext = avformat_alloc_context();
	// 打开音频文件
	int ret = avformat_open_input(&avFormatContext, in_file.c_str(), NULL, NULL);
	if (ret != 0) {
		printf("打开文件失败\n");
		return -1;
	}
	// 输出音频文件的信息
	av_dump_format(avFormatContext, 0, in_file.c_str(), 0);
	// 获取音频文件的流信息
	ret = avformat_find_stream_info(avFormatContext, NULL);
	if (ret < 0) {
		printf("获取流信息失败\n");
		return -1;
	}
	// 查找音频流在文件的所有流集合中的位置
	int streamIndex = 0;
	for (int i = 0; i < avFormatContext->nb_streams; ++i) {
		enum AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
		if (avMediaType == AVMEDIA_TYPE_AUDIO) {  //这边和视频不一样，是AUDIO
			streamIndex = i;
		}
	}
	// 拿到对应音频流的参数
	AVCodecParameters *avCodecParameters = avFormatContext->streams[streamIndex]->codecpar;
	// 获取解码器的标识ID
	enum AVCodecID avCodecId = avCodecParameters->codec_id;
	// 通过获取的ID，获取对应的解码器
	AVCodec *avCodec = avcodec_find_decoder(avCodecId);
	// 创建一个解码器上下文对象
	AVCodecContext *avCodecContext = avcodec_alloc_context3(NULL);
	if (avCodecContext == NULL) {
		//创建解码器上下文失败
		printf("创建解码器上下文失败\n");
		return -1;
	}
	// 将新的API中的 codecpar 转成 AVCodecContext
	avcodec_parameters_to_context(avCodecContext, avCodecParameters);
	ret = avcodec_open2(avCodecContext, avCodec, NULL);
	if (ret < 0) {
		printf("打开解码器失败\n");
		return -1;
	}
	printf("decodec name: %s\n", avCodec->name);

	//压缩数据包
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//解压缩后存放的数据帧的对象
	AVFrame *inFrame = av_frame_alloc();
	//frame->16bit 44100 PCM 统一音频采样格式与采样率
	//创建swrcontext上下文件
	SwrContext *swrContext = swr_alloc();
	//音频格式  输入的采样设置参数
	AVSampleFormat inFormat = avCodecContext->sample_fmt;
	// 出入的采样格式
	AVSampleFormat  outFormat = AV_SAMPLE_FMT_S16;
	// 输入采样率
	int inSampleRate = avCodecContext->sample_rate;
	// 输出采样率
	int outSampleRate = 44100;
	// 输入声道布局
	uint64_t in_ch_layout = avCodecContext->channel_layout;
	//输出声道布局
	uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
	//给Swrcontext 分配空间，设置公共参数
	swr_alloc_set_opts(swrContext, out_ch_layout, outFormat, outSampleRate,
		in_ch_layout, inFormat, inSampleRate, 0, NULL
	);
	// 初始化
	swr_init(swrContext);
	// 获取声道数量
	int outChannelCount = av_get_channel_layout_nb_channels(out_ch_layout);

	int currentIndex = 0;
	printf("声道数量%d\n", outChannelCount);
	// 设置音频缓冲区间 16bit   44100  PCM数据, 双声道
	int out_size = outSampleRate / 8 /** av_get_bytes_per_sample(outFormat)*/;
	uint8_t *out_buffer = (uint8_t *)av_malloc(out_size);
	// 创建pcm的文件对象
	FILE *fp_pcm = fopen(out_file.c_str(), "wb");

	//开始读取源文件，进行解码
	while (av_read_frame(avFormatContext, packet) >= 0) {
		if (packet->stream_index == streamIndex) {
			avcodec_send_packet(avCodecContext, packet);
			//解码
			ret = avcodec_receive_frame(avCodecContext, inFrame);
			if (ret == 0) {
				//将每一帧数据转换成pcm
				ret = swr_convert(swrContext, &out_buffer, out_size,
					(const uint8_t **)inFrame->data, inFrame->nb_samples);
				if (ret < 0) {
					printf("swr_convert failed %d\n", ret);
					continue;
				}

				//获取实际的缓存大小
				int out_buffer_size = av_samples_get_buffer_size(NULL, outChannelCount, inFrame->nb_samples, outFormat, 1);

				int numBytes = av_get_bytes_per_sample(outFormat);
				int out_size = inFrame->nb_samples * numBytes * outChannelCount;

				// 写入文件
				fwrite(out_buffer, 1, out_buffer_size, fp_pcm);
			}
			printf("正在解码%d\n", currentIndex++);
		}
	}
	// 及时释放
	fclose(fp_pcm);
	av_frame_free(&inFrame);
	av_free(out_buffer);
	swr_free(&swrContext);
	avcodec_close(avCodecContext);
	avformat_close_input(&avFormatContext);

	return 0;
}

int test_audio_3() {
	printf("########################test_audio_3#################################\n");
	printf("###ffmpeg\n");
	printf("###decode mp4(aac), mkv, mp3 to pcm, save to file and play!\n");

	// 注册FFmpeg的所有组件
	av_register_all();

	std::string str_mp4 = "H:/movie/维京：王者之战.HD.720p.国俄双语中字.mkv";
	//std::string str_mp4 = "H:/movie/VID_20240906_204935.mp4";
	std::string in_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.wav";
	std::string out_file = "F:/Projs/WorkDir/Projects/Nico_Face_Reco/audio_data/sample-15s.pcm";

	// AVFormatContext 对象创建
	AVFormatContext *avFormatContext = avformat_alloc_context();
	// 打开音频文件
	int ret = avformat_open_input(&avFormatContext, in_file.c_str(), NULL, NULL);
	if (ret != 0) {
		printf("打开文件失败\n");
		return -1;
	}
	// 输出音频文件的信息
	av_dump_format(avFormatContext, 0, in_file.c_str(), 0);
	// 获取音频文件的流信息
	ret = avformat_find_stream_info(avFormatContext, NULL);
	if (ret < 0) {
		printf("获取流信息失败\n");
		return -1;
	}
	// 查找音频流在文件的所有流集合中的位置
	int streamIndex = 0;
	for (int i = 0; i < avFormatContext->nb_streams; ++i) {
		enum AVMediaType avMediaType = avFormatContext->streams[i]->codecpar->codec_type;
		if (avMediaType == AVMEDIA_TYPE_AUDIO) {  //这边和视频不一样，是AUDIO
			streamIndex = i;
		}
	}
	// 拿到对应音频流的参数
	AVCodecParameters *avCodecParameters = avFormatContext->streams[streamIndex]->codecpar;
	// 获取解码器的标识ID
	enum AVCodecID avCodecId = avCodecParameters->codec_id;
	// 通过获取的ID，获取对应的解码器
	AVCodec *avCodec = avcodec_find_decoder(avCodecId);
	// 创建一个解码器上下文对象
	AVCodecContext *avCodecContext = avcodec_alloc_context3(NULL);
	if (avCodecContext == NULL) {
		//创建解码器上下文失败
		printf("创建解码器上下文失败\n");
		return -1;
	}
	// 将新的API中的 codecpar 转成 AVCodecContext
	avcodec_parameters_to_context(avCodecContext, avCodecParameters);
	ret = avcodec_open2(avCodecContext, avCodec, NULL);
	if (ret < 0) {
		printf("打开解码器失败\n");
		return -1;
	}
	printf("decodec name: %s\n", avCodec->name);

	//压缩数据包
	AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	//解压缩后存放的数据帧的对象
	AVFrame *inFrame = av_frame_alloc();
	//frame->16bit 44100 PCM 统一音频采样格式与采样率
	//创建swrcontext上下文件
	SwrContext *swrContext = swr_alloc();
	//音频格式  输入的采样设置参数
	AVSampleFormat inFormat = avCodecContext->sample_fmt;
	// 出入的采样格式
	AVSampleFormat  outFormat = AV_SAMPLE_FMT_S16;
	// 输入采样率
	int inSampleRate = avCodecContext->sample_rate;
	// 输出采样率
	int outSampleRate = 44100;
	// 输入声道布局
	uint64_t in_ch_layout = avCodecContext->channel_layout;
	//输出声道布局
	uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
	//给Swrcontext 分配空间，设置公共参数
	swr_alloc_set_opts(swrContext, out_ch_layout, outFormat, outSampleRate,
		in_ch_layout, inFormat, inSampleRate, 0, NULL
	);
	// 初始化
	swr_init(swrContext);
	// 获取声道数量
	int outChannelCount = av_get_channel_layout_nb_channels(out_ch_layout);

	int currentIndex = 0;
	printf("声道数量%d\n", outChannelCount);
	// 设置音频缓冲区间 16bit   44100  PCM数据, 双声道
	int out_size = outSampleRate / 8 /** av_get_bytes_per_sample(outFormat) * outChannelCount*/;
	uint8_t *out_buffer = (uint8_t *)av_malloc(out_size);
	// 创建pcm的文件对象
	FILE *fp_pcm = fopen(out_file.c_str(), "wb");

	//SDL Init
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	// 获取可用扬声器列表，不需要可以忽略
	int deviceCount = SDL_GetNumAudioDevices(0);
	printf("SDL_GetNumAudioDevices %d\n", deviceCount);
	for (int i = 0; i < deviceCount; i++) {
		printf("Audio Device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
	}

	SDL_AudioSpec spec;
	spec.freq = outSampleRate;
	spec.format = AUDIO_S16SYS;
	spec.channels = outChannelCount;
	spec.silence = 0;
	spec.samples = 1024;
	spec.callback = nullptr;

	// 指定扬声器，不需要第一个参数可以填nullptr
	SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(1, 0), false, &spec, nullptr, false);
	printf("device_id %d\n", device_id);
	if (device_id == 0) {
		printf("can't open audio.\n");
		return -1;
	}

	SDL_PauseAudioDevice(device_id, 0);

	//开始读取源文件，进行解码
	while (av_read_frame(avFormatContext, packet) >= 0) {
		if (packet->stream_index == streamIndex) {
			avcodec_send_packet(avCodecContext, packet);
			//解码
			ret = avcodec_receive_frame(avCodecContext, inFrame);
			if (ret == 0) {
				//重采样
				ret = swr_convert(swrContext, &out_buffer, out_size,
					(const uint8_t **)inFrame->data, inFrame->nb_samples);
				if (ret < 0) {
					printf("swr_convert failed %d\n", ret);
					continue;
				}

				//获取实际的缓存大小
				int out_buffer_size = av_samples_get_buffer_size(NULL, outChannelCount, inFrame->nb_samples, outFormat, 1);

				int numBytes = av_get_bytes_per_sample(avCodecContext->sample_fmt);
				int outSize = inFrame->nb_samples * numBytes * avCodecContext->channels;

				// 写入文件
				fwrite(out_buffer, 1, out_buffer_size, fp_pcm);

				//SDL feed data
				int out_samples = ret;
				SDL_QueueAudio(device_id, out_buffer, out_samples * spec.channels * av_get_bytes_per_sample(outFormat));
			}
			printf("正在解码%d\n", currentIndex++);
		}
	}

	SDL_Delay(1000 * 30); // 等音频播完
	SDL_CloseAudioDevice(device_id);
	SDL_Quit();

	// 及时释放
	fclose(fp_pcm);
	av_frame_free(&inFrame);
	av_free(out_buffer);
	swr_free(&swrContext);
	avcodec_close(avCodecContext);
	avformat_close_input(&avFormatContext);

	return 0;
}