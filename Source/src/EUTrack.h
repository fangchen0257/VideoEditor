#pragma once
#include "EUType.h"
#include "EUProducer.h"


class CEUTractor;

class CEUTrack
{
public:
    CEUTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUTrack();

    shared_ptr<Mlt::Playlist> playlist() { return m_playlist; }
    shared_ptr<CEUProducer> clip(int clipIndex);

    int clipIndex(int position);

protected:
    void consolidateBlanks();
    void removeBlankPlaceholder();

protected:
    CEUTractor& m_tractor;
    shared_ptr<Mlt::Playlist> m_playlist;
};
