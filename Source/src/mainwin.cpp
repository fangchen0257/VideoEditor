#include "mainwin.h"
#include <qpainter.h>
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>
#include "qmltypesregister.h"
#include <QSplitter>
#include "timeline.h"
#include <qtablewidget.h>
#include <qlineedit.h>

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
        pSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pVboxMain->addWidget(pSplitter);

        QQuickWidget* pVideoWidget = new QQuickWidget;
        if (nullptr == pVideoWidget) break;
        QQmlContext* pQmlContext = pVideoWidget->rootContext();
        QmlTypesRegister::instance().setGlobalCariable(pQmlContext);
        pVideoWidget->setSource((QUrl("qrc:/qmls/widgets/VideoHomeView.qml")));
        pVideoWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pSplitter->addWidget(pVideoWidget);

#if 0
        CTimeLine* pTimeLine = new CTimeLine;
        if (nullptr == pTimeLine) break;
        pSplitter->addWidget(pTimeLine);
        pSplitter->setStretchFactor(0,3);
        pSplitter->setStretchFactor(1,1);
#endif

        QWidget* pWidget = new QWidget;
        pWidget->setMinimumHeight(150);
        QVBoxLayout* pVboxMain1 = new QVBoxLayout;
        if (nullptr == pVboxMain1) break;
        pVboxMain1->setContentsMargins(0, 0, 0, 0);
        pVboxMain1->setSpacing(0);
        pWidget->setLayout(pVboxMain1);
        QQuickWidget* pOperWidget = new QQuickWidget;
        if (nullptr == pOperWidget) break;
        pOperWidget->setSource((QUrl("qrc:/qmls/timeline/TimeLineView.qml")));
        pOperWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        pVboxMain1->addWidget(pOperWidget, 1);

        QSpacerItem* pVItem = new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding);
        if (nullptr == pVItem) break;
        //pVboxMain1->addSpacerItem(pVItem);

        QTableWidget* pTableWidget = new QTableWidget(3,3);
        if (nullptr == pTableWidget) break;
        //pTableWidget->setMinimumHeight(100);
        //pTableWidget->move(0,40);
        pVboxMain1->addWidget(pTableWidget, 3);

        QLineEdit* pEidt = new QLineEdit;
        //pVboxMain1->addWidget(pEidt);

        pSplitter->addWidget(pWidget);

    } while(0);
}
