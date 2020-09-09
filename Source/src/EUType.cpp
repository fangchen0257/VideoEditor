#include "EUType.h"


Mlt::Profile profile;
Mlt::Profile previewProfile;

bool playerGPU = false;
int drawMethod = 0;


void setProfile(Mlt::Profile& profile, const char* profileName)
{
    Mlt::Profile tmp(profileName);
    profile.set_colorspace(tmp.colorspace());
    profile.set_frame_rate(tmp.frame_rate_num(), tmp.frame_rate_den());
    profile.set_width(tmp.width());
    profile.set_height(tmp.height());
    profile.set_progressive(tmp.progressive());
    profile.set_sample_aspect(tmp.sample_aspect_num(), tmp.sample_aspect_den());
    profile.set_display_aspect(tmp.display_aspect_num(), tmp.display_aspect_den());
}

void profileFromProducer(Mlt::Profile& profile, const char* urlOrXml)
{
    do
    {
        Mlt::Producer producer(profile, urlOrXml);
        profile.from_producer(producer);
        profile.set_width(coerceMultiple(profile.width()));
        profile.set_height(coerceMultiple(profile.height()));

    } while (false);
}

string XML(Mlt::Service* service, bool withProfile, bool withMetadata)
{
    string strXML;

    do
    {
        CHECK_BREAK(!service);

        static const char* propertyName = "string";
        Mlt::Consumer c(profile, "xml", propertyName);
        Mlt::Service s(service->get_service());
        CHECK_BREAK(!s.is_valid());

        int ignore = s.get_int("ignore_points");
        if (ignore)
        {
            s.set("ignore_points", 0);
        }

        c.set("time_format", "clock");
        if (!withMetadata)
        {
            c.set("no_meta", 1);
        }

        c.set("no_profile", !withProfile);
        c.set("store", "shotcut");
        c.set("root", "");
        c.connect(s);
        c.start();
        if (ignore)
        {
            s.set("ignore_points", ignore);
        }
        strXML = c.get(propertyName);

    } while (false);

    return strXML;
}

QImage image(Mlt::Frame* frame, int width, int height)
{
    QImage img;

    do
    {
        if (!frame || !frame->is_valid())
        {
            img = QImage(width, height, QImage::Format_ARGB32);
            img.fill(QColor(Qt::red).rgb());
            break;
        }

        if (width > 0 && height > 0)
        {
            frame->set("rescale.interp", "bilinear");
            frame->set("deinterlace_method", "onefield");
            frame->set("top_field_first", -1);
        }

        mlt_image_format format = mlt_image_rgb24a;
        const uchar *image = frame->get_image(format, width, height);
        if (!image)
        {
            img = QImage(width, height, QImage::Format_ARGB32);
            img.fill(QColor(Qt::green).rgb());
            break;
        }

        QImage temp(width, height, QImage::Format_ARGB32);
        memcpy(temp.scanLine(0), image, size_t(width * height * 4));
        img = temp.rgbSwapped();

    } while (false);

    return img;
}

int coerceMultiple(int value, int multiple)
{
    return (value + multiple - 1) / multiple * multiple;
}

