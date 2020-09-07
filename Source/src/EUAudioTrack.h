#pragma once
#include "EUTrack.h"


class CEUAudioTrack : public CEUTrack
{
public:
    CEUAudioTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUAudioTrack();

    bool appendClip(const string &urlOrXml);
    bool appendClip(Mlt::Producer &clip);
    bool overwrite(const string &urlOrXml, int position);
    bool overwrite(Mlt::Producer& clip, int position);
    bool trimClipIn(int clipIndex, int delta);
    bool trimClipOut(int clipIndex, int delta);
    bool moveClip(int clipIndex, int position);
    bool splitClip(int clipIndex, int position);
    bool liftClip(int clipIndex);
};
