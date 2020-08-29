#pragma once
#include "EUPlaylist.h"

class CEUTractor
{
public:
    CEUTractor();
    virtual ~CEUTractor ();

    shared_ptr<Mlt::Tractor> tractor() const { return m_tractor; }
    shared_ptr<CEUPlaylist> playlist(TrackIndex index);

    int getDuration();

private:
    int open();
    void close();

    void addBackgroundTrack();
    void addAudioTrack();
    void addMainVideoTrack();
    void addSubVideoTrack();

    bool isFiltered(Mlt::Producer* producer = nullptr) const;

private:
    Mlt::Profile m_profile;
    shared_ptr<Mlt::Tractor> m_tractor;
    bool m_bPlayerGPU;
};

