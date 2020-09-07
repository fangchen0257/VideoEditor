#include "timeline.h"
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>

#define MINIZIM_HEIGHT 150
CTimeLine::CTimeLine(QWidget *parent)
    :QWidget(parent)
    ,m_pEffectView(nullptr)
{
    Layout();
}

void CTimeLine::Layout()
{
    do
    {
        QVBoxLayout* pVboxMain = new QVBoxLayout;
        if (nullptr == pVboxMain) break;
        pVboxMain->setContentsMargins(0, 0, 0, 0);
        pVboxMain->setSpacing(0);
        setLayout(pVboxMain);
        setMinimumHeight(MINIZIM_HEIGHT);

        QQuickWidget* pOperWidget = new QQuickWidget;
        if (nullptr == pOperWidget) break;
        pOperWidget->setFixedHeight(45);
        pOperWidget->setSource((QUrl("qrc:/qmls/timeline/TrackHeader.qml")));
        pOperWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pVboxMain->addWidget(pOperWidget);

        m_pEffectView = new CEffectView(ROW_CNT-1,COL_CNT);
        if (nullptr == m_pEffectView) break;
        pVboxMain->addWidget(m_pEffectView);
    } while(0);
}
