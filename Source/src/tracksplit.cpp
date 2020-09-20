#include "tracksplit.h"
#include <qpainter.h>
#include <qevent.h>

#define HEADER_HEIGHT 40
CTrackSplit::CTrackSplit(QWidget *parent)
    :QWidget(parent)
    ,m_pBtnSplit(nullptr)
    ,m_pLabMark(nullptr)
    ,m_bIsPressed(false)
    ,m_InitializePosX(0)
{
    Layout();
}

void CTrackSplit::SetPosX(int posX)
{
    m_InitializePosX = posX;
}

int CTrackSplit::InitializePos()
{
    return m_InitializePosX;
}

void CTrackSplit::Layout()
{
    do
    {
        m_pLabMark = new QLabel(this);
        if (nullptr == m_pLabMark) break;
        QPixmap pm(":/res/red.png");
        m_pLabMark->setPixmap(pm);
        m_pLabMark->installEventFilter(this);
        setFixedSize(pm.size());

        QVector<QString> vecRes;
        vecRes<<":/res/trim.png";
        vecRes<<":/res/trim_hover.png";
        vecRes<<":/res/trim_click.png";
        vecRes<<":/res/trim_disable.png";
        m_pBtnSplit = new CImagButton(vecRes, -1, this);
        if (nullptr == m_pBtnSplit) break;
        connect(m_pBtnSplit, SIGNAL(sigClick(int)), this, SLOT(slotSpliterBtnClicked(int)));
        setFixedWidth(SPLITER_FIXED_WIDTH);
    } while(0);
}

void CTrackSplit::InitRegionMask()
{
    do
    {
        if (nullptr==m_pBtnSplit || nullptr==m_pLabMark) break;

        QRect rtLabMark = m_pLabMark->geometry();
        QRect rt = rect();
        QRegion region(rtLabMark.left(),rtLabMark.top(),rtLabMark.width(),HEADER_HEIGHT);

        int xOffset = 2;
        region += QRegion(rt.width()/2-xOffset,HEADER_HEIGHT,xOffset*2,rt.bottom());
        region += QRegion(m_pBtnSplit->geometry());
        setMask(region);
    } while(0);
}

bool CTrackSplit::eventFilter(QObject *pWatched, QEvent *pEvent)
{
    do
    {
        if (nullptr==pEvent || nullptr==pWatched) break;

        if (pWatched == m_pLabMark)
        {
            int type = pEvent->type();
            if (QEvent::MouseButtonPress == type)
            {
                m_bIsPressed = true;
                QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                if (nullptr != pMouseEvent)
                {
                    m_ptMousePressed = pMouseEvent->pos();
                }
            }
            else if (QEvent::MouseButtonRelease == type)
            {
                m_bIsPressed = false;
            }
            else if (QEvent::MouseMove == type)
            {
                if (m_bIsPressed)
                {
                    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                    if (nullptr == pMouseEvent) break;

                    QPoint pt = pMouseEvent->pos();
                    QPoint ptCur = pos();

                    int x = ptCur.x()+((pt-m_ptMousePressed).x());
                    int y = ptCur.y();
                    if (x >= m_InitializePosX)
                    {
                        sigSpliterMove(x, y);
                    }
                }
            }
        }
    } while(0);

    return false;
}

void CTrackSplit::showEvent(QShowEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QSize sz = size();
    if (nullptr != m_pBtnSplit)
    {
        m_pBtnSplit->move(sz.width()-m_pBtnSplit->width(),sz.height()/2);
    }

    if (nullptr != m_pLabMark)
    {
        m_pLabMark->move((sz.width()-m_pLabMark->width())/2, 0);
    }
    InitRegionMask();
}

void CTrackSplit::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter paint(this);
    paint.setPen(QColor(239, 80, 80));

    QRect rt = rect();
    paint.drawLine(QPoint(rt.width()/2,0), QPoint(rt.width()/2,rt.height()));
}

void CTrackSplit::slotSpliterBtnClicked(int btnId)
{
    Q_UNUSED(btnId);
    emit sigSpliterBtnClicked(pos().x());
}

void CTrackSplit::slotSplit(int position)
{

}
