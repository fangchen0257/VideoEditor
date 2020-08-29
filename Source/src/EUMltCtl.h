#ifndef MLTCONTROLLER_H
#define MLTCONTROLLER_H

#include <Mlt.h>

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
    //static CMltCtl *Instance();
    CMltCtl();
    virtual ~CMltCtl ();

    void EUInit( void *szUser, MltCtlRecvFrameFun szFun, const char* profile = "" );
    int EUOpen (const char* url);
    int EUSetProducer(Mlt::Producer* producer);
    void EUClose ();
    
    void EUPlay ();
    void EUPause ();
    void EUStop();
    
    void ReleaseFrame(FrameData frame);

    void EUSeek (int pos );
    void EUSetVolume (double volume);
    
    int EUGetDuration();
    
    int Open();

private:
    Mlt::Profile* m_profile;
    Mlt::Producer* m_producer;
    Mlt::Consumer* m_consumer;
    
    void *m_userInfo;
    MltCtlRecvFrameFun m_recvFrameFun;

    void frameReceived(FrameData frame , mlt_position timePos );
    static void on_frame_show (mlt_consumer, void* userInfo, mlt_frame frame);
};

#endif // MLTCONTROLLER_H
