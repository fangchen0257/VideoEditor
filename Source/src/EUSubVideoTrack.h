#pragma once
#include "EUTrack.h"


class CEUSubVideoTrack : public CEUTrack
{
public:
    CEUSubVideoTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUSubVideoTrack();

    shared_ptr<Mlt::Filter> filter(int clipIndex);

    bool appendClip(const string &urlOrXml);
    bool appendClip(Mlt::Producer &clip);
    bool overwrite(const string &urlOrXml, int position);
    bool overwrite(Mlt::Producer& clip, int position);
    bool trimClipIn(int clipIndex, int delta);
    bool trimClipOut(int clipIndex, int delta);
    bool moveClip(int clipIndex, int position);
    bool splitClip(int clipIndex, int position);
    bool liftClip(int clipIndex);

    const char* fillProperty();
    const char* distortProperty();
    const char* rectProperty();
    const char* valignProperty();
    const char* halignProperty();

protected:
    bool addFilter(Mlt::Producer &clip);
};
