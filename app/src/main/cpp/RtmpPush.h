
#ifndef SURFACERECODEDEMO_RTMPPUSH_H
#define SURFACERECODEDEMO_RTMPPUSH_H

#include <malloc.h>
#include <cstring>
#include "Queue.h"
#include "CallJava.h"

extern "C" {
#include "librtmp/rtmp.h"
};

class RtmpPush {

public:
    CallJava *callJava = NULL;
    RTMP *rtmp = NULL;
    char *url = NULL;
    Queue *queue = NULL;
    pthread_t push_thrad = NULL;
    bool isStartPush;
    long startTime;

public:
    RtmpPush(const char *url, CallJava *callJava);

    ~RtmpPush();

    void init();

    void pushSPSPPS(char *sps, int spsLen, char *pps, int ppsLen);

    void pushVideoData(char *data, int dataLen, bool keyFrame);

    void pushAudioData(char *data, int dataLen);

    void pushStop();

};


#endif //SURFACERECODEDEMO_RTMPPUSH_H
