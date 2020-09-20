#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QTableWidget>
#include "QtLib/box.h"
#include "trackitem.h"
#include "EUTractor.h"
#include "QtLib/public.h"
#include "tracksplit.h"
#include "scalecell.h"

enum _TRACK_OPER_
{
    OPER_VISIBLE,
    OPER_MUTE,
    OPER_LOCK
};

enum _ROW_ITEMS_
{
    ROW_SCALE,
    ROW_VIDEO,
    ROW_PICINPIC,
    ROW_FILTERS,
    ROW_TEXT,
    ROW_MUSIC,
    ROW_TRANSITIONS,
    ROW_CNT
};

enum _COLUMN_ITEMS_
{
    COL_EFFECT_NAME,
    COL_OPERA_REGION,
    COL_CNT
};

#define FIRST_COLUMN_WIDTH 150
class CEffectView: public QTableWidget
{
    Q_OBJECT
public:
    CEffectView(QWidget* parent = nullptr);
    CEffectView(int row, int column, QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* pEvent);
    bool viewportEvent(QEvent* pEvent);

private:
    void Layout();
    void InitFirstColumn();
    CBox* InitCellItem(QString strIcon, QString strText, QVector<QString> vecResLock, QVector<QString> vecVisible, QVector<QString> vecMute);
    void InitTrackContainer();
    void InitScaleCellWidget();
    void InitSpliter();

private:
    CTrackItem* AppendClip(int type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo);
    QString trackItemText(QString strMediaPath);
    void SelectItem(CTrackItem* pCurItem);
    void RefreshTrackItems(int type);
    void DeleteTrackItems(QVector<CTrackItem*>& vecTrackItems);
    void ResetColumnWidth();
    void ResetSpliter(bool bVisible);
    void ResetProducer(int type, int positon);
    void AdjustSpliterPos();
    double GetPixelPerFrame();

private:
    void addItem2Vector(int type, CTrackItem* pItem);
    CItemShadow* itemShadow(int type);
    CBox* itemContainer(int type);
    int itemWidth(int type);
    QVector<CTrackItem*> getAllTrackItems();
    int getSpliterMaximum();
    int getSpliterCurFrame();
    bool isTrackProducer();
    int horScrollBarValue();

private:
    shared_ptr<CEUMainVideoTrack> mainVideoTrack();
    shared_ptr<CEUSubVideoTrack> subVideoTrack();

signals:
    void sigColumnWidthChanged(int);

private slots:
    void slotAddMedia2Track(int type, const QVariant& media);
    void slotAddPIP2Track(int type,const QVariant& media);
    void slotTrackItemSelect(CTrackItem* pItem);
    void slotScaleValueChanged(int value);
    void slotClipTrim(CTrackItem* pItem, int in,int out);
    void slotClipMove(CTrackItem* pItem, int clipIndex, int position);
    void slotItemDelete(CTrackItem* pItem);
    void slotSpliterMove(int x, int y);
    void slotSpliterBtnClicked(int x);
    void slotVideoPoregressValueChanged(int value);
    void slotScaleCellClick();

private:
    CScaleCell* m_pScaleCell;   //刻度尺
    CTrackSplit* m_pSpliter;
    CTrackItem* m_pCurrentItem;

    QVector<QPair<CBox*, CItemShadow*>> m_vecItemContainer;
    QVector<CTrackItem*> m_vecTrackItems4Video;
    QVector<CTrackItem*> m_vecTrackItems4PIP;
    int     m_regionWidthOnMax;
    bool    m_bSetCurrentFrame;
};

#endif // EFFECTVIEW_H
