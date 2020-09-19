#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <qwidget.h>
#include <qlabel.h>
#include "EUProducer.h"
#include <QMouseEvent>

typedef enum _TRACK_ITEM_TYPE_
{
    ITEM_VIDEO,
    ITEM_PIP,
    ITEM_FILTER,
    ITEM_TEXT,
    ITEM_MUSIC,
    ITEM_TRANSITION,
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

#define IMG_THUMB_W 55
#define IMG_THUMB_H 30
#define ITEM_HEIGHT 35
class CTrackItem : public QWidget
{
    Q_OBJECT
public:
    CTrackItem(TRACK_TYPE type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo, QWidget* parent = nullptr);
    bool IsSelect();
    void SetSelect(bool bSelect);
    void SetBg(QColor clrBg);
    void SetShadowItem(CItemShadow* pItemShadow);
    void ResetPixelPerFrame(double pixelPerFrm);
    void ResetItem(double pixelPerFrame);
    void ResetItem(shared_ptr<Mlt::ClipInfo> clipInfo, QImage imgThumb);
    shared_ptr<Mlt::ClipInfo> GetClipInfo();
    TRACK_TYPE type();

protected:
    void showEvent(QShowEvent* pEvent);
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);
    bool eventFilter(QObject* pWatched, QEvent* pEvent);
    void keyPressEvent(QKeyEvent *event);

private:
    void Layout();
    void InitTrims();
    QLabel* Init1Trim(QString strPm);
    void ResetTrims();
    void ResetMediaText();
    void ResetThumb();
    void ResetGeometry(double pixelPerFrame);
    void DeleteItem();

private:
    QRect calcGeometry(QPoint pt, bool bRightTrim);
    bool HandleLeftTrimEvent(QEvent *pEvent);
    bool HandleRightTrimEvent(QEvent *pEvent);
    void OnLeftTrimMouseRelease();
    void OnRightTrimMouseRelease();
    void OnItemMoveFinish(int xOffset);

signals:
    void sigItemSelect(CTrackItem*);
    void sigClipTrim(CTrackItem*,int, int);
    void sigMoveClip(CTrackItem*,int,int);
    void sigItemDelete(CTrackItem*);

protected:
    shared_ptr<Mlt::ClipInfo> m_clipInfo;
    QImage m_imgThumb;
    QColor m_clrBg;
    QString m_strText;
    int m_position;             //剪辑在playlist中的开始位置,第一个剪辑该值为0
    int m_frameLength;          //原始的帧长度
    int m_curFrameCount;        //编辑后的帧长度，总是小于原始帧长度
    TRACK_TYPE m_type;
    QLabel* m_pLabText;
    QLabel* m_pLabThumb;

    bool m_bSelected;
    bool m_bIsPressed;
    bool m_bIsLeftTrimPressed;
    bool m_bIsRightTrimPressed;
    QCursor m_cursorTrim;
    QLabel* m_pLabTrimLeft;
    QLabel* m_pLabTrimRight;
    CItemShadow* m_pItemShadow;

    QPoint m_wndPos;
    QPoint m_ptRelative;
    double m_pixelPerFrm;
};

#endif // TRACKITEM_H
