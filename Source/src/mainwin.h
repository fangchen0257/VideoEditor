#ifndef CMAINWIN_H
#define CMAINWIN_H

#include <QMainWindow>
#include "QtLib/winfrmtitle.h"

class CMainWin : public QMainWindow
{
    Q_OBJECT
public:
    CMainWin(QWidget* parent = nullptr);
    void OnMax();
    bool IsMaximized();

protected:
    void paintEvent(QPaintEvent* pEvent) override;
    void closeEvent(QCloseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
private:
    void Layout();

private:
    WinFrmTitle* m_pWinTitle;
};

#endif // CMAINWIN_H
