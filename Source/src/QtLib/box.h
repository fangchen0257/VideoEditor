#ifndef BOX_H
#define BOX_H

#include <QWidget>
#include <qboxlayout.h>

class CBox : public QWidget
{
    Q_OBJECT
public:
    CBox(bool bIsVbox);

public:
    void PackStart(QWidget* pWidget, bool bExpand=false, int padding=0, Qt::Alignment align = Qt::AlignLeft);
    void PackStart(QLayout* pLayout, int padding=0, int stretch=0);
    void AddSpacerItem(bool bHorizonal);
    void SetMargins(int l, int t, int r, int b);
    void SetSpacing(int spacing);
    void AddSpacing(int spacing);

private:
    QBoxLayout* m_pBox;
};

#endif // BOX_H
