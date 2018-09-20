#include "RtmpPush.h"


RtmpPush::RtmpPush(const char *url, CallJava *callJava) {
    this->url = static_cast<char *>(malloc(512));
    strcpy(this->url, url);
    this->queue = new Queue();

    this->callJava = callJava;
}


RtmpPush::~RtmpPush() {
    if (this->queue) {
        delete (queue);
        queue = NULL;
    }

    if (this->url) {
        free(this->url);
        this->url = NULL;
    }
}


void *callbackPush(void *data) {
    RtmpPush *rtmpPush = static_cast<RtmpPush *>(data);
    rtmpPush->isStartPush = false;
    rtmpPush->callJava->conn(THREAD_CHILD);

    rtmpPush->rtmp = RTMP_Alloc();
    RTMP_Init(rtmpPush->rtmp);
    rtmpPush->rtmp->Link.timeout = 10;
    rtmpPush->rtmp->Link.lFlags |= RTMP_LF_LIVE;
    RTMP_SetupURL(rtmpPush->rtmp, rtmpPush->url);
    RTMP_EnableWrite(rtmpPush->rtmp);
    if (!RTMP_Connect(rtmpPush->rtmp, NULL)) {
        LOGE("RTMP can not connect  url: %s", rtmpPush->url);
        rtmpPush->callJava->connf(THREAD_CHILD, "RTMP can not connect  url ");
        goto end;
    }

    LOGE("RTMP connect success, url: %s", rtmpPush->url);
    if (!RTMP_ConnectStream(rtmpPush->rtmp, 0)) {
        LOGE("RTMP can not connect stream url: %s", rtmpPush->url);
        rtmpPush->callJava->connf(THREAD_CHILD, "RTMP can not connect stream url");
        goto end;
    }
    rtmpPush->callJava->conns(THREAD_CHILD);
    LOGE("RTMP connect stream success, url: %s", rtmpPush->url);

    rtmpPush->isStartPush = true;
    rtmpPush->startTime = RTMP_GetTime();

    while (rtmpPush->isStartPush) {
        RTMPPacket *packet = rtmpPush->queue->getRtmpPacket();
        if (packet) {
            // queue 缓存队列大小
            int result = RTMP_SendPacket(rtmpPush->rtmp, packet, 1);
            LOGD("RTMP_SendPacket result:%d", result);
            RTMPPacket_Free(packet);
            free(packet);
            packet = NULL;
        }
    }


    end:
    RTMP_Close(rtmpPush->rtmp);
    RTMP_Free(rtmpPush->rtmp);
    rtmpPush->rtmp = NULL;
    pthread_exit(&rtmpPush->push_thrad);
}


void RtmpPush::init() {
    pthread_create(&push_thrad, NULL, callbackPush, this);
}

void RtmpPush::pushSPSPPS(char *sps, int spsLen, char *pps, int ppsLen) {
    if (!this->queue) return;
    int bodySize = spsLen + ppsLen + 16;
    RTMPPacket *rtmpPacket = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
    RTMPPacket_Alloc(rtmpPacket, bodySize);
    RTMPPacket_Reset(rtmpPacket);

    char *body = rtmpPacket->m_body;

    int i = 0;
    //frame type(4bit)和CodecId(4bit)合成一个字节(byte)
    //frame type 关键帧1  非关键帧2
    //CodecId  7表示avc
    body[i++] = 0x17;

    //fixed 4byte
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    //configurationVersion： 版本 1byte
    body[i++] = 0x01;

    //AVCProfileIndication：Profile 1byte  sps[1]
    body[i++] = sps[1];

    //compatibility：  兼容性 1byte  sps[2]
    body[i++] = sps[2];

    //AVCLevelIndication： ProfileLevel 1byte  sps[3]
    body[i++] = sps[3];

    //lengthSizeMinusOne： 包长数据所使用的字节数  1byte
    body[i++] = 0xff;

    //sps个数 1byte
    body[i++] = 0xe1;
    //sps长度 2byte
    body[i++] = (spsLen >> 8) & 0xff;
    body[i++] = spsLen & 0xff;

    //sps data 内容
    memcpy(&body[i], sps, spsLen);
    i += spsLen;
    //pps个数 1byte
    body[i++] = 0x01;
    //pps长度 2byte
    body[i++] = (ppsLen >> 8) & 0xff;
    body[i++] = ppsLen & 0xff;
    //pps data 内容
    memcpy(&body[i], pps, ppsLen);


    rtmpPacket->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    rtmpPacket->m_nBodySize = bodySize;
    rtmpPacket->m_nTimeStamp = 0;
    rtmpPacket->m_hasAbsTimestamp = 0;
    rtmpPacket->m_nChannel = 0x04;//音频或者视频
    rtmpPacket->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    rtmpPacket->m_nInfoField2 = this->rtmp->m_stream_id;

    queue->putRtmpPacket(rtmpPacket);

}

void RtmpPush::pushVideoData(char *data, int dataLen, bool keyFrame) {
    if (!this->queue) return;
    int bodySize = dataLen + 9;
    RTMPPacket *rtmpPacket = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
    RTMPPacket_Alloc(rtmpPacket, bodySize);
    RTMPPacket_Reset(rtmpPacket);

    char *body = rtmpPacket->m_body;

    int i = 0;
    //frame type(4bit)和CodecId(4bit)合成一个字节(byte)
    //frame type 关键帧1  非关键帧2
    //CodecId  7表示avc
    if (keyFrame) {
        body[i++] = 0x17;
    } else {
        body[i++] = 0x27;
    }

    //fixed 4byte   0x01表示NALU单元
    body[i++] = 0x01;
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    //dataLen  4byte
    body[i++] = (dataLen >> 24) & 0xff;
    body[i++] = (dataLen >> 16) & 0xff;
    body[i++] = (dataLen >> 8) & 0xff;
    body[i++] = dataLen & 0xff;

    //data
    memcpy(&body[i], data, dataLen);

    rtmpPacket->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    rtmpPacket->m_nBodySize = bodySize;
    //持续播放时间
    rtmpPacket->m_nTimeStamp = RTMP_GetTime() - this->startTime;
    //进入直播播放开始时间
    rtmpPacket->m_hasAbsTimestamp = 0;
    rtmpPacket->m_nChannel = 0x04;//音频或者视频
    rtmpPacket->m_headerType = RTMP_PACKET_SIZE_LARGE;
    rtmpPacket->m_nInfoField2 = this->rtmp->m_stream_id;

    queue->putRtmpPacket(rtmpPacket);


}

void RtmpPush::pushAudioData(char *data, int dataLen) {
    if (!this->queue) return;
    int bodySize = dataLen + 2;
    RTMPPacket *rtmpPacket = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
    RTMPPacket_Alloc(rtmpPacket, bodySize);
    RTMPPacket_Reset(rtmpPacket);

    char *body = rtmpPacket->m_body;
    //前四位表示音频数据格式  10（十进制）表示AAC，16进制就是A
    //第5-6位的数值表示采样率，0 = 5.5 kHz，1 = 11 kHz，2 = 22 kHz，3(11) = 44 kHz。
    //第7位表示采样精度，0 = 8bits，1 = 16bits。
    //第8位表示音频类型，0 = mono，1 = stereo
    //这里是44100 立体声 16bit 二进制就是1111   16进制就是F
    body[0] = 0xAF;

    //0x00 aac头信息     0x01 aac 原始数据
    //这里都用0x01都可以
    body[1] = 0x01;

    //data
    memcpy(&body[2], data, dataLen);

    rtmpPacket->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    rtmpPacket->m_nBodySize = bodySize;
    //持续播放时间
    rtmpPacket->m_nTimeStamp = RTMP_GetTime() - this->startTime;
    //进入直播播放开始时间
    rtmpPacket->m_hasAbsTimestamp = 0;
    rtmpPacket->m_nChannel = 0x04;//音频或者视频
    rtmpPacket->m_headerType = RTMP_PACKET_SIZE_LARGE;
    rtmpPacket->m_nInfoField2 = this->rtmp->m_stream_id;

    queue->putRtmpPacket(rtmpPacket);
}

void RtmpPush::pushStop() {
    this->isStartPush = false;
    queue->notifyQueue();
    pthread_join(push_thrad,NULL);
}

