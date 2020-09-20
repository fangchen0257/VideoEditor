#ifndef CTRACKSPLIT_H
#define CTRACKSPLIT_H

#include <qwidget.h>
#include <qlabel.h>
#include "QtLib/ImgButton.h"

#define SPLITER_FIXED_WIDTH 55
class CTrackSplit : public QWidget
{
    Q_OBJECT
public:
    CTrackSplit(QWidget* parent = nullptr);
    void SetPosX(int posX);
    int InitializePos();
    void InitRegionMask();

private:
    void Layout();

protected:
    bool eventFilter(QObject* pWatched, QEvent* pEvent);
    void showEvent(QShowEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);

private slots:
    void slotSpliterBtnClicked(int btnId);

signals:
    void sigSpliterMove(int, int);
    void sigSpliterBtnClicked(int);

private slots:
    void slotSplit(int position);

private:
    CImagButton* m_pBtnSplit;
    QLabel* m_pLabMark;

    bool    m_bIsPressed;
    QPoint  m_ptMousePressed;
    int     m_InitializePosX;   //初始位置
};

#endif // CTRACKSPLIT_H
