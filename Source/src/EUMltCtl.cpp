#include "EUMltCtl.h"

Mlt::Profile CMltCtl::m_profile(kDefaultMltProfile);
string CMltCtl::m_profileName = kDefaultMltProfile;

CMltCtl::CMltCtl()
    : m_userInfo(nullptr)
    , m_recvFrameFun(nullptr)
{
}

CMltCtl::~CMltCtl ()
{
    EUClose();
    Mlt::Factory::close();
}

// MLT consumer-frame-show event handler
void CMltCtl::on_frame_show (mlt_consumer, void* userInfo, mlt_frame frame_ptr)
{
    CMltCtl *szMltCtl = static_cast<CMltCtl*>(userInfo);
    Mlt::Frame* frame = new Mlt::Frame(frame_ptr);

    mlt_image_format ft = mlt_image_yuv420p;
    int w = 0, h = 0;
    unsigned char *t =frame->get_image(ft, w, h);

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

void CMltCtl::frameReceived(FrameData frame, mlt_position timePos)
{
    do
    {
        CHECK_BREAK(!m_recvFrameFun);

        mlt_position curPlayPos = static_cast<mlt_position>(timePos / m_profile.fps());
        if (curPlayPos * m_profile.fps() < timePos)
        {
            ++curPlayPos;
        }

        m_recvFrameFun(m_userInfo, frame, curPlayPos);

    } while (false);
}

void CMltCtl::EUInit (void *szUser, MltCtlRecvFrameFun szFun)
{
    Mlt::Factory::init();
    m_userInfo = szUser;
    m_recvFrameFun = szFun;
}

int CMltCtl::EUOpen (const char* url)
{
    int error = 0;

    do
    {
        EUClose ();
        m_producer.reset(new Mlt::Producer(m_profile, url));
        error = Open();

    } while (false);
    
    return error;
}

int CMltCtl::EUSetProducer(shared_ptr<Mlt::Producer> producer)
{
    int error = 0;

    do
    {
        CHECK_BREAK(m_producer == producer);

        EUClose();
        m_producer = producer;
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

    m_consumer.reset();
    m_producer.reset();
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

void CMltCtl::EUSeek (int duration )
{
    do
    {
        CHECK_BREAK(!m_producer);

        int position = static_cast<int>(duration * m_profile.fps());
        EUSeekToPos(position);

    } while (false);
}

void CMltCtl::EUSeekToPos ( int position )
{
    do
    {
        CHECK_BREAK(!m_producer);

        double fSpeed = m_producer->get_speed();
        m_producer->set_speed( 0 );
        m_producer->seek( position );
        m_producer->set_speed( fSpeed );

        CHECK_BREAK( !m_consumer || !m_consumer->is_valid() );

        if( m_consumer->is_stopped() )
        {
            m_consumer->start();
        }
        else
        {
            m_consumer->purge();
        }

    } while (false);
}

int CMltCtl::EUGetDuration()
{
    int duration = 0;

    do
    {
        CHECK_BREAK(!m_producer);

        duration = static_cast<int>(m_producer->get_length() / m_profile.fps());

    } while (false);

    return duration;
}

int CMltCtl::Open()
{
    int error = 0;

    do
    {
        CHECK_BREAK(!m_producer);

        if (!m_producer->is_valid ())
        {
            error = 1;
            m_producer.reset();
            break;
        }

#ifdef _MSC_VER
        m_consumer.reset(Mlt::Consumer (m_profile, "sdl"));
#else
        mlt_service_type type = m_producer->type();
        if (producer_type == type)
        {
            m_profile.from_producer(*m_producer);

            int w = m_producer->get_int("width");
            int h = m_producer->get_int("height");

            float ratio = 1.0f *w/h;
            h = (h >> 4) << 4 ;
            w = (int(ratio * h)>>4)  <<4 ;

            m_profile.set_width(w);
            m_profile.set_height(h);
        }
        else
        {
            Mlt::Profile tmp(m_profileName.c_str());
            m_profile.set_colorspace(tmp.colorspace());
            m_profile.set_frame_rate(tmp.frame_rate_num(), tmp.frame_rate_den());
            m_profile.set_width(tmp.width());
            m_profile.set_height(tmp.height());
            m_profile.set_progressive(tmp.progressive());
            m_profile.set_sample_aspect(tmp.sample_aspect_num(), tmp.sample_aspect_den());
            m_profile.set_display_aspect(tmp.display_aspect_num(), tmp.display_aspect_den());
        }

        m_consumer.reset(new Mlt::Consumer(m_profile, "sdl_audio"));
#endif
        if (!m_consumer->is_valid ())
        {
            error = 2;
            m_consumer.reset();
            m_producer.reset();
            break;
        }

        //m_consumer->set("rescale", "none");
        m_consumer->connect (*m_producer);
        m_consumer->listen ("consumer-frame-show", this, reinterpret_cast<mlt_listener>(on_frame_show));
        m_consumer->start ();

    } while (false);

    return error;
}
