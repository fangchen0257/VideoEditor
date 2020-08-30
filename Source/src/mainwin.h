#ifndef CMAINWIN_H
#define CMAINWIN_H

#include <QMainWindow>
#include "QtLib/winfrmtitle.h"

class CMainWin : public QMainWindow
{
    Q_OBJECT
public:
    CMainWin(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* pEvent);

private:
    void Layout();

private:
    WinFrmTitle* m_pWinTitle;
};

#endif // CMAINWIN_H
