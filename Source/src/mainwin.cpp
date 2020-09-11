#include "mainwin.h"
#include <qpainter.h>
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>
#include "qmltypesregister.h"
#include <QSplitter>
#include "timeline.h"

CMainWin::CMainWin(QWidget *parent)
    :QMainWindow(parent, Qt::FramelessWindowHint|Qt::Window|Qt::WindowMinimizeButtonHint)
    ,m_pWinTitle(nullptr)
{
    QmlTypesRegister::instance().RegisterCommonType();
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

void CMainWin::closeEvent(QCloseEvent *e)
{
    QmlTypesRegister::instance().UtinityObject()->stopPlay();
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

        QSplitter* pSplitter = new QSplitter(Qt::Vertical);
        if (nullptr == pSplitter) break;
        pSplitter->setStyleSheet("QSplitter::handle {background-color:#151921; image:url(:/res/drag.png);}");
        pVboxMain->addWidget(pSplitter);

        QQuickWidget* pVideoWidget = new QQuickWidget;
        if (nullptr == pVideoWidget) break;
        QQmlContext* pQmlContext = pVideoWidget->rootContext();
        QmlTypesRegister::instance().setGlobalCariable(pQmlContext);
        pVideoWidget->setSource((QUrl("qrc:/qmls/widgets/VideoHomeView.qml")));
        pVideoWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pSplitter->addWidget(pVideoWidget);

        CTimeLine* pTimeLine = new CTimeLine(QmlTypesRegister::instance().UtinityObject());
        if (nullptr == pTimeLine) break;
        pSplitter->addWidget(pTimeLine);

        QList<int> listSize;
        listSize << 400 << 300;
        pSplitter->setSizes(listSize);
    } while(0);
}
