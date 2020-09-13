#ifndef CTIMELINE_H
#define CTIMELINE_H

#include <QWidget>
#include "effectview.h"
#include "globalutinityobject.h"
class CTimeLine : public QWidget
{
    Q_OBJECT
public:
    CTimeLine(GlobalUtinityObject* pGlobalObject, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void Layout();
    void InitFreezonView(CEffectView* pOriginView);
    void UpdateFreezonViewGeometry();

private slots:
    void slotColumnWidthChanged(int value);

private:
    CEffectView* m_pEffectView;
    CEffectView* m_pEffectFreezonView;
    GlobalUtinityObject* m_pGlobalObject;
};

#endif // CTIMELINE_H
