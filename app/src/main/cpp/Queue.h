#include <queue>
#include <pthread.h>
#include "AndroidLog.h"

extern "C" {
    #include "librtmp/Rtmp.h"
}

class Queue {
public:
    std::queue<RTMPPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;

public:
    Queue();

    ~Queue();

    int putRtmpPacket(RTMPPacket *packet);

    RTMPPacket *getRtmpPacket();

    void clean();

    void notifyQueue();


};


