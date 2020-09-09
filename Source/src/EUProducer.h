#pragma once
#include "EUType.h"


class CEUProducer
{
public:
    CEUProducer(const char* urlOrXml);
    CEUProducer(Mlt::Profile& profile, const char* urlOrXml);
    CEUProducer(shared_ptr<Mlt::Producer> producer);
    virtual ~CEUProducer();

    shared_ptr<Mlt::Producer> producer() { return m_producer; }

    shared_ptr<Mlt::Filter> getFilter(const char* id);
    void addFilter(Mlt::Filter& filter);
    void removeFilter(const char* id);

    bool isAudio();

    int width() { return m_producer ? m_producer->get_int("meta.media.width") : 0; }
    int height() { return m_producer ? m_producer->get_int("meta.media.height") : 0; }
    double fps() { return m_producer ? (m_producer->get_double("meta.media.frame_rate_num") / m_producer->get_double("meta.media.frame_rate_den")) : 0.0; }
    int progressive() { return m_producer ? m_producer->get_int("meta.media.progressive") : 0; }

    double brightnessLevel();
    void setBrightnessLevel(double value);

    double contrastLevel();
    void setContrastLevel(double value);

    mlt_rect positionAndSize();
    void setPositionAndSize(mlt_rect rect);

    double volume();
    void setVolume(double value);

    string xml();
    QImage image(int width, int height, int frameNumber = 0);

protected:
    void createProducer(Mlt::Profile& profile, const char* urlOrXml);
    bool addDefaultFilter();

protected:
    Mlt::Profile m_profile;
    shared_ptr<Mlt::Producer> m_producer;
};
