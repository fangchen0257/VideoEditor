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

private:
    void Layout();

private:
    CEffectView* m_pEffectView;
    GlobalUtinityObject* m_pGlobalObject;
};

#endif // CTIMELINE_H
