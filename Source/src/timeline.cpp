#include "timeline.h"
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>
#include "qmltypesregister.h"

#define MINIZIM_HEIGHT 150
CTimeLine::CTimeLine(GlobalUtinityObject* pGlobalObject, QWidget *parent)
    :QWidget(parent)
    ,m_pEffectView(nullptr)
    ,m_pGlobalObject(pGlobalObject)
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
        QQmlContext* pContext = pOperWidget->rootContext();
        QmlTypesRegister::instance().setGlobalCariable(pContext);
        pOperWidget->setFixedHeight(45);
        pOperWidget->setSource((QUrl("qrc:/qmls/timeline/TrackHeader.qml")));
        pOperWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pVboxMain->addWidget(pOperWidget);

        m_pEffectView = new CEffectView(ROW_CNT-1,COL_CNT);
        if (nullptr == m_pEffectView) break;
        connect(m_pGlobalObject, SIGNAL(sigAddMedia2Track(int,const QVariant&)), m_pEffectView, SLOT(slotAddMedia2Track(int,const QVariant&)));
        connect(m_pGlobalObject, SIGNAL(sigScaleSliderValueChanged(int)), m_pEffectView, SLOT(slotScaleValueChanged(int)));
        pVboxMain->addWidget(m_pEffectView);
    } while(0);
}
