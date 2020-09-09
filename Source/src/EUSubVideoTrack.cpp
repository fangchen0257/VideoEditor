#include "EUSubVideoTrack.h"
#include "EUTractor.h"
#include "EUProducer.h"


CEUSubVideoTrack::CEUSubVideoTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist) : CEUTrack(tractor, playlist)
{
}

CEUSubVideoTrack::~CEUSubVideoTrack ()
{
}

bool CEUSubVideoTrack::appendClip(const string &urlOrXml)
{
    bool bRet = true;

    do
    {
        CEUProducer p(profile, urlOrXml.c_str());
        CALL_BREAK(appendClip(p.producer()), bRet);

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::appendClip(shared_ptr<Mlt::Producer> clip)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!clip || !clip->is_valid(), bRet, false);
        FAIL_BREAK(!m_playlist, bRet, false);
        CALL_BREAK(addDefaultFilter(clip), bRet);

        removeBlankPlaceholder();

        int in = clip->get_in();
        int out = clip->get_out();
        clip->set_in_and_out(0, clip->get_length() - 1);
        CALL_BREAK(!m_playlist->append(clip->parent(), in, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::overwrite(const string &urlOrXml, int position)
{
    bool bRet = true;

    do
    {
        CEUProducer p(profile, urlOrXml.c_str());
        CALL_BREAK(overwrite(p.producer(), position), bRet);

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::overwrite(shared_ptr<Mlt::Producer> clip, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!clip || !clip->is_valid(), bRet, false);
        FAIL_BREAK(!m_playlist, bRet, false);
        CALL_BREAK(addDefaultFilter(clip), bRet);

        if (position < 0)
        {
            position = 0;
        }

        removeBlankPlaceholder();

        if (position >= m_playlist->get_playtime() - 1)
        {
            int n = m_playlist->count();
            int length = position - m_playlist->clip_start(n - 1) - m_playlist->clip_length(n - 1);

            if (length > 0)
            {
                CALL_BREAK(!m_playlist->blank(length - 1), bRet);
                ++n;
            }

            int in = clip->get_in();
            int out = clip->get_out();
            clip->set_in_and_out(0, clip->get_length() - 1);
            CALL_BREAK(!m_playlist->append(clip->parent(), in, out), bRet);

            m_tractor.onChanged();
            break;
        }

        int targetIndex = m_playlist->get_clip_index_at(position);
        if (position > m_playlist->clip_start(targetIndex))
        {
            CALL_BREAK(splitClip(targetIndex, position), bRet);
            ++targetIndex;
        }
        else if (position < 0)
        {
            CALL_BREAK(!clip->set_in_and_out(clip->get_in() - position, clip->get_out()), bRet);
            position = 0;
        }

        int length = clip->get_playtime();
        while (length > 0 && targetIndex < m_playlist->count())
        {
            if (m_playlist->clip_length(targetIndex) > length)
            {
                splitClip(targetIndex, position + length);
            }

            length -= m_playlist->clip_length(targetIndex);
            CALL_BREAK(!m_playlist->remove(targetIndex), bRet);
        }
        CHECK_BREAK(!bRet);

        int in = clip->get_in();
        int out = clip->get_out();
        clip->set_in_and_out(0, clip->get_length() - 1);
        CALL_BREAK(!m_playlist->insert(clip->parent(), targetIndex, in, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::trimClipIn(int clipIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        if (info->frame_in + delta < 0)
        {
            delta = -info->frame_in;
        }
        else if ((info->frame_in + delta) >= info->frame_out)
        {
            delta = info->frame_out - info->frame_in - 1;
        }

        int position = info->start + delta;

        string xml = XML(info->producer);
        shared_ptr<Mlt::Producer> clip(new Mlt::Producer(profile, "xml-string", xml.c_str()));
        FAIL_BREAK(!clip || !clip->is_valid(), bRet, false);

        CALL_BREAK(!clip->set_in_and_out(info->frame_in + delta, info->frame_out), bRet);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::trimClipOut(int clipIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        if ((info->frame_out - delta) >= info->length)
        {
            delta = info->frame_out - info->length + 1;
        }
        else if (delta >= info->frame_count)
        {
            delta = info->frame_count - 2;
        }

        int position = info->start;

        string xml = XML(info->producer);
        shared_ptr<Mlt::Producer> clip(new Mlt::Producer(profile, "xml-string", xml.c_str()));
        FAIL_BREAK(!clip || !clip->is_valid(), bRet, false);

        CALL_BREAK(!clip->set_in_and_out(info->frame_in, info->frame_out - delta), bRet);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::moveClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        string xml = XML(info->producer);
        shared_ptr<Mlt::Producer> clip(new Mlt::Producer(profile, "xml-string", xml.c_str()));
        FAIL_BREAK(!clip || !clip->is_valid(), bRet, false);

        clip->set_in_and_out(info->frame_in, info->frame_out);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::splitClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);
        FAIL_BREAK(position <= info->start, bRet, false);
        FAIL_BREAK(position >= info->start + info->frame_count, bRet, false);

        Mlt::Producer producer(profile, "xml-string", XML(info->producer).c_str());
        int in = info->frame_in;
        int out = info->frame_out;
        int duration = position - m_playlist->clip_start(clipIndex);

        if (m_playlist->is_blank(clipIndex))
        {
            m_playlist->insert_blank(clipIndex, duration - 1);
        }
        else
        {
            CALL_BREAK(!m_playlist->insert(producer, clipIndex, in, in + duration - 1), bRet);
        }

        CALL_BREAK(!m_playlist->resize_clip(clipIndex + 1, in + duration, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::liftClip(int clipIndex)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);
        FAIL_BREAK(clipIndex >= m_playlist->count(), bRet, false);

        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUSubVideoTrack::addDefaultFilter(shared_ptr<Mlt::Producer> clip)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!clip, bRet, false);

        CEUProducer p(clip);
        CHECK_BREAK(!p.width() || !p.height());

        auto f = p.getFilter(kDefaultPositionAndSizeId);
        CHECK_BREAK(f);

        mlt_rect rect;
        double width = p.width(), height = p.height(), radio = 0.0;

        if (width > height)
        {
            radio = height / width;
            rect.x = profile.width() / 4;
            rect.w = profile.width() / 2;
            rect.h = rect.w * radio;
            rect.y = (profile.height() - rect.h) / 2;
        }
        else
        {
            radio = width / height;
            rect.y = profile.height() / 4;
            rect.h = profile.height() / 2;
            rect.w = rect.h * radio;
            rect.x = (profile.width() - rect.w) / 2;
        }

        rect.o = 1.0;

        shared_ptr<Mlt::Filter> filter;

        if (playerGPU)
        {
            filter.reset(new Mlt::Filter(profile, "movit.rect"));
            FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
            filter->set("fill", 1);
            filter->set("distort", 1);
            filter->set("rect", rect);
            filter->set("valign", "left");
            filter->set("halign", "top");
        }
        else
        {
            filter.reset(new Mlt::Filter(profile, "affine"));
            FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
            filter->set("transition.fill", 1);
            filter->set("transition.distort", 1);
            filter->set("transition.rect", rect);
            filter->set("transition.valign", "left");
            filter->set("transition.halign", "top");
        }

        filter->set(kFilterId, kDefaultPositionAndSizeId);
        p.addFilter(*filter);

    } while (false);

    return bRet;
}


