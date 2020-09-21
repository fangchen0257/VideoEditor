#include "effecthorizonheader.h"
#include "effectview.h"
#include <qpainter.h>
#include <qdebug.h>

#define SCALE_UNIT 10       //最小刻度对应的像素宽度
#define UNIT_FRAME 3        //最小刻度对应的帧数
#define LARGE_SCALE 12      //大刻度像素高度
#define SMALL_SCALE 8       //小刻度像素高度
#define FRAME_PER_SEC 30    //fps

const int scaleFactor[20] = {300,270,180,120,80,60,40,30,25,20,18,16,14,12,10,8,6,3,2,1};
CEffectHorizonHeader::CEffectHorizonHeader(QWidget *parent)
    :QHeaderView(Qt::Horizontal, parent)
    ,m_pParent(parent)
    ,m_factor(10)
    ,m_scaleCount(0)
{
    setSectionsClickable(true);
    setSectionResizeMode(QHeaderView::Fixed);
    setFixedHeight(40);
}

void CEffectHorizonHeader::setScaleFactor(int factor)
{
    m_factor = factor;
    viewport()->update();
}

double CEffectHorizonHeader::getPixelPerFrame()
{
    double fPixelPerFrame = (SCALE_UNIT/(UNIT_FRAME*getScaleFactor()*1.0));
    return fPixelPerFrame;
}

void CEffectHorizonHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    do
    {
        if (nullptr == painter) break;

        painter->fillRect(rect, QBrush(QColor(33,39,53)));
        if (0 == logicalIndex)
        {
            painter->save();
            painter->setPen(QColor(21,25,33));
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            painter->restore();
        }
        else
        {
            paintScale(painter, rect, m_factor);
        }
    } while(0);
}

void CEffectHorizonHeader::paintScale(QPainter *painter, const QRect &rect, int factor) const
{
    Q_UNUSED(factor);
    if (nullptr==painter || !rect.isValid()) return;

    //draw bottom line
    painter->save();
    painter->setPen(QColor(44,63,85));
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());

    //draw scale
    painter->setPen(QColor(53,83,111));
    int h = rect.height();
    int scale = 0;
    for (int x=rect.left(); x<=rect.right(); x+=SCALE_UNIT, scale++)
    {
        bool bDrawLargeScale = !(scale%10);
        int yOffset = bDrawLargeScale?LARGE_SCALE:SMALL_SCALE;
        QPoint pt1 = QPoint(x, (h-yOffset));
        QPoint pt2 = QPoint(x, h);
        painter->drawLine(pt1, pt2);
        if (bDrawLargeScale)
        {
            QString strTime = calcTimeStr(scale);

            QFont textFont("Segoe UI");
            textFont.setPixelSize(10);
            QFontMetrics fm(textFont);

            int textW = fm.width(strTime);
            int textH = fm.height();
            QRect rtText(pt1.x(),pt1.y()-textH,textW,textH);
            painter->setFont(textFont);
            painter->drawText(rtText, strTime);
        }
    }
    m_scaleCount = scale;
    painter->restore();
}

#define HOUR_FRAME      (30*60*60)
#define MINUTE_FRAME    (30*60)
#define SECOND_FRAME    30
QString CEffectHorizonHeader::calcTimeStr(int scale) const
{
    QString strTime;
    do
    {
        int frames = scale*UNIT_FRAME*getScaleFactor();
        int hour=0, minute=0, second=0, frame=0;
        if (frames < SECOND_FRAME)
        {
            frame = frames;
        }
        else
        {
            if (frames >= HOUR_FRAME)
            {
                hour = (frames/HOUR_FRAME);
                frames = frames%HOUR_FRAME;
            }

            if (frames >= MINUTE_FRAME)
            {
                minute = (frames/MINUTE_FRAME);
                frames = frames%MINUTE_FRAME;
            }

            if (frames >= SECOND_FRAME)
            {
                second = (frames/SECOND_FRAME);
                frame = frames%SECOND_FRAME;
            }
        }

        strTime = QString("%1:%2:%3:%4").arg(hour,2,10,QChar('0')).arg(minute,2,10,QChar('0')).
                arg(second,2,10,QChar('0')).arg(frame,2,10,QChar('0'));
    } while(0);

    return strTime;
}

int CEffectHorizonHeader::getScaleFactor() const
{
    if (m_factor<1 || m_factor>20) return  0;
    return scaleFactor[m_factor-1];
}
