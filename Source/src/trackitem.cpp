#include "trackitem.h"
#include "QtLib/box.h"
#include "QtLib/ImgButton.h"
#include <qlabel.h>
#include <qpainter.h>
#include <QMouseEvent>

CTrackItem::CTrackItem(TRACK_TYPE type, QString strImg, QString strText, QWidget *parent)
    :QWidget(parent)
    ,m_clrBg(QColor(0,187,244))
    ,m_strImg(strImg)
    ,m_strText(strText)
    ,m_type(type)
    ,m_bSelected(false)
    ,m_bIsPressed(false)
    ,m_cursorTrim(QPixmap(":/res/clip_trim_mouse.png"))
    ,m_pLabTrimLeft(nullptr)
    ,m_pLabTrimRight(nullptr)
{
    Layout();
}

bool CTrackItem::IsSelect()
{
    return m_bSelected;
}

void CTrackItem::SetSelect(bool bSelect)
{
    m_bSelected = bSelect;
    ResetTrims();
    update();
}

void CTrackItem::SetBg(QColor clrBg)
{
    m_clrBg = clrBg;
    update();
}

void CTrackItem::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, true);

    paint.setPen(Qt::NoPen);
    if (m_bSelected){
        paint.setPen(QColor(253, 172, 4));
    }
    paint.setBrush(m_clrBg);

    QRect rtDraw = rect();
    rtDraw.adjust(0,0,-1,-1);
    paint.drawRoundedRect(rtDraw, 4.0, 4.0);
}

void CTrackItem::mousePressEvent(QMouseEvent *pEvent)
{
    m_bSelected = true;
    m_bIsPressed = true;
    m_ptRelative = pEvent->pos();
    ResetTrims();

    QWidget::mousePressEvent(pEvent);
}

void CTrackItem::mouseReleaseEvent(QMouseEvent *pEvent)
{
    m_bIsPressed = false;
    QWidget::mouseReleaseEvent(pEvent);
}

void CTrackItem::mouseMoveEvent(QMouseEvent *pEvent)
{
    do
    {
        if (m_bIsPressed)
        {
            QPoint ptPos = pos();
            int xOffset = (pEvent->pos()-m_ptRelative).x();
            move(ptPos.x()+xOffset,ptPos.y());
        }
    } while(0);

    QWidget::mouseMoveEvent(pEvent);
}

bool CTrackItem::eventFilter(QObject *pWatched, QEvent *pEvent)
{
    bool bHandled = false;
    do
    {
        if (nullptr==pWatched || nullptr==pEvent) break;

        if (pWatched==m_pLabTrimLeft || pWatched==m_pLabTrimRight)
        {
            QEvent::Type type = pEvent->type();
            if (type==QEvent::MouseButtonPress || type==QEvent::MouseButtonRelease)
            {
                bHandled = true;
            }
        }
    } while(0);

    return  bHandled;
}

void CTrackItem::Layout()
{
    do
    {
        setAttribute(Qt::WA_TranslucentBackground);
        setFixedHeight(ITEM_HEIGHT);
        setFixedWidth(300);

        QHBoxLayout* pHboxMain = new QHBoxLayout;
        if (nullptr == pHboxMain) break;
        pHboxMain->setSpacing(5);
        pHboxMain->setContentsMargins(5,2,0,2);
        setLayout(pHboxMain);

        CImagButton* pImg = new CImagButton(m_strImg);
        if (nullptr == pImg) break;
        pHboxMain->addWidget(pImg);

        QLabel* pLabText = new QLabel(m_strText);
        if (nullptr == pLabText) break;
        pHboxMain->addWidget(pLabText);

        QSpacerItem* pHItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred);
        if (nullptr == pHItem) break;
        pHboxMain->addSpacerItem(pHItem);
        setCursor(QCursor(Qt::OpenHandCursor));

        InitTrims();
    } while(0);
}

void CTrackItem::InitTrims()
{
    m_pLabTrimLeft = Init1Trim(":/res/trim_left.png");
    m_pLabTrimRight = Init1Trim(":/res/trim_right.png");
}

QLabel *CTrackItem::Init1Trim(QString strPm)
{
    QLabel* pLabTrim = nullptr;
    do
    {
        pLabTrim = new QLabel(this);
        if (nullptr == pLabTrim) break;
        pLabTrim->installEventFilter(this);
        QPixmap pm(strPm);
        pLabTrim->setPixmap(pm);
        pLabTrim->setFixedSize(pm.size());
        pLabTrim->setCursor(m_cursorTrim);
        pLabTrim->setVisible(false);
    } while(0);

    return pLabTrim;
}

void CTrackItem::ResetTrims()
{
    do
    {
        if (nullptr==m_pLabTrimLeft || nullptr==m_pLabTrimRight) break;

        m_pLabTrimLeft->setVisible(m_bSelected);
        m_pLabTrimRight->setVisible(m_bSelected);
        m_pLabTrimLeft->move(0,1);
        m_pLabTrimRight->move(width() - m_pLabTrimRight->width(), 1);
    } while(0);
}
