#include "EUProducer.h"


const int kMaxImageDurationSecs = 3600 * 4;
const double kDefaultImageOutSecs = 4.0;


CEUProducer::CEUProducer(const char* urlOrXml)
{
    profileFromProducer(m_profile, urlOrXml);
    createProducer(m_profile, urlOrXml);
    addDefaultFilter();
}

CEUProducer::CEUProducer(Mlt::Profile& profile, const char* urlOrXml)
{
    createProducer(profile, urlOrXml);
    addDefaultFilter();
}

CEUProducer::CEUProducer(shared_ptr<Mlt::Producer> producer)
    : m_producer(producer)
{
    addDefaultFilter();
}

CEUProducer::~CEUProducer ()
{
    m_producer.reset();
}

shared_ptr<Mlt::Filter> CEUProducer::getFilter(const char* id)
{
    shared_ptr<Mlt::Filter> filter;

    do
    {
        CHECK_BREAK(!id || !m_producer);

        int nCount = m_producer->parent().filter_count();
        for (int i = 0; i < nCount; ++i)
        {
            shared_ptr<Mlt::Filter> f(m_producer->parent().filter(i));
            CHECK_CONTINUE(!f);

            const char* fid = f->get(kFilterId);
            CHECK_CONTINUE(!fid || strcmp(id, fid));

            filter = f;
            break;
        }

    } while (false);

    return filter;
}

void CEUProducer::addFilter(Mlt::Filter& filter)
{
    do
    {
        CHECK_BREAK(!m_producer);
        m_producer->parent().attach(filter);

    } while (false);
}

void CEUProducer::removeFilter(const char* id)
{
    do
    {
        CHECK_BREAK(!m_producer);

        auto f = getFilter(id);
        CHECK_BREAK(!f);

        m_producer->parent().detach(*f);

    } while (false);
}

bool CEUProducer::isAudio()
{
    bool bRet = true;

    do
    {
        CHECK_BREAK(!m_producer);

        int n = m_producer->parent().get_int("meta.media.nb_streams");
        for (int i = 0; i < n; i++)
        {
            QString key = QString("meta.media.%1.stream.type").arg(i);
            QString streamType(m_producer->parent().get(key.toLatin1().constData()));
            CHECK_CONTINUE(streamType != "video")

            bRet = false;
            break;
        }

    } while (false);

    return bRet;
}

double CEUProducer::brightnessLevel()
{
    double value = 0.0;

    do
    {
        auto f = getFilter(kDefaultBrightnessId);
        CHECK_BREAK(!f);

        if (playerGPU)
        {
            value = f->get_double("opacity");
        }
        else
        {
            value = f->get_double("level");
        }

    } while (false);

    return value;
}

void CEUProducer::setBrightnessLevel(double value)
{
    do
    {
        auto f = getFilter(kDefaultBrightnessId);
        CHECK_BREAK(!f);

        if (playerGPU)
        {
            f->set("opacity", value);
        }
        else
        {
            f->set("level", value);
        }

    } while (false);
}

double CEUProducer::contrastLevel()
{
    double value = 0.0;

    do
    {
        auto f = getFilter(kDefaultContrastId);
        CHECK_BREAK(!f);

        value = f->get_double("value");

    } while (false);

    return value;
}

void CEUProducer::setContrastLevel(double value)
{
    do
    {
        auto f = getFilter(kDefaultContrastId);
        CHECK_BREAK(!f);

        f->set("value", value);
        double gammaFactor = f->get_double("gamma_factor");

        f->set("gamma_r", (1.0 - value) * gammaFactor);
        f->set("gamma_g", (1.0 - value) * gammaFactor);
        f->set("gamma_b", (1.0 - value) * gammaFactor);
        f->set("gain_r",  value * gammaFactor);
        f->set("gain_g",  value * gammaFactor);
        f->set("gain_b",  value * gammaFactor);

    } while (false);
}

mlt_rect CEUProducer::positionAndSize()
{
    mlt_rect rect;
    memset(&rect, 0, sizeof(rect));

    do
    {
        auto f = getFilter(kDefaultPositionAndSizeId);
        CHECK_BREAK(!f);

        if (playerGPU)
        {
            rect = f->get_rect("rect");
        }
        else
        {
            rect = f->get_rect("transition.rect");
        }

    } while (false);

    return rect;
}

void CEUProducer::setPositionAndSize(mlt_rect rect)
{
    do
    {
        auto f = getFilter(kDefaultPositionAndSizeId);
        CHECK_BREAK(!f);

        if (playerGPU)
        {
            f->set("rect", rect);
        }
        else
        {
            f->set("transition.rect", rect);
        }

    } while (false);
}

double CEUProducer::volume()
{
    double value = 0.0;

    do
    {
        auto f = getFilter(kDefaultVolumneId);
        CHECK_BREAK(!f);

        value = f->get_double("level");

    } while (false);

    return value;
}

void CEUProducer::setVolume(double value)
{
    do
    {
        auto f = getFilter(kDefaultVolumneId);
        CHECK_BREAK(!f);

        value = f->set("level", value);

    } while (false);
}

string CEUProducer::xml()
{
    return XML(m_producer.get());
}

QImage CEUProducer::image(int width, int height, int frameNumber)
{
    QImage img;

    do
    {
        CHECK_BREAK(!m_producer);

        const char* file = m_producer->parent().get("resource");

        Mlt::Profile tmpProfile;
        profileFromProducer(tmpProfile, file);
        auto producer = CEUProducer(tmpProfile, file).producer();
        CHECK_BREAK(!producer || !producer->is_valid());

        Mlt::Filter scaler(tmpProfile, "swscale");
        Mlt::Filter padder(tmpProfile, "resize");
        Mlt::Filter converter(tmpProfile, "avcolor_space");
        producer->attach(scaler);
        producer->attach(padder);
        producer->attach(converter);

        producer->set_in_and_out(m_producer->get_in(), m_producer->get_out());
        producer->seek(frameNumber);

        img = ::image(producer->get_frame(), width, height);

    } while (false);

    return img;
}

void CEUProducer::createProducer(Mlt::Profile& profile, const char* urlOrXml)
{
    do
    {
        CHECK_BREAK(!urlOrXml || strlen(urlOrXml) <= 0);

        if ('<' == urlOrXml[0])
        {
            m_producer.reset(new Mlt::Producer(profile, "xml-string", urlOrXml));
        }
        else
        {
            m_producer.reset(new Mlt::Producer(profile, urlOrXml));
        }

        CHECK_BREAK(!m_producer || !m_producer->is_valid());

        if (!qstrcmp(m_producer->get("mlt_service"), "avformat"))
        {
            m_producer->set("mlt_service", "avformat-novalidate");
            m_producer->set("mute_on_pause", 0);
        }

        Mlt::Profile decodeProfile;
        decodeProfile.from_producer(*m_producer);

        string serviceName = m_producer->get("mlt_service");
        CHECK_BREAK(("pixbuf" != serviceName) && ("qimage" != serviceName));

        m_producer->set("ttl", 1);
        m_producer->set("length", m_producer->frames_to_time(qRound(profile.fps() * kMaxImageDurationSecs), mlt_time_clock));
        m_producer->set("out", qRound(profile.fps() * kDefaultImageOutSecs) - 1);

    } while (false);
}

bool CEUProducer::addDefaultFilter()
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter;
        auto f = getFilter(kDefaultBrightnessId);
        if (!f)
        {
            if (playerGPU)
            {
                filter.reset(new Mlt::Filter(profile, "movit.opacity"));
                FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
                filter->set("alpha", 1.0);
                filter->set("opacity", 1.0);
            }
            else
            {
                filter.reset(new Mlt::Filter(profile, "brightness"));
                FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
                filter->set("level", 1.0);
            }

            filter->set(kFilterId, kDefaultBrightnessId);
            CALL_BREAK(!m_producer->parent().attach(*filter), bRet);
        }

        f = getFilter(kDefaultContrastId);
        if (!f)
        {
            if (playerGPU)
            {
                filter.reset(new Mlt::Filter(profile, "movit.lift_gamma_gain"));
            }
            else
            {
                filter.reset(new Mlt::Filter(profile, "lift_gamma_gain"));
            }

            FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
            filter->set(kFilterId, kDefaultContrastId);
            filter->set("gamma_r", 1.0);
            filter->set("gamma_g", 1.0);
            filter->set("gamma_b", 1.0);
            filter->set("gain_r", 1.0);
            filter->set("gain_g", 1.0);
            filter->set("gain_b", 1.0);
            filter->set("gamma_factor", 2.0);
            filter->set("value", 0.5);
            CALL_BREAK(!m_producer->parent().attach(*filter), bRet);
        }

        f = getFilter(kDefaultVolumneId);
        if (!f)
        {
            filter.reset(new Mlt::Filter(profile, "volume"));
            FAIL_BREAK(!filter || !filter->is_valid(), bRet, false);
            filter->set(kFilterId, kDefaultVolumneId);
            filter->set("level", 0.0);
            CALL_BREAK(!m_producer->parent().attach(*filter), bRet);
        }

    } while (false);

    return bRet;
}



