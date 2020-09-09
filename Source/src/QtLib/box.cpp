#include "box.h"
#include <qpainter.h>

CBox::CBox(bool bIsVbox)
{
    m_bDrawFrm = false;

    if (bIsVbox)
    {
        m_pBox = new QVBoxLayout;
    }
    else
    {
        m_pBox = new QHBoxLayout;
    }

    if (nullptr != m_pBox)
    {
        m_pBox->setContentsMargins(0,0,0,0);
        m_pBox->setSpacing(0);
        setLayout(m_pBox);
    }
}

void CBox::PackStart(QWidget *pWidget, bool bExpand, int padding, Qt::Alignment align)
{
    do
    {
        if (nullptr==pWidget || nullptr==m_pBox) break;

        if (bExpand)
        {
            pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }

        m_pBox->addWidget(pWidget, 0, align);
        m_pBox->addSpacing(padding);
    } while(0);
}

void CBox::PackStart(QLayout *pLayout, int padding, int stretch)
{
    do
    {
        if (nullptr==pLayout || nullptr==m_pBox) break;

        m_pBox->addLayout(pLayout, stretch);
        m_pBox->addSpacing(padding);
    } while(0);
}

void CBox::AddSpacerItem(bool bHorizonal)
{
    QSizePolicy::Policy hPolicy = QSizePolicy::Expanding;
    QSizePolicy::Policy vPolicy = QSizePolicy::Preferred;
    if (!bHorizonal)
    {
        hPolicy = QSizePolicy::Preferred;
        vPolicy = QSizePolicy::Expanding;
    }

    QSpacerItem* pItem = new QSpacerItem(0,0,hPolicy,vPolicy);
    if (nullptr!=pItem && nullptr!=m_pBox)
    {
        m_pBox->addSpacerItem(pItem);
    }
}

void CBox::SetMargins(int l, int t, int r, int b)
{
    if (nullptr != m_pBox)
    {
        m_pBox->setContentsMargins(l,t,r,b);
    }
}

void CBox::SetSpacing(int spacing)
{
    if (nullptr != m_pBox)
    {
        m_pBox->setSpacing(spacing);
    }
}

void CBox::AddSpacing(int spacing)
{
    if (nullptr != m_pBox)
    {
        m_pBox->addSpacing(spacing);
    }
}

void CBox::SetDrawFrame(bool bDrawFrame)
{
    m_bDrawFrm = bDrawFrame;
    update();
}

void CBox::paintEvent(QPaintEvent *pEvent)
{
    if (!m_bDrawFrm){
        return QWidget::paintEvent(pEvent);
    }

    QPainter paint(this);
    paint.setPen(Qt::red);
    paint.setBrush(Qt::NoBrush);

    QRect rtDraw = rect();
    rtDraw.adjust(0,0,-1,-1);
    paint.drawRect(rtDraw);
}
