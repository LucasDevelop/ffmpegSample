#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

extern "C" {
#include "libavutil/avutil.h"
}

#define print_av_err(ret,msg) \
        char errInfo[1024]; \
        av_strerror(ret,errInfo,sizeof(errInfo)); \
        cout<<msg<<",err:"<<errInfo<<endl;



#endif