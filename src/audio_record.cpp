#include "audio_record.hpp"
#include "log.hpp"
#include <fstream>
#include <unistd.h>

using namespace std;

void AudioRecord::initDevice()
{
    avdevice_register_all();
    const AVInputFormat *inputFormat = av_find_input_format(input_device_name);
    if (inputFormat == nullptr)
    {
        cout << "没有找到设备:" << input_device_name << endl;
        return;
    }
    int ret = avformat_open_input(&formatContext, format_device, inputFormat, nullptr);
    if (ret != 0)
    {
        print_av_err(ret, "打开音频设备失败！") return;
    }
    // 读取设备 采样率/声道数
    AVCodecParameters *params = formatContext->streams[0]->codecpar;
    sample_rate = params->sample_rate;
    channels = params->channels;
    cout << "sample rate:" << params->sample_rate << endl;
    cout << "sample format:" <<params->format<< endl;
    cout << "channels:" << params->channels << endl;

}

void run(AudioRecord *record)
{
    AVPacket packet;
    ofstream pcmFile(record->pcm_file, ios::out);
    while (record->is_recording)
    {
        int ret = av_read_frame(record->formatContext, &packet);
        if(ret == 0){
            cout<<"写入数据:"<<packet.size<<endl;
            pcmFile.write((const char*)packet.data,packet.size);
        }
        else if (ret == AVERROR(EAGAIN))
        {
            print_av_err(ret, "音频数据未准备好，")
            //usleep(1000*1000);
            continue;
        }
        else
        {
            print_av_err(ret, "读取音频数据发生错误，");
            break;
        }
         pcmFile.flush();
    }
   
    pcmFile.close();
}

void AudioRecord::start_record()
{
    if (is_recording)
        return;
    is_recording = true;
    t = new thread(run, this);
}

void AudioRecord::stop_record()
{
    if (!is_recording)
        return;
    is_recording = false;
    t = nullptr;
    avformat_close_input(&formatContext);
}

AudioRecord::AudioRecord()
{
    initDevice();
    cout << "create AudioRecord" << endl;
}

AudioRecord::~AudioRecord()
{
    stop_record();
    cout << "destroy AudioRecord" << endl;
}
