#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <qwidget.h>
#include <qlabel.h>
#include "EUProducer.h"
#include <QMouseEvent>

typedef enum _TRACK_ITEM_TYPE_
{
    ITEM_VIDEO,
    ITEM_TRANSITION,
    ITEM_PIP,
    ITEM_FILTER,
    ITEM_TEXT,
    ITEM_MUSIC
}TRACK_TYPE;

class CItemShadow : public QWidget
{
public:
    CItemShadow(QColor clgBg = QColor(0,0,0,127), QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* pEvent);

private:
    QColor m_clgBg;
};


#define ITEM_HEIGHT 35
class CTrackItem : public QWidget
{
    Q_OBJECT
public:
    CTrackItem(TRACK_TYPE type, QString strText, shared_ptr<CEUProducer> pProducer, QWidget* parent = nullptr);
    bool IsSelect();
    void SetSelect(bool bSelect);
    void SetBg(QColor clrBg);
    void ResetPixelPerFrame(double pixelPerFrm);

protected:
    void showEvent(QShowEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);
    bool eventFilter(QObject* pWatched, QEvent* pEvent);

private:
    void Layout();
    void InitTrims();
    void InitShadowItem();
    QLabel* Init1Trim(QString strPm);
    void ResetTrims();
    QImage clipImage();
    void ResetMediaText();
    bool HandleLeftTrimEvent(QEvent *pEvent);
    bool HandleRightTrimEvent(QEvent *pEvent);

signals:
    void sigItemSelect(CTrackItem*);
    void sigClipTrim(int, int);

protected:
    shared_ptr<CEUProducer> m_pProducer;
    QColor m_clrBg;
    QString m_strText;
    int m_frmWidth;
    TRACK_TYPE m_type;
    QLabel* m_pLabText;

    bool m_bSelected;
    bool m_bIsPressed;
    bool m_bIsLeftTrimPressed;
    bool m_bIsRightTrimPressed;
    QCursor m_cursorTrim;
    QLabel* m_pLabTrimLeft;
    QLabel* m_pLabTrimRight;
    CItemShadow* m_pItemShadow;

    QPoint m_ptRelative;
    double m_pixelPerFrm;
};

#endif // TRACKITEM_H
