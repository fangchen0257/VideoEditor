#ifndef CTIMELINE_H
#define CTIMELINE_H

#include <QWidget>
#include "effectview.h"
class CTimeLine : public QWidget
{
    Q_OBJECT
public:
    CTimeLine(QWidget* parent = nullptr);

private:
    void Layout();

private:
    CEffectView* m_pEffectView;
};

#endif // CTIMELINE_H
