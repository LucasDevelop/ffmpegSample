#ifndef AUDIO_RESAMPLE_HPP
#define AUDIO_RESAMPLE_HPP
#include "log.hpp"

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"
}

class AudioResample
{
private:
 //创建重采样上下文
    SwrContext *ctx =nullptr;

    //输入缓冲区
    uint8_t **inBuf=nullptr;
    //输入缓冲区大小
    int inBufSize = 0;
    //输入声道数布局中的声道数
    int inNbChannles=0;
    //输入每个样本大小  声道数*采样格式
    int inBytesPerSample=0;
    //输入缓冲区样本数量
    int inSamples = 0;

    //输出缓冲区
    uint8_t **outBuf = nullptr;
    //输出缓冲区大小
    int outBufSize = 0;
    //输出声道数布局中的声道数
    int outNbChannels=0;
    //输出每个样本大小
    int outBytesPerSample=0;
    //输出缓冲区样本数 out_sample_rate * inSamples/in_sample_rate
    int outSamples=0;
public:
    //in params
    int in_sample_rate =48000;
    AVSampleFormat in_sample_fmt = AV_SAMPLE_FMT_S16;
    int in_channels = AV_CH_LAYOUT_STEREO;
    char *in_pcm_file_path = "/Users/lucas/Downloads/audio.pcm";
    
   

    //out params
    int out_sample_rate = 44100;
    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int out_channels = AV_CH_LAYOUT_MONO;
    char *out_pcm_file_path = "/Users/lucas/Downloads/audio_resample.pcm";
    
    AudioResample(/* args */);
    ~AudioResample();

    /**
     * @brief 初始化编码器
     * 
     * @return int 0：success else failed。
     */
    int init_codec();

    /**
     * @brief PCM重采样
     * @return int 重采样大小
     */
    int convert();
};





#endif