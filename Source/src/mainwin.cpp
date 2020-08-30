#include "mainwin.h"
#include <qpainter.h>
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>
#include "qmltypesregister.h"

CMainWin::CMainWin(QWidget *parent)
    :QMainWindow(parent, Qt::FramelessWindowHint|Qt::Window|Qt::WindowMinimizeButtonHint)
    ,m_pWinTitle(nullptr)
{
    Layout();
}

void CMainWin::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);

    QPainter paint(this);
    paint.setPen(QColor(0x18,0x84,0xd7));
    paint.setBrush(QColor(0x2e,0x2f,0x30));

    QRect rt = rect();
    rt.adjust(0,0,-1,-1);
    paint.drawRect(rt);
}

void CMainWin::Layout()
{
    do
    {
        QWidget* pCentralWidget = new QWidget;
        if (nullptr == pCentralWidget) break;

        QVBoxLayout* pVboxMain = new QVBoxLayout;
        if (nullptr == pVboxMain) break;
        pVboxMain->setContentsMargins(1, 1, 1, 1);
        pVboxMain->setSpacing(0);
        pCentralWidget->setLayout(pVboxMain);
        setCentralWidget(pCentralWidget);

        m_pWinTitle = new WinFrmTitle(QObject::tr("Video Editor"), this);
        if (nullptr == m_pWinTitle) break;
        pVboxMain->addWidget(m_pWinTitle);

        QmlTypesRegister::instance().RegisterCommonType();
        QQuickWidget* pQMLWidget = new QQuickWidget;
        if (nullptr == pQMLWidget) break;
        QmlTypesRegister::instance().setGlobalCariable(pQMLWidget->rootContext());
        pQMLWidget->setSource((QUrl("qrc:/qmls/widgets/VideoHomeView.qml")));
        pQMLWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pVboxMain->addWidget(pQMLWidget);
    } while(0);
}
