#include "EUTractor.h"


CEUTractor::CEUTractor()
{
}

CEUTractor::~CEUTractor ()
{
}

bool CEUTractor::Init()
{
    bool bRet = true;

    do
    {
        m_tractor.reset(new Mlt::Tractor(profile));
        addBackgroundTrack(m_tractor);
        addMainTrack(m_tractor);
        addSubTrack(m_tractor);
        addAudioTrack(m_tractor);

        int nCacheSize = QThread::idealThreadCount() + MaxTrack * 2;
        mlt_service_cache_set_size(nullptr, "producer_avformat", max(4, nCacheSize));

    } while (false);

    return bRet;
}

int CEUTractor::getLength()
{
    int nDuration = 0;

    do
    {
        CHECK_BREAK(!m_tractor);

        int nFilterCount = m_tractor->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> filter(m_tractor->filter(i));
            CHECK_CONTINUE(!filter || !filter->is_valid());
            nDuration = max(nDuration, filter->get_out());
        }

        int nCount = m_tractor->count();
        for (int i = 0; i < nCount; ++i)
        {
            shared_ptr<Mlt::Producer> track(m_tractor->track(i));
            CHECK_CONTINUE(!track || !track->is_valid());
            nDuration = max(nDuration, track->get_length());
        }

    } while (false);

    return nDuration;
}

void CEUTractor::onChanged()
{
    adjustBackgroundDuration();
    adjustFilterOrder();
}

void CEUTractor::addBackgroundTrack(shared_ptr<Mlt::Tractor> &tractor)
{
    do
    {
        CHECK_BREAK(!tractor->is_valid());

        m_playlist[BackgroundTrack].reset(new Mlt::Playlist(profile));
        m_playlist[BackgroundTrack]->set("id", kBackgroundTrackId);
        Mlt::Producer producer(profile, "color:0");
        producer.set("mlt_image_format", "rgb24a");
        producer.set("length", 1);
        producer.set("id", "black");
        // Allow mixing against frames produced by this producer.
        producer.set("set.test_audio", 0);
        m_playlist[BackgroundTrack]->append(producer);
        tractor->set_track(*m_playlist[BackgroundTrack], BackgroundTrack);

    } while (false);
}

void CEUTractor::addMainTrack(shared_ptr<Mlt::Tractor> &tractor)
{
    do
    {
        CHECK_BREAK(!tractor->is_valid());

        m_playlist[MainVideoTrack].reset(new Mlt::Playlist(profile));
        m_playlist[MainVideoTrack]->set(kVideoTrackProperty, 1);
        m_playlist[MainVideoTrack]->blank(0);
        tractor->set_track(*m_playlist[MainVideoTrack], MainVideoTrack);

        Mlt::Transition mix(profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        tractor->plant_transition(mix, 0, MainVideoTrack);

    } while (false);
}

void CEUTractor::addSubTrack(shared_ptr<Mlt::Tractor> &tractor)
{
    do
    {
        CHECK_BREAK(!tractor->is_valid());

        m_playlist[SubVideoTrack_0].reset(new Mlt::Playlist(profile));
        m_playlist[SubVideoTrack_0]->set(kVideoTrackProperty, 1);
        m_playlist[SubVideoTrack_0]->blank(0);
        tractor->set_track(*m_playlist[SubVideoTrack_0], SubVideoTrack_0);

        Mlt::Transition mix(profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        tractor->plant_transition(mix, 0, SubVideoTrack_0);

        Mlt::Transition composite(profile, playerGPU ? "movit.overlay" : "frei0r.cairoblend");
        tractor->plant_transition(composite, MainVideoTrack, SubVideoTrack_0);

    } while (false);
}

void CEUTractor::addAudioTrack(shared_ptr<Mlt::Tractor> &tractor)
{
    do
    {
        CHECK_BREAK(!tractor->is_valid());

        m_playlist[AudioTrack_0].reset( new Mlt::Playlist(profile));
        m_playlist[AudioTrack_0]->set(kAudioTrackProperty, 1);
        m_playlist[AudioTrack_0]->set("hide", 1);
        m_playlist[AudioTrack_0]->blank(0);
        tractor->set_track(*m_playlist[AudioTrack_0], AudioTrack_0);

        Mlt::Transition mix(profile, "mix");
        mix.set("always_active", 1);
        mix.set("sum", 1);
        tractor->plant_transition(mix, 0, AudioTrack_0);

    } while (false);
}

void CEUTractor::adjustBackgroundDuration()
{
    do
    {
        CHECK_BREAK(!m_tractor);

        int length = getLength();

        shared_ptr<Mlt::Producer> track(m_tractor->track(0));
        CHECK_BREAK(!track || !track->is_valid());

        shared_ptr<Mlt::Playlist> playlist(new Mlt::Playlist(*track));
        CHECK_BREAK(playlist->count() <= 0);

        shared_ptr<Mlt::Producer> clip(playlist->get_clip(0));
        CHECK_BREAK(!clip);

        int originalLength = clip->parent().get_length();
        CHECK_BREAK(length == originalLength);

        clip->parent().set("length", clip->parent().frames_to_time(length, mlt_time_clock));
        clip->parent().set_in_and_out(0, length - 1);
        clip->set("length", clip->parent().frames_to_time(length, mlt_time_clock));
        clip->set_in_and_out(0, length - 1);
        playlist->resize_clip(0, 0, length - 1);

    } while (false);
}

void CEUTractor::adjustFilterOrder()
{
    do
    {
        CHECK_BREAK(!m_tractor);

        map<int64_t, int> mapFilter;

        int nFilterCount = m_tractor->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> filter(m_tractor->filter(i));
            CHECK_CONTINUE(!filter);

            int priority = filter->get_int(kFilterPriority);
            int in = filter->get_in();
            mapFilter[(static_cast<int64_t>(priority) << 32) + in] = i;
        }

        int i = 0;
        for (auto it = mapFilter.begin(); it != mapFilter.end(); ++it, ++i)
        {
            CHECK_CONTINUE(it->second == i);
            m_tractor->move_filter(it->second, i);

            for (auto filter : mapFilter)
            {
                CHECK_CONTINUE(filter.second != i);
                filter.second = it->second;
                break;
            }

            it->second = i;
        }

    } while (false);
}

