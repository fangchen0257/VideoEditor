#include "EUMltCtl.h"
#include "EUProducer.h"


CMltCtl::CMltCtl()
    : m_userInfo(nullptr)
    , m_recvFrameFun(nullptr)
{
}

CMltCtl::~CMltCtl ()
{
    EUClose();
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
    data.pos = mlt_frame_get_position( frame_ptr );

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
        CHECK_BREAK(!m_producer);

        mlt_position curPlayPos = static_cast<mlt_position>(timePos / m_producer->get_fps());
        if (curPlayPos * m_producer->get_fps() < timePos)
        {
            ++curPlayPos;
        }

        m_recvFrameFun(m_userInfo, frame, curPlayPos);

    } while (false);
}

void CMltCtl::EUInit (void *szUser, MltCtlRecvFrameFun szFun)
{
    m_userInfo = szUser;
    m_recvFrameFun = szFun;
}

int CMltCtl::EUOpen (const char* url)
{
    int error = 0;

    do
    {
        EUClose ();
        profileFromProducer(m_profile, url);

        //手动设置帧率，避免某些视频卡顿
        double fps = m_profile.fps() * 100;
        int f = static_cast<int>(fps);
        m_profile.set_frame_rate(f, 100);

        m_producer = CEUProducer(m_profile, url).producer();
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
        m_producer->set_speed (1.0);
    }
    
    if (m_consumer)
    {

        m_consumer->start();
        refreshConsumer();
    }
}

void CMltCtl::EUPause ()
{
    if (m_producer)
    {
       m_producer->set_speed(0);
       m_producer->seek(m_consumer->position());
       m_consumer->purge();
       m_consumer->start();
       refreshConsumer();
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

    refreshConsumer();
}

void CMltCtl::ReleaseFrame(FrameData frame)
{
    Mlt::Frame* f = static_cast<Mlt::Frame*>(frame.frame);
    delete f;
}

void CMltCtl::refreshConsumer(bool scrubAudio)
{
    if (m_consumer)
    {
        m_consumer->set("scrub_audio", scrubAudio);
        m_consumer->set("refresh", 1);
    }
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

        int position = static_cast<int>(duration * m_producer->get_fps());
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
        duration = static_cast<int>(m_producer->get_playtime() - 1 / m_producer->get_fps());

    } while (false);

    return duration;
}

int CMltCtl::EUGetLength()
{
    int length = 0;

    do
    {
        CHECK_BREAK(!m_producer);
        length = m_producer->get_playtime();

    } while (false);

    return length;
}

double CMltCtl::EUGetFps()
{
    double fps = 0;

    do
    {
        CHECK_BREAK(!m_producer);
        fps = m_producer->get_fps();
    } while (false);

    return fps;
}

int CMltCtl::EUGetPosition()
{
    int position = 0;

    do
    {
        CHECK_BREAK(!m_producer);
        position = m_producer->position();

    } while (false);

    return position;
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
        m_consumer.reset(Mlt::Consumer(m_profile, "sdl"));
#else
        mlt_service_type type = m_producer->type();
        if (producer_type == type)
        {
            m_consumer.reset(new Mlt::Consumer(m_profile, "sdl2_audio"));

            if (!m_profile.progressive())
            {
                m_consumer->set("progressive", 0);
            }
            m_consumer->set("rescale", "bilinear");
            m_consumer->set("deinterlace_method", "onefield");
            m_consumer->set("buffer", qMax(25, qRound(m_profile.fps())));
            m_consumer->set("prefill", qMax(1, qRound(m_profile.fps() / 25.0)));
            m_consumer->set("drop_max", qRound(m_profile.fps() / 4.0));
        }
        else
        {
            m_consumer.reset(new Mlt::Consumer(profile, "sdl2_audio"));

            m_consumer->set("terminate_on_pause", 0);
            if (!profile.progressive())
            {
                m_consumer->set("0.progressive", 0);
            }
            m_consumer->set("0.rescale", "bilinear");
            m_consumer->set("0.deinterlace_method", "onefield");
            m_consumer->set("0.buffer", qMax(25, qRound(profile.fps())));
            m_consumer->set("0.prefill", qMax(1, qRound(profile.fps() / 25.0)));
            m_consumer->set("0.drop_max", qRound(profile.fps() / 4.0));
        }
#endif
        if (!m_consumer->is_valid ())
        {
            error = 2;
            m_consumer.reset();
            m_producer.reset();
            break;
        }

        m_consumer->connect (*m_producer);
        m_consumer->listen ("consumer-frame-show", this, reinterpret_cast<mlt_listener>(on_frame_show));
        m_consumer->start ();

    } while (false);

    return error;
}

shared_ptr<Mlt::Producer> CMltCtl::EuGetCurrentProducer()
{
    return m_producer;
}
