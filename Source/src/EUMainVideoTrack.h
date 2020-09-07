#pragma once
#include "EUTrack.h"


class CEUMainVideoTrack : public CEUTrack
{
public:
    CEUMainVideoTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist);
    virtual ~CEUMainVideoTrack();

    shared_ptr<Mlt::Transition> transition(int clipIndex);

    bool isTransition(int clipIndex) const;

    bool appendClip(const string &urlOrXml);
    bool appendClip(Mlt::Producer &clip);
    bool overwrite(const string &urlOrXml, int position);
    bool overwrite(Mlt::Producer& clip, int position);
    bool trimClipIn(int clipIndex, int delta);
    bool trimClipOut(int clipIndex, int delta);
    bool moveClip(int clipIndex, int position);
    bool splitClip(int clipIndex, int position);
    bool removeClip(int clipIndex);
    bool addTransition(int clipIndex, int length);
    bool resizeTransition(int clipIndex, int length);

protected:
    void clearMixReferences(int clipIndex, bool in, bool out);
};
