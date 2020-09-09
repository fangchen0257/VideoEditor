#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <qwidget.h>
#include <qlabel.h>

typedef enum _TRACK_ITEM_TYPE_
{
    ITEM_VIDEO,
    ITEM_TRANSITION,
    ITEM_PIP,
    ITEM_FILTER,
    ITEM_TEXT,
    ITEM_MUSIC
}TRACK_TYPE;

#define ITEM_HEIGHT 35
class CTrackItem : public QWidget
{
public:
    CTrackItem(TRACK_TYPE type, QString strImg, QString strText, QWidget* parent = nullptr);
    bool IsSelect();
    void SetSelect(bool bSelect);
    void SetBg(QColor clrBg);

protected:
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);
    bool eventFilter(QObject* pWatched, QEvent* pEvent);

private:
    void Layout();
    void InitTrims();
    QLabel* Init1Trim(QString strPm);
    void ResetTrims();

protected:
    QColor m_clrBg;
    QString m_strImg;
    QString m_strText;
    TRACK_TYPE m_type;

    bool m_bSelected;
    bool m_bIsPressed;
    QCursor m_cursorTrim;
    QLabel* m_pLabTrimLeft;
    QLabel* m_pLabTrimRight;

    QPoint m_ptRelative;
};

#endif // TRACKITEM_H
