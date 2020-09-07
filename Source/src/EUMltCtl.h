#ifndef MLTCONTROLLER_H
#define MLTCONTROLLER_H

#include "EUType.h"

struct FrameData
{
    void *frame;
    unsigned char *image;
    int width;
    int height;
};

class CMltCtl
{
    typedef void (*MltCtlRecvFrameFun)( void *szUserInfo, FrameData frame, mlt_position timePos );
public:
    CMltCtl();
    virtual ~CMltCtl ();

    static Mlt::Profile& profile() { return m_profile; }
    static void setProfile(const string &profile_name) { m_profileName = profile_name; }

    void EUInit( void *szUser, MltCtlRecvFrameFun szFun);
    int EUOpen(const char* url);
    int EUSetProducer(shared_ptr<Mlt::Producer> producer);
    void EUClose();
    
    void EUPlay();
    void EUPause();
    void EUStop();
    
    void ReleaseFrame(FrameData frame);

    void EUSeek(int duration);
    void EUSeekToPos(int position);
    void EUSetVolume(double volume);
    
    int EUGetDuration();
    
    int Open();

private:
    static Mlt::Profile m_profile;
    static string m_profileName;

    shared_ptr<Mlt::Producer> m_producer;
    shared_ptr<Mlt::Consumer> m_consumer;
    
    void *m_userInfo;
    MltCtlRecvFrameFun m_recvFrameFun;

    void frameReceived(FrameData frame , mlt_position timePos );
    static void on_frame_show (mlt_consumer, void* userInfo, mlt_frame frame);
};

#endif // MLTCONTROLLER_H
