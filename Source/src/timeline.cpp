#include "timeline.h"
#include <QVBoxLayout>
#include <QtQuickWidgets/QtQuickWidgets>
#include "qmltypesregister.h"

#define MINIZIM_HEIGHT 150
CTimeLine::CTimeLine(GlobalUtinityObject* pGlobalObject, QWidget *parent)
    :QWidget(parent)
    ,m_pEffectView(nullptr)
    ,m_pEffectFreezonView(nullptr)
    ,m_pGlobalObject(pGlobalObject)
{
    Layout();
}

bool CTimeLine::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_pEffectView)
    {
        int type = event->type();
        if (type == QEvent::Resize)
        {
            UpdateFreezonViewGeometry();
        }
    }

    if (watched == this)
    {
        int type = event->type();
        if (type==QEvent::Show || type==QEvent::Resize)
        {
            UpdateFreezonViewGeometry();
        }
    }

    return false;
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
        m_pEffectView->installEventFilter(this);
        connect(m_pEffectView, SIGNAL(sigColumnWidthChanged(int)), SLOT(slotColumnWidthChanged(int)));
        connect(m_pGlobalObject, SIGNAL(sigAddMedia2Track(int,const QVariant&)), m_pEffectView, SLOT(slotAddMedia2Track(int,const QVariant&)));
        connect(m_pGlobalObject, SIGNAL(sigAddPIP2Track(int,const QVariant&)), m_pEffectView, SLOT(slotAddPIP2Track(int,const QVariant&)));
        connect(m_pGlobalObject, SIGNAL(sigScaleSliderValueChanged(int)), m_pEffectView, SLOT(slotScaleValueChanged(int)));
        connect(m_pGlobalObject, SIGNAL(sigVideoProgressValueChanged(int)), m_pEffectView, SLOT(slotVideoPoregressValueChanged(int)));
        pVboxMain->addWidget(m_pEffectView);

        InitFreezonView(m_pEffectView);
        installEventFilter(this);
    } while(0);
}

void CTimeLine::InitFreezonView(CEffectView *pOriginView)
{
    do
    {
        if (nullptr == pOriginView) break;

        m_pEffectFreezonView = new CEffectView(ROW_CNT-1,COL_CNT,pOriginView);
        if (nullptr == m_pEffectFreezonView) break;
        m_pEffectFreezonView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pEffectFreezonView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pEffectFreezonView->setFocusPolicy(Qt::NoFocus);
        m_pEffectFreezonView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        connect(pOriginView->verticalScrollBar(), &QAbstractSlider::valueChanged,
                m_pEffectFreezonView->verticalScrollBar(), &QAbstractSlider::setValue);
        connect(m_pEffectFreezonView->verticalScrollBar(), &QAbstractSlider::valueChanged,
                pOriginView->verticalScrollBar(), &QAbstractSlider::setValue);
        for (int col = 1; col < COL_CNT; ++col)
        {
              m_pEffectFreezonView->setColumnHidden(col, true);
        }
    } while(0);
}

void CTimeLine::UpdateFreezonViewGeometry()
{
    m_pEffectFreezonView->setGeometry(m_pEffectView->verticalHeader()->width() + m_pEffectView->frameWidth(),
                                 m_pEffectView->frameWidth(), m_pEffectView->columnWidth(0),
                                      m_pEffectView->viewport()->height()+m_pEffectView->horizontalHeader()->height());
}

void CTimeLine::slotColumnWidthChanged(int width)
{
    Q_UNUSED(width);

    UpdateFreezonViewGeometry();
}
