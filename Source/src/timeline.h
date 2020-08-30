#ifndef CTIMELINE_H
#define CTIMELINE_H

#include <QWidget>
class CTimeLine : public QWidget
{
    Q_OBJECT
public:
    CTimeLine(QWidget* parent = nullptr);

private:
    void Layout();
};

#endif // CTIMELINE_H
