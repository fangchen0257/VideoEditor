#include "EUMainVideoTrack.h"
#include "EUTractor.h"


CEUMainVideoTrack::CEUMainVideoTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist) : CEUTrack(tractor, playlist)
{
}

CEUMainVideoTrack::~CEUMainVideoTrack ()
{
}

shared_ptr<Mlt::Transition> CEUMainVideoTrack::transition(int clipIndex)
{
    shared_ptr<Mlt::Transition> transition;

    do
    {
        CHECK_BREAK(!m_playlist);
        CHECK_BREAK(!isTransition(clipIndex));

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        CHECK_BREAK(!producer || !producer->is_valid());

        shared_ptr<Mlt::Service> service(producer->producer());
        for (; service && service->is_valid(); service.reset(service->producer()))
        {
            CHECK_CONTINUE(service->type() != transition_type);

            char* name = service->get("mlt_service");
            CHECK_CONTINUE(!name);

            if (0 == strcmp("luma", name) || 0 == strcmp("movit.luma_mix", name))
            {
                transition.reset(new Mlt::Transition(*service));
                break;
            }
        }

    } while (false);

    return transition;
}

bool CEUMainVideoTrack::isTransition(int clipIndex) const
{
    bool bRet = false;

    do
    {
        CHECK_BREAK(!m_playlist);

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        if (producer && producer->parent().get(kTransitionProperty))
        {
            bRet = true;
        }

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::appendClip(const string &urlOrXml)
{
    bool bRet = true;

    do
    {
        shared_ptr<Mlt::Producer> producer = createProducer(profile, urlOrXml.c_str());
        FAIL_BREAK(!producer, bRet, false);

        CALL_BREAK(appendClip(*producer), bRet);

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::appendClip(Mlt::Producer &clip)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        CALL_BREAK(clip.is_valid(), bRet);

        removeBlankPlaceholder();

        int in = clip.get_in();
        int out = clip.get_out();
        clip.set_in_and_out(0, clip.get_length() - 1);
        CALL_BREAK(!m_playlist->append(clip.parent(), in, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::overwrite(const string &urlOrXml, int position)
{
    bool bRet = true;

    do
    {
        shared_ptr<Mlt::Producer> producer = createProducer(profile, urlOrXml.c_str());
        FAIL_BREAK(!producer, bRet, false);

        CALL_BREAK(overwrite(*producer, position), bRet);

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::overwrite(Mlt::Producer& clip, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        CALL_BREAK(clip.is_valid(), bRet);

        removeBlankPlaceholder();

        int targetIndex = m_playlist->get_clip_index_at(position);
        clearMixReferences(targetIndex, true, false);

        int in = clip.get_in();
        int out = clip.get_out();
        clip.set_in_and_out(0, clip.get_length() - 1);
        CALL_BREAK(!m_playlist->insert(clip, targetIndex, in + 100, out - 100), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::trimClipIn(int clipIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        CHECK_BREAK(0 == delta);
        CHECK_BREAK(isTransition(clipIndex));

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        if (info->frame_in + delta < 0)
        {
            delta = -info->frame_in;
        }
        else if (info->frame_in + delta >= info->frame_out)
        {
            delta = info->frame_out - info->frame_in - 1;
        }

        clearMixReferences(clipIndex, true, false);

        CALL_BREAK(!m_playlist->resize_clip(clipIndex, info->frame_in + delta, info->frame_out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::trimClipOut(int clipIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        CHECK_BREAK(0 == delta);
        CHECK_BREAK(isTransition(clipIndex));

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        if ((info->frame_out - delta) >= info->length)
        {
            delta = info->frame_out - info->length + 1;
        }
        else if ((info->frame_out - delta) <= info->frame_in)
        {
            delta = info->frame_out - info->frame_in - 1;
        }

        clearMixReferences(clipIndex, false, true);

        CALL_BREAK(!m_playlist->resize_clip(clipIndex, info->frame_in, info->frame_out - delta), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::moveClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        int targetIndex = m_playlist->get_clip_index_at(position);
        CHECK_BREAK(targetIndex == clipIndex);

        clearMixReferences(clipIndex, true, true);
        clearMixReferences(targetIndex, true, true);

        CALL_BREAK(!m_playlist->move(clipIndex, targetIndex), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::splitClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        Mlt::Producer newProducer(profile, "xml-string", XML(info->producer).c_str());
        int in = info->frame_in;
        int out = info->frame_out;
        int duration = position - m_playlist->clip_start(clipIndex);

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        FAIL_BREAK(!producer, bRet, false);

        if (producer->get_data("mix_in"))
        {
            Mlt::Service service(mlt_service(producer->get_data("mix_in")));
            service.set("mix_out", newProducer.get_parent(), 0);
            producer->set("mix_in", nullptr, 0);

            Mlt::Producer p(service);
            Mlt::Tractor tractor(p.parent());
            if (tractor.is_valid())
            {
                shared_ptr<Mlt::Producer> track_b(tractor.track(1));
                track_b.reset(newProducer.cut(track_b->get_in(), track_b->get_out()));
                tractor.set_track(*track_b, 1);
            }
        }

        CALL_BREAK(!m_playlist->insert(newProducer, clipIndex, in, in + duration - 1), bRet);
        CALL_BREAK(!m_playlist->resize_clip(clipIndex + 1, in + duration, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::removeClip(int clipIndex)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        CALL_BREAK(!m_playlist->remove(clipIndex), bRet);

        consolidateBlanks();
        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::addTransition(int clipIndex, int length)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        FAIL_BREAK(isTransition(clipIndex), bRet, false);
        FAIL_BREAK(isTransition(clipIndex + 1), bRet, false);

        CALL_BREAK(!m_playlist->mix(clipIndex, length), bRet);

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex + 1));
        FAIL_BREAK(!producer, bRet, false);
        producer->parent().set(kTransitionProperty, kDefaultTransition);

        Mlt::Transition dissolve(profile, m_tractor.playerGPU()? "movit.luma_mix" : "luma");
        Mlt::Transition crossFade(profile, "mix:-1");
        if (!m_tractor.playerGPU())
        {
            dissolve.set("alpha_over", 1);
        }

        CALL_BREAK(!m_playlist->mix_add(clipIndex + 1, &dissolve), bRet);
        CALL_BREAK(!m_playlist->mix_add(clipIndex + 1, &crossFade), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUMainVideoTrack::resizeTransition(int clipIndex, int length)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        FAIL_BREAK(!isTransition(clipIndex), bRet, false);

        CALL_BREAK(!m_playlist->resize_clip(clipIndex, 0, length - 1), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

void CEUMainVideoTrack::clearMixReferences(int clipIndex, bool in, bool out)
{
    do
    {
        CHECK_BREAK(!m_playlist);

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        CHECK_BREAK(!producer || !producer->is_valid());

        if (in && producer->get_data("mix_in"))
        {
            Mlt::Service service(mlt_service(producer->get_data("mix_in")));
            service.set("mix_out", nullptr, 0);
            producer->set("mix_in", nullptr, 0);
        }

        if (out && producer->get_data("mix_out"))
        {
            Mlt::Service service(mlt_service(producer->get_data("mix_out")));
            service.set("mix_in", nullptr, 0);
            producer->set("mix_out", nullptr, 0);
        }

    } while (false);
}


