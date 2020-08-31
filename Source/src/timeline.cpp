#include "timeline.h"
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>

#define MINIZIM_HEIGHT 150
CTimeLine::CTimeLine(QWidget *parent)
    :QWidget(parent)
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

        QTableWidget* pTableWidget = new QTableWidget(3,3);
        if (nullptr == pTableWidget) break;
        pVboxMain->addWidget(pTableWidget);
    } while(0);
}
