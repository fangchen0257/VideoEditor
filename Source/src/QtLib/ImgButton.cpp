#include "ImgButton.h"
#include <qpainter.h>
#include <qpoint.h>
#include <QMouseEvent>

CImagButton::CImagButton(QVector<QString> vecRes, int btnId, QWidget* parent)
    :QPushButton(parent)
{
    init(vecRes, btnId);
}

CImagButton::CImagButton(QString strRes, int btnId)
{
    QVector<QString> vecRes;
    for (int i=0; i<BTN_CNT; ++i)
    {
        vecRes.push_back(strRes);
    }
    init(vecRes, btnId);
}

void CImagButton::SetImgRes(QVector<QString> vecRes)
{
    m_vecImgRes = vecRes;
    update();
}

void CImagButton::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter paint(this);
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::NoBrush);

    if (!isEnabled()) {
        m_btnStatus = BTN_DISABLE;
    }

    if (m_vecImgRes.size() == BTN_CNT)
    {
        QPixmap pm(m_vecImgRes[m_btnStatus]);
        paint.drawPixmap(0,0,pm);
    }
}

void CImagButton::enterEvent(QEvent *pEvent)
{
    m_btnStatus = BTN_HOVER;
    update();

    QPushButton::enterEvent(pEvent);
}

void CImagButton::leaveEvent(QEvent *pEvent)
{
    m_btnStatus = BTN_NORMAL;
    update();

    QPushButton::leaveEvent(pEvent);
}

void CImagButton::mousePressEvent(QMouseEvent *pEvent)
{
    m_btnStatus = BTN_PRESS;
    update();

    QPushButton::mousePressEvent(pEvent);
}

void CImagButton::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if (ptInRect(QPoint(pEvent->x(),pEvent->y()), rect()))
    {
        emit sigClick(m_bindId);
        return;
    }

    return QPushButton::mouseReleaseEvent(pEvent);
}

void CImagButton::init(QVector<QString> vecRes, int btnId)
{
    m_bindId = btnId;
    m_btnStatus = BTN_NORMAL;
    if (vecRes.size() == BTN_CNT)
    {
        m_vecImgRes = vecRes;
        QPixmap pm(m_vecImgRes[BTN_NORMAL]);
        setFixedSize(pm.size());
    }
}

bool CImagButton::ptInRect(QPoint pt, QRect rect)
{
    return rect.contains(pt);
}
