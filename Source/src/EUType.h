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
#define kFilterId "filterId"
#define kHasDefaultFilter "hasDefaultFilter"
#define kSubVideo "subVideo"

#define kDefaultMltProfile "atsc_1080p_25"
#define kDefaultTransition "lumaMix"
#define kDefaultBrightnessId "defaultBrightness20200909"
#define kDefaultContrastId "defaultContrast20200909"
#define kDefaultPositionAndSizeId "defaultPositionAndSize20200909"
#define kDefaultVolumneId "defaultVolume20200909"


extern Mlt::Profile profile;

extern bool playerGPU;
extern int drawMethod;

void setProfile(Mlt::Profile& profile, const char* profileName);
void profileFromProducer(Mlt::Profile& profile, const char* urlOrXml);
string XML(Mlt::Service* service, bool withProfile = false, bool withMetadata = false);
QImage image(Mlt::Frame* frame, int width, int height);
int coerceMultiple(int value, int multiple = 2);

