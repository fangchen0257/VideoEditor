#include "winfrmtitle.h"
#include <QHBoxLayout>
#include <qlabel.h>
#include <QSpacerItem>
#include <QMouseEvent>
#include <qpainter.h>

#define TITLE_HEIGHT 40
WinFrmTitle::WinFrmTitle(QString strTitle, QWidget *parent)
{
    m_pbtnMax = nullptr;
    m_strTitle = strTitle;
    m_pMainWin = parent;
    m_bPressed = false;
    m_bIsMaximized = false;
    Layout();
}

void WinFrmTitle::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter paint(this);
    paint.setPen(Qt::NoPen);
    paint.fillRect(rect(), QBrush(QColor(33,39,53)));
}

void WinFrmTitle::mousePressEvent(QMouseEvent *pEvent)
{
    m_ptPress = pEvent->pos();
    m_bPressed = true;
}

void WinFrmTitle::mouseReleaseEvent(QMouseEvent *pEvent)
{
    Q_UNUSED(pEvent);
    m_bPressed = false;
}

void WinFrmTitle::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    Q_UNUSED(pEvent);
    slotBtnClick(TITLE_BTN_MAX);
}

void WinFrmTitle::mouseMoveEvent(QMouseEvent *pEvent)
{
    if (nullptr == pEvent || Qt::LeftButton != pEvent->buttons() || !m_bPressed)
            return;

    if (nullptr != m_pMainWin)
    {
        QPoint ptParent = m_pMainWin->pos();
        ptParent.setX(ptParent.x() + pEvent->x() - m_ptPress.x());
        ptParent.setY(ptParent.y() + pEvent->y() - m_ptPress.y());
        m_pMainWin->move(ptParent);
    }
}

void WinFrmTitle::Layout()
{
    do
    {
        QHBoxLayout* pHboxMain = new QHBoxLayout;
        if (nullptr == pHboxMain) break;
        pHboxMain->setContentsMargins(20, 0, 20, 0);
        pHboxMain->setSpacing(12);
        setLayout(pHboxMain);
        setFixedHeight(TITLE_HEIGHT);

        QLabel* pTitle = new QLabel(m_strTitle);
        if (nullptr == pTitle) break;
        pTitle->setStyleSheet("QLabel{color:#ffffff; font:18px; font-family:Segoe UI Semibold;font-weight:bold;}");
        pHboxMain->addWidget(pTitle);

        QSpacerItem* pHitem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred);
        if (nullptr == pHitem) break;
        pHboxMain->addSpacerItem(pHitem);

        QVector<QString> vecRes;
        vecRes.push_back(":/res/min_normal.png");
        vecRes.push_back(":/res/min_hover.png");
        vecRes.push_back(":/res/min_pressed.png");
        vecRes.push_back(":/res/min_normal.png");
        CImagButton* pBtnMin = CreateImgButton(vecRes, TITLE_BTN_MIN);
        if (nullptr == pBtnMin) break;
        pHboxMain->addWidget(pBtnMin);

        vecRes.clear();
        vecRes.push_back(":/res/normax_normal.png");
        vecRes.push_back(":/res/normax_hover.png");
        vecRes.push_back(":/res/normax_pressed.png");
        vecRes.push_back(":/res/normax_normal.png");
        m_pbtnMax = CreateImgButton(vecRes, TITLE_BTN_MAX);
        if (nullptr == m_pbtnMax) break;
        pHboxMain->addWidget(m_pbtnMax);

        vecRes.clear();
        vecRes.push_back(":/res/close_normal.png");
        vecRes.push_back(":/res/close_hover.png");
        vecRes.push_back(":/res/close_pressed.png");
        vecRes.push_back(":/res/close_normal.png");
        CImagButton* pBtnClose = CreateImgButton(vecRes, TITLE_BTN_CLOSE);
        if (nullptr == pBtnClose) break;
        pHboxMain->addWidget(pBtnClose);
    } while(0);
}

CImagButton *WinFrmTitle::CreateImgButton(QVector<QString> vecRes, int btnId)
{
    CImagButton* pImgBtn = new CImagButton(vecRes, btnId);
    if (nullptr != pImgBtn)
    {
        connect(pImgBtn, SIGNAL(sigClick(int)), this, SLOT(slotBtnClick(int)));
    }
    return pImgBtn;
}

void WinFrmTitle::ResetMaxBtnImg(bool bIsMaximized)
{
    if (nullptr == m_pbtnMax) return;

    QVector<QString> vecRes;
    if (bIsMaximized)
    {
        vecRes.push_back(":/res/max_normal.png");
        vecRes.push_back(":/res/max_hover.png");
        vecRes.push_back(":/res/max_pressed.png");
        vecRes.push_back(":/res/max_normal.png");
    }
    else
    {
        vecRes.push_back(":/res/normax_normal.png");
        vecRes.push_back(":/res/normax_hover.png");
        vecRes.push_back(":/res/normax_pressed.png");
        vecRes.push_back(":/res/normax_normal.png");
    }
    m_pbtnMax->SetImgRes(vecRes);
}

void WinFrmTitle::slotBtnClick(int btnId)
{
    if (nullptr == m_pMainWin) return;

    switch (btnId)
    {
    case TITLE_BTN_MIN:
        m_pMainWin->showMinimized();
        break;
    case TITLE_BTN_MAX:
        {
            if (m_bIsMaximized)
            {
                m_pMainWin->showNormal();
            }
            else
            {
                m_pMainWin->showMaximized();
            }
            m_bIsMaximized = !m_bIsMaximized;
            ResetMaxBtnImg(m_bIsMaximized);
        }
        break;
    case TITLE_BTN_CLOSE:
        m_pMainWin->close();
        break;
    }
}
