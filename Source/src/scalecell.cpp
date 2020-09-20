#include "scalecell.h"

#define SCALE_UNIT 10       //最小刻度对应的像素宽度
#define UNIT_FRAME 3        //最小刻度对应的帧数
#define LARGE_SCALE 12      //大刻度像素高度
#define SMALL_SCALE 8       //小刻度像素高度
#define FRAME_PER_SEC 30    //fps

const int scaleFactor[20] = {300,270,180,120,80,60,40,30,25,20,18,16,14,12,10,8,6,3,2,1};
CScaleCell::CScaleCell(QWidget *parent)
    :QWidget(parent)
    ,m_factor(10)
    ,m_scaleCount(0)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void CScaleCell::setScaleFactor(int factor)
{
    m_factor = factor;
    update();
}

double CScaleCell::getPixelPerFrame()
{
    double fPixelPerFrame = 0.0;
    do
    {
        int frameCount = m_scaleCount*UNIT_FRAME*getScaleFactor();
        if (0 == frameCount) break;

        fPixelPerFrame = (width()/(frameCount*1.0));
    } while(0);

    return fPixelPerFrame;
}

void CScaleCell::paintEvent(QPaintEvent *pEvent)
{
    do
    {
        Q_UNUSED(pEvent);

        QPainter paint(this);

        QRect rt = rect();
        paint.fillRect(rt, QBrush(QColor(33,39,53)));

        //draw bottom line
        paint.setPen(QColor(44,63,85));
        paint.drawLine(rt.bottomLeft(), rt.bottomRight());

        //draw scale
        paint.setPen(QColor(53,83,111));
        int h = rt.height();
        int scale = 0;
        for (int x=rt.left(); x<=rt.right(); x+=SCALE_UNIT, scale++)
        {
            bool bDrawLargeScale = !(scale%10);
            int yOffset = bDrawLargeScale?LARGE_SCALE:SMALL_SCALE;
            QPoint pt1 = QPoint(x, (h-yOffset));
            QPoint pt2 = QPoint(x, h);
            paint.drawLine(pt1, pt2);
            if (bDrawLargeScale)
            {
                QString strTime = calcTimeStr(scale);

                QFont textFont("Segoe UI");
                textFont.setPixelSize(10);
                QFontMetrics fm(textFont);

                int textW = fm.width(strTime);
                int textH = fm.height();
                QRect rtText(pt1.x(),pt1.y()-textH,textW,textH);
                paint.setFont(textFont);
                paint.drawText(rtText, strTime);
            }
        }
        m_scaleCount = scale;
    } while(0);
}

void CScaleCell::mouseReleaseEvent(QMouseEvent *pEvent)
{
    emit sigScaleClick();
    return QWidget::mouseReleaseEvent(pEvent);
}

#define HOUR_FRAME      (30*60*60)
#define MINUTE_FRAME    (30*60)
#define SECOND_FRAME    30
QString CScaleCell::calcTimeStr(int scale) const
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

int CScaleCell::getScaleFactor() const
{
    if (m_factor<1 || m_factor>20) return  0;
    return scaleFactor[m_factor-1];
}
