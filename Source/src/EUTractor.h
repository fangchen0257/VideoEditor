#pragma once
#include "EUMltCtl.h"
#include "EUMainVideoTrack.h"
#include "EUSubVideoTrack.h"
#include "EUAudioTrack.h"
#include "EUFilterTrack.h"

class CEUTractor
{
public:
    CEUTractor();
    virtual ~CEUTractor();

    bool Init();
    bool playerGPU() { return m_bPlayerGPU; }
    void setPlayerGPU(bool value) { m_bPlayerGPU = value; }

    shared_ptr<Mlt::Producer> producer() const { return shared_ptr<Mlt::Producer>(m_tractor); }
    shared_ptr<CEUMainVideoTrack> mainVideoTrack() { return shared_ptr<CEUMainVideoTrack>(new CEUMainVideoTrack(*this, m_playlist[MainVideoTrack])); }
    shared_ptr<CEUSubVideoTrack> subVideoTrack() { return shared_ptr<CEUSubVideoTrack>(new CEUSubVideoTrack(*this, m_playlist[SubVideoTrack_0])); }
    shared_ptr<CEUFilterTrack> effectTrack() { return shared_ptr<CEUFilterTrack>(new CEUFilterTrack(*this, EffectFilter)); }
    shared_ptr<CEUFilterTrack> textTrack() { return shared_ptr<CEUFilterTrack>(new CEUFilterTrack(*this, TextFilter)); }
    shared_ptr<CEUAudioTrack> audioTrack() { return shared_ptr<CEUAudioTrack>(new CEUAudioTrack(*this, m_playlist[AudioTrack_0])); }

    int getLength();
    void onChanged();

    static string XML(Mlt::Service* service, bool withProfile = false, bool withMetadata = false);

private:
    void addBackgroundTrack(shared_ptr<Mlt::Tractor> &tractor);
    void addMainTrack(shared_ptr<Mlt::Tractor> &tractor);
    void addSubTrack(shared_ptr<Mlt::Tractor> &tractor);
    void addAudioTrack(shared_ptr<Mlt::Tractor> &tractor);

    void adjustBackgroundDuration();
    void adjustFilterOrder();

private:
    shared_ptr<Mlt::Tractor> m_tractor;
    shared_ptr<Mlt::Playlist> m_playlist[MaxTrack];
    bool m_bPlayerGPU;
};

