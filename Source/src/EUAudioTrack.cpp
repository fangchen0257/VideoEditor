#include "EUAudioTrack.h"
#include "EUTractor.h"


CEUAudioTrack::CEUAudioTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist) : CEUTrack(tractor, playlist)
{
}

CEUAudioTrack::~CEUAudioTrack ()
{
}

bool CEUAudioTrack::appendClip(const string &urlOrXml)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(urlOrXml.empty(), bRet, false);

        shared_ptr<Mlt::Producer> producer;

        if ('<' == urlOrXml[0])
        {
            producer.reset(new Mlt::Producer(CMltCtl::profile(), "xml-string", urlOrXml.c_str()));
        }
        else
        {
            producer.reset(new Mlt::Producer(CMltCtl::profile(), urlOrXml.c_str()));
        }

        CALL_BREAK(appendClip(*producer), bRet);

    } while (false);

    return bRet;
}

bool CEUAudioTrack::appendClip(Mlt::Producer &clip)
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

bool CEUAudioTrack::overwrite(const string &urlOrXml, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(urlOrXml.empty(), bRet, false);

        shared_ptr<Mlt::Producer> producer;

        if ('<' == urlOrXml[0])
        {
            producer.reset(new Mlt::Producer(CMltCtl::profile(), "xml-string", urlOrXml.c_str()));
        }
        else
        {
            producer.reset(new Mlt::Producer(CMltCtl::profile(), urlOrXml.c_str()));
        }

        CALL_BREAK(overwrite(*producer, position), bRet);

    } while (false);

    return bRet;
}

bool CEUAudioTrack::overwrite(Mlt::Producer& clip, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

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

            int in = clip.get_in();
            int out = clip.get_out();
            clip.set_in_and_out(0, clip.get_length() - 1);
            CALL_BREAK(!m_playlist->append(clip.parent(), in, out), bRet);

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
            CALL_BREAK(!clip.set_in_and_out(clip.get_in() - position, clip.get_out()), bRet);
            position = 0;
        }

        int length = clip.get_playtime();
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

        int in = clip.get_in();
        int out = clip.get_out();
        clip.set_in_and_out(0, clip.get_length() - 1);
        CALL_BREAK(!m_playlist->insert(clip.parent(), targetIndex, in, out), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUAudioTrack::trimClipIn(int clipIndex, int delta)
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

        string xml = CEUTractor::XML(info->producer);
        Mlt::Producer clip(CMltCtl::profile(), "xml-string", xml.c_str());
        CALL_BREAK(clip.is_valid(), bRet);

        CALL_BREAK(!clip.set_in_and_out(info->frame_in + delta, info->frame_out), bRet);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUAudioTrack::trimClipOut(int clipIndex, int delta)
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

        string xml = CEUTractor::XML(info->producer);
        Mlt::Producer clip(CMltCtl::profile(), "xml-string", xml.c_str());
        CALL_BREAK(clip.is_valid(), bRet);

        CALL_BREAK(!clip.set_in_and_out(info->frame_in, info->frame_out - delta), bRet);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUAudioTrack::moveClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);

        string xml = CEUTractor::XML(info->producer);
        Mlt::Producer clip(CMltCtl::profile(), "xml-string", xml.c_str());
        CALL_BREAK(clip.is_valid(), bRet);

        clip.set_in_and_out(info->frame_in, info->frame_out);
        delete m_playlist->replace_with_blank(clipIndex);
        consolidateBlanks();

        CALL_BREAK(overwrite(clip, position), bRet);

    } while (false);

    return bRet;
}

bool CEUAudioTrack::splitClip(int clipIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_playlist, bRet, false);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        FAIL_BREAK(!info, bRet, false);
        FAIL_BREAK(position <= info->start, bRet, false);
        FAIL_BREAK(position >= info->start + info->frame_count, bRet, false);

        Mlt::Producer producer(CMltCtl::profile(), "xml-string", CEUTractor::XML(info->producer).c_str());
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

bool CEUAudioTrack::liftClip(int clipIndex)
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


