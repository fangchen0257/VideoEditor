#include "EUMltCtl.h"
#include <QDebug>

CMltCtl::CMltCtl()
: m_profile (nullptr)
, m_producer (nullptr)
, m_consumer (nullptr)
, m_userInfo(nullptr)
, m_recvFrameFun(nullptr)
{
}

/*

CMltCtl *CMltCtl::Instance()
{
    static CMltCtl *anInstance = NULL;
    
    if( NULL == anInstance )
    {
        anInstance = new CMltCtl();
    }
    
    return anInstance;
}

*/

CMltCtl::~CMltCtl ()
{
    EUClose();
    Mlt::Factory::close();
}

// MLT consumer-frame-show event handler
void CMltCtl::on_frame_show (mlt_consumer, void* userInfo, mlt_frame frame_ptr)
{
    CMltCtl *szMltCtl = static_cast<CMltCtl*> (userInfo);
    Mlt::Frame* frame = new Mlt::Frame (frame_ptr);

    mlt_image_format ft = mlt_image_yuv420p;
    int w,h;
    unsigned char *t =frame->get_image(ft,w,h);

    FrameData data;
    data.frame = frame;
    data.image = t;
    data.width = w;
    data.height = h;

    if( szMltCtl )
    {
        szMltCtl->frameReceived( data, mlt_frame_get_position( frame_ptr ) );
    }
}

void CMltCtl::frameReceived(FrameData frame, mlt_position timePos )
{
    if( m_recvFrameFun )
    {
        mlt_position curPlayPos = 0;
        
        if( m_profile )
        {
            curPlayPos = static_cast<mlt_position>(timePos / m_profile->fps());
            if (curPlayPos *  m_profile->fps() < timePos)
                ++curPlayPos;
        }
        
        m_recvFrameFun( m_userInfo, frame, curPlayPos );
    }
}

void CMltCtl::EUInit ( void *szUser, MltCtlRecvFrameFun szFun, const char* profile )
{
    Mlt::Factory::init();
    m_profile = new Mlt::Profile( profile );
//    m_profile->set_explicit(false);
    m_userInfo = szUser;
    m_recvFrameFun = szFun;
}

int CMltCtl::EUOpen (const char* url)
{
    int error = 0;

    EUClose ();
    
    m_producer = new Mlt::Producer (*m_profile, url);
    
    error = Open();
    
    return error;
}

int CMltCtl::EUSetProducer(Mlt::Producer* producer)
{
    int error = 0;

    do
    {
        EUClose();

        if (!producer)
        {
            error = 1;
            break;
        }

        m_producer = new Mlt::Producer(*producer);

        error = Open();

    } while (false);

    return error;
}

void CMltCtl::EUClose ()
{
    if( m_consumer && !m_consumer->is_stopped() )
    {
        m_consumer->stop();
    }

    if (m_consumer)
    {
        delete m_consumer;
        m_consumer = nullptr;
    }
    
    if (m_producer)
    {
        delete m_producer;
        m_producer = nullptr;
    }
}

void CMltCtl::EUPlay ()
{
    if (m_producer)
    {
        m_producer->set_speed (1);
    }
    
    if (m_consumer)
    {
        m_consumer->start();
        m_consumer->set("refresh", 1);
    }
}

void CMltCtl::EUPause ()
{
    if (m_producer)
    {
       m_producer->set_speed(0);
       m_producer->seek(m_consumer->position() + 1);
       m_consumer->purge();
       m_consumer->start();
    }
}

void CMltCtl::EUStop()
{
    if (m_consumer && !m_consumer->is_stopped())
    {
        m_consumer->stop();
    }
    if (m_producer)
    {
        m_producer->seek(0);
    }
}

void CMltCtl::ReleaseFrame(FrameData frame)
{
    Mlt::Frame* f = static_cast<Mlt::Frame*>(frame.frame);
    delete f;
}

void CMltCtl::EUSetVolume (double volume)
{
    if (m_consumer)
    {
        m_consumer->set ("volume", volume);
    }
}

void CMltCtl::EUSeek (int pos )
{
    if( m_producer )
    {
        double fSpeed = m_producer->get_speed();
        
        m_producer->set_speed( 0 );
        m_producer->seek( static_cast<int>(pos * m_profile->fps()) );
        m_producer->set_speed( fSpeed );
        
        if( m_consumer && m_consumer->is_valid() )
        {
            if( m_consumer->is_stopped() )
            {
                m_consumer->start();
            }
            else
            {
                m_consumer->purge();
            }
        }
    }
}

int CMltCtl::EUGetDuration()
{
    if( m_producer )
    {
        return static_cast<int>(m_producer->get_length() / m_profile->fps());
    }
    
    return 0;
}

int CMltCtl::Open()
{
    int error = 0;

    do
    {
        if (!m_producer)
        {
            break;
        }

        if (!m_producer->is_valid ())
        {
            error = 1;
            delete m_producer;
            m_producer = nullptr;
            break;
        }

#ifdef _MSC_VER
        m_consumer = new Mlt::Consumer (*m_profile, "sdl");
#else
        int w = m_producer->get_int("width");
        int h = m_producer->get_int("height");

        float ratio = 1.0f *w/h;
        h = (h >> 4) << 4 ;
        w = (int(ratio * h)>>4)  <<4 ;

        m_profile->set_width(w);
        m_profile->set_height(h);

        m_consumer = new Mlt::Consumer (*m_profile, "sdl_audio");
#endif
        if (!m_consumer->is_valid ())
        {
            error = 2;
            delete m_consumer;
            m_consumer = nullptr;
            delete m_producer;
            m_producer = nullptr;
            break;
        }

        //m_consumer->set("rescale", "none");
        m_consumer->connect (*m_producer);
        m_consumer->listen ("consumer-frame-show", this, reinterpret_cast<mlt_listener>(on_frame_show));
        m_consumer->start ();

    } while (false);

    return error;
}
