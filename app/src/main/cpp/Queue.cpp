#include "Queue.h"

Queue::Queue() {
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

Queue::~Queue() {
    clean();
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int Queue::putRtmpPacket(RTMPPacket *packet) {
    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(packet);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

RTMPPacket *Queue::getRtmpPacket() {
    pthread_mutex_lock(&mutexPacket);

    RTMPPacket *rtmpPacket = NULL;
    if (!queuePacket.empty()) {
        rtmpPacket = queuePacket.front();
        queuePacket.pop();
    } else {
        pthread_cond_wait(&condPacket, &mutexPacket);
    }
    pthread_mutex_unlock(&mutexPacket);
    return rtmpPacket;
}

void Queue::clean() {
    notifyQueue();
    pthread_mutex_lock(&mutexPacket);
    while (true) {
        if (queuePacket.empty()) {
            break;
        }
        RTMPPacket *rtmpPacket = queuePacket.front();
        queuePacket.pop();
        RTMPPacket_Free(rtmpPacket);
        rtmpPacket = NULL;
    }

    pthread_mutex_unlock(&mutexPacket);
}

void Queue::notifyQueue() {
    pthread_mutex_lock(&mutexPacket);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);
}
