#ifndef WINFRMTITLE_H
#define WINFRMTITLE_H

#include <qwidget.h>
#include <qstring.h>
#include "ImgButton.h"

enum _TITLE_BTN_
{
    TITLE_BTN_MIN   = 1,
    TITLE_BTN_MAX   = 1 << 1,
    TITLE_BTN_CLOSE = 1 << 2
};

class WinFrmTitle : public QWidget
{
    Q_OBJECT
public:
    WinFrmTitle(QString strTitle, QWidget* parent);

protected:
    void paintEvent(QPaintEvent* pEvent) override;
    void mousePressEvent(QMouseEvent* pEvent) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;

private:
    void Layout();
    CImagButton* CreateImgButton(QVector<QString> vecRes, int btnId);
    void ResetMaxBtnImg(bool bIsMaximized);

private slots:
    void slotBtnClick(int btnId);

private:
    QWidget*        m_pMainWin;
    CImagButton*    m_pbtnMax;

    QString m_strTitle;
    bool    m_bPressed;
    QPoint	m_ptPress;
    bool    m_bIsMaximized;
};

#endif // WINFRMTITLE_H
