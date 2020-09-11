#include "trackitem.h"
#include "QtLib/box.h"
#include "QtLib/ImgButton.h"
#include <qlabel.h>
#include <qpainter.h>

#define IMG_THUMB_W 55
#define IMG_THUMB_H 30
CTrackItem::CTrackItem(TRACK_TYPE type, QString strText, shared_ptr<CEUProducer> pProducer, QWidget *parent)
    :QWidget(parent)
    ,m_pProducer(pProducer)
    ,m_clrBg(QColor(0,187,244))
    ,m_strText(strText)
    ,m_type(type)
    ,m_pLabText(nullptr)
    ,m_bSelected(false)
    ,m_bIsPressed(false)
    ,m_bIsLeftTrimPressed(false)
    ,m_bIsRightTrimPressed(false)
    ,m_cursorTrim(QPixmap(":/res/clip_trim_mouse.png"))
    ,m_pLabTrimLeft(nullptr)
    ,m_pLabTrimRight(nullptr)
    ,m_pItemShadow(nullptr)
{
    if (nullptr != m_pProducer)
    {
        m_frmWidth = m_pProducer->producer()->get_playtime();
    }
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

void CTrackItem::ResetPixelPerFrame(double pixelPerFrm)
{
    m_pixelPerFrm = pixelPerFrm;
    int width = static_cast<int>(m_frmWidth*pixelPerFrm);
    setFixedWidth(width);
    ResetTrims();
    ResetMediaText();
    if (nullptr != m_pItemShadow) {
        m_pItemShadow->setFixedSize(size());
    }

    update();
}

void CTrackItem::showEvent(QShowEvent *pEvent)
{
    Q_UNUSED(pEvent);
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
    emit sigItemSelect(this);

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

            int x = ptPos.x()+xOffset;
            int y = ptPos.y();
            qDebug() << "x:" << x << "y:" << y;
            if (x >= 0){ move(x,y); }
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

        if (pWatched == m_pLabTrimLeft)
        {
            bHandled = HandleLeftTrimEvent(pEvent);
        }
        else if (pWatched == m_pLabTrimRight)
        {
            bHandled = HandleRightTrimEvent(pEvent);
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

        QHBoxLayout* pHboxMain = new QHBoxLayout;
        if (nullptr == pHboxMain) break;
        pHboxMain->setSpacing(5);
        pHboxMain->setContentsMargins(5,2,0,2);
        setLayout(pHboxMain);

        QPixmap pmThumb = QPixmap::fromImage(clipImage());
        QLabel* pLabThumb = new QLabel;
        if (nullptr == pLabThumb) break;
        pLabThumb->setPixmap(pmThumb);
        pLabThumb->setFixedSize(pmThumb.size());
        pHboxMain->addWidget(pLabThumb);

        m_pLabText = new QLabel(m_strText);
        if (nullptr == m_pLabText) break;
        m_pLabText->setStyleSheet("QLabel {color:white;font-size:12px;}");
        pHboxMain->addWidget(m_pLabText);

        QSpacerItem* pHItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred);
        if (nullptr == pHItem) break;
        pHboxMain->addSpacerItem(pHItem);
        setCursor(QCursor(Qt::OpenHandCursor));

        InitTrims();
        InitShadowItem();
    } while(0);
}

void CTrackItem::InitTrims()
{
    m_pLabTrimLeft = Init1Trim(":/res/trim_left.png");
    m_pLabTrimRight = Init1Trim(":/res/trim_right.png");
}

void CTrackItem::InitShadowItem()
{
    m_pItemShadow = new CItemShadow(QColor(0,0,0,127), this);
    if (nullptr != m_pItemShadow)
    {
        m_pItemShadow->setVisible(false);
    }
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

QImage CTrackItem::clipImage()
{
    if (nullptr == m_pProducer) return QImage();
    QImage imgThumb = m_pProducer->image(IMG_THUMB_W,IMG_THUMB_H);
    return imgThumb;
}

void CTrackItem::ResetMediaText()
{
    if (nullptr == m_pLabText) return;

    int xOffset = 10;
    int itemWidth = width();

    QFont font("Segoe UI");font.setPixelSize(12);
    QFontMetrics fm(font);
    QString strText = fm.elidedText(m_strText, Qt::ElideRight, itemWidth-IMG_THUMB_W-xOffset);
    m_pLabText->setText(strText);
}

bool CTrackItem::HandleLeftTrimEvent(QEvent *pEvent)
{
    bool bHandled = false;
    do
    {
        if (nullptr == pEvent) break;

        QEvent::Type type = pEvent->type();
        if (type==QEvent::MouseButtonPress || type==QEvent::MouseButtonRelease || type==QEvent::MouseMove)
        {
            if (type == QEvent::MouseButtonPress)
            {
                m_bIsLeftTrimPressed = true;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                m_bIsLeftTrimPressed = false;
            }
            else
            {
                QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                QPoint pt = pMouseEvent->pos();
                qDebug() << "x:" << pt.x() << "y:" << pt.y();
            }
            bHandled = true;
        }
    } while(0);

    return bHandled;
}

bool CTrackItem::HandleRightTrimEvent(QEvent *pEvent)
{
    bool bHandled = false;
    do
    {
        if (nullptr == pEvent) break;

        QEvent::Type type = pEvent->type();
        if (type==QEvent::MouseButtonPress || type==QEvent::MouseButtonRelease || type==QEvent::MouseMove)
        {
            if (type == QEvent::MouseButtonPress)
            {
                m_bIsRightTrimPressed = true;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                if (nullptr != m_pItemShadow) {
                    m_pItemShadow->setVisible(false);
                }
                emit sigClipTrim(0,1000);
                m_bIsRightTrimPressed = false;
            }
            else
            {
                QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                QPoint pt = pMouseEvent->pos();
                if (nullptr != m_pItemShadow)
                {
                    m_pItemShadow->setVisible(true);
                    m_pItemShadow->setFixedWidth(width()+pt.x());
                }
            }
            bHandled = true;
        }
    } while(0);

    return bHandled;
}


CItemShadow::CItemShadow(QColor clgBg, QWidget *parent)
    :QWidget(parent)
    ,m_clgBg(clgBg)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void CItemShadow::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.setPen(Qt::NoPen);
    paint.setBrush(m_clgBg);
    paint.drawRoundedRect(rect(), 4.0, 4.0);
}
