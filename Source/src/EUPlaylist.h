#pragma once
#include "EUType.h"

class CEUPlaylist
{
public:
    CEUPlaylist(const shared_ptr<Mlt::Tractor> &tractor, int trackIndex);
    virtual ~CEUPlaylist ();

    shared_ptr<Mlt::Playlist> playlist() { return m_playlist; }

    int clipIndex(int position);
    bool trimClipInValid(int clipIndex, int delta);
    bool trimClipOutValid(int clipIndex, int delta);
    bool isTransition(int clipIndex) const;

    int trimClipIn(int clipIndex, int delta);
    int trimClipOut(int clipIndex, int delta);
    bool moveClip(int clipIndex, int position);
    int overwriteClip(Mlt::Producer& clip, int position, bool seek = true);
    int overwrite(Mlt::Producer& clip, int position, bool seek = true);
    int insertClip(Mlt::Producer& clip, int position, bool seek = true);
    int appendClip(Mlt::Producer &clip);
    void removeClip(int clipIndex);
    void liftClip(int clipIndex);
    void splitClip(int clipIndex, int position);
    void joinClips(int clipIndex);
    void appendFromPlaylist(Mlt::Playlist* playlist, int trackIndex);
    void overwriteFromPlaylist(Mlt::Playlist& playlist, int trackIndex, int position);

    bool addTransitionValid(int clipIndex, int position);
    int addTransition(int clipIndex, int position);
    void removeTransition(int clipIndex);
    void removeTransitionByTrimIn(int clipIndex, int delta);
    void removeTransitionByTrimOut(int clipIndex, int delta);
    bool trimTransitionInValid(int clipIndex, int delta);
    void trimTransitionIn(int clipIndex, int delta);
    bool trimTransitionOutValid(int clipIndex, int delta);
    void trimTransitionOut(int clipIndex, int delta);
    bool addTransitionByTrimInValid(int clipIndex, int delta);
    void addTransitionByTrimIn(int clipIndex, int delta);
    bool addTransitionByTrimOutValid(int clipIndex, int delta);
    void addTransitionByTrimOut(int clipIndex, int delta);
    bool removeTransitionByTrimInValid(int clipIndex, int delta);
    bool removeTransitionByTrimOutValid(int clipIndex, int delta);

private:
    void moveClipToEnd(Mlt::Playlist& playlist, int clipIndex, int position);
    void moveClipInBlank(Mlt::Playlist& playlist, int clipIndex, int position, int duration = 0);
    void consolidateBlanks(Mlt::Playlist& playlist);
    void removeBlankPlaceholder(Mlt::Playlist& playlist);
    void removeRegion(int position, int length);
    void clearMixReferences(int clipIndex);
    int getDuration();

private:
    shared_ptr<Mlt::Tractor> m_tractor;
    shared_ptr<Mlt::Playlist> m_playlist;
    
};
