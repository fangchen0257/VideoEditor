#ifndef CUIPUBLIC_H
#define CUIPUBLIC_H

#include <qwidget.h>
class CUIPublic
{
public:
    static CUIPublic& instance();
    ~CUIPublic();
    void SetMainWin(QWidget* pMainWin);
    QWidget* GetMainWin();

private:
    CUIPublic();

private:
    QWidget* m_pMainWin;
};

#endif // CUIPUBLIC_H
