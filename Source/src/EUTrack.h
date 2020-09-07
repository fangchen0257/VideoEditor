#pragma once
#include "EUType.h"


class CEUTractor;

class CEUTrack
{
public:
    CEUTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUTrack();

    shared_ptr<Mlt::Playlist> playlist() { return m_playlist; }

    int clipIndex(int position);
    string xml(int clipIndex);
    QImage image(int clipIndex, int width, int height, int frameNumber = 0);

protected:
    void consolidateBlanks();
    void removeBlankPlaceholder();

protected:
    CEUTractor& m_tractor;
    shared_ptr<Mlt::Playlist> m_playlist;
};
