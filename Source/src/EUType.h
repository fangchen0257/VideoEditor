#pragma once

#include <Mlt.h>
#include <qdebug.h>
#include <QImage>
#include <QThread>
#include <string.h>
#include <memory>
#include <string>
#include <list>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

#define CHECK_BREAK(c) if (c) { break; }
#define CHECK_CONTINUE(c) if (c) { continue; }
#define FAIL_BREAK(c, ret, err) if (c) { ret = err; break; }
#define CALL_BREAK(call, ret) if (!(ret = call)) { break; }


typedef enum {
    BackgroundTrack = 0,
    MainVideoTrack,
    SubVideoTrack_0,
    AudioTrack_0,
    MaxTrack
} TrackIndex;

typedef enum {
    EffectFilter = 0,
    TextFilter
} FilterPriority;


#define kAudioTrackProperty "xvideo:audio"
#define kTransitionProperty "xvideo:transition"
#define kVideoTrackProperty "xvideo:video"

#define kBackgroundTrackId "background"
#define kFilterPriority "filterPriority"
#define kHasDefaultFilter "hasDefaultFilter"

#define kDefaultMltProfile "atsc_1080p_25"
#define kDefaultTransition "lumaMix"
#define kDefaultSubVideoFilter "defaultSubVideoFilter"


extern Mlt::Profile profile;
extern Mlt::Profile previewProfile;
extern bool playerGPU;

void setProfile(Mlt::Profile& profile, const char* profileName);
shared_ptr<Mlt::Producer> createProducer(Mlt::Profile& profile, const char* urlOrXml);
string XML(Mlt::Service* service, bool withProfile = false, bool withMetadata = false);
QImage IMAGE(Mlt::Frame* frame, int width, int height);
QImage IMAGE(Mlt::Producer* producer, int width, int height, int frameNumber);

