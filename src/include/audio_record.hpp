#ifndef AUDIO_RECORD_HPP
#define AUDIO_RECORD_HPP

#include <string>
#include <thread>

extern "C"
{
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}

using namespace std;

class AudioRecord
{
private:
    void initDevice();

public:
    const char *input_device_name = "avfoundation";
    const char *format_device = ":0";
    int sample_rate = 0;
    int channels = 0;
    const char *sample_format = "s16le";

    char* pcm_file = "/Users/lucas/Downloads/audio.pcm";
    bool is_recording = false;
    thread *t=nullptr;

    AVFormatContext *formatContext = nullptr;

    AudioRecord();
    ~AudioRecord();
    void start_record();
    void stop_record();
};

#endif