#ifndef MLTCONTROLLER_H
#define MLTCONTROLLER_H

#include "EUType.h"

struct FrameData
{
    void *frame;
    unsigned char *image;
    int width;
    int height;
    int pos;
};

class CMltCtl
{
    typedef void (*MltCtlRecvFrameFun)( void *szUserInfo, FrameData frame, mlt_position timePos );
public:
    CMltCtl();
    virtual ~CMltCtl ();

    void EUInit( void *szUser, MltCtlRecvFrameFun szFun);
    int EUOpen(const char* url);
    int EUSetProducer(shared_ptr<Mlt::Producer> producer);
    void EUClose();
    
    void EUPlay();
    void EUPause();
    void EUStop();
    
    void ReleaseFrame(FrameData frame);
    void refreshConsumer(bool scrubAudio = false);

    void EUSeek(int duration);
    void EUSeekToPos(int position);
    void EUSetVolume(double volume);
    
    int EUGetDuration();
    int EUGetLength();
    double EUGetFps();

    int Open();
    shared_ptr<Mlt::Producer> EuGetCurrentProducer();

private:
    Mlt::Profile m_profile;

    shared_ptr<Mlt::Producer> m_producer;
    shared_ptr<Mlt::Consumer> m_consumer;
    
    void *m_userInfo;
    MltCtlRecvFrameFun m_recvFrameFun;

    void frameReceived(FrameData frame , mlt_position timePos );
    static void on_frame_show (mlt_consumer, void* userInfo, mlt_frame frame);
};

#endif // MLTCONTROLLER_H
