#include <iostream>
#include "audio_record.hpp"
#include <unistd.h>
#include "audio_resample.hpp"

#include <fstream>

extern "C"
{
#include "libavdevice/avdevice.h"
}
using namespace std;


void test(){
    char * filePath = "/Users/lucas/Downloads/audio.pcm";
    ifstream file(filePath);
    if (file.is_open())
    {
        uint8_t **buf = (uint8_t**)malloc(sizeof(uint8_t *)*2);
        buf[0] = (uint8_t*)malloc(sizeof(uint8_t)*1024);
        // buf[0][0] = 68;
        // cout<<"res:"<<sizeof(buf)<<endl;
        file.read((char *)buf[0],1024);

        // uint8_t *buf =(uint8_t*)malloc(1020*sizeof(uint8_t));
        // file.read((char *)buf,1024);

        // char buf[1024] ={0};
        // file.read(buf,1024);
        cout<<"read size:"<<file.gcount()<<endl;
    }
}

int main(void)
{
    // test();
    // AudioRecord record;
    // record.start_record();
    // sleep(10);
    // record.stop_record();

    //重采样
    AudioResample resample;
    resample.init_codec();
    resample.convert();
    cout<<"resample complete"<<endl;
}
