#include <iostream>
#include <fstream>
#include <type_traits>
#include "audio_resample.hpp"
using namespace std;
/**
 * 重采样步骤
 * 1.创建重采样上下文 swr_alloc_set_opts
 * 2.初始化重采样上下文 swr_init
 * 3.创建输入和输出缓冲区 av_samples_alloc_array_and_samples
 * 4.重采样 swr_convert
 * 5.释放重采样上下文 swr_free
 */

AudioResample::AudioResample(/* args */)
{
}

/**
 * @brief 初始化编码器
 *
 * @return int 0：success else failed。
 */
int AudioResample::init_codec()
{
    cout << "init_codec start" << endl;
    // 输入声道数布局中的声道数
    inNbChannles = av_get_channel_layout_nb_channels(in_channels);
    // 输入每个样本大小  声道数*采样格式
    inBytesPerSample = inNbChannles * av_get_bytes_per_sample(in_sample_fmt);
    inSamples = 1024;

    cout<<"inNbChannles:"<<inNbChannles<<",inBytesPerSample:"<<inBytesPerSample<<endl;

    // 输出声道数布局中的声道数
    outNbChannels = av_get_channel_layout_nb_channels(out_channels);
    // 输出每个样本大小
    outBytesPerSample = outNbChannels * av_get_bytes_per_sample(out_sample_fmt);
    // 输出缓冲区样本数 out_sample_rate * inSamples/in_sample_rate
    outSamples = av_rescale_rnd(out_sample_rate, inSamples, in_sample_rate, AV_ROUND_UP);

    // 创建重采样上下文
    ctx = swr_alloc_set_opts(nullptr,
                             out_channels, out_sample_fmt, out_sample_rate,
                             in_channels, in_sample_fmt, in_sample_rate,
                             0, nullptr);
    int ret = 0;
    if (!ctx)
    {
        cout << "创建重采样上下文失败!" << endl;
        goto end;
    }
    // 初始化上下文
    ret = swr_init(ctx);
    if (ret < 0)
    {
        print_av_err(ret, "初始化重采样上下文失败")
        goto end;
    }

    // 创建输入缓冲区
    ret = av_samples_alloc_array_and_samples(&inBuf, &inBufSize, inNbChannles, inSamples, in_sample_fmt, 0);
    // cout<<"inBufSize:"<<extent<decltype(inBuf[0]), 0>::value<<endl;
    inBuf[0][0]=1;
    cout<<"ret:"<<ret<<endl;
    cout<<"2----["<<inBuf[0]<<"]-----"<<endl;
    if (ret < 0)
    {
        print_av_err(ret, "创建输入缓冲区失败") goto end;
    }

    // 创建输出缓冲区
    ret = av_samples_alloc_array_and_samples(&outBuf, &outBufSize, outNbChannels, outSamples, out_sample_fmt, 0);
    if (ret < 0)
    {
        print_av_err(ret, "创建输出缓冲区失败") goto end;
    }

end:
    if (inBuf)
        av_freep(&inBuf[0]);
    av_freep(&inBuf);
    if (outBuf)
        av_freep(&outBuf[0]);
    av_freep(&outBuf);
    swr_free(&ctx);
    cout << "init_codec end" << endl;
    return 0;
}

/**
 * @brief PCM重采样
 *
 * @return int 重采样大小
 */
int AudioResample::convert()
{
    cout << "start convert" << endl;
    // 打开文件
    ifstream in_pcm_file(in_pcm_file_path);
    if (!in_pcm_file.is_open())
    {
        cout << "无法打开文件：" << in_pcm_file_path << endl;
        return -1;
    }

    ofstream out_pcm_file(out_pcm_file_path);
    if (!out_pcm_file.is_open())
    {
        cout << "无法打开文件：" << out_pcm_file_path << endl;
        return -1;
    }
    int ret = 0;
    int len = 0;
    cout << "开始重采样" << endl;
    // 开始重采样
    for (;;)
    {
        cout << "inBufSize:" << inBufSize << endl;
        in_pcm_file.read((char *)inBuf[0], inBufSize);
        cout << "bbb" << endl;
        // 实际读取大小
        len = in_pcm_file.gcount();
        cout << "read size:" << len << endl;
        if (len < 0)
        {
            cout << "pcm文件读取完毕" << endl;
            break;
        }
        // 重新计算样本数,因为读取最后一组数据时读取到的数据大小不一定是inBufSize
        inSamples = len / inBytesPerSample;
        // 如果重采样成功，返回重采样后的样本数
        ret = swr_convert(ctx, outBuf, outSamples, (const uint8_t **)inBuf, inSamples);
        if (ret < 0)
        {
            print_av_err(ret, "转换失败") goto end;
        }
        // 将重采样后的数据写入文件
        out_pcm_file.write((const char *)outBuf[0], ret * outBytesPerSample);
    }

    // 处理残留的数据
    while ((ret = swr_convert(ctx, outBuf, outSamples, nullptr, 0)) > 0)
    {
        // TODO
        out_pcm_file.write((const char *)outBuf[0], ret * outBytesPerSample);
        cout << "残留样本数:" << ret << endl;
    }
    out_pcm_file.flush();
end:
    in_pcm_file.close();
    out_pcm_file.close();
    if (inBuf)
        av_freep(&inBuf[0]);
    av_freep(&inBuf);
    if (outBuf)
        av_freep(&outBuf[0]);
    av_freep(&outBuf);
    swr_free(&ctx);
    cout << "end convert" << endl;
    return 0;
}

AudioResample::~AudioResample()
{
}