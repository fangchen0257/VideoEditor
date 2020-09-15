#include "trackitem.h"
#include "QtLib/box.h"
#include "QtLib/ImgButton.h"
#include <qlabel.h>
#include <qpainter.h>

CTrackItem::CTrackItem(TRACK_TYPE type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo, QWidget *parent)
    :QWidget(parent)
    ,m_clipInfo(clipInfo)
    ,m_imgThumb(imgThumb)
    ,m_clrBg(QColor(0,187,244))
    ,m_strText(strText)
    ,m_type(type)
    ,m_pLabText(nullptr)
    ,m_pLabThumb(nullptr)
    ,m_bSelected(false)
    ,m_bIsPressed(false)
    ,m_bIsLeftTrimPressed(false)
    ,m_bIsRightTrimPressed(false)
    ,m_cursorTrim(QPixmap(":/res/clip_trim_mouse.png"))
    ,m_pLabTrimLeft(nullptr)
    ,m_pLabTrimRight(nullptr)
    ,m_pItemShadow(nullptr)
{
    if (nullptr != m_clipInfo)
    {
        m_frameLength = m_clipInfo->length;
        m_curFrameCount = m_clipInfo->frame_count;
        m_position = m_clipInfo->start;
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

    if (m_bSelected)
    {
        setFocus();
    }
}

void CTrackItem::SetBg(QColor clrBg)
{
    m_clrBg = clrBg;
    update();
}

void CTrackItem::SetShadowItem(CItemShadow *pItemShadow)
{
    m_pItemShadow = pItemShadow;
    stackUnder(m_pItemShadow);
}

void CTrackItem::ResetPixelPerFrame(double pixelPerFrm)
{
    m_pixelPerFrm = pixelPerFrm;
    ResetItem(pixelPerFrm);
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
    m_bIsPressed = true;
    m_ptRelative = pEvent->pos();
    m_wndPos = pos();
    emit sigItemSelect(this);

    QWidget::mousePressEvent(pEvent);
}

void CTrackItem::mouseReleaseEvent(QMouseEvent *pEvent)
{
    m_bIsPressed = false;
    if (nullptr != m_pItemShadow) {
        m_pItemShadow->setVisible(false);
    }
    QPoint ptCurrent = pos();
    if (ptCurrent != m_wndPos)
    {
        OnItemMoveFinish((ptCurrent-m_wndPos).x());
    }
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
            if (x >= 0)
            {
                move(x,y);
                if (nullptr != m_pItemShadow)
                {
                    m_pItemShadow->setVisible(true);
                    m_pItemShadow->move(x,y);
                    m_pItemShadow->setFixedSize(size());
                }
            }
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

void CTrackItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        DeleteItem();
    }
    return QWidget::keyPressEvent(event);
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

        QPixmap pmThumb = QPixmap::fromImage(m_imgThumb);
        m_pLabThumb = new QLabel;
        if (nullptr == m_pLabThumb) break;
        m_pLabThumb->setPixmap(pmThumb);
        m_pLabThumb->setFixedSize(pmThumb.size());
        pHboxMain->addWidget(m_pLabThumb);

        m_pLabText = new QLabel(m_strText);
        if (nullptr == m_pLabText) break;
        m_pLabText->setStyleSheet("QLabel {color:white;font-size:12px;}");
        pHboxMain->addWidget(m_pLabText);

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
        m_pLabTrimLeft->move(0, 0);
        m_pLabTrimRight->move(width()-m_pLabTrimRight->width(), 0);
    } while(0);
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

void CTrackItem::ResetThumb()
{
    if (nullptr != m_pLabThumb)
    {
        QPixmap pm = QPixmap::fromImage(m_imgThumb);
        m_pLabThumb->setPixmap(pm);
        m_pLabThumb->setFixedSize(pm.size());
    }
}

void CTrackItem::ResetGeometry(double pixelPerFrame)
{
    int width = static_cast<int>(m_curFrameCount*pixelPerFrame);
    move(m_position*pixelPerFrame, 5);
    setFixedWidth(width);
}

void CTrackItem::ResetItem(double pixelPerFrame)
{
    ResetGeometry(pixelPerFrame);
    ResetTrims();
    ResetMediaText();
    ResetThumb();
    update();
}

shared_ptr<Mlt::ClipInfo> CTrackItem::GetClipInfo()
{
    return m_clipInfo;
}

TRACK_TYPE CTrackItem::type()
{
    return m_type;
}

void CTrackItem::ResetItem(shared_ptr<Mlt::ClipInfo> clipInfo, QImage imgThumb)
{
    m_imgThumb = imgThumb;
    m_clipInfo = clipInfo;
    if (nullptr != m_clipInfo)
    {
        m_curFrameCount = m_clipInfo->frame_count;
    }

    ResetItem(m_pixelPerFrm);
}

void CTrackItem::DeleteItem()
{
    emit sigItemDelete(this);
}

QRect CTrackItem::calcGeometry(QPoint pt, bool bRightTrim)
{
    QRect rtGeometry;

    QPoint ptGlobal = mapToParent(QPoint(0,0));
    rtGeometry.setTopLeft(ptGlobal);
    if (bRightTrim)
    {
        rtGeometry.setSize(QSize(width()+pt.x(),height()));
    }
    else
    {
        rtGeometry.setSize(QSize(width()-pt.x(),height()));
    }

    return rtGeometry;
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
                OnLeftTrimMouseRelease();
            }
            else
            {
                if (m_bIsLeftTrimPressed)
                {
                    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                    QPoint pt = pMouseEvent->pos();
                    QRect rtGeometry = calcGeometry(pt, false);

                    m_pItemShadow->setVisible(true);
                    m_pItemShadow->move(rtGeometry.topLeft());
                    m_pItemShadow->setFixedSize(rtGeometry.size());
                }
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
        if (nullptr==pEvent || nullptr==m_pItemShadow) break;

        QEvent::Type type = pEvent->type();
        if (type==QEvent::MouseButtonPress || type==QEvent::MouseButtonRelease || type==QEvent::MouseMove)
        {
            if (type == QEvent::MouseButtonPress)
            {
                m_bIsRightTrimPressed = true;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
               OnRightTrimMouseRelease();
            }
            else
            {
                if (m_bIsRightTrimPressed)
                {
                    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
                    QPoint pt = pMouseEvent->pos();
                    QRect rtGeometry = calcGeometry(pt, true);

                    m_pItemShadow->setVisible(true);
                    m_pItemShadow->move(rtGeometry.topLeft());
                    m_pItemShadow->setFixedSize(rtGeometry.size());
                }
            }
            bHandled = true;
        }
    } while(0);

    return bHandled;
}

void CTrackItem::OnLeftTrimMouseRelease()
{
    do
    {
        m_bIsLeftTrimPressed = false;
        if (nullptr != m_pItemShadow)
        {
            m_pItemShadow->setVisible(false);
        }

        int currentW = m_pItemShadow->width();
        if (0 == currentW)
        {
            DeleteItem();
            break;
        }

        int frame = static_cast<int>(currentW/m_pixelPerFrm);
        if (frame >= m_frameLength)
        {
            frame = m_frameLength;
        }

        int in = m_curFrameCount - frame;
        emit sigClipTrim(this, in, 0);
    } while(0);
}

void CTrackItem::OnRightTrimMouseRelease()
{
    do
    {
        m_bIsRightTrimPressed = false;
        if (nullptr != m_pItemShadow)
        {
            m_pItemShadow->setVisible(false);
        }

        int currentW = m_pItemShadow->width();
        if (0 == currentW)
        {
            DeleteItem();
            break;
        }

        int frame = static_cast<int>(currentW/m_pixelPerFrm);
        if (frame >= m_frameLength)
        {
            frame = m_frameLength;
        }

        int out = m_curFrameCount - frame;
        emit sigClipTrim(this, 0, out);
    } while(0);
}

void CTrackItem::OnItemMoveFinish(int xOffset)
{
    if (nullptr==m_clipInfo || 0.0==m_pixelPerFrm) return;

    int clipIndex = m_clipInfo->clip;
    int position = static_cast<int>(m_position+xOffset/m_pixelPerFrm);
    qDebug() << "OnItemMoveFinish" << clipIndex << position << xOffset << m_pixelPerFrm;

    emit sigMoveClip(this, clipIndex, position);
}

CItemShadow::CItemShadow(QColor clgBg, QWidget *parent)
    :QWidget(parent, Qt::FramelessWindowHint)
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
    paint.drawRect(rect());
}
