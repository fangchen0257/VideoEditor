#include "EUTractor.h"


CEUTractor::CEUTractor()
    : m_profile(kDefaultMltProfile)
{
    m_bPlayerGPU = true;

    open();
}

CEUTractor::~CEUTractor ()
{
    close();
}

shared_ptr<CEUPlaylist> CEUTractor::playlist(TrackIndex index)
{
    shared_ptr<CEUPlaylist> playlist;

    do
    {
        if (!m_tractor)
        {
            break;
        }

        playlist.reset(new CEUPlaylist(m_tractor, index));

    } while (false);

    return playlist;
}

int CEUTractor::open()
{
    int error = 0;

    do
    {
        if (m_tractor)
            break;

        m_tractor.reset(new Mlt::Tractor(m_profile));

        addBackgroundTrack();
        addMainVideoTrack();
        addSubVideoTrack();
        addAudioTrack();

    } while (false);

    return error;
}

void CEUTractor::close()
{
}

int CEUTractor::getDuration()
{
    int nDuration = 0;

    do
    {
        if (!m_tractor)
        {
            break;
        }

        int nCount = m_tractor->count();
        for (int i = 0; i < nCount; ++i)
        {
            shared_ptr<Mlt::Producer> track(m_tractor->track(i));
            if (track && track->is_valid())
            {
                nDuration = MAX(nDuration, track->get_length());
            }
        }

    } while (false);

    return nDuration;
}

void CEUTractor::addBackgroundTrack()
{
    do
    {
        if (!m_tractor)
        {
            break;
        }

        Mlt::Playlist playlist(m_profile);
        playlist.set("id", kBackgroundTrackId);
        Mlt::Producer producer(m_profile, "color:0");
        producer.set("mlt_image_format", "rgb24a");
        producer.set("length", 1);
        producer.set("id", "black");
        // Allow mixing against frames produced by this producer.
        producer.set("set.test_audio", 0);
        playlist.append(producer);
        m_tractor->set_track(playlist, m_tractor->count());

    } while (false);
}

void CEUTractor::addAudioTrack()
{
    do
    {
        if (!m_tractor)
        {
            break;
        }

        Mlt::Playlist playlist(m_profile);
        playlist.set(kAudioTrackProperty, 1);
        playlist.set("hide", 1);
        playlist.blank(0);
        m_tractor->set_track(playlist, AudioTrack);

        Mlt::Transition mix(m_profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        m_tractor->plant_transition(mix, 0, AudioTrack);

    } while (false);
}

void CEUTractor::addMainVideoTrack()
{
    do
    {
        if (!m_tractor)
        {
            break;
        }

        Mlt::Playlist playlist(m_profile);
        playlist.set(kVideoTrackProperty, 1);
        playlist.blank(0);
        m_tractor->set_track(playlist, MainTrack);

        Mlt::Transition mix(m_profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        m_tractor->plant_transition(mix, 0, MainTrack);

    } while (false);
}

void CEUTractor::addSubVideoTrack()
{
    do
    {
        if (!m_tractor)
        {
            break;
        }

        Mlt::Playlist playlist(m_profile);
        playlist.set(kVideoTrackProperty, 1);
        playlist.blank(0);
        m_tractor->set_track(playlist, SubTrack_0);

        Mlt::Transition mix(m_profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        m_tractor->plant_transition(mix, 0, SubTrack_0);

        Mlt::Transition composite(m_profile, m_bPlayerGPU ? "movit.overlay" : "frei0r.cairoblend");
        m_tractor->plant_transition(composite, MainTrack, SubTrack_0);

    } while (false);
}

bool CEUTractor::isFiltered(Mlt::Producer* producer) const
{
    bool bFiltered = false;

    do
    {
        if (!producer)
        {
            producer = m_tractor.get();
        }

        if (!producer || !producer->is_valid())
        {
            break;
        }

        int nCount = producer->filter_count();
        for (int i = 0; i < nCount; i++)
        {
            shared_ptr<Mlt::Filter> pFilter(producer->filter(i));
            if (!pFilter || !pFilter->is_valid())
            {
                continue;
            }

            if (pFilter->get_int("_loader"))
            {
                bFiltered = true;
                break;
            }
        }

    } while (false);

    return bFiltered;
}



