#include "EUPlaylist.h"

CEUPlaylist::CEUPlaylist(const shared_ptr<Mlt::Tractor> &tractor, int trackIndex)
{
    do
    {
        if (!tractor)
            break;

        m_tractor = tractor;

        shared_ptr<Mlt::Producer> track(tractor->track(trackIndex));
        if (!track)
            break;

        m_playlist.reset(new Mlt::Playlist(*track));

    } while (false);
}

CEUPlaylist::~CEUPlaylist ()
{
}

int CEUPlaylist::clipIndex(int position)
{
    int nIndex = -1;

    do
    {
        if (!m_playlist)
        {
            break;
        }

        nIndex = m_playlist->get_clip_index_at(position);

    } while (false);

    return nIndex;
}

bool CEUPlaylist::trimClipInValid(int clipIndex, int delta)
{
    bool bRet = false;

    do
    {
        if (!m_playlist)
        {
            break;
        }

        bRet = true;

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));

        if (!info || (info->frame_in + delta) < 0 || (info->frame_in + delta) > info->frame_out)
            bRet = false;
        else if (delta < 0 && clipIndex <= 0)
            bRet = false;
        else if (delta < 0 && clipIndex > 0 && !m_playlist->is_blank(clipIndex - 1))
            bRet = false;
        else if (delta > 0 && clipIndex > 0 && isTransition(clipIndex - 1))
            bRet = false;

    } while (false);

    return bRet;
}

bool CEUPlaylist::trimClipOutValid(int clipIndex, int delta)
{
    bool bRet = false;

    do
    {
        if (!m_playlist)
        {
            break;
        }

        bRet = true;

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        if (!info || (info->frame_out - delta) >= info->length || (info->frame_out - delta) < info->frame_in)
            bRet = false;
        else if (delta < 0 && (clipIndex + 1) < m_playlist->count() && !m_playlist->is_blank(clipIndex + 1))
            bRet = false;
        else if (delta > 0 && (clipIndex + 1) < m_playlist->count() && isTransition(clipIndex + 1))
            bRet = false;

    } while (false);

    return bRet;
}

bool CEUPlaylist::isTransition(int clipIndex) const
{
    bool bRet = false;

    do
    {
        if (!m_playlist)
        {
            break;
        }

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        if (producer && producer->parent().get(kShotcutTransitionProperty))
        {
            bRet = true;
        }

    } while (false);

    return bRet;
}

