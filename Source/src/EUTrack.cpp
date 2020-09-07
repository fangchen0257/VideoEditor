#include "EUTrack.h"
#include "EUTractor.h"


CEUTrack::CEUTrack(CEUTractor& tractor, shared_ptr<Mlt::Playlist> playlist)
    : m_tractor(tractor)
    , m_playlist(playlist)
{
}

CEUTrack::~CEUTrack ()
{
}

int CEUTrack::clipIndex(int position)
{
    int nIndex = -1;

    do
    {
        CHECK_BREAK(!m_playlist);
        nIndex = m_playlist->get_clip_index_at(position);

    } while (false);

    return nIndex;
}

string CEUTrack::xml(int clipIndex)
{
    string xml;

    do
    {
        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        CHECK_BREAK(!info);

        xml = CEUTractor::XML(info->cut);

    } while (false);

    return xml;
}

QImage CEUTrack::image(int clipIndex, int width, int height, int frameNumber)
{
    QImage img;

    do
    {
        CHECK_BREAK(!m_playlist);

        shared_ptr<Mlt::ClipInfo> info(m_playlist->clip_info(clipIndex));
        CHECK_BREAK(!info);

        frameNumber += info->frame_in;
        frameNumber = min(frameNumber, info->frame_out);

        shared_ptr<Mlt::Producer> producer(m_playlist->get_clip(clipIndex));
        CHECK_BREAK(!producer);

        CHECK_BREAK(!!producer->seek(frameNumber));
        shared_ptr<Mlt::Frame> frame(producer->get_frame());
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

void CEUTrack::consolidateBlanks()
{
    do
    {
        CHECK_BREAK(!m_playlist);

        for (int i = 1; i < m_playlist->count(); i++)
        {
            if (m_playlist->is_blank(i - 1) && m_playlist->is_blank(i))
            {
                int out = m_playlist->clip_length(i - 1) + m_playlist->clip_length(i) - 1;
                m_playlist->resize_clip(i - 1, 0, out);
                m_playlist->remove(i--);
            }
        }
        if (m_playlist->count() > 0)
        {
            int i = m_playlist->count() - 1;
            if (m_playlist->is_blank(i))
            {
                m_playlist->remove(i);
            }
        }
        if (m_playlist->count() == 0)
        {
            m_playlist->blank(0);
        }

    } while (false);
}

void CEUTrack::removeBlankPlaceholder()
{
    do
    {
        CHECK_BREAK(!m_playlist);

        if (m_playlist->count() == 1 && m_playlist->is_blank(0))
        {
            m_playlist->remove(0);
        }

    } while (false);
}



