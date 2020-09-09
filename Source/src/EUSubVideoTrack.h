#pragma once
#include "EUTrack.h"


class CEUSubVideoTrack : public CEUTrack
{
public:
    CEUSubVideoTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUSubVideoTrack();

    bool appendClip(const string &urlOrXml);
    bool appendClip(shared_ptr<Mlt::Producer> clip);
    bool overwrite(const string &urlOrXml, int position);
    bool overwrite(shared_ptr<Mlt::Producer> clip, int position);
    bool trimClipIn(int clipIndex, int delta);
    bool trimClipOut(int clipIndex, int delta);
    bool moveClip(int clipIndex, int position);
    bool splitClip(int clipIndex, int position);
    bool liftClip(int clipIndex);

protected:
    bool addDefaultFilter(shared_ptr<Mlt::Producer> clip);
};
