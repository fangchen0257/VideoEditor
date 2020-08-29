#pragma once

#include <Mlt.h>
#include <memory>

using namespace std;


typedef enum {
    PlaylistTrackType = 0,
    BlackTrackType,
    SilentTrackType,
    AudioTrackType,
    VideoTrackType
} TrackType;

typedef enum {
    BackgroundTrack = 0,
    MainTrack,
    SubTrack_0,
    AudioTrack
} TrackIndex;


#define kAudioTrackProperty "xvideo:audio"
#define kShotcutTransitionProperty "xvideo:transition"
#define kVideoTrackProperty "xvideo:video"

#define kBackgroundTrackId "background"

#define kDefaultMltProfile "atsc_1080p_25"
